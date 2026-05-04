#include "world.h"
#include <SDL3/SDL_rect.h>
#include <iostream>
#include <algorithm>
#include "keyboard_input.h"
#include "states.h"
#include "fsm.h"
#include "game_object.h"
#include "level.h"
#include "vec.h"
#include "physics.h"

World::World(const Level& level, Audio& audio, GameObject* player, Events& events)
    : tilemap{level.width, level.height}, audio{&audio}, player{player}, events{events},
      quad_tree{AABB{{level.width / 2.0f, level.height / 2.0f}, {level.width / 2.0f, level.height / 2.0f}}} {
    load_level(level);
}

World::~World() {
    for (auto obj: game_objects) {
        if (obj == player) continue;
        delete obj;
    }
}

void World::add_platform(float x, float y, float width, float height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            tilemap(x + j, y + i) = Tile{};
        }
    }
}

bool World::collides(const Vec<float>& position) const {
    int x = std::floor(position.x);
    int y = std::floor(position.y);
    return tilemap(x, y).blocking;
}


void World::move_to(Vec<float>& position, const Vec<int>& size, Vec<float>& velocity) {
    float epsilon = 0.001f;
    auto left = position.x;
    auto right = position.x + size.x - epsilon;
    auto bottom = position.y;
    auto top = position.y + size.y - epsilon;
    // test sides first. if both collide move backward
    // bottom side
    if (collides({left, bottom}) && collides({right, bottom})) {
        position.y = std::ceil(position.y);
        velocity.y = 0;
    }
    // top side
    else if (collides({left, top}) && collides({right, top})) {
        position.y = std::floor(position.y);
        velocity.y = 0;
    }
    // left side
    if (collides({left, bottom}) && collides({left, top})) {
        position.x = std::ceil(position.x);
        velocity.x = 0;
    }
    // right side
    else if (collides({right, bottom}) && collides({right, top})) {
        position.x = std::floor(position.x);
        velocity.x = 0;
    }
    // test corners next, move back in smaller axis
    if (collides({left, bottom})) {
        float dx = std::ceil(position.x) - position.x;
        float dy = std::ceil(position.y) - position.y;
        if (dx > dy) {
            position.y = std::ceil(position.y);
            velocity.y = 0;
        } else {
            position.x = std::ceil(position.x);
            velocity.x = 0;
        }
    } else if (collides({left, top})) {
        float dx = std::ceil(position.x) - position.x;
        float dy = position.y - std::floor(position.y);
        if (dx > dy) {
            position.y = std::floor(position.y);
            velocity.y = 0;
        } else {
            position.x = std::ceil(position.x);
            velocity.x = 0;
        }
    } else if (collides({right, bottom})) {
        float dx = position.x - std::floor(position.x);
        float dy = std::ceil(position.y) - position.y;
        if (dx > dy) {
            position.y = std::ceil(position.y);
            velocity.y = 0;
        } else {
            position.x = std::floor(position.x);
            velocity.x = 0;
        }
    } else if (collides({right, top})) {
        float dx = position.x - std::floor(position.x);
        float dy = position.y - std::floor(position.y);
        if (dx > dy) {
            position.y = std::floor(position.y);
            velocity.y = 0;
        } else {
            position.x = std::floor(position.x);
            velocity.x = 0;
        }
    }
}

