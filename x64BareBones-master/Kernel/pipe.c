#include <pipe.h>
#define BAD_ID(id) (((id) < 0) || ((id) >= AMOUNT_OF_PIPES))

typedef struct pipeCDT {
    tPid pids[2];
	uint8_t reserved;
	uint8_t was_closed_by_reader;
	uint8_t initialized_qtty;
	uint8_t buffer[PIPE_BUFFER_SIZE];
	uint64_t current_read;
	uint64_t current_write;
	uint64_t data_available_sem;
	uint64_t can_write_sem;
}pipeCDT;

static pipeCDT pipes_array[AMOUNT_OF_PIPES];
static int64_t pipeGetFree();

tPid pipeGetPid(int64_t id, tPipeMode mode){
    if ( ( ! ( ( mode == READER ) || ( mode == WRITER ) ) || BAD_ID ( id ) ) ) {
		return -1;
	}
	return pipes_array[id].pids[mode];
}

void pipeInit()
{
	for ( int i = 0; i < AMOUNT_OF_PIPES ; i++ ) {
		pipes_array[i].pids[WRITER] = pipes_array[i].pids[READER] = -1;
	}
}

int64_t pipeOpenPid ( int64_t id, pipe_mode_t mode, pid_t pid )
{
	if ( ! ( ( mode == READER ) || ( mode == WRITER ) ) || BAD_ID ( id ) || ( pipes_array[id].pids[mode] != -1 ) ) {
		return -1;
	}
	my_sem_open ( 2 * id + SEM_AMOUNT_USER, 0, 1 );
	my_sem_open ( 2 * id + 1 + SEM_AMOUNT_USER, 0, 1 );

	pipes_array[id].data_available_sem = 2 * id + SEM_AMOUNT_USER;
	pipes_array[id].can_write_sem = 2 * id + 1 + SEM_AMOUNT_USER;


	pipes_array[id].pids[mode] = pid;
	pipes_array[id].initialized_qtty++;
	return 0;
}

int64_t pipeOpen ( int64_t id, tPipeMode mode )
{
	return pipeOpenPid ( id, mode, getPid() );
}

int64_t pipeOpenFree ( tPipeMode mode )
{
	int64_t id = pipeGetFree();
	if ( pipeOpen ( id, mode ) == 0 ) {
		return id;
	}
	return -1;
}

int64_t pipeRead ( int64_t id, uint8_t * buffer, uint64_t amount )
{
	if ( BAD_ID ( id ) || pipes_array[id].pids[READER] != getPid() ) {
		return -1;
	}

	uint64_t i = 0;
	if ( my_sem_wait ( pipes_array[id].data_available_sem, 1 ) == -1 ) {
		return -1;
	}
	int max_write = pipes_array[id].current_write;

	while ( i < amount && ( pipes_array[id].current_read < max_write ) && pipes_array[id].buffer[pipes_array[id].current_read] != EOF  ) {
		buffer[i++] = pipes_array[id].buffer[pipes_array[id].current_read++];
	}

	if ( pipes_array[id].current_read < max_write || ( pipes_array[id].current_read > 0 && ( pipes_array[id].buffer[pipes_array[id].current_read - 1] == EOF ) ) ) {
		sem_post_if_value_is_zero ( pipes_array[id].data_available_sem, 1 );
	}
	if ( pipes_array[id].current_read == PIPE_BUFFER_SIZE ) {
		my_sem_post ( pipes_array[id].can_write_sem, 1 );
	}

	return i;
}

int64_t pipeWrite ( int64_t id, uint8_t * buffer, uint64_t amount, tPid pid )
{
	if ( BAD_ID ( id ) || pipes_array[id].pids[WRITER] != pid || pipes_array[id].was_closed_by_reader ) {
		return -1;
	}
	int i = 0;
	for ( ; i < amount; i++ ) {
		pipes_array[id].buffer[pipes_array[id].current_write ++] = buffer[i];
		if ( pipes_array[id].current_write == PIPE_BUFFER_SIZE ) {
			sem_post_if_value_is_zero ( pipes_array[id].data_available_sem, 1 );
			my_sem_wait ( pipes_array[id].can_write_sem, 1 );
			pipes_array[id].current_write = 0;
			pipes_array[id].current_read = 0;
			if ( pipes_array[id].was_closed_by_reader ) {
				return -1;
			}
		}
	}
	if ( pipes_array[id].current_write != 0 ) {
		sem_post_if_value_is_zero ( pipes_array[id].data_available_sem, 1 );
	}

	return i;
}

int64_t pipeClose ( int64_t id, tPid pid )
{
	int flag = -1;
	if ( BAD_ID ( id ) || pid >= PCB_AMOUNT || pid < 0 ) {
		return flag;
	}


	if ( pipes_array[id].pids[WRITER] == pid ) {
		uint8_t end_of_file[] = {EOF};
		pipeWrite ( id, end_of_file, 1, pid );
		flag = 0;
		pipes_array[id].pids[WRITER] = -1;
	}
	if ( pipes_array[id].pids[READER] == pid ) {
		pipes_array[id].pids[READER] = -1;
		my_sem_post_no_yield ( pipes_array[id].can_write_sem );
		pipes_array[id].was_closed_by_reader = 1;
		flag = 0;
	}
	if ( pipes_array[id].initialized_qtty == 2 && !flag && pipes_array[id].pids[WRITER] == -1 &&  pipes_array[id].pids[READER] == -1 ) {
		pipes_array[id].current_read = pipes_array[id].current_write = pipes_array[id].was_closed_by_reader = pipes_array[id].reserved = 0 ;
	}
	if ( flag == 0 ) {
		my_sem_close ( pipes_array[id].data_available_sem, 1 );
		my_sem_close ( pipes_array[id].can_write_sem, 1 );
	}
	return flag;
}

static int64_t pipeGetFree()
{
	for ( int i = 0; i < AMOUNT_OF_PIPES ; i++ ) {
		if ( pipes_array[i].pids[READER] == -1 && pipes_array[i].pids[WRITER] == -1 && !pipes_array[i].reserved ) {
			return i;
		}
	}
	return -1;
}

tPid pipeReserve()
{
	for ( int i = 0; i < AMOUNT_OF_PIPES ; i++ ) {
		if ( pipes_array[i].pids[READER] == -1 && pipes_array[i].pids[WRITER] == -1 && !pipes_array[i].reserved ) {
			pipes_array[i].reserved = 1;
			return i;
		}
	}
	return -1;
}