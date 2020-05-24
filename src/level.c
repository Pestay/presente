#include "level.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

level *level_new(int size_x, int size_y){
    // Ask memory for the level struct
    level *lvl = malloc(sizeof(level));

    // Set the size
    lvl->size_x = size_x;
    lvl->size_y = size_y;
    lvl->cell_size = 10;

    // Ask memory for the cell* array
    lvl->cells  = malloc(lvl->size_y * sizeof(char *));

    // Ask memory for each row
    for(int y=0;y<lvl->size_y;y++){
        lvl->cells[y] = malloc(lvl->size_x * sizeof(char));
    }

    // Initialize level
    for(int y=0;y<lvl->size_y;y++){
        for(int x=0;x<lvl->size_x;x++){
            lvl->cells[y][x] = '.';
        }
    }

    // Sets the central cell to open so that we may have a better generation
    lvl->cells[lvl->size_y/2][lvl->size_x/2] = '.';

    // Retreieve level's pointer
    return lvl;
}

int level_get(const level *lvl, int x, int y){
    // For cells outside the matrix:
    if(x<0) return '#';
    if(y<0) return '#';
    if(x>=lvl->size_x) return '#';
    if(y>=lvl->size_y) return '#';
    // Regular cells
    return lvl->cells[y][x];
}

void level_fill_random(level *lvl, float wall_chance){

    // Add each of the walls
    for(int y=0;y<lvl->size_y;y++){
        for(int x=0; x<lvl->size_x;x++){
            if(rand() % 10 + 1 < wall_chance) {
                lvl->cells[y][x] = '#';
            }
        }
    }
}

int level_count_neighbours(level *lvl, int x, int y){
    // Counts the amount of neighbours of a certain cell
    int count = 0;
    // Checks only the vecinity of the cell
    // (-1, -1) (0, -1) (1, -1)
    // (-1, 0)  (0, 0)  (1, 0)
    // (-1, 1)  (0, 1)  (1, 1)
    // Like this
    for (int i =-1; i < 2; i++){
        for (int j =-1; j < 2; j++) {

            int neighbour_x = x+i;
            int neighbour_y = y+j;

            // Don't check the middle, because is the cell being analyzed
            if( i == 0 && j == 0) {
                continue;
            }
            // If out of bounds we interpret as walls, since we assume the level is closed off with walls
            else if(neighbour_x < 0 || neighbour_y < 0 || neighbour_x >= lvl->size_x || neighbour_y > lvl->size_y) {
                count += 1;
            }
            // If cell is wall add to the count
            else if(lvl->cells[neighbour_x][neighbour_y] == '#') {
                count += 1;
            }
        }
    }
    // retrieve the count
    return count;
}

level *level_sim(level *lvl, int deathLimit, int birthLimit){
    // Generate a new level based on the Game of Life
    level *newlvl = level_new(lvl->size_x, lvl->size_y);

    for(int x = 0; x < lvl->size_x; x++){
        for(int y = 0; y < lvl->size_y; y++) {
            //Check if any of the neighbours is a wall
            int nbs = level_count_neighbours(lvl, x, y);
            // If below a certain limit set by the argument we replace with open cell (dead cell)
            if(lvl->cells[x][y] == '#'){
                if(nbs < deathLimit) {
                    newlvl->cells[x][y] = '.';
                } else {
                    newlvl->cells[x][y] = '#';
                }
            // Else if over a certain limit a new cell is born ( made into wall )
            } else {
                if(nbs > birthLimit) {
                    newlvl->cells[x][y] = '#';
                } else {
                    newlvl->cells[x][y] = '.';
                }
            }
        }
    }
    //return the level
    return newlvl;
}

level *level_proc_gen(int size_x, int size_y, int numberOfSteps, float wall_chance, int deathLimit, int birthLimit) {
    // Generate a level
    level *proclvl = level_new( size_x, size_y);
    // Fill it randomly
    level_fill_random(proclvl, wall_chance);
    // With a set number of steps we do the simulation (algorithm)
    for (int i = 0; i < numberOfSteps; i++)
    {
        proclvl = level_sim(proclvl,deathLimit,birthLimit);
    }
    // Connect every open cell
    level_connect(proclvl);
    // We get the level
    return proclvl;
}

void level_is_connected(int coord_x, int coord_y, level *lvl){
    // Recurse function of the Flood fill algorithm
    // Checks if cell is "painted", if true then stop
    if(lvl->cells[coord_x][coord_y] == '*'){
        return;
    } 
    // Checks if cell is different as the one we want to "paint", if true stop
    else if (lvl->cells[coord_x][coord_y] != '.'){
        return;
    } 
    // Since the cell we have left are the ones that are the ones we want to paint
    // we start to paint
    else {
        // "paint" that cell (marks them as visited)
        lvl->cells[coord_x][coord_y] = '*';
        // Then runs the algorithm if every direction that is inside the array
        if(coord_y + 1 < lvl->size_y)
            level_is_connected(coord_x, coord_y + 1, lvl);
        if(coord_y - 1 >= 0)
            level_is_connected(coord_x, coord_y - 1, lvl);
        if(coord_x + 1 < lvl->size_x)
            level_is_connected(coord_x + 1, coord_y, lvl);
        if(coord_x - 1 >= 0)
            level_is_connected(coord_x - 1, coord_y, lvl);
        // Once done we'll have a modified level with "painted" cells
        // which will allow to us identify whether the cell is connected or not
        return;
    }
}

void level_connect(level *lvl){
    // We start at a point in the middle
    // it could be random but the way it is set, the middle has a higher chance
    // of generating a big and interesting map
    int initial_x = lvl->size_x/2;
    int initial_y = lvl->size_y/2;
    // Check if cell chosen is blocked off
    while(lvl->cells[initial_x][initial_y] == '#'){
        // Change the initial position until an empty cell is found
        initial_x += 1;
        initial_y += 1;
    }
    // Check which cells are connected with this point
    level_is_connected(initial_x,initial_y,lvl);
    // Since the flood added a new symbol, just replace that symbol with empty cells,
    // and any other case just add walls
    for(int x = 0; x < lvl->size_x; x++){
        for(int y = 0; y < lvl->size_y; y++){
            if(lvl->cells[x][y] == '*')
                lvl->cells[x][y] = '.';
            else if(lvl->cells[x][y] == '.')
                lvl->cells[x][y] = '#';
        }
    }
}

// Free level memory
void level_free(level *lvl){
    // Free each row first
    for(int y=0;y<lvl->size_y;y++){
        free(lvl->cells[y]);
    }
    // Free char* array
    free(lvl->cells);
    // Free the level
    free(lvl);
}
