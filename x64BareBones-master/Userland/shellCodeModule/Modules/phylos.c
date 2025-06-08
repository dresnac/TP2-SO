// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <phylos.h>
#include <stddef.h>



char* itoa ( int value, char* str, size_t size, int base )
{
	if ( str == NULL || size == 0 ) {
		return NULL;
	}

	if ( base < 2 || base > 36 ) {
		*str = '\0';
		return NULL;
	}

	char buffer[66];
	char* ptr = buffer + sizeof ( buffer ) - 1;
	*ptr = '\0';

	int is_negative = 0;


	if ( value == 0 ) {
		if ( size < 2 ) {
			*str = '\0';
			return NULL;
		}
		str[0] = '0';
		str[1] = '\0';
		return str;
	}

	uint64_t uvalue;
	if ( value < 0 && base == 10 ) {
		is_negative = 1;
		uvalue = ( uint64_t ) ( -value );
	} else {
		uvalue = ( uint64_t ) value;
	}

	while ( uvalue != 0 ) {
		int remainder = uvalue % base;
		* ( --ptr ) = ( remainder > 9 ) ? ( remainder - 10 ) + 'a' : remainder + '0';
		uvalue /= base;
	}

	if ( is_negative ) {
		* ( --ptr ) = '-';
	}

	size_t len = ( buffer + sizeof ( buffer ) - 1 ) - ptr;

	if ( len + 1 > size ) {
		*str = '\0';
		return NULL;
	}

	for ( size_t i = 0; i < len + 1; ++i ) {
		str[i] = ptr[i];
	}

	return str;
}


typedef struct {
	int state;
	int64_t left_fork;
	int64_t right_fork;
} philosopher_t;

static void think ( int i );
static void eat ( int i );
static int64_t philosopher ( char ** argv, uint64_t argc );
static int64_t createProcess ( int64_t i );
static int64_t addFirstPhylos();

static void addPhilosopher();
static void removePhilosopher();
static void displayState();
static int8_t handleKeyboard ( char key );
static void displayHeader();
static void cleanAll();
static int64_t openMutexes();
static void closeMutexes();

static int64_t forks[MAX_PHILOS];
static philosopher_t philos_array[MAX_PHILOS];
static int64_t philosophers_pids[MAX_PHILOS];
static int64_t state_mutex;
static int64_t added_min_sem;
static int64_t num_philos_mutex;
static int64_t last_mutex;
static int64_t second_last_mutex;
static int64_t num_philosophers = 0;




static void think ( int i )
{
	libcSleep ( THINK_CONSTANT * get_uniform ( GET_UNIFORM_CONSTANT ) );
}

static void eat ( int i )
{
	libcSemWait ( state_mutex );
	philos_array[i].state = EATING;
	displayState();
	libcSemPost ( state_mutex );

	libcSleep ( SLEEP_CONSTANT * get_uniform ( GET_UNIFORM_CONSTANT ) );

	libcSemWait ( state_mutex );
	philos_array[i].state = THINKING;
	displayState();
	libcSemPost ( state_mutex );
}

static int64_t philosopher ( char ** argv, uint64_t argc )
{
	int64_t flag = 0;
	int i = libcSatoi ( argv[1], &flag );
	if ( flag == 0 ) {
		libcfPrintf ( STDERR, "Error: numero de filosofo invalido\n" );
		return -1;
	}

	if ( i < MIN_PHILOS ) {
		libcSemWait ( added_min_sem );
	}

	while ( 1 ) {
		think ( i );


		uint8_t have_last_sem = 0;
		uint8_t have_second_last_sem = 0;

		libcSemWait ( num_philos_mutex );

		if ( i == num_philosophers - 1 ) {
			libcSemWait ( last_mutex );
			have_last_sem = 1;
		} else if ( i == num_philosophers - 2 ) {
			libcSemWait ( second_last_mutex );
			have_second_last_sem = 1;
		}
		libcSemPost ( num_philos_mutex );



		if ( i % 2 == 0 ) {
			libcSemWait ( philos_array[i].left_fork );
			libcSemWait ( philos_array[i].right_fork );
		} else {
			libcSemWait ( philos_array[i].right_fork );
			libcSemWait ( philos_array[i].left_fork );
		}



		eat ( i );

		libcSemPost ( philos_array[i].right_fork );
		libcSemPost ( philos_array[i].left_fork );


		if ( have_last_sem ) {
			libcSemPost ( last_mutex );
		} else if ( have_second_last_sem ) {
			libcSemPost ( second_last_mutex );
		}



	}
	return 0;
}


