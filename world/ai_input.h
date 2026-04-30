#pragma once
#include "game_object.h"
#include "input.h"

class Ai_Input : public Input{
public:
    void get_input() override;
    void handle_input(World& world, GameObject& obj) override;

};