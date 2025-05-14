#ifndef BULLET_H
#define BULLET_H
#include <SDL.h>
#include <SDL_image.h>

struct Bullet {
    float x, y, angle, speed;
    int lifetime;
    int dirX, dirY;
    SDL_Rect rect;
    SDL_Texture* texture;
    Bullet(float startX, float startY, float tankAngle);
    void move();
    void draw(SDL_Renderer* renderer);
};

#endif
