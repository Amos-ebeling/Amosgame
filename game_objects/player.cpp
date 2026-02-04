#include "player.h"

Player::Player(const Vec<float>& position, const Vec<float>& size)
    : position{position}, size{size}, velocity{0,0}{

}



void Player::handle_input() {
    const bool *key_states = SDL_GetKeyboardState(NULL);

    velocity.x = velocity.y = 14;
    if (key_states[SDL_SCANCODE_W]) {
        velocity.y += -32;
    }
    if (key_states[SDL_SCANCODE_S]) {
        velocity.y += 32;
    }
    if (key_states[SDL_SCANCODE_A]) {
        velocity.x += -32;
    }
    if (key_states[SDL_SCANCODE_D]) {
        velocity.x += 32;
    }
}

void Player::update() {}

std::pair<SDL_FRect, Color> Player::get_sprite() const {
    return {{position.x, position.y, size.x, size.y}, {0, 0, 255, 255}};
}


