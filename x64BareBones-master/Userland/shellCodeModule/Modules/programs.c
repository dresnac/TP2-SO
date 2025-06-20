#include <programs.h>

int64_t loop ( char ** argv, int argc )
{
	if ( argc != 2 ) {
		libcfPrintf ( STDERR, "Uso: loop <seconds>\n" );
		return 1;
	}
	int64_t satoi_flag;
	int64_t seconds = libcSatoi ( argv[1], &satoi_flag );
	if ( seconds < 0 || !satoi_flag ) {
		libcfPrintf ( STDERR, "Error: Cantidad de segundos invalida\n" );
		return 1;
	}
	int64_t ans = 1;
	while ( ans > 0 ) {
		libcSleep ( seconds );
		ans = libcPrintf ( "Hola! soy un loop con pid: %d\n", libcGetPid() );
	}
	return 0;
}

int64_t wc ( char ** argv, int argc )
{
	if ( argc != 1 ) {
		libcfPrintf ( STDERR, "Uso: wc\n" );
		return 1;
	}
	char c;
	char prev = 0;
	uint64_t lines = 1;
	while ( ( c = libcGetChar() ) != 0 ) {
		libcPutChar ( c );
		if ( c == '\n' ) {
			lines = lines + 1;
		}
		prev = c;
	}

	if ( prev == 0 || prev == '\n' ) {
		lines--;
	}
	libcPrintf ( "\nLineas: %d\n", lines );
	return 0;
}


int64_t filter()
{
	char ans;
	while ( ( ans = libcGetChar() ) > 0 ) {
		if ( ! ( ans == 'a' || ans == 'A' || ans == 'e' || ans == 'E' || ans == 'i' || ans == 'I' || ans == 'o' || ans == 'O' || ans == 'u' || ans == 'U' ) ) {
			libcPrintf ( "%c", ans );
		}
	}
	return 0;
}

int64_t cat()
{
	char ans;
	while ( ( ans = libcGetChar() ) > 0 ) {
		libcPrintf ( "%c", ans );
	}
	libcPrintf ( "\n" );
	return 0;
}


int64_t psProgram()
{

	ProcessInfoList * process_list = sys_ps();
	if ( process_list == NULL ) {
		libcfPrintf ( STDERR, "Error al obtener lista de procesos\n" );
		return 1;
	}
	libcPrintf ( "Cantidad de procesos: %d\n", process_list->amount_of_processes );
	libcPrintf ( "PID | Ground      | Prio  | Stack Base Ptr| Last Stack Addr |    RSP     | Status | STDOUT| STDERR| STDIN | Name       \n" );
	libcPrintf ( "----|-------------|-------|---------------|-----------------|------------|--------|-------|-------|-------|-----------\n" );

	for ( int i = 0; i < process_list->amount_of_processes; i++ ) {

		if ( process_list->processes[i].pid < 10 ) {
			libcPrintf ( "%d   | ", process_list->processes[i].pid );
		} else {
			libcPrintf ( "%d  | ", process_list->processes[i].pid );
		}

		libcPrintf ( "%s  | ", process_list->processes[i].is_background ? "Background" : "Foreground" );
		libcPrintf ( "%s| ", process_list->processes[i].priority == LOW ? "Low   " : ( process_list->processes[i].priority == MEDIUM ? "Medium" : "High  " ) );
		libcPrintf ( "0x%x    | ", process_list->processes[i].lowest_stack_address + STACK_SIZE );
		libcPrintf ( "0x%x      | ", process_list->processes[i].lowest_stack_address );
		libcPrintf ( "0x%x | ", process_list->processes[i].stack_pointer );
		libcPrintf ( "%s| ", process_list->processes[i].status == READY ? "Ready  " : ( process_list->processes[i].status == ZOMBIE ? "Zombie " : "Blocked" ) );

		for ( int j = 0; j < CANT_FDS; j++ ) {
			char * fds_print[] = {"STDOUT", "STDERR", "STDIN "};
			if ( process_list->processes[i].fds[j] == -1 ) {
				libcPrintf ( "  -   | " );
			} else if ( process_list->processes[i].fds[j] >= 0 && process_list->processes[i].fds[j] <= MAX_COMMON_FD ) {
				libcPrintf ( "%s| ", fds_print[process_list->processes[i].fds[j]] );
			} else {
				int to_print =  process_list->processes[i].fds[j];
				if ( to_print < 10 ) {
					libcPrintf ( "  %d   | ", to_print );
				} else {
					libcPrintf ( "  %d  | ", to_print );
				}
			}
		}
		libcPrintf ( "%s\n", process_list->processes[i].name ? process_list->processes[i].name : "No name" );

	}
	sys_free_ps ( process_list );
	return 0;
}

int64_t mem()
{
	MemoryInfo info[2];
	if ( sys_mem_info ( info ) < 0 ) {
		libcfPrintf ( STDERR, "Error al obtener mem info\n" );
		return 1;
	}
	libcPrintf ( "Memoria total en userland: %x\n", info[USER_MEM_INFO].total_size );
	libcPrintf ( "Memoria libre en userland: %x\n", info[USER_MEM_INFO].free );

	libcPrintf ( "\nMemoria total en kernel: %x\n", info[KERNEL_MEM_INFO].total_size );
	libcPrintf ( "Memoria libre en kernel: %x\n", info[KERNEL_MEM_INFO].free );
	return 0;
}