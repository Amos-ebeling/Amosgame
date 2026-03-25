#include "world.h"
#include <SDL3/SDL_rect.h>
#include <algorithm>
#include "keyboard_input.h"
#include "states.h"
#include "fsm.h"
#include "game_object.h"
#include "level.h"
#include "vec.h"
#include "physics.h"

World::World(const Level& level)
    :tilemap(level.width, level.height){
    load_level(level);
}

void World::add_platform(float x, float y, float width, float height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            tilemap(x+j, y+i) = Tile{};
        }
    }
}

bool World::collides(const Vec<float>& position) const {
    int x = std::floor(position.x);
    int y = std::floor(position.y);
    return tilemap(x,y).blocking;
}

GameObject* World::create_player(const Level& level) {
    // Create FSM
    Transitions transitions = {
        {{StateType::Standing, Transition::Move}, StateType::Running},
        {{StateType::Running, Transition::Stop}, StateType::Standing},
        {{StateType::Running, Transition::Move}, StateType::Running},

    };
    States states = {
        {StateType::Standing, new Standing()},
        {StateType::Running, new Running()}
    };
    FSM* fsm = new FSM{transitions, states, StateType::Standing};

    //player input
    Keyboard_Input* input = new Keyboard_Input();

    player = new GameObject({1,1}, *this, fsm, input, Color {160, 0, 255, 255});
    return player;
}

void World::move_to(Vec<float>& position, const Vec<int>& size, Vec<float>& velocity) {
    // test sides first. if both collide move backward
    // bottom side
    if (collides(position) && collides({position.x + size.x, position.y})) {
        position.y = std::ceil(position.y);
        velocity.y = 0;
    }
    // top side
    else if (collides({position.x, position.y + size.y}) && collides({position.x + size.x, position.y + size.y})) {
        position.y = std::floor(position.y);
        velocity.y = 0;
    }
    // left side
    if (collides(position) && collides({position.x, position.y + size.y})) {
        position.x = std::ceil(position.x);
        velocity.x = 0;
    }
    // right side
    else if (collides({position.x + size.x, position.y}) && collides({position.x + size.x, position.y + size.y})) {
        position.x = std::floor(position.x);
        velocity.x = 0;
    }

    //mine
    auto TL = Vec<float>{position.x, position.y + size.y};
    auto TR = Vec<float>{position.x + size.x, position.y + size.y};
    auto BL = position;
    auto BR = Vec<float>{position.x  + size.x, position.y};

    if (collides(TL)) {
        float dx = (std::ceil(TL.x) - TL.x);
        float dy = (TL.y - std::floor(TL.y));
        if (dx < dy){
            position.x = std::ceil(position.x);
            velocity.x = 0;
        }
        else {
            position.y = std::floor(position.y);
            velocity.y = 0;
        }
    }
    if (collides(TR)) {
        float dx = (TR.x - std::floor(TR.x));
        float dy = (TR.y - std::floor(TR.y));
        if (dx < dy){
            position.x = std::floor(position.x);
            velocity.x = 0;
        }
        else {
            position.y = std::floor(position.y);
            velocity.y = 0;
        }
    }
    if (collides(BL)) {
        float dx = (std::ceil(BL.x) - BL.x);
        float dy = (std::ceil(BL.y) - BL.y);
        if (dx < dy){
            position.x = std::ceil(position.x);
            velocity.x = 0;
        }
        else {
            position.y = std::ceil(position.y);
            velocity.y = 0;
        }
    }
    if (collides(BR)) {
        float dx = (BR.x - std::floor(BR.x));
        float dy = (std::ceil(BR.y) - BR.y);
        if (dx < dy){
            position.x = std::floor(position.x);
            velocity.x = 0;
        }
        else {
            position.y = std::ceil(position.y);
            velocity.y = 0;
        }
    }
}

void World::update(float dt) {
    //currently updating player
    auto position = player->physics.position;
    auto velocity = player->physics.velocity;
    auto acceleration = player->physics.acceleration;

    velocity += 0.5f * acceleration * dt;
    position += velocity * dt;
    velocity += 0.5f * acceleration * dt;
    velocity.x *= player->physics.damping;
    velocity.y *= player->physics.damping;

    velocity.x = std::clamp(velocity.x, -player->physics.terminal_velocity, player->physics.terminal_velocity);
    velocity.y = std::clamp(velocity.y, -player->physics.terminal_velocity, player->physics.terminal_velocity);

    //check for collisions
    Vec<float> future_position{position.x, player->physics.position.y};
    Vec<float> future_veloctiy{velocity.x, 0};
    move_to(future_position, player->size, future_veloctiy);

    // now y direction after (maybe) moving in x
    future_veloctiy.y = velocity.y;
    future_position.y = position.y;
    move_to(future_position, player->size, future_veloctiy);
    // update the player position and velocity
    player->physics.position = future_position;
    player->physics.velocity = future_veloctiy;
}

void World::load_level(const Level& level) {
    for (const auto& [pos, tile_id] : level.tile_locations) {
        tilemap(pos.x, pos.y) = level.tile_types.at(tile_id);
    }
}