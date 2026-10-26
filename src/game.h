#ifndef GAME_H
#define GAME_H

#define mapWidth 32
#define mapHeight 70
#define textureSize 64

#include <SDL3/SDL.h>
#include <stdbool.h>

extern const float PI;
extern const float deg;
extern const float FOV;
extern const int tileSize;
extern int rays;
extern int rows;
extern float px, py, pa;
extern int portal51X, portal51Y;
extern int portal52X, portal52Y;
extern int windowWidth, windowHeight;
extern float dt;
extern int map[mapHeight][mapWidth];
extern int wallTextureColors[textureSize][textureSize];
extern int ceilingTextureColors[textureSize][textureSize];
extern int floorTextureColors[textureSize][textureSize];
extern void loadTextures(SDL_Renderer *renderer);
extern SDL_Texture *wallTexture;

int getMapTile(int x, int y);
bool isWall(int tile);
void initPortalPositions(void);
void renderWalls(SDL_Renderer *renderer);
void renderFloorAndCeiling(SDL_Renderer *renderer);
void renderGame(SDL_Renderer *renderer);

#endif
