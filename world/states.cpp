#include "states.h"
#include "action.h"
#include "game_object.h"
#include "world.h"

// Helper function

bool on_platform(World& world, GameObject& obj) {
    constexpr float epsilon = 1e-4;
    Vec<float> left_foot{obj.physics.position.x + epsilon, obj.physics.position.y - epsilon};
    Vec<float> right_foot{obj.physics.position.x + obj.size.x - epsilon, obj.physics.position.y - epsilon};
    return world.collides(left_foot) || world.collides(right_foot);
}

// Standing
void Standing::on_enter(World&, GameObject& obj) {
    obj.color = {160, 0, 255, 255};
    obj.set_sprite("idle");
    obj.physics.acceleration.x = 0;
    obj.physics.acceleration.y = 0;
}

Action* Standing::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveLeft();
    }
    if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight();
    }
    if (action_type == ActionType::MoveDown) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveDown();
    }
    if (action_type == ActionType::MoveUp) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveUp();
    }
    return nullptr;
}

void Running::on_enter(World&, GameObject& obj) {
    obj.color = {255, 255, 0, 255};
    if (obj.physics.velocity.x > 0) {
        obj.set_sprite("walking_right");
    }
    if (obj.physics.velocity.x <= 0) {
        obj.set_sprite("walking_left");
    }
}

Action* Running::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::None) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
    if (action_type == ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveLeft();
    }
    if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight();
    }
    if (action_type == ActionType::MoveUp) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveUp();
    }
    if (action_type == ActionType::MoveDown) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveDown();
    }
    return nullptr;
}

