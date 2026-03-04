//
// Created by zhimo on 2/25/2026.
//
#include "game_object.h"
#include "fsm.h"
#include "keyboard_input.h"


void Keyboard_Input::get_input() {
    const bool *key_states = SDL_GetKeyboardState(NULL);

    if (key_states[SDL_SCANCODE_D]) {
        next_action_type = ActionType::MoveRight;
    }
    else if (key_states[SDL_SCANCODE_A]) {
        next_action_type = ActionType::MoveLeft;
    }
    else if (key_states[SDL_SCANCODE_W]) {
        next_action_type = ActionType::MoveUp;
    }
    else if (key_states[SDL_SCANCODE_S]) {
        next_action_type = ActionType::MoveDown;
    }
}

void Keyboard_Input::handle_input(World& world, GameObject& obj) {
    Action* action = obj.fsm->current_state->input(world, obj, next_action_type);
    next_action_type = ActionType::None;
    if (action != nullptr) {
        action->perform(world, obj);
        delete action;
    }
}

void Keyboard_Input::collect_discrete_event(SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.repeat == 0) {
    }
}
