#ifndef TPE_ARQUI_LIBC_H
#define TPE_ARQUI_LIBC_H

#include <stdint.h>
#include <stdarg.h>
#include <drivers_information.h>
#include <shared_defs.h>
#include <syscalls.h>
#include <stddef.h>
#include <shared_libc.h>

char libcGetChar();
void libcPutChar ( char c );
int64_t libcBeep ( uint64_t frequency, uint64_t duration );
int64_t libcClearScreen();
int64_t libcSetFontSize ( uint64_t size );
char * libcNumToString ( uint64_t num, uint64_t base, char *buffer, size_t buffer_size );
int64_t libcPuts (  char * str );
int64_t libcfPutc ( char c, uint64_t fd );
int64_t libcfPrintf ( uint64_t fd,  char * fmt, ... );
int64_t libcPrintf (  char * fmt, ... );
char * libcGets ( char * buffer, int n );
int64_t libcStrlen(char *str);
int64_t libcStrcmp (  char * str1,  char * str2 );
int64_t libcStrnocasecmp ( char *str1,  char *str2 );
void libcPrintRegisterSnapshot();
int64_t libcDrawPixel ( uint64_t x, uint64_t y, color color );
int64_t libcDrawRectangle ( uint64_t x, uint64_t y, uint64_t width, uint64_t height, color color );
int64_t libcDrawLetter ( uint64_t x, uint64_t y, char letter, color color, uint64_t font_size );
int64_t libcEnterTextMode();
int64_t libcEnterVideoMode();
int64_t libcGetTime ( LocalTime * time );
void * libcMalloc ( uint64_t size );
void libcFree ( void * ptr );
tPid libcCreateProcess ( mainFunction rip, uint64_t priority, char ** argv, uint64_t argc, tFd fds[] );
tPid libcGetPid();
int64_t libcNice ( tPid pid, uint64_t newPrio );
int64_t libcYield();
int64_t libcKill ( tPid pid );
int64_t libcBlock ( tPid pid );
int64_t libcUnblock ( tPid pid );
tPid libcWait ( tPid pid, int64_t * ret );
int64_t libcSemOpen ( int64_t sem_id, int64_t value );
int64_t libcSemOpenGetId ( int64_t value );
int64_t libcSemWait ( int64_t sem_id );
int64_t libcSemPost ( int64_t sem_id );
int64_t libcSemClose ( int64_t sem_id );
int64_t libcTicksSleep ( uint64_t ticks );
int64_t libcSleep ( uint64_t seconds );
int8_t libcGetStatus ( tPid pid );
int64_t libcPipeOpen ( int64_t id, tPipeMode mode );
int64_t libcPipeOpenFree ( tPipeMode mode );
int64_t libcPipeRead ( int64_t id, uint8_t * buffer, uint64_t amount );
int64_t libcPipeWrite ( int64_t id, uint8_t * buffer, uint64_t amount );
int64_t libcPipeClose ( int64_t id );
int64_t libcPipeReserve();
int64_t libcSatoi ( char * str, int64_t * flag );
int64_t libcGetMyFds ( tFd fds[CANT_FDS] );

#endif
