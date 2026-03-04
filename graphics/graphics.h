#pragma once
#include <string>
#include <SDL3/SDL.h>
#include "vec.h"
#include "sprite.h"

class Color {
public:
    int red{178}, green{0}, blue{255}, alpha{255};

};
class Graphics {
public:
    Graphics(const std::string& title, int window_width, int window_height);
    void clear();
    void update();

    void draw(const SDL_FRect& rect, const Color& color, bool filled = true);
    void draw_sprite(const Vec<float>& pixel, const Sprite& Sprite);

    int get_texture_id(const std::string& image_filename);

    const int width, height;

private:
    std::string title;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SDL_Texture*> textures;
    std::unordered_map<std::string, int> texture_ids;
};


