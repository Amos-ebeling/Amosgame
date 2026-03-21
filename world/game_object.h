#pragma once
#include <utility>
#include "vec.h"
#include <map>
#include "animated_sprite.h"
#include "graphics.h"
#include "physics.h"

class World;
class FSM;
class Input;

using Sprites = std::map<std::string, AnimatedSprite>;

class GameObject {
public:
    GameObject(const Vec<int>& size, World& world, FSM* fsm, Input* input, Color color);
    ~GameObject();
    void update(World& world, double dt);
    std::pair<Vec<float>, Color> get_sprite() const;
    void set_sprite(const std::string& next_sprite);
    //GameObject data
    Physics physics;
    Vec<int> size;
    FSM* fsm;
    Color color;
    Input* input;
    Sprites sprites;
    Sprite sprite;
    std::string sprite_name;
};