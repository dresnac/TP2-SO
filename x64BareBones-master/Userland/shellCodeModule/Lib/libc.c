#include <libc.h>

#define C5  523
#define E5  659
#define G5  784
#define B5  987
#define C6  1046
#define REST 0

char libcGetChar()
{
	uint8_t c = 0;
	return sys_read(&c, 1) > 0 ? (char)c : 0;
}

void libcPutChar ( char c )
{
	sys_write ( STDOUT, ( uint8_t * ) &c, 1 );
}



void libcPlaySimpleMelody() {
    libcBeep(C5, 5);   
    libcBeep(E5, 5);
    libcBeep(G5, 5);
    libcBeep(B5, 5);
    libcBeep(C6, 6);   

    libcBeep(B5, 4);
    libcBeep(G5, 4);
    libcBeep(E5, 4);
    libcBeep(G5, 4);
    libcBeep(C6, 4);
}

int64_t libcBeep ( uint64_t frequency, uint64_t duration )
{
	return sys_beep ( frequency, duration );
}


int64_t libcClearScreen()
{
	return sys_clear_screen();
}


int64_t libcSetFontSize ( uint64_t size )
{
	return sys_set_font_size ( size );
}



char * libcNumToString ( uint64_t num, uint64_t base, char *buffer, size_t buffer_size )
{
	if ( base < 2 || base > 16 || buffer_size < 2 ) {
		return NULL;
	}

	char *ptr = &buffer[buffer_size - 1];
	*ptr = '\0';

	do {
		if ( ptr == buffer ) {
			return NULL;
		}
		*--ptr = "0123456789ABCDEF"[num % base];
		num /= base;
	} while ( num != 0 );

	return ptr;
}

int64_t libcPuts (  char * str )
{
	return sys_write ( STDOUT, ( uint8_t * ) str, sharedLibcStrlen ( str ) );
}

int64_t libcfPutc ( char c, uint64_t fd )
{
	if ( c < 0 ) {
		return -1;
	}
	return sys_write ( fd, ( uint8_t * ) &c, 1 ) == -1 ? -1 : 1;
}

#define BUFF_SIZE 64


static int64_t libcvfPrintf ( uint64_t fd, char *fmt, va_list argv )
{
	uint64_t flag = 0;
	int64_t written = 0;
	char buffer[BUFF_SIZE];

	for ( uint64_t i = 0; fmt[i] != '\0'; i++ ) {
		if ( fmt[i] == '%' && !flag ) {
			flag = 1;
			i++;
		}

		if ( !flag ) {
			if ( libcfPutc ( fmt[i], fd ) == -1 ) {
				return -1;
			}
			flag = 0;
			written++;
			continue;
		}

		switch ( fmt[i] ) {
		case 'c':
			if ( libcfPutc ( va_arg ( argv, int ), fd ) == -1 ) {
				return -1;
			}
			written++;
			break;
		case 'd':
			if ( ( written += libcvfPrintf ( fd, libcNumToString ( va_arg ( argv, uint64_t ), 10, buffer, BUFF_SIZE ), argv ) ) == -1 ) {
				return -1;
			}
			break;
		case 'x':
			if ( libcvfPrintf ( fd, "0x", argv ) == -1 ||
			        ( written += libcvfPrintf ( fd, libcNumToString ( va_arg ( argv, uint64_t ), 16, buffer, BUFF_SIZE ), argv ) ) == -1 ) {
				return -1;
			}
			break;
		case 's':
			if ( ( written += libcvfPrintf ( fd, va_arg ( argv, char * ), argv ) ) == -1 ) {
				return -1;
			}
			break;
		case '%':
			if ( libcfPutc ( '%', fd ) == -1 ) {
				return -1;
			}
			written++;
			break;
		default:
			return -1;
		}
		flag = 0;
	}

	return written;
}