void World::update(double dt) {
    sheep_spawn_elapsed += dt;
    for (int i = 0; i < game_objects.size(); ++i) {
        auto& obj = game_objects.at(i);
        obj->update(*this, dt);
        update_object(obj, dt);
        touch_tiles(*obj);
    }

    for (auto obj: projectiles) {
        obj->update(*this, dt);
        update_object(obj, dt);
    }
    //check player collision
    build_quadtree();
    std::vector<GameObject*> collides_with = quad_tree.query_range(player->get_bounding_box());
    for (auto& obj: collides_with) {
        if (obj == player) continue;
        if (obj->obj_name == "sheep") {
            player->sheep_count++;
            obj->is_alive = false;
        } else {
            player->take_damage(obj->damage);
        }
    }
    //check for collision with projectile and enemy
    for (auto& projectile: projectiles) {
        std::vector<GameObject*> p_collides_with = quad_tree.query_range(projectile->get_bounding_box());
        for (auto& obj: p_collides_with) {
            if (obj == player) continue;
            obj->take_damage(projectile->damage);
            projectile->elapsed += projectile->lifetime + 10;
        }
    }

    //check for player death
    if (!player->is_alive) {
        end_game = true;
        return;
    }

    auto itr = std::stable_partition(game_objects.begin(), game_objects.end(),
                                     [](GameObject* obj) { return obj->is_alive; });
    std::for_each(itr, game_objects.end(), [](GameObject* p) { delete p; });
    game_objects.erase(itr, game_objects.end());

    //check for old projectiles
    auto p_itr = std::stable_partition(projectiles.begin(), projectiles.end(),
                                       [](Projectile* obj) { return obj->elapsed <= obj->lifetime; });
    std::for_each(p_itr, projectiles.end(), [](Projectile* p) { delete p; });
    projectiles.erase(p_itr, projectiles.end());


}

void World::update_object(GameObject* obj, double dt) {
    //currently updating player
    auto position = obj->physics.position;
    auto velocity = obj->physics.velocity;
    auto acceleration = obj->physics.acceleration;

    velocity += 0.5f * acceleration * static_cast<float>(dt);
    position += velocity * static_cast<float>(dt);
    velocity += 0.5f * acceleration * static_cast<float>(dt);
    velocity.x *= obj->physics.damping;
    velocity.y *= obj->physics.damping;

    velocity.x = std::clamp(velocity.x, -obj->physics.terminal_velocity, obj->physics.terminal_velocity);
    velocity.y = std::clamp(velocity.y, -obj->physics.terminal_velocity, obj->physics.terminal_velocity);

    //check for collisions
    Vec<float> future_position{position.x, obj->physics.position.y};
    Vec<float> future_veloctiy{velocity.x, 0};
    move_to(future_position, obj->size, future_veloctiy);

    // now y direction after (maybe) moving in x
    future_veloctiy.y = velocity.y;
    future_position.y = position.y;
    move_to(future_position, obj->size, future_veloctiy);
    // update the player position and velocity
    obj->physics.position = future_position;
    obj->physics.velocity = future_veloctiy;
}


void World::load_level(const Level& level) {
    for (const auto& [pos, tile_id]: level.tile_locations) {
        tilemap(pos.x, pos.y) = level.tile_types.at(tile_id);
    }
    audio->load_sounds(level.sounds);

    //get all enemies
    for (const auto& [pos, enemy_name]: level.enemy_locations) {
        auto enemy = new GameObject{enemy_name, nullptr, nullptr, {255, 255, 0, 255}};
        enemy->physics.position = pos;

        if (enemy_name == "sheep") {
            sheeps.push_back(dynamic_cast<Sheep*>(enemy));
        }
        game_objects.push_back(enemy);
    }
    game_objects.push_back(player);
}

void World::touch_tiles(GameObject& obj) {
    float epsilon = 0.001f;


    const std::vector<Vec<float> > tiles{
        {obj.physics.position.x - epsilon, obj.physics.position.y},
        {obj.physics.position.x, obj.physics.position.y + obj.size.y + epsilon},
        {obj.physics.position.x + obj.size.x + epsilon, obj.physics.position.y},
        {obj.physics.position.x, obj.physics.position.y - epsilon}
    };

    for (const auto& p: tiles) {
        if (&obj != player) continue;
        int x = static_cast<int>(std::floor(p.x));
        int y = static_cast<int>(std::floor(p.y));
        Tile& tile = tilemap(x, y);
        if (!tile.event_name.empty()) {
            auto itr = events.find(tile.event_name);
            if (itr == events.end()) {
                throw std::runtime_error("Cannot find event: " + tile.event_name);
            }
            itr->second->perform(*this, obj);
        }
    }
}

void World::build_quadtree() {
    quad_tree.clear();

    for (auto obj: game_objects) {
        quad_tree.insert(obj);
    }
}