static int64_t createProcess ( int64_t i )
{
	char philo_num_buff[5];
	char * philo_number_str = itoa ( i, philo_num_buff, 5, 10 );
	if ( philo_number_str == NULL ) {
		return -1;
	}
	char *args[] = {"filo", philo_number_str};
	tFd fds[CANT_FDS];

	if ( libcGetMyFds ( fds ) == -1 ) {
		libcfPrintf ( STDERR, "Error obteniendo fds\n" );
		return -1;
	}
	return libcCreateProcess ( ( mainFunction ) &philosopher, LOW, args, 2, fds );
}


static int64_t addFirstPhylos()
{

	for ( int i = 0; i < MIN_PHILOS; i++ ) {
		philos_array[i].left_fork = forks[i] = libcSemOpenGetId ( 1 );
		if ( forks[i] < 0 ) {
			for ( int j = 0; j < i; j++ ) {
				libcSemClose ( forks[j] );
			}
			return -1;
		}
	}
	for ( int i = 0; i < MIN_PHILOS - 1; i++ ) {
		philos_array[i].right_fork = forks[i + 1];
	}
	philos_array[MIN_PHILOS - 1].right_fork = forks[0];

	for ( int i = 0; i < MIN_PHILOS ; i++ ) {
		philosophers_pids[i] = createProcess ( i );
		if ( philosophers_pids[i] < 0 ) {
			for ( int j = 0; j < MIN_PHILOS; j++ ) {
				libcSemClose ( forks[j] );
			}
			for ( int j = 0; j < i; j++ ) {
				libcKill ( philosophers_pids[j] );
			}
			return -1;
		}

	}
	num_philosophers = MIN_PHILOS;

	for ( int i = 0; i < MIN_PHILOS ; i++ ) {
		libcSemPost ( added_min_sem );
	}
	return num_philosophers;
}


static void addPhilosopher()
{
	if ( num_philosophers >= MAX_PHILOS ) {
		libcfPrintf ( STDERR, "Error: numero maximo de filosofos alcanzado\n" );
		return;
	}


	int64_t new_sem;
	if ( ( new_sem = libcSemOpenGetId ( 1 ) ) < 0 ) {
		libcfPrintf ( STDERR, "Error: no se pudo abrir sem para agregar filosofo\n" );
		return;
	}

	libcSemWait ( num_philos_mutex );
	int i = num_philosophers;
	philos_array[i].state = THINKING;
	forks[i] = new_sem;
	libcSemWait ( last_mutex );


	philos_array[i - 1].right_fork = new_sem;
	philos_array[i].left_fork = new_sem;
	philos_array[i].right_fork = philos_array[0].left_fork;
	philosophers_pids[i] = createProcess ( i );
	if ( philosophers_pids[i] >= 0 ) {
		num_philosophers++;
	} else {
		libcSemClose ( new_sem );
		philos_array[i - 1].right_fork = philos_array[0].left_fork;
		libcfPrintf ( STDERR, "No se pudo crear filosofo" );
	}

	libcSemPost ( last_mutex );
	libcSemPost ( num_philos_mutex );

}

