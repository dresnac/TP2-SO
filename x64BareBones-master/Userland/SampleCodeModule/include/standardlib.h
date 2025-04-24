#ifndef STANDARDLIB_H
#define STANDARDLIB_H

#include <stdarg.h>
#include <stdint.h>
#include <syscalls.h>
#include <localtime.h>

typedef enum
{
    STDIN = 0,
    STDOUT,
    STDERR,
    STDMARK,
} FileDescriptor;


char getChar();
void putChar(char c);
void beep(uint64_t frequency, uint64_t duration);
int64_t clear_screen();
uint64_t strlen(const char *str);
void strcopy(const char * s, char * t);
char * numToString(uint64_t num, uint64_t base);
int64_t puts(const char * str);
int64_t fputc(char c, FileDescriptor fd);
void time(LocalTime * time);
int64_t fprintf(FileDescriptor fd, const char * fmt, ...);
int64_t printf(const char * fmt, ...);
char* gets(char* buffer, int n);
int64_t strcmp(const char *str1, const char *str2);
void print_regs();
void zoom(int n);
void adjustTime(LocalTime * time);
int64_t draw_rectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint64_t color);
unsigned long getTicks();
char readInput();

#endif