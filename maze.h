#ifndef MAZE_H
#define MAZE_H

#include <SDL.h>

const int MAZE_WIDTH = 11 ;
const int MAZE_HEIGHT = 11;
extern int maze[MAZE_HEIGHT][MAZE_WIDTH];

void generateMaze();
void drawMaze(SDL_Renderer* renderer);

#endif
