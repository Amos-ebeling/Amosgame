#include "physics.h"

Physics::Physics(Vec<float> position, Vec<float> velocity, Vec<float> acceleration)
    :position{position}, velocity{velocity}, acceleration{acceleration}{
}

Physics::Physics(Vec<float> position, Vec<float> velocity, Vec<float> acceleration, float damping, float walk_acceleration, float terminal_velocity)
    :position{position}, velocity{velocity}, acceleration{acceleration}, damping{damping}, walk_acceleration{walk_acceleration}, terminal_velocity{terminal_velocity}{
}
