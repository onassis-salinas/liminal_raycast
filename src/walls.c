#include "game.h"
#include <SDL3/SDL_render.h>
#include <math.h>
#include <stdio.h>

void renderWalls(SDL_Renderer *renderer) {
  for (int i = -(rays / 2); i < (rays / 2); i++) {
    float angle = pa + (i * (deg / 10));

    if (angle < 0)
      angle += 2 * PI;
    if (angle > 2 * PI)
      angle -= +2 * PI;

    float x1 = px, y1 = py;
    float x2 = px, y2 = py;
    float prevX, prevY;

    int color1 = 0, color2 = 0;

    float size1 = 0;
    float size2 = 0;

    for (int j = 0; j < 25; j++) {
      prevX = x1;
      prevY = y1;

      if (angle > PI / 2 && angle < 1.5 * PI) {
        x1 = ((int)(x1 / tileSize)) * tileSize - 0.0003f;
      } else {
        x1 = ((int)(x1 / tileSize)) * tileSize + tileSize;
      }
      y1 = (tan(angle) * (x1 - prevX)) + prevY;
      int tile = getMapTile((int)(x1 / tileSize), (int)(y1 / tileSize));

      size1 += sqrt(pow(x1 - prevX, 2) + pow(y1 - prevY, 2)) * cos(angle - pa);

      if (isWall(tile)) {
        color1 = tile;
        break;
      } else if (tile == 8) {
        float offset = y1 - (int)(y1 / tileSize) * tileSize;
        float newX = (portal9X * tileSize) + tileSize;
        float newY = (portal9Y * tileSize) + offset;

        x1 = newX, y1 = newY;
        x2 = newX, y2 = newY;

        size2 = size1;
      } else if (tile == 9) {
        float offset = y1 - (int)(y1 / tileSize) * tileSize;
        float newX = (portal8X * tileSize);
        float newY = (portal8Y * tileSize) + offset;

        x1 = newX, y1 = newY;
        x2 = newX, y2 = newY;

        size2 = size1;
      } else if (j == 14) {
        color1 = 99;
      }
    }

    float rayTan = tanf(angle);
    if (fabsf(rayTan) > 0.0001f) {
      for (int j = 0; j < 25; j++) {
        prevX = x2;
        prevY = y2;

        if (angle > PI && angle < 2 * PI) {
          y2 = ((int)(y2 / tileSize)) * tileSize - 0.0003f;
        } else {
          y2 = ((int)(y2 / tileSize)) * tileSize + tileSize;
        }
        x2 = (y2 - prevY) / rayTan + prevX;

        int tileX = (int)(x2 / tileSize), tileY = (int)(y2 / tileSize);
        int tile = getMapTile(tileX, tileY);
        int tileUp = getMapTile(tileX, tileY - 1);
        int tileDown = getMapTile(tileX, tileY + 1);

        size2 +=
            sqrt(pow(x2 - prevX, 2) + pow(y2 - prevY, 2)) * cos(angle - pa);

        if (isWall(tile)) {
          color2 = tile;
          break;
        } else if (j == 14) {
          color2 = 99;
        }
      }
    } else {
      size2 = INFINITY;
    }

    float wallHeight;
    float collisionX, collisionY;
    float shade;
    float distance = fmin(size1, size2);
    int texX;

    if (size1 < size2) {
      collisionX = x1, collisionY = y1;

      texX = fmod(collisionY, tileSize) * ((float)textureSize / tileSize);
    } else {
      collisionX = x2, collisionY = y2;
      texX = fmod(collisionX, tileSize) * ((float)textureSize / tileSize);
    }

    shade = 1 - fmin(distance / 250, 1.0f);
    wallHeight = windowHeight / (fmax(distance, 0.01f) / 30);

    float width = (float)windowWidth / rays;
    float positionX = (i + (rays / 2.0)) * width;
    float positionY = (windowHeight - wallHeight) / 2;

    SDL_FRect destination = {positionX, positionY, width, wallHeight};
    SDL_FRect source = {texX, 0, 1, 64};
    SDL_SetTextureColorModFloat(wallTexture, shade, shade, shade);
    SDL_RenderTexture(renderer, wallTexture, &source, &destination);
  }
}

void renderFloorAndCeiling(SDL_Renderer *renderer) {
  for (int y = 0; y < windowHeight; y++) {
    float dirX = cosf(pa);
    float dirY = sinf(pa);

    float halfFov = (rays / 2.0f) * (deg / 10.0f);
    float planeScale = tanf(halfFov);
    float planeX = -dirY * planeScale;
    float planeY = dirX * planeScale;

    float rayDirX0 = dirX - planeX;
    float rayDirY0 = dirY - planeY;
    float rayDirX1 = dirX + planeX;
    float rayDirY1 = dirY + planeY;

    int p;
    if (y > windowHeight / 2) {
      p = y - windowHeight / 2;
    } else {
      p = windowHeight / 2 - y;
    }

    float posZ = 0.5 * windowHeight;
    float rowDistance = 30.0f * windowHeight / (2.0f * p * tileSize);

    float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / windowWidth;
    float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / windowWidth;

    float floorX = (px / tileSize) + rowDistance * rayDirX0;
    float floorY = (py / tileSize) + rowDistance * rayDirY0;

    float shade = 1.0f - fminf(rowDistance / 12.0f, 1.0f);
    if (shade <= 0.0f)
      continue;

    for (int x = 0; x < windowWidth; ++x) {
      int cellX = (int)(floorX);
      int cellY = (int)(floorY);

      int tx = (int)(textureSize * (floorX - cellX)) & (textureSize - 1);
      int ty = (int)(textureSize * (floorY - cellY)) & (textureSize - 1);

      floorX += floorStepX;
      floorY += floorStepY;

      int floorTexture = 3;
      int ceilingTexture = 6;

      int r, g, b;

      if (y > windowHeight / 2) {
        int color = floorTextureColors[ty][tx];
        if (color == 1) {
          r = 148 * shade, g = 138 * shade, b = 78 * shade;
        } else if (color == 2) {
          r = 132 * shade, g = 122 * shade, b = 66 * shade;
        } else if (color == 3) {
          r = 164 * shade, g = 152 * shade, b = 86 * shade;
        }
      } else {
        int color = ceilingTextureColors[ty][tx];
        if (color == 1) {
          r = 209 * shade, g = 208 * shade, b = 114 * shade;
        } else if (color == 2) {
          r = 192 * shade, g = 191 * shade, b = 107 * shade;
        }
      }

      SDL_SetRenderDrawColor(renderer, r, g, b, 255);
      SDL_RenderPoint(renderer, x, y);
    }
  }
}