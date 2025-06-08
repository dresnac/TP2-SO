#include <keyboard_driver.h>
#include <libasm.h>


/*
* buffer --> is "circular". If it fills up, it overwrites the oldest data.
 */
#define cant_function_keys 12
#define special_key_pressed_map_idx(code) ((code) -FIRST_SPECIAL_KEY)


static uint8_t buffer[BUFFER_SIZE];
static uint64_t buffer_dim = 0;
static uint64_t buffer_current = 0;
static uint8_t reg_shot_flag = 0;
extern uint16_t pressed_key_shift_map[][2];
static void f1key ( void );
static functionKey function_key_fun_array[cant_function_keys] = {f1key, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static uint16_t special_key_pressed_map[SPECIAL_KEYS_CANT] = {0};
static PCB * blocked;

static void f1key ( void )
{
	reg_shot_flag = 1;
}

void setKeyboardBlockedNull()
{
	blocked = NULL;
}
PCB * getKeyboardBlocked()
{
	return blocked;
}

int64_t stdinRead ( uint8_t * buff, uint64_t amount )
{
	if ( blocked != NULL ) {	// a process is already waiting to get a key...
		return -1;
	}

	uint64_t i = 0;

	if ( !bufferHasNext() ) {
		blocked = getRunning();
		blockCurrent();
	}

	while ( i < amount && bufferHasNext() && buffer[buffer_current] != EOF ) {
		buff[i] = getCurrent();
		i++;
	}

	if ( buffer[buffer_current] == EOF ) {
		getCurrent();
	}
	return i;
}


void setFKeyFunction ( uint64_t key_number, functionKey f )
{
	if ( key_number == 0 || key_number > cant_function_keys ) {
		return;
	}
	key_number--;
	function_key_fun_array[key_number] = f;
}

static void functionKeyHandler ( uint64_t code )
{
	int64_t i = -1;
	switch ( code ) {
	case F1:
		i = 0;
		break;
	case F2:
		i = 1;
		break;
	case F3:
		i = 2;
		break;
	case F4:
		i = 3;
		break;
	case F5:
		i = 4;
		break;
	case F6:
		i = 5;
		break;
	case F7:
		i = 6;
		break;
	case F8:
		i = 7;
		break;
	case F9:
		i = 8;
		break;
	case F10:
		i = 9;
		break;
	case F11:
		i = 10;
		break;
	case F12:
		i = 11;
		break;
	}
	if ( i != -1 && function_key_fun_array[i] != 0 ) {
		function_key_fun_array[i]();
	}

}


static uint8_t isReleased ( uint8_t key )
{
	return ( key & 0x80 );
}
static uint8_t isPressed ( uint8_t key )
{
	return !isReleased ( key );
}

static int isSpecialKey ( uint16_t code )
{
	return ( code >= FIRST_SPECIAL_KEY ) && ( code <= LAST_SPECIAL_KEY );
}

static int specialKeyPressed ( uint16_t code )
{
	if ( !isSpecialKey ( code ) ) {
		return -1;
	}
	return special_key_pressed_map[special_key_pressed_map_idx ( code )];
}


static int capsLockPressed()
{
	return specialKeyPressed ( CAPS_LOCK );
}
static int shiftPressed()
{
	return ( specialKeyPressed ( LEFT_SHIFT ) || specialKeyPressed ( RIGHT_SHIFT ) ) ? 1 : 0;
}
static int shiftCapsLockPressed()
{
	return ( shiftPressed() ^capsLockPressed() ); //xor
}

static uint8_t releasedKeyToPressedMask ( uint8_t key )
{
	return key & 0x7f;
}



uint64_t bufferHasNext()
{
	return ( buffer_dim > 0 ) && ( buffer_current < buffer_dim );
}

uint64_t getCurrent()
{
	if ( bufferHasNext() ) {
		return buffer[buffer_current++];
	}
	return 0;
}

void keyboardHandler()
{
	reg_shot_flag = 0;
	uint8_t key = getKey();


	uint8_t key_is_pressed = isPressed ( key ) ? 1 : 0;

	if ( !key_is_pressed ) {
		key = releasedKeyToPressedMask ( key ); //table is for pressed keys
	}

	uint16_t code = pressed_key_shift_map[key][shiftCapsLockPressed()];


	if ( isSpecialKey ( code ) ) {
		if ( code != CAPS_LOCK && code != NUM_LOCK && code != SCROLL_LOCK ) {
			special_key_pressed_map[special_key_pressed_map_idx ( code )] = key_is_pressed;
		} else if ( key_is_pressed ) {
			special_key_pressed_map[special_key_pressed_map_idx ( code )] = 1 - special_key_pressed_map[special_key_pressed_map_idx ( code )];
		}
	}

	if ( !key_is_pressed ) {
		return;
	}

	functionKeyHandler ( code );

	if ( !isSpecialKey ( code ) && specialKeyPressed ( LEFT_CONTROL ) ) {
		if ( code == 'D' || code == 'd' ) {
			code = EOF;
		} else if ( code == 'c' || code == 'C' ) {
			ctrlcHandler();
			return;
		}
	}
	if ( isSpecialKey ( code ) || code > LAST_ASCII ) {
		return;
	}

	buffer[buffer_dim] = code;
	if ( buffer_dim < BUFFER_SIZE ) {
		buffer_dim++;
	} else if ( buffer_current == buffer_dim ) {
		buffer[0] = code;
		buffer_dim = 1;
		buffer_current = 0;
	} else {
		return;
	}
	if ( blocked != NULL ) {
		ready ( blocked );
		blocked = NULL;
	}

}

uint8_t shouldTakeRegShot()
{
	return reg_shot_flag;
}