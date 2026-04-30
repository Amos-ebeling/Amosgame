//
// Created by zhimo on 4/20/2026.
//

#include "game_object.h"

#pragma once

class Projectile : public GameObject{
public:
    Projectile(std::string name, FSM* fsm, Input* input, double lifetime = 1.0);

    double lifetime;
    double elapsed{0.0};

    void update(World& world, double dt) override;
};