#include <test_prio.h>

int64_t prio[TOTAL_PROCESSES] = {LOW, MEDIUM, HIGH};

int64_t test_prio()
{
	tPid pids[TOTAL_PROCESSES];

	char * argv[] = {"test_prio_aux", WAIT_STRING};
	uint64_t i;

	libcPrintf ( "\n" );
	for ( i = 0; i < TOTAL_PROCESSES; i++ ) {
		tFd fds[] = {STDOUT, STDERR, -1};
		pids[i] = libcCreateProcess ( ( mainFunction ) endless_loop_print_main, 0, argv, 2, fds );
		if ( pids[i] < 0 ) {
			libcfPrintf ( STDERR, "Error: Failed to create process number %d\n", i + 1 );
		}
	}
	bussy_wait ( WAIT );
	libcPrintf ( "\nCHANGING PRIORITIES ...\n" );

	for ( i = 0; i < TOTAL_PROCESSES; i++ ) {
		libcNice ( pids[i], prio[i] );
		libcPrintf ( "Pid %d with priority %s\n", pids[i], prio[i] == 0 ? "LOW" : ( prio[i] == 1 ? "MEDIUM" : "HIGH" ) );
	}
	bussy_wait ( WAIT );
	libcPrintf ( "\nBLOCKING...\n" );

	for ( i = 0; i < TOTAL_PROCESSES; i++ ) {
		libcBlock ( pids[i] );
	}

	libcPrintf ( "CHANGING PRIORITIES WHILE BLOCKED (TO MEDIUM)...\n" );

	for ( i = 0; i < TOTAL_PROCESSES; i++ ) {
		libcNice ( pids[i], MEDIUM );
	}
	libcPrintf ( "UNBLOCKING...\n" );

	for ( i = 0; i < TOTAL_PROCESSES; i++ ) {
		libcUnblock ( pids[i] );
	}
	bussy_wait ( WAIT );
	libcPrintf ( "\nKILLING...\n" );

	for ( i = 0; i < TOTAL_PROCESSES; i++ ) {
		libcKill ( pids[i] );
	}
	libcKill ( libcGetPid() );
	return 0;
}