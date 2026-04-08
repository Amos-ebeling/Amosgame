//
// Created by zhimo on 3/25/2026.
//
#include "world.h"
#include "events.h"
#include "game_object.h"
#include <iostream>

void NextLevel::perform(World& world, GameObject& obj) {
    world.end_level = true;
    std::cout<<"Next Level!\n";
}
