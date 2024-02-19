package server.player.motion.dto;

import io.micronaut.serde.annotation.Serdeable;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.List;

@Data
@AllArgsConstructor
@NoArgsConstructor
@Serdeable
public class PlayerMotionList {
    List<PlayerMotion> playerMotionList;
}
