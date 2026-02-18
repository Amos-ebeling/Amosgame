#include "world.h"
#include <SDL3/SDL_rect.h>
#include <algorithm>

#include "game_object.h"
#include "vec.h"
#include "physics.h"

World::World(int width, int height)
    :tilemap{width, height}{}

void World::add_platform(float x, float y, float width, float height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            tilemap(x+j, y+i) = Tile::Platform;
        }
    }
}

bool World::collides(const Vec<float>& position) const {
    int x = std::floor(position.x);
    int y = std::floor(position.y);
    return tilemap(x,y) == Tile::Platform;
}

GameObject* World::create_player() {
    player = std::make_unique<GameObject>(Vec<float> {10, 5}, Vec<int>{1, 1}, *this);
    return player.get();
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
