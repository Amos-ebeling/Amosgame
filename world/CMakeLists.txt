project(world)

add_library(world
        world.cpp world.h
        tilemap.cpp tilemap.h
        camera.h camera.cpp
        action.cpp action.h
        game_object.cpp game_object.h
        fsm.cpp fsm.h
        states.cpp states.h
        level.cpp
        keyboard_input.cpp keyboard_input.h
        input.cpp input.h
        audio.cpp action.h
        events.h events.cpp
        quadtree.cpp quadtree.h
        projectile.cpp projectile.h
        ai_input.h ai_input.cpp
        sheep.h sheep.cpp)


target_include_directories(world PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(world PUBLIC SDL3::SDL3 SDL3_mixer::SDL3_mixer utilities graphics)