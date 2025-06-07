// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <shell.h>

typedef struct {
	char **args[2];
	uint64_t argc[2];
} Command;


static void help();
static void to_utc_minus_3 ( LocalTime * time );
static void free_args ( char ** args, uint64_t argc );
static void free_cmd_args ( Command * cmd );
static int64_t call_function_process ( module m, char ** args, uint64_t argc, uint64_t fds[CANT_FDS] );
static int64_t piped_command_parse ( char shell_buffer[], Command *cmd );
static char ** command_parse ( char shell_buffer[], uint64_t *argc, int64_t *pipe_pos, int64_t *pipe_count );
static void interpret();
static void help();
static void show_current_time();
static void show_current_time();
static void get_regs();


static uint64_t font_size = 1;
#define BUILT_IN 1



static module modules[] = {
{"help", "", "Displays all available operating system modules.", help, BUILT_IN},

};




int main()
{
	libc_set_font_size ( font_size );
	libc_puts ( WELCOME );
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


static int64_t call_function_process ( module m, char ** args, uint64_t argc, uint64_t fds[CANT_FDS] )
{
	if ( m.is_built_in ) {
		m.function ( args, argc );
		free_args ( args, argc );
		return 0;
	}
	int64_t ans = libc_create_process ( ( main_function ) m.function, LOW, args, argc, fds );

	if ( ans < 0 ) {
		libc_fprintf ( STDERR, "Error: Could not create process\n" );
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
	if ( sharedLibcStrlen ( shell_buffer ) == 0 ) {
		return;
	}

	Command cmd;
	if ( piped_command_parse ( shell_buffer, &cmd ) != 0 ) {
		libc_fprintf ( STDERR, "Invalid Command! Try Again >:(\n" );
		return;
	}
	if ( cmd.argc[0] == -1 || cmd.argc[1] == -1 ) {
		libc_fprintf ( STDERR, "Not enough memory to create process\n" );
	}

	int found_idx[2] = {-1, -1};
	int has_pipe = parse_command_and_get_modules ( &cmd, found_idx );

	if ( found_idx[0] == -1 ) {
		libc_fprintf ( STDERR, "Error: Invalid Command! Try Again.\n" );
		return;
	}

	uint64_t fds[] = {STDOUT, STDERR, STDIN};
	uint64_t writer_fds[] = {STDOUT, STDERR, STDIN};
	uint64_t reader_fds[] = {STDOUT, STDERR, STDIN};

	if ( !has_pipe ) {
		uint8_t is_bckg = ( libc_strcmp ( cmd.args[0][cmd.argc[0] - 1], "&" ) == 0 );
		if ( is_bckg && modules[found_idx[0]].is_built_in ) {
			libc_fprintf ( STDERR, "Error: Cannot use built-in in background\n" );
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
			libc_printf ( "Pid: %d in background\n", pid );
		}
		return;
	}
	if ( found_idx[1] == -1 ) {
		libc_fprintf ( STDERR, "Error: Invalid Command! Try Again.\n" );
		return;
	}
	if ( modules[found_idx[0]].is_built_in || modules[found_idx[1]].is_built_in ) {
		libc_fprintf ( STDERR, "Cannot use built-in in pipe\n" );
		free_cmd_args ( &cmd );
		return;
	}
	uint64_t fd = libc_pipe_reserve();

	if ( fd < 0 ) {
		libc_fprintf ( STDERR, "Error: Could not open pipe\n" );
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
		libc_printf ( "pid: %d and %d in background.\n", pid1, pid2 );
	}
	return;



}


static void help()
{
	libc_printf ( "\n\n-------------------------------                      Built-in functions                      -----------------------------------\n\n" );
	for ( int i = 0; i < MAX_MODULES; i++ ) {
		if ( i == NUM_BUILT_INS ) {
			libc_printf ( "\n\n-------------------------------                      Processes                                ----------------------------------\n\n" );
		}
		if ( i == MAX_MODULES - NUM_TESTS ) {
			libc_printf ( "\n\n-------------------------------                      Functionality tests                      ----------------------------------\n\n" );
		}
		libc_printf ( "- %s%s%s%s\n", modules[i].name, libc_strcmp ( modules[i].args, "" ) == 0 ? ": " : "", modules[i].args, modules[i].desc );
	}
	libc_printf ( "-------------------------------                                                              -----------------------------------\n\n" );
}



static void show_current_time()
{
	LocalTime time;
	libc_get_time ( &time );
	to_utc_minus_3 ( &time );
	libc_printf ( "%d/%d/%d [d/m/y]\n", time.dias, time.mes, time.anio );
	int64_t h = time.horas;
	libc_printf ( "%d:%d:%d [hour/min/sec] (Argentina)\n", h, time.minutos, time.segundos ); // la hora es -3 para que este en tiempo argentino.

	return;
}


static void to_utc_minus_3 ( LocalTime * time )
{

	if ( time->horas < 3 ) {
		time->horas += 21;
		time->dias--;
		if ( time->dias == 0 ) {
			time->mes--;
			if ( time->mes == 0 ) {
				time->mes = 12;
				time->anio--;
			}
			if ( time->mes == 2 ) {
				time->dias = 28;
				if ( time->anio % 4 == 0 ) {
					time->dias = 29;
				}
			} else if ( time->mes == 4 || time->mes == 6 || time->mes == 9 || time->mes == 11 ) {
				time->dias = 30;
			} else {
				time->dias = 31;
			}
		}
	} else {
		time->horas = time->horas - 3;
	}
}


static void get_regs()
{
	libc_print_register_snapshot();
	return;
}


