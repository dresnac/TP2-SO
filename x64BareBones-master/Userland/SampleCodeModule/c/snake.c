#include <snake.h>

#define SINGLE_START_X  5
#define SINGLE_START_Y (SIDE/2)
#define PLAYER1_START_X 5
#define PLAYER1_START_Y (SIDE/3)
#define PLAYER2_START_X 5
#define PLAYER2_START_Y ((2*SIDE)/3)
#define APPLE_START_X  13
#define APPLE_START_Y (SIDE/2)

#define TICKS_PER_MOVEMENT 6

typedef enum{
    LEFT = 0, UP, DOWN, RIGHT
}Direction;

int playSingle(int level);
void initSingle(BlockMatrix matrix, SnakeType * snake, Coords * apple);
void fillInitialMatrix(BlockMatrix matrix);
void render(BlockMatrix matrix);
void changeDirection(Direction * dir, char pressedKey, int player);
int updatePos(BlockMatrix matrix, SnakeType * snake, Coords * apple, Direction dir);
int random_in_range(unsigned int max);
unsigned int lcg_random();
void collision();
void food(BlockMatrix matrix, Coords * apple);


int playSnake(GameModeType gameMode, int level){
    int toRet = 0;
    switch(gameMode){
        case SINGLE: toRet = playSingle(level); break;
        case MULTIPLAYER: toRet = playMultiplayer(level); break;
        default: return GAMEMODE_ERR;
    }
    return toRet;
}

int points=0;
int singleGameMode = 0;

int playSingle(int level){
    singleGameMode = 1;
    BlockMatrix matrix;
    SnakeType snake;
    Coords apple;
    initSingle(matrix, &snake, &apple);
    render(matrix);
    while(getChar() != 'd');  //Hasta que no se haga avanzar la serpiente no arranca
    Direction dir = RIGHT;
    unsigned long ticksAtStart = getTicks(); 
    char c;
    char aux;
    int end = 0;
    int wait = 0;
    unsigned long ticks = 0;
    printf("Puntos: %d", points);

    int flag;
    while(end == 0){
        flag = 0;
        aux = readInput(&flag);
        if(flag != 0){
            c = aux;
            changeDirection(&dir, c, 1);
        }
        
        if((ticks = getTicks() - ticksAtStart) != 0 && ticks % (TICKS_PER_MOVEMENT/level) == 0 && !wait){
            end = updatePos(matrix, &snake, &apple, dir);
            render(matrix);
            wait = 1;
        }
        if(ticks != 1 && ticks % (TICKS_PER_MOVEMENT/level) == 1){    //dejo pasar un tick
            wait = 0;    
        }
    }
    char z;
    while((z = getChar()) != 'e' && z != 'r');
    if(z == 'r'){
        points = 0;
        clear_screen();
        playSingle(level);
    }

}

void initSingle(BlockMatrix matrix, SnakeType * snake, Coords * apple){
    int x = SINGLE_START_X;
    int y = SINGLE_START_Y;
    snake->type = SNAKE1;
    snake->dim = START_LEN;
    fillInitialMatrix(matrix);
    for(int i=0; i < START_LEN; i++){
        matrix[x-i][y] = snake->type;
        snake->body[i] = (Coords){x-i, y};
    }
    apple->x = APPLE_START_X;
    apple->y = APPLE_START_Y;
    matrix[apple->x][apple->y] = APPLE;
}

void fillInitialMatrix(BlockMatrix matrix){
    for(int i=0; i < SIDE; i++){
        for(int j=0; j < SIDE; j++){
            if(i == 0 || i == (SIDE-1) || j == 0 || j == (SIDE-1)){
                matrix[i][j] = LIMIT;
            }else{
                matrix[i][j] = BACKGROUND;
            }
        }
    }
}

void render(BlockMatrix matrix){
    for(int i=0; i < SIDE; i++){
        for(int j=0; j < SIDE; j++){
            draw_rectangle(MATRIX_START_X+BLOCK_SIDE*i, MATRIX_START_Y+BLOCK_SIDE*j, BLOCK_SIDE, BLOCK_SIDE, colorByBlock[matrix[i][j]]);
        }
    }
}

void changeDirection(Direction * dir, char pressedKey, int player){
    if(player == 1){
        switch(pressedKey){
        case 'a': *dir = LEFT; break;
        case 'w': *dir = UP; break;
        case 's': *dir = DOWN; break;
        case 'd': *dir = RIGHT; break;
        default: break; 
        }
    }else{
        switch(pressedKey){
        case 'j': *dir = LEFT; break;
        case 'i': *dir = UP; break;
        case 'k': *dir = DOWN; break;
        case 'l': *dir = RIGHT; break;
        default: break; 
        }
    }
}

