#include "game.h"
#include <SDL3/SDL_keyboard.h>
#include <math.h>

const float PI = 3.14159265;
const float deg = 0.0174533;
const float FOV = 1.3962634;
const int tileSize = 20;
int rays = 380;
int rows = 180;
float baseSpeed = 80;
float baseRotationSpeed = 2;

float px = tileSize * 15.5, py = tileSize * 2.5, pa = 0;

int portal52X, portal52Y, portal51X, portal51Y;

int getMapTile(int x, int y) {

  if (x >= mapWidth) {
    x = mapWidth - 1;
  }
  if (x < 0) {
    x = 0;
  }
  if (y >= mapHeight) {
    y = mapHeight - 1;
  }
  if (y < 0) {
    y = 0;
  }

  return map[y][x];
}

void initPortalPositions() {
  for (int y = 0; y < mapHeight; y++) {
    for (int x = 0; x < mapWidth; x++) {
      if (getMapTile(x, y) == 52) {
        portal52X = x;
        portal52Y = y;
      }
      if (getMapTile(x, y) == 51) {
        portal51X = x;
        portal51Y = y;
      }
    }
  }
}

bool isWall(int tile) { return tile == 1 || tile == 2; }

void calculatePlayerCollisions(float nextX, float nextY) {
  int nextXTile = getMapTile((int)(nextX / tileSize), (int)(py / tileSize));
  int nextYTile = getMapTile((int)(px / tileSize), (int)(nextY / tileSize));
  int nextTile = getMapTile((int)(nextX / tileSize), (int)(nextY / tileSize));

  if (!isWall(nextXTile)) {
    px = nextX;
  }

  if (!isWall(nextYTile)) {
    py = nextY;
  }

  if (nextTile == 52) {
    px = (portal51X * tileSize + tileSize) + fmod(px, tileSize);
    py = (portal51Y * tileSize) + fmod(py, tileSize);
  } else if (nextTile == 51) {
    px = (portal52X * tileSize - tileSize) + fmod(px, tileSize);
    py = (portal52Y * tileSize) + fmod(py, tileSize);
  }
}

void movePlayer() {
  const bool *keyStates = SDL_GetKeyboardState(NULL);
  float speed = baseSpeed * dt;
  float rotationSpeed = baseRotationSpeed * dt;

  // TODO: Make speed normal if going sideways and forward
  if (keyStates[SDL_SCANCODE_W]) {
    float nextX = px + cos(pa) * speed;
    float nextY = py + sin(pa) * speed;
    calculatePlayerCollisions(nextX, nextY);
  }
  if (keyStates[SDL_SCANCODE_S]) {
    float nextX = px + cos(pa) * -speed;
    float nextY = py + sin(pa) * -speed;
    calculatePlayerCollisions(nextX, nextY);
  }
  if (keyStates[SDL_SCANCODE_A]) {
    float virtualAngle = pa + 90 * deg;
    float nextX = px + cos(virtualAngle) * -speed;
    float nextY = py + sin(virtualAngle) * -speed;
    calculatePlayerCollisions(nextX, nextY);
  }
  if (keyStates[SDL_SCANCODE_D]) {
    float virtualAngle = pa - 90 * deg;
    float nextX = px + cos(virtualAngle) * -speed;
    float nextY = py + sin(virtualAngle) * -speed;
    calculatePlayerCollisions(nextX, nextY);
  }
  if (keyStates[SDL_SCANCODE_LEFT]) {
    pa -= rotationSpeed;
  }
  if (keyStates[SDL_SCANCODE_RIGHT]) {
    pa += rotationSpeed;
  }

  if (pa < 0)
    pa += 2 * PI;
  if (pa > 2 * PI)
    pa -= +2 * PI;
}

void renderGame(SDL_Renderer *renderer) {
  movePlayer();
  renderFloorAndCeiling(renderer);
  renderWalls(renderer);
}