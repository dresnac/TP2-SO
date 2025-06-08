#ifndef LIB_H
#define LIB_H

#include <stdint.h>


char * cpuVendor ( char * result );
int inb ( int port );
void outb ( int port, int value );
uint8_t rtc ( uint8_t mode );
uint8_t getKey();


#endif