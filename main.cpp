#include <iostream>
#include <vector>
#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include "tank.h"
#include "bullet.h"
#include "data.h"
#include "maze.h"

std::vector<Bullet> bullets;

void shoot(Tank& tank) {
    if (tank.cooldown <= 0) {
        float bulletX = tank.x + cos(tank.angle * M_PI / 180.0) * (tank.rect.w / 2 + 20);
        float bulletY = tank.y + sin(tank.angle * M_PI / 180.0) * (tank.rect.h / 2 + 20);

        int gridX = static_cast<int>(bulletX / 60);
        int gridY = static_cast<int>(bulletY / 60);

        bullets.push_back(Bullet(bulletX, bulletY, tank.angle));
        tank.cooldown = bullets.back().lifetime;
    }
}

std::pair<int, int> generateRandomTankPosition() {
    int randomX = (rand() % (MAZE_WIDTH - 2) + 1) * 60;
    int randomY = (rand() % (MAZE_HEIGHT - 2) + 1) * 60;

    //Kiểm tra xem vị trí có nằm trong tường không
    if (maze[randomY / 60][randomX / 60] == 1) {
        return generateRandomTankPosition();
    }
    return {randomX, randomY}; //Trả về vị trí hợp lệ
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Tank Battle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    srand(time(NULL));
    generateMaze();
    int randomX1, randomY1, randomX2, randomY2;
    // Chỉ sinh xe tăng ở vị trí không có tường
    std::pair<int, int> pos1 = generateRandomTankPosition();
    std::pair<int, int> pos2 = generateRandomTankPosition();

    Tank tank1(pos1.first, pos1.second, {255, 255, 255}, renderer);
    Tank tank2(pos2.first, pos2.second, {255, 255, 0}, renderer);

    bool running = true;
    SDL_Event event;
    const Uint8* keyStates = SDL_GetKeyboardState(NULL);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
                tank1 = Tank(100, 100, {255, 255, 255}, renderer);
                tank2 = Tank(600, 400, {255, 128, 0}, renderer);
                bullets.clear();
            }
        }
        if (!tank1.alive) {
            printf("NO.2 TANK IS WINNER!\n");
            running = false;
        }
        else if (!tank2.alive) {
            printf("No.1 TANK IS WINNER!\n");
            running = false;
        }
        keyStates = SDL_GetKeyboardState(NULL);
        if (tank1.alive) {
            if (keyStates[SDL_SCANCODE_W]) tank1.move(1);
            if (keyStates[SDL_SCANCODE_S]) tank1.move(-1);
            if (keyStates[SDL_SCANCODE_A]) tank1.rotate(-1);
            if (keyStates[SDL_SCANCODE_D]) tank1.rotate(1);
            if (keyStates[SDL_SCANCODE_SPACE]) shoot(tank1);
        }

        if (tank2.alive) {
            if (keyStates[SDL_SCANCODE_UP]) tank2.move(1);
            if (keyStates[SDL_SCANCODE_DOWN]) tank2.move(-1);
            if (keyStates[SDL_SCANCODE_LEFT]) tank2.rotate(-1);
            if (keyStates[SDL_SCANCODE_RIGHT]) tank2.rotate(1);
            if (keyStates[SDL_SCANCODE_RETURN]) shoot(tank2);
        }

        tank1.updateCooldown();
        tank2.updateCooldown();

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        drawMaze(renderer);
        if (tank1.alive) tank1.draw(renderer);
        if (tank2.alive) tank2.draw(renderer);
        //Xử lý va chạm giữa đạn với xe tăng
        for (auto& bullet : bullets) {
            bullet.move();
            bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return b.lifetime <= 0; }), bullets.end());
            bullet.draw(renderer);
            if (tank1.alive && SDL_HasIntersection(&bullet.rect, &tank1.rect)) {
                tank1.alive = false;
                bullet.lifetime = 0;
            }
            if (tank2.alive && SDL_HasIntersection(&bullet.rect, &tank2.rect)) {
                tank2.alive = false;
                bullet.lifetime = 0;
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
