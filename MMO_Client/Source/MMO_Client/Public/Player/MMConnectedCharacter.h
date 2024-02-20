// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMO_ClientCharacter.h"
#include "Interfaces/IHttpRequest.h"
#include "MMConnectedCharacter.generated.h"

class FHttpModule;

USTRUCT(BlueprintType)
struct FProxyCharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMO")
	FString PlayerName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMO")
	FVector PlayerLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMO")
	FRotator PlayerRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMO")
	FVector PlayerVelocity;
};

struct FMotion
{
	FVector Location;
	FRotator Rotation;
	FVector Velocity;
};

UCLASS()
class MMO_CLIENT_API AMMConnectedCharacter : public AMMO_ClientCharacter
{
	GENERATED_BODY()

public:
	AMMConnectedCharacter();
	
	UFUNCTION(BlueprintCallable, Category = "MMO")
	void UpdateLocationOnServer();

	void OnUpdateLocationOnServerResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent, Category = "MMO")
	void NotifyNewCharacter(const FProxyCharacter& ProxyCharacter);

	UFUNCTION(BlueprintImplementableEvent, Category = "MMO")
	void NotifyUpdateCharacter(const FProxyCharacter& ProxyCharacter);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	FString PathToAPI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMO")
	UClass* ProxyCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMO")
	TMap<FString, FProxyCharacter> ProxyCharacters;

private:
	FHttpModule* Http;
};
