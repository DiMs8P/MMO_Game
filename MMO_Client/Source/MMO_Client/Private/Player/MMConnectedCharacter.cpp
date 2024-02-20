// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MMConnectedCharacter.h"

#include "HttpModule.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

AMMConnectedCharacter::AMMConnectedCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// configure your base API path
	PathToAPI = "http://localhost:8081/";
}

void AMMConnectedCharacter::UpdateLocationOnServer()
{
	APlayerController* PC = Cast<APlayerController>(this->Controller);
	if (PC && PC->PlayerState)
	{
		FString PlayerName = PC->PlayerState->GetPlayerName();
		Http = &FHttpModule::Get();
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &AMMConnectedCharacter::OnUpdateLocationOnServerResponseReceived);

		// Set your input params - this is the PlayerMotion DTO in Java
		TSharedPtr<FJsonObject> Params = MakeShareable(new FJsonObject);
		Params->SetStringField(TEXT("playerName"), PlayerName);

		TSharedPtr<FJsonObject> InputMotion = MakeShareable(new FJsonObject);
		InputMotion->SetNumberField(TEXT("x"), GetActorLocation().X);
		InputMotion->SetNumberField(TEXT("y"), GetActorLocation().Y);
		InputMotion->SetNumberField(TEXT("z"), GetActorLocation().Z);

		InputMotion->SetNumberField(FString(TEXT("roll")), GetActorRotation().Roll);
		InputMotion->SetNumberField(FString(TEXT("pitch")), GetActorRotation().Pitch);
		InputMotion->SetNumberField(FString(TEXT("yaw")), GetActorRotation().Yaw);

		InputMotion->SetNumberField(FString(TEXT("vx")), GetCharacterMovement()->GetLastUpdateVelocity().X);
		InputMotion->SetNumberField(FString(TEXT("vy")), GetCharacterMovement()->GetLastUpdateVelocity().Y);
		InputMotion->SetNumberField(FString(TEXT("vz")), GetCharacterMovement()->GetLastUpdateVelocity().Z);

		Params->SetObjectField(FString(TEXT("motion")), InputMotion);

		FString ParamsString;
		TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&ParamsString);
		FJsonSerializer::Serialize(Params.ToSharedRef(), JsonWriter);

		FString Url = PathToAPI + FString(TEXT("player/update-motion"));

		Request->SetURL(Url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/json"));
		Request->SetHeader("Accept", TEXT("application/json"));

		Request->SetContentAsString(ParamsString);
		Request->ProcessRequest();
	}
}

void AMMConnectedCharacter::OnUpdateLocationOnServerResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful) {
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			if (JsonObject->HasField("playerMotionList"))
			{
				TArray<TSharedPtr<FJsonValue>> Rows = JsonObject->GetArrayField("playerMotionList");

				for (int i = 0; i < Rows.Num(); i++)
				{
					FProxyCharacter tempProxyCharacter;

					TSharedPtr<FJsonObject> TempRow = Rows[i]->AsObject();

					tempProxyCharacter.PlayerName = TempRow->GetStringField("playerName");
					TSharedPtr<FJsonObject> Motion = TempRow->GetObjectField("motion");

					tempProxyCharacter.PlayerLocation.X = Motion->GetNumberField("x");
					tempProxyCharacter.PlayerLocation.Y = Motion->GetNumberField("y");
					tempProxyCharacter.PlayerLocation.Z = Motion->GetNumberField("z");

					tempProxyCharacter.PlayerRotation.Pitch = Motion->GetNumberField("pitch");
					tempProxyCharacter.PlayerRotation.Roll = Motion->GetNumberField("roll");
					tempProxyCharacter.PlayerRotation.Yaw = Motion->GetNumberField("yaw");

					tempProxyCharacter.PlayerVelocity.X = Motion->GetNumberField("vx");
					tempProxyCharacter.PlayerVelocity.Y = Motion->GetNumberField("vy");
					tempProxyCharacter.PlayerVelocity.Z = Motion->GetNumberField("vz");

					// Update
					if (ProxyCharacters.Contains(tempProxyCharacter.PlayerName))
					{
						// Update
						ProxyCharacters[tempProxyCharacter.PlayerName].PlayerLocation = tempProxyCharacter.
							PlayerLocation;
						ProxyCharacters[tempProxyCharacter.PlayerName].PlayerRotation = tempProxyCharacter.
							PlayerRotation;
						ProxyCharacters[tempProxyCharacter.PlayerName].PlayerVelocity = tempProxyCharacter.
							PlayerVelocity;


						NotifyUpdateCharacter(tempProxyCharacter);
					}
					else // Add
					{
						ProxyCharacters.Add(tempProxyCharacter.PlayerName, tempProxyCharacter);
						NotifyNewCharacter(tempProxyCharacter);
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnUpdateLocationOnServerResponseReceived Server received no data"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnUpdateLocationOnServerResponseReceived Error accessing server"));
	}
}

void AMMConnectedCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
