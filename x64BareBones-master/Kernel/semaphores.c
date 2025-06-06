#include <semaphores.h>

typedef struct semStructure{
    uint64_t value;
	uint8_t lock;
	uint8_t qtty_open;
	queue_adt queue;
}semStructure;

semStructure sem_array[SEM_AMOUNT_USER + KERNEL_SEM_ID_LIMIT]={0};

static int8_t isValidId ( int64_t sem_id, uint8_t is_kernel );
static int isValueZero ( semStructure *sem );
static int alwaysTrue ( semStructure *sem );
static int64_t postIfCondition ( int64_t sem_id, int ( *condition ) ( semStructure * ), uint8_t is_kernel, uint8_t yield );
static int64_t openIdAfterAcquire ( int64_t sem_id, int value, uint8_t is_kernel );

int cmp ( elemTypePtr e1, elemTypePtr e2 )
{
	return e1 - e2;
}

int64_t mySemPost ( int64_t sem_id, uint8_t is_kernel )
{
	return postIfCondition ( sem_id, alwaysTrue, is_kernel, 1 );
}

int64_t mySemPostNoYield ( int sem_id )
{
	return postIfCondition ( sem_id, alwaysTrue, 1, 0 );
}

int64_t semPostIfValueIsZero ( int64_t sem_id, uint8_t is_kernel )
{
	return postIfCondition ( sem_id, isValueZero, is_kernel, 1 );
}

int64_t mySemOpenGetId ( int value )
{

	for ( int i = 0; i < SEM_AMOUNT_USER ; i++ ) {
		acquire ( &sem_array[i].lock );
		if ( sem_array[i].qtty_open == 0 ) {
			if ( openIdAfterAcquire ( i, value, 0 ) == 0 ) {
				return i;
			}
			return -1;
		}
		release ( &sem_array[i].lock );
	}
	return -1;
}

int64_t mySemOpen ( int64_t sem_id, int value, uint8_t is_kernel )
{
	if ( !isValidId ( sem_id, is_kernel ) ) {
		return -1;
	}

	acquire ( &sem_array[sem_id].lock );
	return openIdAfterAcquire ( sem_id, value, is_kernel );
}

static int64_t openIdAfterAcquire ( int64_t sem_id, int value, uint8_t is_kernel )
{
	if ( !isValidId ( sem_id, is_kernel ) ) {
		return -1;
	}

	if ( sem_array[sem_id].qtty_open ) {
		sem_array[sem_id].qtty_open++;
		release ( &sem_array[sem_id].lock );
		return 0;
	}


	queue_adt queue = newQueue();
	if ( queue == NULL ) {
		release ( &sem_array[sem_id].lock );
		return -1;
	}

	sem_array[sem_id].qtty_open = 1;
	sem_array[sem_id].value = value;
	sem_array[sem_id].queue = queue;
	release ( &sem_array[sem_id].lock );
	return 0;
}

int64_t mySemWait ( int64_t sem_id, uint8_t is_kernel )
{

	if ( !isValidId ( sem_id, is_kernel ) ) {
		return -1;
	}

	acquire ( &sem_array[sem_id].lock );

	if ( !sem_array[sem_id].qtty_open ) {
		release ( &sem_array[sem_id].lock );
		return -1;
	}

	if ( sem_array[sem_id].value > 0 ) {
		sem_array[sem_id].value --;
		release ( &sem_array[sem_id].lock );
		return 0;
	}

	PCB * running_pcb = getRunning();

	if ( enqueue ( sem_array[sem_id].queue, running_pcb ) == -1 ) {
		release ( &sem_array[sem_id].lock );
		return -1;
	}
	blockCurrentNoYield();
	running_pcb->blocked_by_sem = sem_id;

	release ( &sem_array[sem_id].lock );
	schedulerYield();
	running_pcb->blocked_by_sem = -1;
	return 0;
}

int64_t mySemClose ( int64_t sem_id, uint8_t is_kernel )
{

	if ( !isValidId ( sem_id, is_kernel ) ) {
		return -1;
	}

	acquire ( &sem_array[sem_id].lock );

	if ( sem_array[sem_id].qtty_open == 0 ) {
		release ( &sem_array[sem_id].lock );
		return -1;
	}

	sem_array[sem_id].qtty_open --;
	if ( sem_array[sem_id].qtty_open == 0 ) {
		while ( !queueIsEmpty ( sem_array[sem_id].queue ) ) {
			PCB * pcb = dequeue ( sem_array[sem_id].queue );
			ready ( pcb );
		}
		freeQueue ( sem_array[sem_id].queue );
	}

	release ( &sem_array[sem_id].lock );
	return 0;
}

int64_t semDeleteFromBlockedQueue ( PCB * pcb )
{

	if ( pcb == NULL ) {
		return -1;
	}
	if ( pcb->blocked_by_sem != -1 ) {
		deleteFromQueue ( sem_array[pcb->blocked_by_sem ].queue, pcb );
	}
	return 0;
}

static int64_t postIfCondition ( int64_t sem_id, int ( *condition ) ( semStructure * ), uint8_t is_kernel, uint8_t yield )
{
	if ( !isValidId ( sem_id, is_kernel ) ) {
		return -1;
	}

	acquire ( &sem_array[sem_id].lock );

	if ( !sem_array[sem_id].qtty_open ) {
		release ( &sem_array[sem_id].lock );
		return -1;
	}

	if ( condition ( &sem_array[sem_id] ) ) {
		if ( queueIsEmpty ( sem_array[sem_id].queue ) ) {
			sem_array[sem_id].value++;
		} else {
			PCB *to_unblock = dequeue ( sem_array[sem_id].queue );
			ready ( to_unblock );
		}
	}

	release ( &sem_array[sem_id].lock );
	if ( yield ) {
		schedulerYield();
	}

	return 0;
}

static int isValueZero ( semStructure *sem )
{
	return sem->value == 0;
}

static int alwaysTrue ( semStructure *sem )
{
	return 1;
}

static int8_t isValidId ( int64_t sem_id, uint8_t is_kernel )
{
	if ( is_kernel ) {
		return sem_id >= SEM_AMOUNT_USER && sem_id < KERNEL_SEM_ID_LIMIT;
	}
	return  sem_id < SEM_AMOUNT_USER && sem_id >= 0;
}