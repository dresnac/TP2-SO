#include <test_proc.h>

enum State { STATE_RUNNING,
             STATE_BLOCKED,
             STATE_KILLED
           };

typedef struct p_rq {
	tPid pid;
	enum State state;
} p_rq;


int64_t test_processes ( char * argv[], uint64_t argc )
{
	uint64_t rq;
	uint64_t alive = 0;
	uint64_t action;
	int64_t max_processes;
	int64_t satoi_flag;
	if ( argc != 2 || ( max_processes = libcSatoi ( argv[1], &satoi_flag ) ) < 0 || !satoi_flag ) {
		libcfPrintf ( STDERR, "Usage: testproc <max_processes>\n" );
		return -1;
	}

	if ( max_processes > 20 || max_processes == 0 ) {
		libcfPrintf ( STDERR, "<max_processes> must be between 1 and 20\n" );
		return -1;
	}

	p_rq p_rqs[max_processes];

	while ( 1 ) {
		for ( rq = 0; rq < max_processes; rq++ ) {
			tFd fds[] = {STDOUT, STDERR, -1};
			char * argv[] = {"test_proc_aux"};
			p_rqs[rq].pid = libcCreateProcess ( ( mainFunction ) endless_loop, 0, argv, 1, fds );
			if ( p_rqs[rq].pid < 0 ) {
				libcfPrintf ( STDERR, "ERROR creating process\n" );
				return -1;
			} else {
				p_rqs[rq].state = STATE_RUNNING;
				alive++;
			}
		}

		while ( alive > 0 ) {

			for ( rq = 0; rq < max_processes; rq++ ) {
				action = get_uniform ( 100 ) % 2;

				switch ( action ) {
				case 0:
					if ( p_rqs[rq].state == STATE_RUNNING || p_rqs[rq].state == STATE_BLOCKED ) {
						if ( libcKill ( p_rqs[rq].pid ) == -1 ) {
							libcfPrintf ( STDERR, "ERROR killing process\n" );
							return -1;
						}
						p_rqs[rq].state = STATE_KILLED;
						alive--;

					}
					break;

				case 1:
					if ( p_rqs[rq].state == STATE_RUNNING ) {
						if ( libcBlock ( p_rqs[rq].pid ) == -1 ) {
							libcfPrintf ( STDERR, "ERROR blocking process\n" );
							return -1;
						}
						p_rqs[rq].state = STATE_BLOCKED;
					}
					break;
				}
			}

			for ( rq = 0; rq < max_processes; rq++ )
				if ( p_rqs[rq].state == STATE_BLOCKED && get_uniform ( 100 ) % 2 ) {
					if ( libcUnblock ( p_rqs[rq].pid ) == -1 ) {
						libcfPrintf ( STDERR, "ERROR unblocking process\n" );
						return -1;
					}
					p_rqs[rq].state = STATE_RUNNING;
				}
		}
	}
}
