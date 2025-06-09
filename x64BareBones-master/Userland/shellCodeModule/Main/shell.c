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
static void killPid ( char ** argv, uint64_t argc );
static void toUtcMinus3 ( LocalTime * time );
static uint8_t diasEnMes(uint8_t mes, uint8_t anio);
static void freeArgs ( char ** args, uint64_t argc );
static void freeCmdArgs ( Command * cmd );
static int64_t callFunctionProcess ( module m, char ** args, uint64_t argc, tFd fds[CANT_FDS] );
static int64_t pipedCommandParse ( char shell_buffer[], Command *cmd );
static char ** commandParse ( char shell_buffer[], uint64_t *argc, int64_t *pipe_pos, int64_t *pipe_count );
static void interpret();
static void help();
static void showCurrentTime();
static void showCurrentTime();
static void getRegs();
static void shellWaitPid ( char ** args, uint64_t argc );
static void shellNice ( char **argv, uint64_t argc );
static void shellBlock ( char **argv, uint64_t argc );


static uint64_t font_size = 1;
#define BUILT_IN 1



static module modules[] = {
	{"help", "", "Presenta funciones disponibles", help, BUILT_IN},
	{"time", "", "Presenta la hora en pantalla", showCurrentTime, BUILT_IN},
	{"getregs", "", "Presenta estado actual de los registros", getRegs, BUILT_IN},
	{"clear", "", "Limpia la pantalla", ( void ( * ) ( char **, uint64_t ) ) libcClearScreen, BUILT_IN},
	{"kill", " <PID> ", "Termina un proceso dado su PID ", killPid, BUILT_IN},
	{"block", " <PID> ", "Cambia el estado de un proceso de READY a BLOCKED segun su PID ", shellBlock, BUILT_IN},
	{"wait", " <PID> ", "Espera a un proceso segun su PID", shellWaitPid, BUILT_IN},
	{"nice", " <PID> <nueva_prio> ", "Cambia la prioridad de un proceso segun su PID y nueva prioridad", shellNice, BUILT_IN},
	{"ps", "", "Presenta la informacion de los procesos", ( void ( * ) ( char **, uint64_t ) ) psProgram, !BUILT_IN},
	{"phylo", "", "Problema de los filosofos hambirentos", ( void ( * ) ( char **, uint64_t ) ) phylo, !BUILT_IN},
	{"cat", "", "Imprime el STDIN exactamente como es recibido", ( void ( * ) ( char **, uint64_t ) ) cat, !BUILT_IN},
	{"loop", " <segundos> ", "Loop que saluda con su PID segun tiempo dado", ( void ( * ) ( char **, uint64_t ) ) loop, !BUILT_IN},
	{"filter", "", "Elimina vocales del texto ingresado", ( void ( * ) ( char **, uint64_t ) ) filter, !BUILT_IN},
	{"wc", "", "Cuenta cantidad de lineas ingresadas", ( void ( * ) ( char **, uint64_t ) ) wc, !BUILT_IN},
	{"mem", "", "Presenta el estado de memoria", ( void ( * ) ( char **, uint64_t ) ) mem, !BUILT_IN},
	{"testproc", " <procesos_max> ", "Test para creacion de proceso", ( void ( * ) ( char **, uint64_t ) ) test_processes, !BUILT_IN},
	{"testsync", " <n> <uso_sem (0 false, int true)> ", "Test para sincronizacion de procesos", ( void ( * ) ( char **, uint64_t ) ) test_sync, !BUILT_IN},
	{"testmm", " <maxima_mem> ", "Test para el uso de malloc y free", ( void ( * ) ( char **, uint64_t ) ) test_mm, !BUILT_IN},
	{"testprio", "", "Test para las prioridades del scheduler", ( void ( * ) ( char **, uint64_t ) ) test_prio, !BUILT_IN},
};




int main()
{
	libcSetFontSize ( font_size );
	libcPuts ( WELCOME );
	libcPuts("\n");
	for(int i=0; i<36; i++){
		libcPuts((char*)colidio_ascii[i]);
	}
	libcPlaySimpleMelody();
	//libcSleep(3);
	libcClearScreen();
	help();
	while ( 1 ) {
		interpret();
	}
}

