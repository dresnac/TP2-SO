#include <video_driver.h>


#define SCREEN_WIDTH vbe_mode_info->width
#define SCREEN_HEIGHT vbe_mode_info->height

#define CHAR_BUFFER_ROWS  48        	// Number of characters available with font size 1 (in x).
#define CHAR_BUFFER_COLS  128        	// Number of characters available with font size 1 (in y).
#define Y_FONT_OFFSET (FONT_HEIGHT * font_size)
#define X_FONT_OFFSET (FONT_WIDTH * font_size)

typedef struct {
	uint64_t x;
	uint64_t y;
} point;

typedef struct {
	uint8_t c;
	uint8_t fd;
} char_buffer_type;

static vbe_info_ptr vbe_mode_info = ( vbe_info_ptr ) 0x0000000000005c00;
extern uint8_t font_bitmap[4096];
static color background_color = {0, 0, 0};
static color font_color = {255, 255, 255};
static color stderr_color = {255, 0, 0};
static uint64_t font_size = 1;
static uint8_t driver_mode = TEXT_MODE;
static point current_screen_point = {0, 0};
static char_buffer_type char_buffer[CHAR_BUFFER_ROWS * CHAR_BUFFER_COLS];
static uint64_t buffer_index = 0;

static uint8_t override_mode = 0;
static uint64_t CHAR_BUFFER_ROWS_zoomed = CHAR_BUFFER_ROWS;
static uint64_t CHAR_BUFFER_COLS_zoomed = CHAR_BUFFER_COLS;



static void libcClearScreen();
static uint64_t inTextMode();
static uint64_t inVideoMode();
static void addCharToBuffer ( uint8_t c, uint8_t fd );
static void printFont ( char_buffer_type letter );
static void libcClearScreen();
static void newLinePrint();
static void newLineBuff();
static void newLine();
static void backSpaceBuffer();
static void backSpace();
static void backSpacePrint();
static void tabulator();
static void reBufferPrint();
static void printBuffer();


void vdriverSetFontColor ( color c )
{
	font_color = c;
}

int64_t vdriverSetFontSize ( uint64_t size )
{
	if ( !inTextMode() ) {
		return -1;
	}

	if ( SCREEN_WIDTH < size * FONT_WIDTH || SCREEN_HEIGHT < size * FONT_HEIGHT ) {
		return -1;
	}
	if ( size == font_size ) {
		return 0;
	}
	font_size = size;
	CHAR_BUFFER_ROWS_zoomed = ( SCREEN_HEIGHT / ( font_size * FONT_HEIGHT ) );
	CHAR_BUFFER_COLS_zoomed = ( SCREEN_WIDTH / ( font_size * FONT_WIDTH ) );
	libcClearScreen();
	return 0;
}


int64_t vdriverGetScreenInformation ( ScreenInformation * screen_information )
{
	screen_information->width = SCREEN_WIDTH;
	screen_information->height = SCREEN_HEIGHT;
	return 1;
}

int64_t vdriverClearScreen ( color color )
{
	if ( driver_mode == TEXT_MODE ) {
		background_color = color;
		current_screen_point.x = current_screen_point.y = 0;
		buffer_index = 0;
	}
	override_mode = 1;
	vdriverDrawRectangle ( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color );
	override_mode = 0;
	return 0;
}



int64_t vdriverSetMode ( uint64_t mode, color c )
{
	if ( ! ( mode == TEXT_MODE || mode == VIDEO_MODE ) ) {
		return -1;
	}
	background_color = c;
	if ( mode == driver_mode ) {
		return 0;
	}
	driver_mode = mode;


	if ( mode == TEXT_MODE ) {
		printBuffer();
	} else {
		vdriverClearScreen ( c );
	}

	return 0;

}


int64_t vdriverWrite ( tFd fd,  char * buffer, int64_t amount )
{
	if ( !inTextMode() ) {
		return 0;
	}
	if ( ! ( fd == STDOUT || fd == STDERR ) ) {
		return 0;
	}

	uint64_t i = 0;
	while ( i < amount && buffer[i] != 0 ) {
		switch ( buffer[i] ) {
		case '\n':
			newLine();
			break;
		case '\b':
			backSpace();
			break;
		case '\t':
			tabulator();
			break;
		default:
			if ( buffer[i] >= FIRST_ASCII_FONT && buffer[i] <= LAST_ASCII_FONT ) {
				char_buffer_type c = {buffer[i], fd};
				addCharToBuffer ( buffer[i], fd );
				printFont ( c );
			}
			break;
		}
		i++;
	}
	return i;
}

int64_t vdriverDrawRectangle ( uint64_t x, uint64_t y, uint64_t width, uint64_t height, color color )
{
	if ( x + width > SCREEN_WIDTH || y + height > SCREEN_HEIGHT ) {
		return -1;
	}
	if ( !inVideoMode() ) {
		return -1;
	}
	for ( uint64_t i = 0; i < width; i++ ) {
		for ( uint64_t j = 0; j < height; j++ ) {
			vdriverDrawPixel ( x + i, y + j, color );
		}
	}
	return 0;
}
int64_t vdriverDrawFont ( uint64_t x, uint64_t y, uint8_t ascii, color color, uint64_t font_size )
{
	if ( ascii < FIRST_ASCII_FONT || ascii > LAST_ASCII_FONT ) {
		return -1;
	}
	if ( !inVideoMode() ) {
		return -1;
	}
	int letter = ( ascii - ' ' ) * 16;
	for ( uint64_t i = 0; i < 16; i++ ) {
		for ( uint64_t j = 0; j < 8; j++ ) {
			if ( ( font_bitmap[letter + i] >> ( 7 - j ) ) & 0x1 ) {
				vdriverDrawRectangle ( ( x + j * font_size ), ( y + i * font_size ), font_size, font_size, color );
			}
		}
	}
	return 0;
}





