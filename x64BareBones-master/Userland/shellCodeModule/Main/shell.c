#include <shell.h>

typedef struct {
	char **args[2];
	uint64_t argc[2];
} Command;

static const char *colidio_ascii[] = {
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%#*****++*##%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#++**##***+++=*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%##**#%%%%@%%%###*#%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@###***#%#*****#%##%%%#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#*####%#+=++===+**++#%%@%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%#%%%%%*===+***+===+=*%%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%#*++==*#%%%#+=*#*+#%%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#%%*+===-+**%##+==*%*+%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@##*===---===++*+=++++-#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%**+==--====++####**+-+@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*+*+====++++*++***+*-+@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%**+**=++****+#%%%#==*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#%**#+++*++*#@@@@#-=%@@@%%%%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%@\n",
"%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@@@%%%%#-==+++++**%@@%+-#@@@@%%%%%%%%%@@@@@@@@@@@@@@@@@@@@@@@%%%%%%%\n",
"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#-==+++++***##*-*@@@@@@%###%%%%%%%@@@%%%%%%%%%%%%%%%%%%%%%%%%\n",
"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*:=+++++++**#*==%@@@@@@%#####%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n",
"%%%%%#########%%%%%%%%%%%%%%%%%%%%%%%#=::=++**+*****+=#@@@@@%%%######%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n",
"%%%%%############%%%%%########%%##**+-::-==+**+#####+*++#%#***########%%%###%%%%%%%##########%%%%###\n",
"%%%%%%%#########%%%%%%%%%%%%%#*+======+-:-=+***##%*=-##:=+++++*################%%%%%################\n",
"%%%%%%%%%####%%%%%%%%%%%%%#**+++*+++===+=--+**++*#*=-#+-======++*#%%%%%%######%%%%%%%%%%%%%%%%%#####\n",
"%%%%%%%%%%%%%%%%%%%%%%%%*+=+=+=:++==-----==-=*+=*#+--*-+====**++=--=*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n",
"@@@@@@%%%%%%%%%%%%%%@%#+=+++=--::---:::::::=--+=+*=-+===-=-+@@@%%%*=:-#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n",
"@@@@@@@@@@@@@@@@@@@@%=-=++*-::-:=-:--::::-:::::-=+=+=:::::=%%%%%%%%%*.=%%%%%%%%@@@@@@@@@@%%%%%%%%%%%\n",
"@@@@@@@@@@@@@@@@@@@@-:-+-#=::---=:-=-::::::::::::-=-.:::::*%%%%%%%%%%=:#@%%%@@@@@@@@@@@@@@@@@@%%%%%%\n",
"@@@@@@@@@@@@@@@@@@@=.++**#--:--=---=-::::::::::::::::::::+#==+###%@#++:=@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@@#.+%*+#@#=---=-=:=::::-=::::::::::::::-##--+**+%#-+=:-%@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@@--%#++#@@+=--+=+:=:::=+**=::::::::::::*%%*=**+-#--+-:-%@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@#.-*####@#====*=+:=:-=*#+*#+-:::::::::+%###*++--*-+=:--%@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@*.:-+*#*+=====++=:--=+-==----:------:=######*=:++-=-:--%@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@#:.:::-------=++-:=---::::------:----########***=--:---%@@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@#.:-=====---:-*+-:==----:------:----*#######*##-=--:--:=%@@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@%-++***++=::--+#-:==-:::::::-------*##%%####*#=-=----:::+%@@@@@@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@@@@@@@@@@@@@#::-===--=::--=%--=+---+++=-***+-+#####***##*+===---:::::+%%%%%@@@@@@@@@@@@@@@@@@@@\n",
"@@@@@%%%%%%@@@@@@*.::-----:::::=#-+=*=-*+:--=*--#=*####*#*###*+===---:::::-#@%%%%@@@@@@@@@@@@@@@@@%%\n",
"@@@@@%%%%%%%%%%%@*.:--===+***=--%=*=++:=*=++-****=*##########+-=+--==-:::::+%%%%%@@@@@@@@@@@@@@@@@%%\n"};


