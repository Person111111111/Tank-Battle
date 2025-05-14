#include "maze.h"
#include "data.h"
#include "tank.h"
#include "data.h"
#include <cmath>

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        SDL_Log("Không thể tải ảnh: %s", IMG_GetError());
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

Tank::Tank(int startX, int startY, SDL_Color c, SDL_Renderer* renderer) {
    x = startX; y = startY;
    speed = 0.05 ;
    angle = -90;
    cooldown = 0;    //Không có cooldown ban đầu
    color = c;
    alive = true;
    rect = {static_cast<int>(x), static_cast<int>(y), TANK_SIZE, TANK_SIZE};
    texture = loadTexture("Tank Texture.PNG", renderer);
}

Tank::~Tank() {
    if (texture) SDL_DestroyTexture(texture);
}

void Tank::move(float forward) {
    float newX = x + cos(angle * M_PI / 180.0) * (speed * forward);
    float newY = y + sin(angle * M_PI / 180.0) * (speed * forward);

    int gridX = static_cast<int>(newX / BLOCK_SIZE);
    int gridY = static_cast<int>(newY / BLOCK_SIZE);

    int leftX   = static_cast<int>((newX - rect.w / 2) / BLOCK_SIZE);
    int rightX  = static_cast<int>((newX + rect.w) / BLOCK_SIZE);
    int topY    = static_cast<int>((newY - rect.h / 2) / BLOCK_SIZE);
    int bottomY = static_cast<int>((newY + rect.h) / BLOCK_SIZE);

    bool canMoveX = (maze[topY][leftX] == 0 && maze[topY][rightX] == 0 && maze[bottomY][leftX] == 0 && maze[bottomY][rightX] == 0);
    bool canMoveY = (maze[topY][leftX] == 0 && maze[bottomY][leftX] == 0 && maze[topY][rightX] == 0 && maze[bottomY][rightX] == 0);

    if (maze[gridY][gridX] == 1) {
        if (canMoveX) newX = x;
        if (canMoveY) newY = y;
        return;
    }
    x = newX;
    y = newY;
    rect.x = static_cast<int>(x);
    rect.y = static_cast<int>(y);
}

void Tank::rotate(float direction) {
    angle += direction*0.05 ;
}

void Tank::updateCooldown() {
    if (cooldown > 0) cooldown--;
}

void Tank::draw(SDL_Renderer* renderer) {
    if (!alive) return; //Huỷ xe tăng nếu bị tiêu diệt
    rect.x = static_cast<int>(x - rect.w / 2);
    rect.y = static_cast<int>(y - rect.h / 2);
    int adjustedX = rect.x;
    int adjustedY = rect.y;

    //Ngăn xe tăng đè lên tường
    if (maze[rect.y / 60][rect.x / 60] == 1) {
        adjustedX += 5;
        adjustedY += 5;
    }
    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
    SDL_Point center = {rect.w / 2, rect.h / 2};
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle + 90, &center, SDL_FLIP_NONE);
}
