#include "action.h"
#include "game_object.h"
#include "projectile.h"
#include "world.h"
#include "audio.h"

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
    obj.physics.acceleration.y = 0;
}

void MoveLeft::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.x = -obj.physics.walk_acceleration;
    obj.physics.acceleration.y = 0;
}

void MoveUp::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.y = obj.physics.walk_acceleration;
    obj.physics.acceleration.x = 0;
}

void MoveDown::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.y = -obj.physics.walk_acceleration;
    obj.physics.acceleration.x = 0;
}

void ThrowStick::perform(World& world, GameObject& obj) {
    auto stick = dynamic_cast<Projectile*>(world.available_items["stick"]());
    stick->physics.position = obj.physics.position;
    stick->physics.velocity = Vec{obj.physics.velocity.x/3.0f, obj.physics.velocity.y/3.0f} * stick->physics.velocity;
    world.projectiles.push_back(stick);
    world.audio->play_sounds("stick_throw");
}
