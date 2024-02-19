package server.common.dto;

import io.micronaut.core.annotation.Introspected;
import io.micronaut.serde.annotation.Serdeable;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
@Serdeable
public class Motion {

    // Position
    Integer x;
    Integer y;
    Integer z;

    // Rotation
    Integer pitch;
    Integer roll;
    Integer yaw;

    // Velocity
    Integer vx;
    Integer vy;
    Integer vz;
}
