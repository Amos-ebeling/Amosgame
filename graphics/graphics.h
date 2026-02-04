#pragma once
#include <string>
#include <SDL3/SDL.h>


class Color {
public:
    int red{178}, green{0}, blue{255}, alpha{255};

};
class Graphics {
public:
    Graphics(const std::string& title, int window_width, int window_height);
    void clear();
    void draw(const SDL_FRect& rect, const Color& color);
    void update();

private:
    std::string title;
    int width, height;
    SDL_Window* window;
    SDL_Renderer* renderer;
};


