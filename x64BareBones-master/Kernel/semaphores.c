#include <semaphores.h>

typedef struct semStructure{
    uint64_t value;
	uint8_t lock;
	uint8_t qtty_open;
	queue_adt queue;
}semStructure;