static void freeArgs ( char ** args, uint64_t argc )
{
	if ( args == NULL ) {
		return;
	}
	for ( int i = 0; i < argc; i++ ) {
		libcFree ( args[i] );
	}
	libcFree ( args );
	return;
}

static void freeCmdArgs ( Command * cmd )
{
	if ( cmd == NULL ) {
		return;
	}
	freeArgs ( cmd->args[0], cmd->argc[0] );
	freeArgs ( cmd->args[1], cmd->argc[1] );
}


static int64_t callFunctionProcess ( module m, char ** args, uint64_t argc, tFd fds[CANT_FDS] )
{
	if ( m.is_built_in ) {
		m.function ( args, argc );
		freeArgs ( args, argc );
		return 0;
	}
	int64_t ans = libcCreateProcess ( ( mainFunction ) m.function, LOW, args, argc, fds );

	if ( ans < 0 ) {
		libcfPrintf ( STDERR, "Error: No se pudo crear el proceso\n" );
	}
	freeArgs ( args, argc );
	return ans;
}


static char ** commandParse ( char shell_buffer[], uint64_t *argc, int64_t *pipe_pos, int64_t *pipe_count )
{
	if ( argc == NULL || pipe_pos == NULL || pipe_count == NULL ) {
		return NULL;
	}
	*pipe_pos = 0;

	if ( shell_buffer[0] == '|' ) {
		( *pipe_count )++;
		return NULL;
	}

	char **args = libcMalloc ( MAX_ARGS * sizeof ( char * ) );
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

		args[args_count] = libcMalloc ( MAX_ARGS_SIZE * sizeof ( char ) );
		if ( args[args_count] == NULL ) {
			for ( int n = 0; n < args_count; n++ ) {
				libcFree ( args[n] );
			}
			libcFree ( args );
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
		libcFree ( args );
		args = NULL;
	}

	return args;
}

static int64_t pipedCommandParse ( char shell_buffer[], Command *cmd )
{
	if ( cmd == NULL ) {
		return -1;
	}
	cmd->args[0] = cmd->args[1] = NULL;
	cmd->argc[0] = cmd->argc[1] = 0;

	int64_t pipe_pos = -1, pipe_count = 0;

	cmd->args[0] = commandParse ( shell_buffer, &cmd->argc[0], &pipe_pos, &pipe_count );
	if ( cmd->args[0] == NULL ) {
		return -1;
	}

	if ( pipe_pos > 0 ) {
		cmd->args[1] = commandParse ( shell_buffer + pipe_pos, &cmd->argc[1], &pipe_pos, &pipe_count );
		if ( cmd->args[1] == NULL || cmd->argc[1] == 0 ) {
			freeCmdArgs ( cmd );
			return -1;
		}
	}
	if ( pipe_count > 1 ) {
		freeCmdArgs ( cmd );
		return -1;
	}

	return 0;
}

static int parseCommandAndGetModules ( Command *cmd, int found_idx[] )
{
	int has_pipe = cmd->args[1] != NULL;
	int look_for_max = has_pipe ? 2 : 1;

	for ( int j = 0; j < look_for_max; j++ ) {
		for ( int i = 0; i < MAX_MODULES && cmd->args[j] != NULL && cmd->argc[j] != 0; i++ ) {
			if ( libcStrcmp ( cmd->args[j][0], modules[i].name ) == 0 ) {
				found_idx[j] = i;
				break;
			}
		}
	}
	return has_pipe;
}