static void help();
static void kill_pid ( char ** argv, uint64_t argc );
static void to_utc_minus_3 ( LocalTime * time );
static void free_args ( char ** args, uint64_t argc );
static void free_cmd_args ( Command * cmd );
static int64_t call_function_process ( module m, char ** args, uint64_t argc, tFd fds[CANT_FDS] );
static int64_t piped_command_parse ( char shell_buffer[], Command *cmd );
static char ** command_parse ( char shell_buffer[], uint64_t *argc, int64_t *pipe_pos, int64_t *pipe_count );
static void interpret();
static void help();
static void show_current_time();
static void show_current_time();
static void get_regs();
static void shell_wait_pid ( char ** args, uint64_t argc );
static void shell_nice ( char **argv, uint64_t argc );
static void shell_block ( char **argv, uint64_t argc );


static uint64_t font_size = 1;
#define BUILT_IN 1



static module modules[] = {
	{"help", "", "Presenta funciones disponibles", help, BUILT_IN},
	{"time", "", "Presenta la hora en pantalla", show_current_time, BUILT_IN},
	{"getregs", "", "Presenta estado actual de los registros", get_regs, BUILT_IN},
	{"clear", "", "Limpia la pantalla", ( void ( * ) ( char **, uint64_t ) ) libc_clear_screen, BUILT_IN},
	{"kill", " <PID>: ", "Termina un proceso dado su PID ", kill_pid, BUILT_IN},
	{"block", " <PID>: ", "Cambia el estado de un proceso de READY a BLOCKED segun su PID ", shell_block, BUILT_IN},
	{"wait", " <PID>: ", "Espera a un proceso segun su PID", shell_wait_pid, BUILT_IN},
	{"nice", " <PID> <nueva_prio>: ", "Cambia la prioridad de un proceso segun su PID y nueva prioridad", shell_nice, BUILT_IN},
	{"ps", "", "Presenta la informacion de los procesos", ( void ( * ) ( char **, uint64_t ) ) ps_program, !BUILT_IN},
	{"phylo", "", "Problema de los filosofos hambirentos", ( void ( * ) ( char **, uint64_t ) ) phylo, !BUILT_IN},
	{"cat", "", "Imprime el STDIN exactamente como es recibido", ( void ( * ) ( char **, uint64_t ) ) cat, !BUILT_IN},
	{"loop", " <segundos>: ", "Loop que saluda con su PID segun tiempo dado", ( void ( * ) ( char **, uint64_t ) ) loop, !BUILT_IN},
	{"filter", "", "Elimina vocales del texto ingresado", ( void ( * ) ( char **, uint64_t ) ) filter, !BUILT_IN},
	{"wc", "", "Cuenta cantidad de lineas ingresadas", ( void ( * ) ( char **, uint64_t ) ) wc, !BUILT_IN},
	{"mem", "", "Presenta el estado de memoria", ( void ( * ) ( char **, uint64_t ) ) mem, !BUILT_IN},
	{"testproc", " <procesos_max>: ", "Test para creacion de proceso", ( void ( * ) ( char **, uint64_t ) ) test_processes, !BUILT_IN},
	{"testsync", " <n> <uso_sem (0 false, int true)>: ", "Test para sincronizacion de procesos", ( void ( * ) ( char **, uint64_t ) ) test_sync, !BUILT_IN},
	{"testmm", " <maxima_mem>: ", "Test para el uso de malloc y free", ( void ( * ) ( char **, uint64_t ) ) test_mm, !BUILT_IN},
	{"testprio", "", "Test para las prioridades del scheduler", ( void ( * ) ( char **, uint64_t ) ) test_prio, !BUILT_IN},
};




int main()
{
	libc_set_font_size ( font_size );
	libc_puts ( WELCOME );
	libc_puts("\n");
	for(int i=0; i<36; i++){
		libc_puts(colidio_ascii[i]);
	}
	libc_sleep(3);
	libc_clear_screen();
	help();
	while ( 1 ) {
		interpret();
	}
}

static void free_args ( char ** args, uint64_t argc )
{
	if ( args == NULL ) {
		return;
	}
	for ( int i = 0; i < argc; i++ ) {
		libc_free ( args[i] );
	}
	libc_free ( args );
	return;
}

static void free_cmd_args ( Command * cmd )
{
	if ( cmd == NULL ) {
		return;
	}
	free_args ( cmd->args[0], cmd->argc[0] );
	free_args ( cmd->args[1], cmd->argc[1] );
}


