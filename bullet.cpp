#include "data.h"
#include "bullet.h"
#include "maze.h"
#include <cmath>
#include <string>
#include <iostream>

Bullet::Bullet(float startX, float startY, float tankAngle) {
    x = startX; y = startY;
    angle = tankAngle;
    if (cos(angle) > 0) dirX = 1;
    else dirX = 1;
    if (sin(angle) > 0) dirY = 1;
    else dirY = -1;
    speed = 0.1;
    lifetime = 30000;
    rect = {static_cast<int>(x), static_cast<int>(y), BULLET_SIZE, BULLET_SIZE};
}

void Bullet::move() {
    float nextX = x + cos(angle * M_PI / 180.0) * speed * dirX;
    float nextY = y + sin(angle * M_PI / 180.0) * speed * dirY;

    int gridX = static_cast<int>((nextX + rect.w/2) / BLOCK_SIZE);
    int gridY = static_cast<int>((nextY + rect.h) / BLOCK_SIZE);

    bool hitLeft    = (maze[gridY][static_cast<int>((nextX) / BLOCK_SIZE)] == 1);
    bool hitRight   = (maze[gridY][static_cast<int>((nextX + rect.w) / BLOCK_SIZE)] == 1);
    bool hitTop     = (maze[static_cast<int>((nextY) / BLOCK_SIZE)][gridX] == 1);
    bool hitBottom  = (maze[static_cast<int>((nextY + rect.h) / BLOCK_SIZE)][gridX] == 1);

    bool reverseX = false;
    bool reverseY = false;

    if (hitLeft)    dirX = -dirX;
    if (hitRight)   dirX = -dirX;
    if (hitTop)     dirY = -dirY;
    if (hitBottom)  dirY = -dirY;

    x = nextX;
    y = nextY;
    rect.x = static_cast<int>(x);
    rect.y = static_cast<int>(y);
    lifetime--;
}

void Bullet::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