int64_t libcfPrintf ( uint64_t fd, char *fmt, ... )
{
	va_list argv;
	va_start ( argv, fmt );

	int64_t out = libcvfPrintf ( fd, fmt, argv );

	va_end ( argv );
	return out;
}

int64_t libcPrintf ( char *fmt, ... )
{
	va_list argv;
	va_start ( argv, fmt );

	int64_t out = libcvfPrintf ( STDOUT, fmt, argv );

	va_end ( argv );
	return out;
}


char * libcGets ( char * buffer, int n )
{
	int c;
	int i = 0;

	while ( ( c = libcGetChar() ) != '\n'  ) {
		if ( c == '\b' && i > 0 ) {
			libcPutChar ( c );
			i--;
		}
		if ( c != '\b' && i < n - 1 && c != 0 ) {
			libcPutChar ( c );
			buffer[i++] = ( char ) c;
		}
	}
	libcPutChar ( '\n' );
	buffer[i] = '\0';

	return buffer;
}


int64_t libcStrcmp (  char * str1,  char * str2 )
{
	while ( *str1 && ( *str1 == *str2 ) ) {
		str1++;
		str2++;
	}
	return * ( unsigned char * ) str1 - * ( unsigned char * ) str2;
}

int64_t libcStrnocasecmp ( char *str1,  char *str2 )
{
	while ( *str1 && *str2 ) {
		char c1 = *str1;
		char c2 = *str2;

		if ( c1 >= 'A' && c1 <= 'Z' ) {
			c1 = c1 + ( 'a' - 'A' );
		}
		if ( c2 >= 'A' && c2 <= 'Z' ) {
			c2 = c2 + ( 'a' - 'A' );
		}

		if ( c1 != c2 ) {
			return ( int64_t ) ( unsigned char ) c1 - ( unsigned char ) c2;
		}

		str1++;
		str2++;
	}

	return ( int64_t ) ( unsigned char ) * str1 - ( unsigned char ) * str2;
}


int64_t libcEnterVideoMode()
{
	return sys_set_mode ( VIDEO_MODE );
}


int64_t libcEnterTextMode()
{
	return sys_set_mode ( TEXT_MODE );
}

int64_t libcDrawPixel ( uint64_t x, uint64_t y, color color )
{
	return sys_put_pixel ( x, y, &color );
}

int64_t libcDrawRectangle ( uint64_t x, uint64_t y, uint64_t width, uint64_t height, color color )
{
	return sys_put_rectangle ( x, y, width, height, &color );
}

int64_t libcDrawLetter ( uint64_t x, uint64_t y, char letter, color color, uint64_t font_size )
{
	return sys_draw_letter ( x, y, &letter, &color, font_size );
}

void libcPrintRegisterSnapshot()
{
	Snapshot snap;
	if ( sys_get_register_snapshot ( &snap ) == -1 ) {
		libcfPrintf ( STDERR, "Error: No register snapshot available. Press F1 to take a snapshot.\n" );
		return;
	}

	libcPuts ( "Register snapshot:\n" );
	libcPrintf ( "rax: %x\n", snap.rax );
	libcPrintf ( "rbx: %x\n", snap.rbx );
	libcPrintf ( "rcx: %x\n", snap.rcx );
	libcPrintf ( "rdx: %x\n", snap.rdx );
	libcPrintf ( "rsi: %x\n", snap.rsi );
	libcPrintf ( "rdi: %x\n", snap.rdi );
	libcPrintf ( "rbp: %x\n", snap.rbp );
	libcPrintf ( "rsp: %x\n", snap.rsp );
	libcPrintf ( "r8:  %x\n", snap.r8 );
	libcPrintf ( "r9:  %x\n", snap.r9 );
	libcPrintf ( "r10: %x\n", snap.r10 );
	libcPrintf ( "r11: %x\n", snap.r11 );
	libcPrintf ( "r12: %x\n", snap.r12 );
	libcPrintf ( "r13: %x\n", snap.r13 );
	libcPrintf ( "r14: %x\n", snap.r14 );
	libcPrintf ( "r15: %x\n", snap.r15 );
	libcPrintf ( "rIP: %x\n", snap.rip );
}

