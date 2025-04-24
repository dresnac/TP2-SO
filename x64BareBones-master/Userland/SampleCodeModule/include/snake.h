#ifndef SNAKE_H
#define SNAKE_H

#include <standardlib.h>
#include <stdint.h>

#define SIDE 19  //Matriz cuadrado de SIDE*SIDE
#define START_LEN 3
#define MAX_LEN (SIDE-1)*(SIDE-1)

#define MATRIX_START_X 80
#define MATRIX_START_Y 80
#define BLOCK_SIDE 20

#define WINNER 10
#define GAMEMODE_ERR 5

typedef struct{
    int x;
    int y;
}Coords;


typedef enum{
    SINGLE = 1, MULTIPLAYER = 2
}GameModeType;

typedef enum{
    BACKGROUND = 0, LIMIT, SNAKE1, SNAKE2, APPLE 
}BlockType;
                                    //NEGRO,    GRIS,       VERDE,      AZUL,        ROJO
static uint64_t colorByBlock[] = {0x00000000, 0x00808080, 0x0000FF00, 0x000000FF, 0x00FF0000};

typedef struct{
    Coords body[MAX_LEN];
    BlockType type;
    int dim;
}SnakeType;

typedef BlockType BlockMatrix[SIDE][SIDE];

int playSnake(GameModeType gameMode, int level);

#endif