static void interpret()
{
	libcPuts ( PROMPT );
	char shell_buffer[MAX_COMMAND_SIZE];
	libcGets ( shell_buffer, MAX_COMMAND_SIZE );
	if ( sharedLibcStrlen ( shell_buffer ) == 0 ) {
		return;
	}

	Command cmd;
	if ( pipedCommandParse ( shell_buffer, &cmd ) != 0 ) {
		libcfPrintf ( STDERR, "Comando invalido, intente nuevamente >:(\n" );
		return;
	}
	if ( cmd.argc[0] == -1 || cmd.argc[1] == -1 ) {
		libcfPrintf ( STDERR, "No hay suficiente memoria para crear proceso\n" );
	}

	int found_idx[2] = {-1, -1};
	int has_pipe = parseCommandAndGetModules ( &cmd, found_idx );

	if ( found_idx[0] == -1 ) {
		libcfPrintf ( STDERR, "Comando invalido, intente nuevamente >:(\n" );
		return;
	}

	tFd fds[] = {STDOUT, STDERR, STDIN};
	tFd writer_fds[] = {STDOUT, STDERR, STDIN};
	tFd reader_fds[] = {STDOUT, STDERR, STDIN};

	if ( !has_pipe ) {
		uint8_t is_bckg = ( libcStrcmp ( cmd.args[0][cmd.argc[0] - 1], "&" ) == 0 );
		if ( is_bckg && modules[found_idx[0]].is_built_in ) {
			libcfPrintf ( STDERR, "Error: No se puede usar built-in en background\n" );
			freeCmdArgs ( &cmd );
			return;
		}
		if ( is_bckg ) {
			fds[STDIN] = -1;
			libcFree ( cmd.args[0][cmd.argc[0] - 1] );
			cmd.argc[0]--;
		}
		int64_t pid = callFunctionProcess ( modules[found_idx[0]], cmd.args[0], cmd.argc[0], fds );
		if ( !is_bckg && pid > 0 ) {
			libcWait ( pid, NULL );
		} else if ( is_bckg ) {
			libcPrintf ( "Pid: %d en background\n", pid );
		}
		return;
	}
	if ( found_idx[1] == -1 ) {
		libcfPrintf ( STDERR, "Error: Comando invalido, intente nuevamente >:(\n" );
		return;
	}
	if ( modules[found_idx[0]].is_built_in || modules[found_idx[1]].is_built_in ) {
		libcfPrintf ( STDERR, "No se puede usar built-in en pipe\n" );
		freeCmdArgs ( &cmd );
		return;
	}
	tFd fd = libcPipeReserve();

	if ( fd < 0 ) {
		libcfPrintf ( STDERR, "Error: No se pudo abrir pipe\n" );
		freeCmdArgs ( &cmd );
		return;
	}

	uint8_t is_bckg = ( libcStrcmp ( cmd.args[1][cmd.argc[1] - 1], "&" ) == 0 ) && !modules[found_idx[1]].is_built_in;
	if ( is_bckg ) {
		libcFree ( cmd.args[1][cmd.argc[1] - 1] );
		cmd.argc[1]--;
	}
	writer_fds[STDOUT] = fd;
	int64_t pid1 = callFunctionProcess ( modules[found_idx[0]], cmd.args[0], cmd.argc[0], writer_fds );
	reader_fds[STDIN] = fd;
	int64_t pid2 = callFunctionProcess ( modules[found_idx[1]], cmd.args[1], cmd.argc[1], reader_fds );
	if ( !is_bckg && pid2 > 0 ) {
		libcWait ( pid2, NULL );
		libcWait ( pid1, NULL );
	} else if ( is_bckg ) {
		libcPrintf ( "pid: %d y %d en background.\n", pid1, pid2 );
	}
	return;



}

static void killPid ( char ** argv, uint64_t argc )
{
	tPid pid;
	int64_t satoi_flag;
	if ( argc != 2 || argv == NULL || ( ( pid = libcSatoi ( argv[1], &satoi_flag ) ) < 0 ) || !satoi_flag ) {
		libcfPrintf ( STDERR, "Uso: kill <pid>\n" );
		return;
	}

	if ( libcKill ( pid ) < 0 ) {
		libcfPrintf ( STDERR, "Error: No se pudo terminar proceso %d\n", pid );
		return;
	}
	libcPrintf("Proceso con pid: %d terminado\n", pid);

}

static void shellWaitPid ( char ** args, uint64_t argc )
{
	if ( argc != 2 ) {
		libcfPrintf ( STDERR, "Uso: wait <pid>\n" );
		return;
	}
	tPid pid;
	int64_t satoi_flag;
	if ( ( pid = libcSatoi ( args[1], &satoi_flag ) ) < 0 || !satoi_flag ) {
		libcfPrintf ( STDERR, "Error: pid debe ser positivo\n" );
		return;
	}
	int64_t ret;
	tPid ans_pid;
	ans_pid = libcWait ( pid, &ret );
	if ( ans_pid == -1 ) {
		libcfPrintf ( STDERR, "Error: No se pudo esperar pid %d\n", pid );
		return;
	}
	libcPrintf ( "Pid %d devolvio %d\n", ans_pid, ret );
	return;
}

