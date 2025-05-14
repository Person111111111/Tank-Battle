#include "maze.h"
#include "data.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>

int maze[MAZE_HEIGHT][MAZE_WIDTH];

void generateMaze() {
    srand(time(NULL));

    //Đặt toàn bộ là tường
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            maze[y][x] = 1;  //Khởi tạo toàn bộ là tường
        }
    }

    // Chọn điểm xuất phát
    int startX = 1, startY = 1;
    maze[startY][startX] = 0;

    //Dùng thuật toán tạo mê cung hợp lý (Prim hoặc DFS)
    std::vector<std::pair<int, int>> stack;
    stack.push_back({startY, startX});

    while (!stack.empty()) {
        auto [y, x] = stack.back();
        stack.pop_back();

        int dirs[4][2] = {{0, -2}, {-2, 0}, {0, 2}, {2, 0}};
        std::random_shuffle(std::begin(dirs), std::end(dirs)); //Ngẫu nhiên hóa hướng đi

        for (auto& dir : dirs) {
            int newY = y + dir[0];
            int newX = x + dir[1];

            if (newX > 0 && newX < MAZE_WIDTH - 1 && newY > 0 && newY < MAZE_HEIGHT - 1 && maze[newY][newX] == 1) {
                maze[newY][newX] = 0;
                maze[y + dir[0] / 2][x + dir[1] / 2] = 0; //Đảm bảo tường liền mạch
                stack.push_back({newY, newX});
            }
        }
    }
}

bool isReachable(int startX, int startY) {
    std::vector<std::vector<bool>> visited(MAZE_HEIGHT, std::vector<bool>(MAZE_WIDTH, false));

    std::queue<std::pair<int, int>> q;
    q.push({startY, startX});
    visited[startY][startX] = true;

    while (!q.empty()) {
        auto [y, x] = q.front();
        q.pop();

        int dirs[4][2] = {{0, -2}, {-2, 0}, {0, 2}, {2, 0}};  //Di chuyển theo đường rộng hơn
        for (auto& dir : dirs) {
            int newY = y + dir[0];
            int newX = x + dir[1];

            if (newX > 0 && newX < MAZE_WIDTH && newY > 0 && newY < MAZE_HEIGHT &&
                !visited[newY][newX] && maze[newY][newX] == 0) {
                q.push({newY, newX});
                visited[newY][newX] = true;
            }
        }
    }

    // Kiểm tra xem có vùng bị cô lập hay không
    for (int y = 0; y < MAZE_HEIGHT; y++)
        for (int x = 0; x < MAZE_WIDTH; x++)
            if (maze[y][x] == 0 && !visited[y][x]) return false; // Có ô bị cô lập

    return true;
}
void fixMazeConnectivity() {
    if (!isReachable(1, 1))
        for (int y = 1; y < MAZE_HEIGHT - 1; y++)
            for (int x = 1; x < MAZE_WIDTH - 1; x++)
                if (maze[y][x] == 1) {
                    maze[y][x] = 0;
                    if (isReachable(1, 1)) return;
                    maze[y][x] = 1;
                }
}

void drawMaze(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    for (int y = 0; y < MAZE_HEIGHT; y++)
        for (int x = 0; x < MAZE_WIDTH; x++)
            if (maze[y][x] == 1) {
                SDL_Rect wallRect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
                SDL_RenderFillRect(renderer, &wallRect);
            }
}
