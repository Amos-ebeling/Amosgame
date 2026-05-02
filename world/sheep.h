#pragma once
#include "game_object.h"

class Sheep : public GameObject{
public:
    Sheep(std::string name, FSM* fsm, Input* input);
};
