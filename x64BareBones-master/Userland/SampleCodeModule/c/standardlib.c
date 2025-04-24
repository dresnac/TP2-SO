#include <standardlib.h>


char getChar(){
    char c;
    int flag = 0;
    while(flag == 0 || c > 255){
        sys_read(STDIN, &c, 1, &flag);
    }
    return c;
}

char readInput(int * flag){
    char c;
    sys_read(STDIN, &c, 1, flag);
    return c;
}

void putChar(char c) {
    sys_write(STDOUT, &c, 1);
}

void beep(uint64_t frequency, uint64_t duration) {
    sys_beep(frequency, duration);
}

int64_t clear_screen() {
    return sys_clear_screen();
}


uint64_t strlen(const char *str) {
    const char *s = str;
    while (*s)
        ++s;
    return s - str;
}

void strcopy(const char * s, char * t){ 
    while(*s){
        *t = *s;
        s++;
        t++;
    }
    *t = 0;
}


char * numToString(uint64_t num, uint64_t base) {
    static char buffer[64];
    char * ptr = &buffer[63];
    *ptr = '\0';
    do {
        *--ptr = "0123456789abcdef"[num % base];
        num /= base;
    } while(num != 0);
    return ptr;
}


int64_t puts(const char * str) {
    int len = strlen(str);
    char aux[len+1];
    strcopy(str, aux);
    aux[len] = '\n';
    aux[len+1] = '\0';
    return sys_write(STDOUT, aux, len+1);  //retorna basura
}


int64_t fputc(char c, FileDescriptor fd) {
    return sys_write(fd, &c, 1);   //retorna basura
}

void time(LocalTime * time){
    sys_time(time);
}


static int64_t vfprintf(FileDescriptor fd, const char *fmt, va_list args)
{
    uint64_t flag = 0;
    uint64_t written = 0;

    for (uint64_t i = 0; fmt[i] != '\0'; i++)
    {
        if (fmt[i] == '%' && !flag)
        {
            flag = 1;
            i++;
        }

        if (!flag)
        {
            fputc(fmt[i], fd);
            flag = 0;
            written++;
            continue;
        }

        switch (fmt[i])
        {
            case 'c':
                fputc(va_arg(args, int), fd);
                written++;
                break;
            case 'd':
                written += vfprintf(fd, numToString(va_arg(args, uint64_t), 10), args);
                break;
            case 'x':
                written += vfprintf(fd, "0x", args);
                written += vfprintf(fd, numToString(va_arg(args, uint64_t), 16), args);
                break;
            case 's':
                written += vfprintf(fd, va_arg(args, char *), args);
                break;
            case '%':
                fputc('%', fd);
                written++;
                break;
            default:
                return -1;
        }

        flag = 0;
    }

    return written;
}


int64_t fprintf(FileDescriptor fd, const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int64_t out = vfprintf(fd, fmt, args);

    va_end(args);
    return out;
}


int64_t printf(const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int64_t out = vfprintf(STDOUT, fmt, args);

    va_end(args);
    return out;
}


char* gets(char* buffer, int n) {
    int c;
    int i = 0;

    while ((c = getChar()) != '\n') {
        if (c == '\b' && i > 0) {
            putChar(c);
            i--;
        }
        if (c != '\b' && i < n - 1) {
            putChar(c);
            buffer[i++] = (char)c;
        }
    }
    putChar('\n');
    buffer[i] = '\0';

    return buffer;
}


int64_t strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

int64_t draw_rectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint64_t color) {
    return sys_put_rectangle(x, y, width, height, color);
}


void print_regs() {
    Snapshot snap;
    int flag = -1;
    sys_get_regs(&snap, &flag);
    if(flag == -1) {
        fprintf(STDERR, "\nNo register snapshot available. Press F1 to take a snapshot.\n");
        return;
    }

    puts("\nRegister snapshot:\n");
    printf("rax: %x\n", snap.rax);
    printf("rbx: %x\n", snap.rbx);
    printf("rcx: %x\n", snap.rcx);
    printf("rdx: %x\n", snap.rdx);
    printf("rsi: %x\n", snap.rsi);
    printf("rdi: %x\n", snap.rdi);
    printf("rbp: %x\n", snap.rbp);
    printf("rsp: %x\n", snap.rsp);
    printf("r8:  %x\n", snap.r8 );
    printf("r9:  %x\n", snap.r9 );
    printf("r10: %x\n", snap.r10);
    printf("r11: %x\n", snap.r11);
    printf("r12: %x\n", snap.r12);
    printf("r13: %x\n", snap.r13);
    printf("r14: %x\n", snap.r14);
    printf("r15: %x\n", snap.r15);
    printf("rIP: %x\n", snap.rip);
}

void zoom(int n){
    int change = 0;
    sys_zoom(n, &change);
    if(change){
        sys_clear_screen();

    }
    else{
        putChar('\n');
    }
}

void adjustTime(LocalTime * time) {

    if (time->horas < 3) {
        time->horas += 21;
        time->dias--;
        if (time->dias == 0) {
            time->mes--;
            if (time->mes == 0) {
                time->mes = 12;
                time->anio--;
            }
            if(time->mes == 2){
                time->dias = 28;
                if(time->anio % 4 == 0){
                    time->dias = 29;
                }
            } else if(time->mes == 4 || time->mes == 6 || time->mes == 9 || time->mes == 11){
                time->dias = 30;
            } else {
                time->dias = 31;
            }
        }
    }
    else{
        time->horas = time->horas - 3;
    }
}
unsigned long getTicks(){
    unsigned long ticks;
    sys_get_ticks(&ticks);
    return ticks;
}