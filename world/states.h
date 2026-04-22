#pragma once
#include "fsm.h"

class Standing:public State {
public:
    void on_enter(World&, GameObject&) override;
    Action* input(World&, GameObject&, ActionType) override;
};

class InAir:public State {
    void on_enter(World&, GameObject&) override;
    void update(World&, GameObject&, double dt) override;

    const double cooldown = 0.1;
    double elapsed = 0;
};

class Running:public State {
public:
    void on_enter(World&, GameObject&) override;
    Action* input(World&, GameObject&, ActionType) override;
};

class AttackAllEnemies : public State {
public:
    virtual void on_enter(World&, GameObject&) override;
    virtual void update(World&, GameObject&, double dt) override;

    double elapsed = 0;
    double cooldown = 2;
};