#ifndef DRAW_H
#define DRAW_H

#include "level.h"
#include "state.h"
#include <raylib.h>

typedef struct {
    Texture2D tex;
}texture;

// Generates a new texture to use
texture *draw_texture_new(char *path);

// Draws the cells of the level in the screen with certain textures provided in the arguments
void draw_level(const level *lvl, texture *floor, texture *wall);

// Draws the whole state and the level in the screen
void draw_state(const level *lvl, const state *sta, texture *floor, texture *wall);

// Frees the memory and texture used
void draw_free(texture *tex);

#endif
