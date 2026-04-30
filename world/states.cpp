#include "states.h"
#include "action.h"
#include "game_object.h"
#include "random.h"
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
    if (action_type == ActionType::MoveUpLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveUpLeft();
    }
    if (action_type == ActionType::MoveDownRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveDownRight();
    }
    if (action_type == ActionType::MoveDownLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveDownLeft();
    }
    if (action_type == ActionType::MoveUpRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveUpRight();
    }
    if (action_type == ActionType::AttackAll) {
        obj.fsm->transition(Transition::AttackAll, world, obj);
    }
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
    if (action_type == ActionType::AttackAll) {
        obj.fsm->transition(Transition::AttackAll, world, obj);
    }
    return nullptr;
}

void Running::on_enter(World&, GameObject& obj) {
    obj.color = {255, 255, 0, 255};
    obj.set_sprite("walking_left");
}

Action* Running::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::MoveUpLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveUpLeft();
    }
    if (action_type == ActionType::MoveDownRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveDownRight();
    }
    if (action_type == ActionType::MoveDownLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveDownLeft();
    }
    if (action_type == ActionType::MoveUpRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveUpRight();
    }
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

//patrolling
void Patrolling::on_enter(World& world, GameObject& obj) {
    //set cooldown to random time 3-10
    elapsed = 0;
    cooldown = randint(3, 10);
    Running::on_enter(world, obj);
}

Action* Patrolling::input(World& world, GameObject& obj, ActionType action_type) {
    if (elapsed >= cooldown) {
        return  Running::input(world, obj, ActionType::None);
    }
    return Running::input(world, obj, action_type);
}

void Patrolling::update(World&, GameObject&, double dt) {
    elapsed += dt;
}




//AttackAll
void AttackAllEnemies::on_enter(World& world, GameObject& obj) {
    obj.color = {255,102, 0, 255};
    for (auto& enemy : world.game_objects) {
        if (enemy == world.player) continue;
        enemy->take_damage(obj.damage);
    }
    elapsed = 0;
}

void AttackAllEnemies::update(World& world, GameObject& obj, double dt) {
    elapsed += dt;
    if (elapsed>= cooldown) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}