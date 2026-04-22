#pragma once

enum class ActionType {
    None,
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
    AttackAll
};

class World;
class GameObject;

class Action {
public:
    virtual ~Action() = default;
    virtual  void perform(World& world, GameObject& obj) = 0;

};


class MoveRight : public Action {
    void perform(World& world, GameObject& obj) override;
};

class MoveLeft : public Action {
    void perform(World& world, GameObject& obj) override;
};

class MoveUp : public Action {
    void perform(World& world, GameObject& obj) override;
};

class MoveDown : public Action {
    void perform(World& world, GameObject& obj) override;
};

class AttackAll : public Action {
    void perform(World& world, GameObject& obj) override;
};