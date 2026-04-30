//
// Created by zhimo on 4/20/2026.
//
#include "projectile.h"

Projectile::Projectile(std::string name, FSM* fsm, Input* input, double lifetime)
    :GameObject{name, fsm, input, {40,142,172,255}}, lifetime{lifetime} {}

void Projectile::update(World& world, double dt) {
    GameObject::update(world, dt);
    elapsed += dt;
}