static void help()
{
	libcPrintf ( "\n\n######################################################################################################################\n\n");
	libcPrintf ("  Comando    Argumentos                             Descripcion\n");
	libcPrintf ("  -------    -------------                          -----------------------------\n");
	for ( int i = 0; i < MAX_MODULES; i++ ) {
		int len_name = sharedLibcStrlen(modules[i].name);
		libcPrintf("  %s", modules[i].name);
		for (int s = len_name; s < 10; s++)
			libcPrintf(" ");

		int len_args = sharedLibcStrlen(modules[i].args);
		libcPrintf("%s", modules[i].args);
		for (int s = len_args; s < 40; s++)
			libcPrintf(" ");

		libcPrintf("%s\n", modules[i].desc);
	}
	libcPrintf ( "\n\n######################################################################################################################\n\n");
}



static void showCurrentTime()
{
	LocalTime time;
	libcGetTime ( &time );
	toUtcMinus3 ( &time );
	libcPrintf ( "%d/%d/%d [d/m/a]\n", time.day, time.month, time.year );
	int64_t h = time.hour;
	libcPrintf ( "%d:%d:%d [hora/min/seg] (Argentina)\n", h, time.minutes, time.seconds ); 

	return;
}

static uint8_t diasEnMes(uint8_t mes, uint8_t anio) {
	if (mes == 2 && anio % 4 == 0)
		return 29; // Febrero bisiesto
	const int dias_mes[] = { 
		0,  // mes 0 no existe
		31, // enero
		28, // febrero
		31, // marzo
		30, // abril
		31, // mayo
		30, // junio
		31, // julio
		31, // agosto
		30, // septiembre
		31, // octubre
		30, // noviembre
		31  // diciembre
	};
	return dias_mes[mes];
}

static void toUtcMinus3 ( LocalTime * time )
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
			time->day = diasEnMes(time->month, time->year);
		}
	} else {
		time->hour = time->hour - 3;
	}
}


static void getRegs()
{
	libcPrintRegisterSnapshot();
	return;
}



static void shellNice ( char **argv, uint64_t argc )
{
	tPid pid;
	int64_t satoi_flag;
	if ( argc != 3 || ( ( pid = libcSatoi ( argv[1], &satoi_flag ) ) < 0 ) || !satoi_flag ) {
		libcfPrintf ( STDERR, "Uso: nice <pid> <nueva_prioridad>\n" );
		return;
	}
	tPriority prio;
	if ( libcStrnocasecmp ( argv[2], "low" ) == 0 ) {
		prio = LOW;
	} else if ( libcStrnocasecmp ( argv[2], "medium" ) == 0 ) {
		prio = MEDIUM;
	} else if ( libcStrnocasecmp ( argv[2], "high" ) == 0 ) {
		prio = HIGH;
	} else {
		libcfPrintf ( STDERR, "Prioridad invalida. Use 'low', 'medium', o 'high' para <nueva_prioridad>.\n" );
		return;
	}
	if ( libcNice ( pid, prio ) < 0 ) {
		libcfPrintf ( STDERR, "Error: No se pudo cambiar la prioridad de pid %d.\n", pid );
	}
}


static void shellBlock ( char **argv, uint64_t argc )
{
	tPid pid;
	int64_t satoi_flag;
	if ( argc != 2 || ( ( pid = libcSatoi ( argv[1], &satoi_flag ) ) < 0 ) || !satoi_flag ) {
		libcfPrintf ( STDERR, "Uso: block <pid>\n" );
		return;
	}
	int8_t status = libcGetStatus ( pid );
	if (	( status != BLOCKED ) && ( status != READY ) ) {
		libcfPrintf ( STDERR, "Error: El estado de pid %d no es READY ni BLOCKED \n", pid );
		return;
	}
	if ( status == BLOCKED ) {
		libcUnblock ( pid );
	} else {
		libcBlock ( pid );
	}
}
