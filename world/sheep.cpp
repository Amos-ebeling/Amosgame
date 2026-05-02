#include "sheep.h"
#include "game_object.h"

Sheep::Sheep(std::string name, FSM* fsm, Input* input)
    :GameObject{name, fsm, input, {120,142,80,255}} {}
