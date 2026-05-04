//
// Created by zhimo on 3/25/2026.
//
#include "world.h"
#include "events.h"
#include "game_object.h"
#include "level.h"
#include <iostream>

#include "ai_input.h"
#include "fsm.h"
#include "states.h"

void NextLevel::perform(World& world, GameObject& obj) {
    if (&obj != world.player) return;
    if (world.player->sheep_count >= 1) {
        world.end_level = true;
    }
}

void SpawnSheep::perform(World& world, GameObject& obj) {
    if (world.sheep_spawn_elapsed < world.sheep_spawn_cooldown) return;
    world.sheep_spawn_elapsed = 0.0;
    Transitions transitions = {
        {{StateType::Standing, Transition::Move}, StateType::Patrolling},
        {{StateType::Patrolling, Transition::Stop}, StateType::Standing}
    };
    States states = {
        {StateType::Standing, new Standing()},
        {StateType::Patrolling, new Patrolling()}
    };
    FSM* fsm = new FSM{transitions, states, StateType::Patrolling};
    Input* input = new Ai_Input{};

    auto sheep = world.available_items["sheep"]();
    sheep->fsm = fsm;
    sheep->input = input;
    input->next_action_type = ActionType::MoveRight;
    sheep->physics.position = obj.physics.position + Vec<float>{1.0f, 1.0f};
    world.game_objects.push_back(sheep);
    world.sheeps.push_back(dynamic_cast<Sheep*>(sheep));
}