static int64_t call_function_process ( module m, char ** args, uint64_t argc, tFd fds[CANT_FDS] )
{
	if ( m.is_built_in ) {
		m.function ( args, argc );
		free_args ( args, argc );
		return 0;
	}
	int64_t ans = libc_create_process ( ( mainFunction ) m.function, LOW, args, argc, fds );

	if ( ans < 0 ) {
		libc_fprintf ( STDERR, "Error: No se pudo crear el proceso\n" );
	}
	free_args ( args, argc );
	return ans;
}


static char ** command_parse ( char shell_buffer[], uint64_t *argc, int64_t *pipe_pos, int64_t *pipe_count )
{
	if ( argc == NULL || pipe_pos == NULL || pipe_count == NULL ) {
		return NULL;
	}
	*pipe_pos = 0;

	if ( shell_buffer[0] == '|' ) {
		( *pipe_count )++;
		return NULL;
	}

	char **args = libc_malloc ( MAX_ARGS * sizeof ( char * ) );
	if ( args == NULL ) {
		*argc = -1;
		return NULL;
	}

	uint64_t args_count = 0;
	int i = 0;

	while ( shell_buffer[i] != '\0' ) {
		while ( shell_buffer[i] == ' ' ) {
			i++;
		}
		if ( shell_buffer[i] == '|' ) {
			( *pipe_count )++;
			*pipe_pos = i + 1;
			break;
		}

		if ( shell_buffer[i] == '\0' ) {
			break;
		}

		args[args_count] = libc_malloc ( MAX_ARGS_SIZE * sizeof ( char ) );
		if ( args[args_count] == NULL ) {
			for ( int n = 0; n < args_count; n++ ) {
				libc_free ( args[n] );
			}
			libc_free ( args );
			*argc = -1;
			return NULL;
		}

		int j = 0;
		while ( shell_buffer[i] != ' ' && shell_buffer[i] != '\0' && shell_buffer[i] != '|' ) {
			args[args_count][j++] = shell_buffer[i++];
		}
		args[args_count][j] = '\0';
		args_count++;
	}

	*argc = args_count;

	if ( args_count == 0 ) {
		libc_free ( args );
		args = NULL;
	}

	return args;
}

static int64_t piped_command_parse ( char shell_buffer[], Command *cmd )
{
	if ( cmd == NULL ) {
		return -1;
	}
	cmd->args[0] = cmd->args[1] = NULL;
	cmd->argc[0] = cmd->argc[1] = 0;

	int64_t pipe_pos = -1, pipe_count = 0;

	cmd->args[0] = command_parse ( shell_buffer, &cmd->argc[0], &pipe_pos, &pipe_count );
	if ( cmd->args[0] == NULL ) {
		return -1;
	}

	if ( pipe_pos > 0 ) {
		cmd->args[1] = command_parse ( shell_buffer + pipe_pos, &cmd->argc[1], &pipe_pos, &pipe_count );
		if ( cmd->args[1] == NULL || cmd->argc[1] == 0 ) {
			free_cmd_args ( cmd );
			return -1;
		}
	}
	if ( pipe_count > 1 ) {
		free_cmd_args ( cmd );
		return -1;
	}

	return 0;
}

static int parse_command_and_get_modules ( Command *cmd, int found_idx[] )
{
	int has_pipe = cmd->args[1] != NULL;
	int look_for_max = has_pipe ? 2 : 1;

	for ( int j = 0; j < look_for_max; j++ ) {
		for ( int i = 0; i < MAX_MODULES && cmd->args[j] != NULL && cmd->argc[j] != 0; i++ ) {
			if ( libc_strcmp ( cmd->args[j][0], modules[i].name ) == 0 ) {
				found_idx[j] = i;
				break;
			}
		}
	}
	return has_pipe;
}

