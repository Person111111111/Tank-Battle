#ifndef TANK_H
#define TANK_H
#include <SDL.h>
#include <SDL_image.h>

struct Tank {
    float x, y, speed, angle;
    int cooldown;
    bool alive;
    SDL_Rect rect;
    SDL_Color color;
    SDL_Texture* texture;

    Tank(int startX, int startY, SDL_Color c, SDL_Renderer* renderer);
    ~Tank();
    void move(float forward);
    void rotate(float direction);
    void updateCooldown();
    void draw(SDL_Renderer* renderer);
};

#endif
