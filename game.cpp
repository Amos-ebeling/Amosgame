#include "game.h"
#include "fsm.h"
#include "asset_manager.h"
#include "input.h"
#include "keyboard_input.h"
#include "states.h"

Game::Game(std::string title, int width, int height)
    :graphics{title, width, height},  camera{graphics, 64}, dt{1.0/60.0}, lag{0.0}, performance_frequency({SDL_GetPerformanceFrequency()}), prev_counter{SDL_GetPerformanceCounter()} {
    //load events
    get_events();

    // load the first level
    Level level{"level_1"};
    AssetManager::get_level_details(graphics, level);

    //give player
    create_player();
    AssetManager::get_game_object_details("player", graphics, *player);

    // create the world for the first level
    world = new World(level, audio, player.get(), events);

    // Give player its assets then put it in the correct state
    player = std::unique_ptr<GameObject>(world->create_player(level));
    AssetManager::get_game_object_details("player", graphics, *player);
    player->fsm->current_state->on_enter(*world, *player);

    // use the spawn location's position
    player->physics.position = {
        static_cast<float>(level.player_spawn_location.x),
        static_cast<float>(level.player_spawn_location.y)
    };

    player->fsm->current_state->on_enter(*world, *player);

    camera.set_location(player->physics.position);
    audio.play_sounds("background", true);
}

Game::~Game() {
    delete world;
    for (auto [_,event]: events) {
        delete event;
    }
}

void Game::handle_event(SDL_Event* event) {
    player->input->collect_discrete_event(event);
}

void Game::input() {
    camera.handle_input();
    player->input->get_input();
}

void Game::update() {
    Uint64 now = SDL_GetPerformanceCounter();
    lag += (now - prev_counter) / (float)performance_frequency;
    prev_counter = now;
    while (lag >= dt) {
        player->input->handle_input(*world, *player);
        player->update(*world, dt);
        world->update(dt);
        //[ut cam ahead of player
        float L = length(player->physics.velocity);
        Vec displacement = 8.0f * player->physics.velocity/(1.0f+L);
        camera.update(player->physics.position+displacement, dt);
        lag -= dt;
        if (world->end_level) {
            load_level();
        }
    }
}

void Game::render() {
    graphics.clear();

    //draw world
    camera.render(world->tilemap);

    //draw player
    camera.render(*player);


    graphics.update();
}

void Game::get_events() {
    events["next_level"] = new NextLevel();
}


void Game::load_level() {
    std::string level_name = "level_" + std::to_string(++current_level);
    Level level{level_name};
    AssetManager::get_level_details(graphics, level);

    //create the world
    delete world;
    world = new World(level, audio, player.get(), events);

    player->physics.position = {static_cast<float>(level.player_spawn_location.x), static_cast<float>(level.player_spawn_location.y)};
    camera.set_location(player->physics.position);
    audio.play_sounds("background", true);
}

void Game::create_player() {
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

    player = std::make_unique<GameObject>(Vec<int>{1,1}, fsm, input, Color {160, 0, 255, 255});
}

