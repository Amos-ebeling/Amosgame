#pragma once

enum class ActionType {
    None,
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
    MoveUpRight,
    MoveDownRight,
    MoveUpLeft,
    MoveDownLeft,
    AttackAll
};

class World;
class GameObject;

class Action {
public:
    virtual ~Action() = default;
    virtual  void perform(World& world, GameObject& obj) = 0;

};


class MoveUpRight : public Action {
    void perform(World& world, GameObject& obj) override;
};

class MoveDownLeft : public Action {
    void perform(World& world, GameObject& obj) override;
};

class MoveUpLeft : public Action {
    void perform(World& world, GameObject& obj) override;
};

class MoveDownRight : public Action {
    void perform(World& world, GameObject& obj) override;
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

class ThrowStick : public Action {
public:
    void perform(World& world, GameObject& obj) override;
};