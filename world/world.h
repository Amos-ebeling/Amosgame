#pragma once

#include <memory>
#include <SDL3/SDL.h>
#include "tilemap.h"
#include "vec.h"
#include "game_object.h"
#include "audio.h"
#include "events.h"
#include "quadtree.h"
#include "projectile.h"

class Level;
class Audio;
class GameObject;

class World {
public:
    World(const Level& level, Audio& audio, GameObject* player, Events& events);
    ~World();
    void add_platform(float x, float y, float width, float height);
    bool collides(const Vec<float>& position) const;
    GameObject* create_player(const Level& level);
    void move_to(Vec<float>& position, const Vec<int>& size, Vec<float>& velocity);
    void update(double dt);
    void update_object(GameObject*, double dt);
    void load_level(const Level& level);

    Tilemap tilemap;
    bool end_level{false};
    bool end_game{false};


    void build_quadtree();

    GameObject* player;
    Audio* audio;
    Events events;
    std::vector<GameObject*> game_objects;
    std::vector<Projectile*> projectiles;
    void touch_tiles(GameObject& obj);

    QuadTree quad_tree;

    //items
    std::map<std::string, std::function<GameObject*()>> available_items;
};