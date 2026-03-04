#include "action.h"
#include "game_object.h"
#include "world.h"

void MoveRight::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.x = obj.physics.walk_acceleration;
}

void MoveLeft::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.x = -obj.physics.walk_acceleration;
}

void MoveUp::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.y = obj.physics.walk_acceleration;
}

void MoveDown::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.y = -obj.physics.walk_acceleration;
}