int64_t vdriverDrawPixel ( uint64_t x, uint64_t y, color color )
{

	if ( x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT ) {
		return -1;
	}
	if ( !inVideoMode() ) {
		return -1;
	}

	uint8_t * framebuffer = ( uint8_t * ) ( uintptr_t ) vbe_mode_info->framebuffer;
	uint64_t offset = ( x * ( ( vbe_mode_info->bpp ) / 8 ) ) + ( y * vbe_mode_info->pitch );
	framebuffer[offset]     =  color.b;
	framebuffer[offset + 1]   =  color.g;
	framebuffer[offset + 2]   =  color.r;
	return 0;
}


//funciones static:
static uint64_t inTextMode()
{
	return ( ( driver_mode == TEXT_MODE ) || override_mode );
}
static uint64_t inVideoMode()
{
	return ( ( driver_mode == VIDEO_MODE ) || override_mode );
}

#define space_per_tab 3
static void tabulator()
{
	char_buffer_type c = {' ', STDOUT};
	for ( int i = 0; i < space_per_tab ; i++ ) {
		addCharToBuffer ( ' ', STDOUT );
		printFont ( c );
	}
}

static void addCharToBuffer ( uint8_t c, uint8_t fd )
{
	if ( buffer_index  >= CHAR_BUFFER_ROWS_zoomed * CHAR_BUFFER_COLS_zoomed ) {
		reBufferPrint();
	}
	char_buffer_type aux = {c, fd};
	char_buffer[buffer_index] = aux;
	buffer_index ++;

}



static void printFont ( char_buffer_type letter )
{

	if ( current_screen_point.x + FONT_WIDTH * font_size - FONT_WIDTH >= SCREEN_WIDTH ) {
		current_screen_point.y += FONT_HEIGHT * font_size;
		current_screen_point.x = 0;
	}
	if ( current_screen_point.y + FONT_HEIGHT * font_size - FONT_HEIGHT >= SCREEN_HEIGHT ) {
		reBufferPrint();
	}
	override_mode = 1;
	color col;
	if ( letter.fd == STDOUT ) {
		col = font_color;
	} else {
		col = stderr_color;
	}
	vdriverDrawFont ( current_screen_point.x, current_screen_point.y, letter.c, col, font_size );
	override_mode = 0;
	current_screen_point.x += FONT_WIDTH * font_size;
}

static void libcClearScreen()
{
	vdriverClearScreen ( background_color );
}

static void newLinePrint()
{
	current_screen_point.x = 0;
	current_screen_point.y = current_screen_point.y + FONT_HEIGHT * font_size;
	if ( current_screen_point.y + FONT_HEIGHT * font_size - FONT_HEIGHT >= SCREEN_HEIGHT ) {
		reBufferPrint();
	}
}

static void newLineBuff()
{
	int remaining_on_row = CHAR_BUFFER_COLS_zoomed - ( buffer_index % CHAR_BUFFER_COLS_zoomed );
	for ( int i = 0 ; i < remaining_on_row; i++ ) {
		addCharToBuffer ( ' ', STDOUT );
	}
}


static void newLine()
{
	newLineBuff();
	newLinePrint();
}

static void backSpacePrint()
{
	if ( current_screen_point.x == 0 && current_screen_point.y == 0 ) {
		return;
	}
	if ( current_screen_point.x == 0 ) {

		current_screen_point.y -= font_size * FONT_HEIGHT; // y es dif a 0
		current_screen_point.x = SCREEN_WIDTH - font_size * FONT_WIDTH;
	} else {
		current_screen_point.x -= font_size * FONT_WIDTH;
	}
	override_mode = 1;
	vdriverDrawRectangle ( current_screen_point.x, current_screen_point.y, X_FONT_OFFSET, FONT_HEIGHT * font_size, background_color );
	override_mode = 0;
}


static void backSpaceBuffer()
{
	if ( buffer_index != 0 ) {
		char_buffer[buffer_index].c = 0;
		char_buffer[--buffer_index].fd = STDOUT;
	}

}

static void backSpace()
{
	backSpacePrint();
	backSpaceBuffer();
}



#define rows_to_rebuffer(rows_in_screen) (((rows_in_screen)/(2))+1)
static void reBufferPrint()
{
	uint64_t aux = buffer_index; // con el clear screen se setea en 0
	libcClearScreen();
	uint64_t j = 0;
	for ( uint64_t i = CHAR_BUFFER_COLS_zoomed * rows_to_rebuffer ( CHAR_BUFFER_ROWS_zoomed ) ; i < aux; i++, j++ ) {
		char_buffer[j] = char_buffer[i];
		printFont ( char_buffer[j] );
	}
	buffer_index = j;
}

static void printBuffer()
{
	uint64_t aux = buffer_index; // con clear screen se borra
	libcClearScreen();
	for ( int i = 0 ; i < aux ; i++ ) {
		vdriverWrite ( char_buffer[i].fd, ( char * ) &char_buffer[i].c, 1 );
	}
}