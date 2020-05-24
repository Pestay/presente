#ifndef LEVEL_H
#define LEVEL_H

#define TILE_SIZE 48

/* A level is a matrix of cells, each cell is represented using chars.
'.' = free cells
'#' = walls
*/
typedef struct {
    int size_x, size_y;
    char **cells;
    int cell_size;
} level;

// Create a new level with the given size, allocates memory for it.
level *level_new(int size_x, int size_y);

// Retrieves the cell at a given position, can be used for cells outside the board.
int level_get(const level *lvl, int x, int y);

// Put randomly placed walls of the given size in the level.
void level_fill_random(level *lvl, float wall_chance);

// Checks around a selected cell for neighbours
int level_count_neighbours(level *lvl, int x, int y);

// Do a simulation of the algorithm 
level *level_sim(level *lvl, int deathLimit, int birthLimit);

// Generates the level by doing the simulation of the algorithm a certain number of times
level *level_proc_gen(int size_x, int size_y, int numberOfSteps, float wall_chance, int deathLimit, int birthLimit);

// Checks if the level is connected by flooding
void level_is_connected(int coord_x, int coord_y, level *lvl);

// Connect the level so that every open cell is reachable 
void level_connect(level *lvl);

// Destroy a level, free its memory.
void level_free(level *lvl);


#endif
