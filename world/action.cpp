#include "action.h"
#include "game_object.h"
#include "projectile.h"
#include "world.h"

void MoveUpRight::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.x = obj.physics.walk_acceleration;
    obj.physics.acceleration.y = obj.physics.walk_acceleration;
}

void MoveDownLeft::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.x = -obj.physics.walk_acceleration;
    obj.physics.acceleration.y = -obj.physics.walk_acceleration;
}

void MoveUpLeft::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.y = obj.physics.walk_acceleration;
    obj.physics.acceleration.x = -obj.physics.walk_acceleration;
}

void MoveDownRight::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.y = -obj.physics.walk_acceleration;
    obj.physics.acceleration.x = obj.physics.walk_acceleration;
}

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

void ThrowStick::perform(World& world, GameObject& obj) {
    auto stick = dynamic_cast<Projectile*>(world.available_items["stick"]());
    stick->physics.position = obj.physics.position;
    world.projectiles.push_back(stick);
}