static void interpret()
{
	libc_puts ( PROMPT );
	char shell_buffer[MAX_COMMAND_SIZE];
	libc_gets ( shell_buffer, MAX_COMMAND_SIZE );
	if ( shared_libc_strlen ( shell_buffer ) == 0 ) {
		return;
	}

	Command cmd;
	if ( piped_command_parse ( shell_buffer, &cmd ) != 0 ) {
		libc_fprintf ( STDERR, "Comando invalido, intente nuevamente >:(\n" );
		return;
	}
	if ( cmd.argc[0] == -1 || cmd.argc[1] == -1 ) {
		libc_fprintf ( STDERR, "No hay suficiente memoria para crear proceso\n" );
	}

	int found_idx[2] = {-1, -1};
	int has_pipe = parse_command_and_get_modules ( &cmd, found_idx );

	if ( found_idx[0] == -1 ) {
		libc_fprintf ( STDERR, "Comando invalido, intente nuevamente >:(\n" );
		return;
	}

	tFd fds[] = {STDOUT, STDERR, STDIN};
	tFd writer_fds[] = {STDOUT, STDERR, STDIN};
	tFd reader_fds[] = {STDOUT, STDERR, STDIN};

	if ( !has_pipe ) {
		uint8_t is_bckg = ( libc_strcmp ( cmd.args[0][cmd.argc[0] - 1], "&" ) == 0 );
		if ( is_bckg && modules[found_idx[0]].is_built_in ) {
			libc_fprintf ( STDERR, "Error: No se puede usar built-in en background\n" );
			free_cmd_args ( &cmd );
			return;
		}
		if ( is_bckg ) {
			fds[STDIN] = -1;
			libc_free ( cmd.args[0][cmd.argc[0] - 1] );
			cmd.argc[0]--;
		}
		int64_t pid = call_function_process ( modules[found_idx[0]], cmd.args[0], cmd.argc[0], fds );
		if ( !is_bckg && pid > 0 ) {
			libc_wait ( pid, NULL );
		} else if ( is_bckg ) {
			libc_printf ( "Pid: %d en background\n", pid );
		}
		return;
	}
	if ( found_idx[1] == -1 ) {
		libc_fprintf ( STDERR, "Error: Comando invalido, intente nuevamente >:(\n" );
		return;
	}
	if ( modules[found_idx[0]].is_built_in || modules[found_idx[1]].is_built_in ) {
		libc_fprintf ( STDERR, "No se puede usar built-in en pipe\n" );
		free_cmd_args ( &cmd );
		return;
	}
	tFd fd = libc_pipe_reserve();

	if ( fd < 0 ) {
		libc_fprintf ( STDERR, "Error: No se pudo abrir pipe\n" );
		free_cmd_args ( &cmd );
		return;
	}

	uint8_t is_bckg = ( libc_strcmp ( cmd.args[1][cmd.argc[1] - 1], "&" ) == 0 ) && !modules[found_idx[1]].is_built_in;
	if ( is_bckg ) {
		libc_free ( cmd.args[1][cmd.argc[1] - 1] );
		cmd.argc[1]--;
	}
	writer_fds[STDOUT] = fd;
	int64_t pid1 = call_function_process ( modules[found_idx[0]], cmd.args[0], cmd.argc[0], writer_fds );
	reader_fds[STDIN] = fd;
	int64_t pid2 = call_function_process ( modules[found_idx[1]], cmd.args[1], cmd.argc[1], reader_fds );
	if ( !is_bckg && pid2 > 0 ) {
		libc_wait ( pid2, NULL );
		libc_wait ( pid1, NULL );
	} else if ( is_bckg ) {
		libc_printf ( "pid: %d y %d en background.\n", pid1, pid2 );
	}
	return;



}

static void kill_pid ( char ** argv, uint64_t argc )
{
	tPid pid;
	int64_t satoi_flag;
	if ( argc != 2 || argv == NULL || ( ( pid = libc_satoi ( argv[1], &satoi_flag ) ) < 0 ) || !satoi_flag ) {
		libc_fprintf ( STDERR, "Uso: kill <pid>\n" );
		return;
	}

	if ( libc_kill ( pid ) < 0 ) {
		libc_fprintf ( STDERR, "Error: No se pudo terminar proceso %d\n", pid );
		return;
	}
	libc_printf("Proceso con pid: %d terminado\n", pid);

}

static void shell_wait_pid ( char ** args, uint64_t argc )
{
	if ( argc != 2 ) {
		libc_fprintf ( STDERR, "Uso: wait <pid>\n" );
		return;
	}
	tPid pid;
	int64_t satoi_flag;
	if ( ( pid = libc_satoi ( args[1], &satoi_flag ) ) < 0 || !satoi_flag ) {
		libc_fprintf ( STDERR, "Error: pid debe ser positivo\n" );
		return;
	}
	int64_t ret;
	tPid ans_pid;
	ans_pid = libc_wait ( pid, &ret );
	if ( ans_pid == -1 ) {
		libc_fprintf ( STDERR, "Error: No se pudo esperar pid %d\n", pid );
		return;
	}
	libc_printf ( "Pid %d devolvio %d\n", ans_pid, ret );
	return;
}

