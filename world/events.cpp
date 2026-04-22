//
// Created by zhimo on 3/25/2026.
//
#include "world.h"
#include "events.h"
#include "game_object.h"
#include <iostream>

void NextLevel::perform(World& world, GameObject& obj) {
    if (&obj != world.player) return;
    world.end_level = true;
}
