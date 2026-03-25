#pragma once

#include <memory>
#include <SDL3/SDL.h>
#include "tilemap.h"
#include "vec.h"
#include "game_object.h"

class Level;
class GameObject;

class World {
public:
    World(const Level& level);

    void add_platform(float x, float y, float width, float height);
    bool collides(const Vec<float>& position) const;
    GameObject* create_player(const Level& level);
    void move_to(Vec<float>& position, const Vec<int>& size, Vec<float>& velocity);
    void update(float dt);
    void load_level(const Level& level);

    Tilemap tilemap;

private:
    GameObject* player;
};