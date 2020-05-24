#include "draw.h"
#include <stdlib.h>
#include <assert.h>
#include <raylib.h>

texture *draw_texture_new(char *path){
    // Assign memory
    texture *tex = malloc(sizeof(texture));

    // Load texture with certain path
    tex->tex = LoadTexture(path);

    return tex;
}

void draw_level(const level *lvl, texture *floor, texture *wall){
    // Draw cells, iterate, including borders outside the level
    for(int y=-1;y<=lvl->size_y;y++){
        for(int x=-1;x<=lvl->size_x;x++){
            char cell = level_get(lvl,x,y);
            // If the cell is a wall, paint it
            if(cell=='#'){
                // We paint the walls with the texture preloaded
                DrawTexture(wall->tex, TILE_SIZE*x, TILE_SIZE*y,WHITE);
            } else if (cell=='.') {
                // We paint the floor aswell
                DrawTexture(floor->tex, TILE_SIZE*x, TILE_SIZE*y,WHITE);
            }
        }
    }
}

void draw_state(const level *lvl, const state *sta, texture *floor, texture *wall){

    // Initialize a camera to be used as drawing context
    Camera2D cam;
    // make the camera follow the player
    cam.target.x = sta->pla.ent.x;
    cam.target.y = sta->pla.ent.y;
    // make it so that the player is in the middle of the screen
    cam.offset.x = GetScreenWidth()/2.0;
    cam.offset.y = GetScreenHeight()/2.0;
    // set the camera rotation to 0
    cam.rotation = 0.0;
    // set the camera zoom to 1
    cam.zoom = 1.0;

    // Draw everything relative to the camera from now on
    BeginMode2D(cam);

    // Draw level
    draw_level(lvl, floor, wall);

    // Draw enemies
    for(int i=0;i<sta->n_enemies;i++){
        // Get a copy of the enemy entity
        entity ent = sta->enemies[i].ent;
        // Initialize a Vector2 that represents the center of the entity position
        Vector2 vec = {ent.x,ent.y};
        // Draw a circle with the radius of the entity, color depends on the enemy type
        if(sta->enemies[i].kind == MINION){
            DrawCircleV(vec,ent.rad,YELLOW);
        }else{
            DrawCircleV(vec,ent.rad,RED);
        }
    }

    // Draw player
    {
        // Get a copy of the player's entity
        entity ent = sta->pla.ent;
        // Initialize a Vector2 that represents the center of the entity position
        Vector2 vec = {ent.x,ent.y};
        // Draw a circle with the radius of the entity
        DrawCircleV(vec,ent.rad,BLUE);
    }

    // Draw bullets
    for(int i=0;i<sta->n_bullets;i++){
        // Get a copy of the bullet entity
        entity ent = sta->bullets[i].ent;
        // Initialize a Vector2 that represents the center of the entity position
        Vector2 vec = {ent.x,ent.y};
        // Draw a circle with the radius of the entity
        DrawCircleV(vec,ent.rad,PINK);
    }

    // Stop drawing relative to the camera
    EndMode2D();
}

void draw_free(texture *tex){
    // Unloading Textures
    UnloadTexture(tex->tex);
    free(tex);
}