int updatePos(BlockMatrix matrix, SnakeType * snake, Coords * apple, Direction dir){
    Coords aux = snake->body[snake->dim - 1];
    for(int i = snake->dim - 1; i > 0; i--){
        snake->body[i] = snake->body[i-1];
    }
    switch(dir){
        case LEFT: snake->body[0].x--; break;
        case UP: snake->body[0].y--; break;
        case DOWN: snake->body[0].y++; break;
        case RIGHT: snake->body[0].x++; break;
    }
    int headX = snake->body[0].x;
    int headY = snake->body[0].y;
    if(matrix[headX][headY] == APPLE){
        snake->body[snake->dim] = aux;
        snake->dim++;
        if(snake->dim == MAX_LEN){
            return WINNER;  //si llenas el tablero ganás
        }
        food(matrix, apple);
    }else if(matrix[headX][headY] == BACKGROUND){
        for(int i=0; i < snake->dim; i++){
            matrix[snake->body[i].x][snake->body[i].y] = snake->type;
        }
        matrix[aux.x][aux.y] = BACKGROUND;
    }else{
        collision();
        return snake->type;
    }

    return 0;
}

void food(BlockMatrix matrix, Coords * apple){
    do{
        int newPos = random_in_range((SIDE-2)*(SIDE-2));
        apple->x = 1 + (newPos / (SIDE-2));
        apple->y = 1 + (newPos % (SIDE-2));
    }while(matrix[apple->x][apple->y] == APPLE || matrix[apple->x][apple->y] == SNAKE1 || matrix[apple->x][apple->y] == SNAKE2);
    matrix[apple->x][apple->y] = APPLE;
    if(singleGameMode){
        addPoints();
    } 
    beep(300, 1);
}

void collision(){
    puts("\nGAMEOVER");
    beep(100, 5);
}

unsigned int seed = 12345;  // Semilla inicial

unsigned int lcg_random() {
    seed = (1103515245 * seed + seed) % (1 << 31);  // m = 2^31
    return seed;
}

int random_in_range(unsigned int max) {
    return lcg_random() % (max);  // Limitar al rango [0, max)
}

void addPoints(){
    points++;
    if(points < 10){
        printf("\b%d", points);
    }else if(points < 100){
        printf("\b\b%d", points);
    }else{
        printf("\b\b\b%d", points);
    }
}

int playMultiplayer(int level){
    singleGameMode = 0;
    BlockMatrix matrix;
    SnakeType snake1;
    SnakeType snake2;
    Coords apple;
    initMulti(matrix, &snake1, &snake2, &apple);
    render(matrix);
    char start;
    while((start = getChar()) != 'd' && start != 'l');  //Hasta que no se haga avanzar la serpiente no arranca
    Direction dir1 = RIGHT;
    Direction dir2 = RIGHT;
    unsigned long ticksAtStart = getTicks(); 
    char c;
    char aux;
    int end = 0;
    int wait = 0;
    unsigned long ticks = 0;

    int flag;
    while(end == 0){
        flag = 0;
        aux = readInput(&flag);
        if(flag != 0){
            c = aux;
            changeDirection(&dir1, c, 1);
            changeDirection(&dir2, c, 2);
        }
        
        if((ticks = getTicks() - ticksAtStart) != 0 && ticks % (TICKS_PER_MOVEMENT/level) == 0 && !wait){
            end = updatePos(matrix, &snake1, &apple, dir1);
            if(end != 0){
                break;
            }
            end = updatePos(matrix, &snake2, &apple, dir2);
            render(matrix);
            wait = 1;
        }
        if(ticks != 1 && ticks % (TICKS_PER_MOVEMENT/level) == 1){    //dejo pasar un tick
            wait = 0;    
        }
    }
    printf("WINNER PLAYER %d", (end == SNAKE1)?2:1);

    char z;
    while((z = getChar()) != 'e' && z != 'r');
    if(z == 'r'){
        points = 0;
        clear_screen();
        playMultiplayer(level);
    }
}


void initMulti(BlockMatrix matrix, SnakeType * snake1, SnakeType * snake2, Coords * apple){
    int x1 = PLAYER1_START_X;
    int y1 = PLAYER1_START_Y;

    int x2 = PLAYER2_START_X;
    int y2 = PLAYER2_START_Y;

    snake1->type = SNAKE1;
    snake1->dim = START_LEN;
    snake2->type = SNAKE2;
    snake2->dim = START_LEN;
    fillInitialMatrix(matrix);
    for(int i=0; i < START_LEN; i++){
        matrix[x1-i][y1] = snake1->type;
        snake1->body[i] = (Coords){x1-i, y1};
        matrix[x2-i][y2] = snake2->type;
        snake2->body[i] = (Coords){x2-i, y2};
    }
    apple->x = APPLE_START_X;
    apple->y = APPLE_START_Y;
    matrix[apple->x][apple->y] = APPLE;
}