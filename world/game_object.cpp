#include "game_object.h"
#include "fsm.h"
#include "action.h"

GameObject::GameObject(const Vec<float>& position, const Vec<int>& size, World& world, FSM* fsm, Color color)
    :physics{position, {0,0}, {0,0}}, size{size}, fsm{fsm}, color{color}{
    physics.acceleration.y = physics.gravity;
    fsm->current_state->on_enter(world, *this);
}

GameObject::~GameObject() {
    delete fsm;
}

void GameObject::input(World& world) {
    const bool *key_states = SDL_GetKeyboardState(NULL);

    // if (key_states[SDL_SCANCODE_W]) {
    //    velocity.y += -32;
    // }
    // if (key_states[SDL_SCANCODE_S]) {
    //     velocity.y += 32;
    // }
    // if (key_states[SDL_SCANCODE_A]) {
    //     // velocity.x += -32;
    //     physics.acceleration.x += -physics.walk_acceleration;
    // }
    // if (key_states[SDL_SCANCODE_D]) {
    //     //velocity.x += 32;
    //     physics.acceleration.x += physics.walk_acceleration;
    // }
    ActionType action_type = ActionType::None;
    if (key_states[SDL_SCANCODE_SPACE]) {
        action_type = ActionType::Jump;
    }
    Action* action = fsm->current_state->input(world, *this, action_type);
    if (action != nullptr) {
        action->perform(world, *this);
        delete action;
    }
}

void GameObject::update(World& world, double dt) {

}

std::pair<Vec<float>, Color> GameObject::get_sprite() const {
    return {physics.position, color};
}