//----------------------------- TP2 ---------------------------------- //


void * libcMalloc ( uint64_t size )
{
	return sys_malloc ( size );
}

void libcFree ( void * p )
{
	return sys_free ( p );
}

tPid libcCreateProcess ( mainFunction rip, uint64_t priority, char ** argv, uint64_t argc, tFd fds[] )
{
	return ( tPid ) sys_create_process ( rip, priority, argv, argc, fds );
}
int64_t libcGetTime ( LocalTime * time )
{
	return sys_get_time ( time );
}

tPid libcGetPid()
{
	return ( tPid ) sys_get_pid();
}

int64_t libcKill ( tPid pid )
{
	return sys_kill ( pid );
}
int64_t libcBlock ( tPid pid )
{
	return sys_block ( pid );
}
int64_t libcNice ( tPid pid, uint64_t newPrio )
{
	return sys_nice ( pid, newPrio );
}

int64_t libcYield()
{
	return sys_yield();
}
int64_t libcUnblock ( tPid pid )
{
	return sys_unblock ( pid );
}

tPid libcWait ( tPid pid, int64_t * ret )
{
	return sys_wait ( pid, ret );
}

int64_t libcSemOpen ( int64_t sem_id, int64_t value )
{
	return sys_sem_open ( sem_id, value );
}

int64_t libcSemWait ( int64_t sem_id )
{
	return sys_sem_wait ( sem_id );
}

int64_t libcSemPost ( int64_t sem_id )
{
	return sys_sem_post ( sem_id );
}

int64_t libcSemClose ( int64_t sem_id )
{
	return sys_sem_close ( sem_id );
}
int64_t libcSemOpenGetId ( int64_t value )
{
	return sys_sem_open_get_id ( value );
}

int8_t libcGetStatus ( tPid pid )
{
	return sys_get_status ( pid );
}
int64_t libcPipeOpen ( int64_t id, tPipeMode mode )
{
	return sys_pipe_open ( id, mode );
}
int64_t libcPipeOpenFree ( tPipeMode mode )
{
	return sys_pipe_open_free ( mode );
}
int64_t libcPipeRead ( int64_t id, uint8_t * buffer, uint64_t amount )
{
	return sys_pipe_read ( id,  buffer,  amount );
}
int64_t libcPipeWrite ( int64_t id, uint8_t * buffer, uint64_t amount )
{
	return sys_pipe_write ( id,  buffer, amount );
}
int64_t libcPipeClose ( int64_t id )
{
	return sys_pipe_close ( id );
}
int64_t libcGetMyFds ( tFd fds[CANT_FDS] )
{
	return sys_get_my_fds ( fds );
}

int64_t libcTicksSleep ( uint64_t ticks )
{
	return sys_ticks_sleep ( ticks );
}

int64_t libcSleep ( uint64_t seconds )
{
	return libcTicksSleep ( seconds * 18 );
}

int64_t libcSatoi ( char * str, int64_t * flag )
{
	uint64_t i = 0;
	int64_t res = 0;
	int8_t sign = 1;

	if ( !str ) {
		if ( flag ) {
			*flag = 0;
		}
		return 0;
	}

	if ( str[i] == '-' ) {
		i++;
		sign = -1;
	}

	for ( ; str[i] != '\0'; ++i ) {
		if ( str[i] < '0' || str[i] > '9' ) {
			if ( flag ) {
				*flag = 0;
			}
			return 0;
		}

		res = res * 10 + str[i] - '0';
	}
	if ( flag ) {
		*flag = 1;
	}
	return res * sign;
}

int64_t libcPipeReserve()
{
	return sys_pipe_reserve();
}