static void removePhilosopher()
{
	if ( num_philosophers == MIN_PHILOS ) {
		libcfPrintf ( STDERR, "Error: numero minimo de filosofos alcanzado\n" );
		return;
	}

	libcSemWait ( num_philos_mutex );
	int i = num_philosophers;
	libcSemWait ( last_mutex );
	libcSemWait ( second_last_mutex );

	philos_array[i - 2].right_fork = philos_array[0].left_fork;
	libcSemClose ( philos_array[i - 1].left_fork );
	libcKill ( philosophers_pids[i - 1] );
	num_philosophers--;

	libcSemPost ( second_last_mutex );
	libcSemPost ( last_mutex );
	libcSemPost ( num_philos_mutex );



}



static void displayState()
{
	libcPrintf ( "" );
	for ( int i = 0; i < num_philosophers; i++ ) {
		if ( philos_array[i].state == EATING ) {
			libcPutChar ( 'E' );
		} else {
			libcPutChar ( '.' );
		}
		libcPutChar ( ' ' );
	}
	libcPrintf ( "\n" );

}

static int8_t handleKeyboard ( char key )
{
	if ( key == 'a' ) {
		addPhilosopher();
	} else if ( key == 'r' ) {
		removePhilosopher();
	} else if ( key == 'e' ) {
		return FINISH;
	}
	return CONTINUE;
}



static void displayHeader()
{
	libcPrintf ( "\n\tInstructions:\n" );
	libcPrintf ( "\tPress - a - Add philosopher\n" );
	libcPrintf ( "\tPress - r - Remove philosopher\n" );
	libcPrintf ( "\tPress - e - Exit\n" );
	libcPrintf ( "\tState of philos_array:\n" );
	libcPrintf ( " - E - Comiendo\n" );
	libcPrintf ( " - . - Pensando\n\n\n" );
}

static void closeMutexes()
{
	libcSemClose ( num_philos_mutex );
	libcSemClose ( added_min_sem );
	libcSemClose ( last_mutex );
	libcSemClose ( second_last_mutex );
	libcSemClose ( state_mutex );
	libcSemClose ( added_min_sem );
}

static void cleanAll()
{

	libcSemWait ( num_philos_mutex );
	for ( int i = 0; i < num_philosophers; i++ ) {
		libcKill ( philosophers_pids[i] );
	}
	for ( int i = 0; i < num_philosophers; i++ ) {
		libcSemClose ( forks[i] );
	}
	closeMutexes();

	return;
}

static int64_t openMutexes()
{
	state_mutex = libcSemOpenGetId ( 1 );
	if ( state_mutex == -1 ) {
		return -1;
	}
	added_min_sem = libcSemOpenGetId ( 0 );
	if ( added_min_sem == -1 ) {
		libcSemClose ( state_mutex );
		return -1;
	}
	num_philos_mutex = libcSemOpenGetId ( 1 );
	if ( num_philos_mutex == -1 ) {
		libcSemClose ( state_mutex );
		libcSemClose ( added_min_sem );
		return -1;
	}
	last_mutex = libcSemOpenGetId ( 1 );
	if ( last_mutex == -1 ) {
		libcSemClose ( state_mutex );
		libcSemClose ( added_min_sem );
		libcSemClose ( num_philos_mutex );
		return -1;
	}
	second_last_mutex = libcSemOpenGetId ( 1 );
	if ( second_last_mutex == -1 ) {
		libcSemClose ( state_mutex );
		libcSemClose ( added_min_sem );
		libcSemClose ( num_philos_mutex );
		libcSemClose ( last_mutex );
		return -1;
	}
	return 0;
}

int64_t phylo ( char ** argv, int argc )
{

	if ( openMutexes() == -1 ) {
		libcfPrintf ( STDERR, "Error abriendo sem\n" );
		return -1;
	}

	displayHeader();
	if ( addFirstPhylos() == -1 ) {
		libcfPrintf ( STDERR, "Error agregando los primeros filosofos\n" );
		closeMutexes();
		return -1;
	}

	int8_t flag = 1;
	char key;
	while ( flag ) {
		key = libcGetChar();
		flag = handleKeyboard ( key );
	}

	cleanAll();

	return 0;
}