static void help()
{
	libc_printf ( "\n\n-------------------------------                     Funciones Built-in                      -----------------------------------\n\n" );
	libc_printf ("  Comando   Argumentos                              Descripcion\n\n");
	for ( int i = 0; i < MAX_MODULES; i++ ) {
		if ( i == NUM_BUILT_INS ) {
			libc_printf ( "\n\n-------------------------------                      Procesos                                ----------------------------------\n\n" );
			libc_printf ("  Comando   Argumentos                              Descripcion\n\n");
		}
		if ( i == MAX_MODULES - NUM_TESTS ) {
			libc_printf ( "\n\n-------------------------------                      Tests de funcionalidad                      ----------------------------------\n\n" );
			libc_printf ("  Comando   Argumentos                              Descripcion\n\n");
		}
		int len_name = libc_strlen(modules[i].name);
		libc_printf("  %s", modules[i].name);
		for (int s = len_name; s < 10; s++)
			libc_printf(" ");

		int len_args = libc_strlen(modules[i].args);
		libc_printf("%s", modules[i].args);
		for (int s = len_args; s < 40; s++)
			libc_printf(" ");

		libc_printf("%s\n", modules[i].desc);
		//libc_printf ( "- %s%s%s%s\n", modules[i].name, libc_strcmp ( modules[i].args, "" ) == 0 ? ": " : "", modules[i].args, modules[i].desc );
	}
	libc_printf ( "-------------------------------                                                              -----------------------------------\n\n" );
}



static void show_current_time()
{
	LocalTime time;
	libc_get_time ( &time );
	to_utc_minus_3 ( &time );
	libc_printf ( "%d/%d/%d [d/m/a]\n", time.day, time.month, time.year );
	int64_t h = time.hour;
	libc_printf ( "%d:%d:%d [hora/min/seg] (Argentina)\n", h, time.minutes, time.seconds ); 

	return;
}


static void to_utc_minus_3 ( LocalTime * time )
{

	if ( time->hour < 3 ) {
		time->hour += 21;
		time->day--;
		if ( time->day == 0 ) {
			time->month--;
			if ( time->month == 0 ) {
				time->month = 12;
				time->year--;
			}
			if ( time->month == 2 ) {
				time->day = 28;
				if ( time->year % 4 == 0 ) {
					time->day = 29;
				}
			} else if ( time->month == 4 || time->month == 6 || time->month == 9 || time->month == 11 ) {
				time->day = 30;
			} else {
				time->day = 31;
			}
		}
	} else {
		time->hour = time->hour - 3;
	}
}


static void get_regs()
{
	libc_print_register_snapshot();
	return;
}



static void shell_nice ( char **argv, uint64_t argc )
{
	tPid pid;
	int64_t satoi_flag;
	if ( argc != 3 || ( ( pid = libc_satoi ( argv[1], &satoi_flag ) ) < 0 ) || !satoi_flag ) {
		libc_fprintf ( STDERR, "Uso: nice <pid> <nueva_prioridad>\n" );
		return;
	}
	tPriority prio;
	if ( libc_strnocasecmp ( argv[2], "low" ) == 0 ) {
		prio = LOW;
	} else if ( libc_strnocasecmp ( argv[2], "medium" ) == 0 ) {
		prio = MEDIUM;
	} else if ( libc_strnocasecmp ( argv[2], "high" ) == 0 ) {
		prio = HIGH;
	} else {
		libc_fprintf ( STDERR, "Prioridad invalida. Use 'low', 'medium', o 'high' para <nueva_prioridad>.\n" );
		return;
	}
	if ( libc_nice ( pid, prio ) < 0 ) {
		libc_fprintf ( STDERR, "Error: No se pudo cambiar la prioridad de pid %d.\n", pid );
	}
}


static void shell_block ( char **argv, uint64_t argc )
{
	tPid pid;
	int64_t satoi_flag;
	if ( argc != 2 || ( ( pid = libc_satoi ( argv[1], &satoi_flag ) ) < 0 ) || !satoi_flag ) {
		libc_fprintf ( STDERR, "Uso: block <pid>\n" );
		return;
	}
	int8_t status = libc_get_status ( pid );
	if (	( status != BLOCKED ) && ( status != READY ) ) {
		libc_fprintf ( STDERR, "Error: El estado de pid %d no es READY ni BLOCKED \n", pid );
		return;
	}
	if ( status == BLOCKED ) {
		libc_unblock ( pid );
	} else {
		libc_block ( pid );
	}
}
