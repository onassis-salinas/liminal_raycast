#ifndef GAME_H
#define GAME_H

#define mapWidth 32
#define mapHeight 70
#define textureSize 64

#include <SDL3/SDL.h>
#include <stdbool.h>

extern const float PI;
extern const float deg;
extern const int tileSize;
extern int rays;
extern float px, py, pa;
extern int portal9X, portal9Y;
extern int portal8X, portal8Y;
extern int windowWidth, windowHeight;
extern float dt;
extern int map[mapHeight][mapWidth];
extern int wallTextureColors[textureSize][textureSize];
extern void loadTextures(SDL_Renderer *renderer);
extern SDL_Texture *wallTexture;

int getMapTile(int x, int y);
bool isWall(int tile);
void initPortalPositions(void);
void renderWalls(SDL_Renderer *renderer);
void renderGame(SDL_Renderer *renderer);

#endif
