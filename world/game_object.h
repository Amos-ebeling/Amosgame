#pragma once
#include <utility>
#include "vec.h"
#include <map>
#include "animated_sprite.h"
#include "graphics.h"
#include "physics.h"
#include "quadtree.h"

class World;
class FSM;
class Input;

using Sprites = std::map<std::string, AnimatedSprite>;

class GameObject {
public:
    GameObject(std::string name, FSM* fsm, Input* input, Color color);
    ~GameObject();
    virtual void update(World& world, double dt);
    std::pair<Vec<float>, Color> get_sprite() const;
    void set_sprite(const std::string& next_sprite);

    AABB get_bounding_box();

    void take_damage(int attack_damage);
    bool flash_sprite() const;

    //GameObject data
    std::string obj_name;
    Physics physics;
    Vec<int> size;
    FSM* fsm;
    Color color;
    Input* input;
    Sprites sprites;
    Sprite sprite;
    std::string sprite_name;

    //combat stuff
    int health;
    int max_health;
    int damage;
    bool is_alive = true;

    double i_frame{0.0};
};