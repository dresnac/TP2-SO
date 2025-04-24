#include <stdint.h>
#include <shell.h>
#include <standardlib.h>
#include <snake.h>
#include <exceptions.h>



#define PROMPT_TEXT "$:> "
#define MAX_CHARS 110
#define MSG_BUFFER_FULL "\nCuidado: Buffer de comandos lleno\n"
#define MORNING "de la maniana"
#define AFTERNOON "de la tarde"

static void interpret(char * buffer);
static void help();
static void showCurrentTime();
static void zoomIn();
static void zoomOut();
static void getRegs();
static void clear();
static void snake();

static module modules[] = {
    {"help", help},
    {"time", showCurrentTime},
    {"zoomin", zoomIn},
    {"zoomout", zoomOut},
    {"getregs", getRegs},
    {"dividebyzero", div0},
    {"opcode", opCode},
    {"clear", clear},
    {"snake", snake}
};


void startShell(){

    char commandBuff[MAX_CHARS] = {0};
    int i=0;
    int newPrompt;
    char c;
    while(1){
        newPrompt = 0;
        printf(PROMPT_TEXT);
        while(i < MAX_CHARS && !newPrompt){
            c = getChar();
            if(c == '\n'){
                commandBuff[i] = '\0';
                interpret(commandBuff);
                i = 0;
                newPrompt = 1;
            }else if(c == '\b'){
                if(i > 0){
                    i--;
                    putChar(c);
                }
            }
            else if(c == 27)
            {
                continue;
            }else{
                commandBuff[i++] = c;
                putChar(c);
            }
            if(i >= MAX_CHARS){  //hay que hacer que cuando llegue al final de la línea siga en la siguiente
                fprintf(STDERR, MSG_BUFFER_FULL);
                i = 0;
                newPrompt = 1;
            }        
        }
    }
    
}


static void interpret(char * buffer){
    int found = 0;
    for(int i = 0; i<MAX_MODULES; i++){
        if(!strcmp(buffer, modules[i].name)){
            modules[i].function();
            found = 1;
        }
    }
    if(!found){
        putChar('\n');
    }
    return;
}

//muestra comandos disponibles
static void help(){
    printf("\n\tCOMANDOS DISPONIBLES\n");
    printf("\thelp\n");
    printf("\ttime\n");
    printf("\tzoomin\n");
    printf("\tzoomout\n");
    printf("\tgetregs\n");
    printf("\tdividebyzero\n");
    printf("\topcode\n");
    printf("\tclear\n");
    printf("\tsnake\n");
}

//muestra la hora actual
static void showCurrentTime(){
    LocalTime currentTime;
    time(&currentTime);  
    adjustTime(&currentTime);
    printf("\nSon las %d:%d con %d segundos %s del dia %d/%d/%d\n",
    currentTime.horas, currentTime.minutos, currentTime.segundos,
    (currentTime.horas >= 12) ? AFTERNOON:MORNING,
    currentTime.dias, currentTime.mes, currentTime.anio );
}


//agranda la pantalla
static void zoomIn(){  //todavía no funca
    zoom(1);
}

//achica la pantalla
static void zoomOut(){
    zoom(-1);
}

//muestra los registros
static void getRegs(){
    print_regs();
}

//borra toda la pantalla y solo deja el primer prompt
static void clear(){
    clear_screen();
}

//juego del snake
static void snake(){
    char gameMode;
    char level;
    puts("\nElegir modo de juego (1. SINGLE / 2. MULTIPLAYER)");
    printf("Ingresar numero: ");
    gameMode = getChar();
    putChar(gameMode);
    puts("\nElegir nivel de dificultad (1.EASY / 2.MEDIUM / 3.HARD)");
    printf("Ingresar numero: "); 
    level = getChar();
    putChar(level);
    int end = 0;
    clear_screen();
    while(!end){
        end = playSnake((gameMode - '0'), (level - '0'));
    }
    clear_screen();
}