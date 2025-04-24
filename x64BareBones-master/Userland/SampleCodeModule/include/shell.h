#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>

#define MAX_MODULES 9

typedef struct module {
    char *name;
    void (*function)();
} module;


void startShell();

#endif