#include <stdint.h>
#include <PCB.h>
#include <scheduler.h>
#include <process.h>
#ifndef _KEYBOARD_DRIVER_H
#define _KEYBOARD_DRIVER_H


#define BUFFER_SIZE 1024
#define FIRST_SPECIAL_KEY LEFT_CONTROL
#define LAST_SPECIAL_KEY  F12
#define SPECIAL_KEYS_CANT  (LAST_SPECIAL_KEY - FIRST_SPECIAL_KEY + 1)
#define LAST_ASCII 127

typedef enum {
	LEFT_CONTROL = 256,         // NOTAR: El valor con el que empieza tiene que ser mas grande al ultimo de la tabla ASCII
	LEFT_SHIFT,
	RIGHT_SHIFT,
	KEYPAD_STAR_PRTSC,
	LEFT_ALT,
	CAPS_LOCK,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	NUM_LOCK,
	SCROLL_LOCK,
	HOME_KEYPAD,
	UP_ARROW_KEYPAD,
	PG_UP_KEYPAD,
	LEFT_ARROW_KEYPAD,
	RIGHT_ARROW_KEYPAD,
	PLUS_KEYPAD,
	END_KEYPAD,
	DOWN_ARROW_KEYPAD,
	PG_DN_KEYPAD,
	INS_KEYPAD,
	DEL_KEYPAD,
	SYS_REQ_AT,
	NONE1, // no hay tecla de scanCode 0x55
	NONE2, // no hay tecla de scan code 0x56
	F11,
	F12
} specialCodes;


typedef void ( *functionKey ) ( void );

void keyboardHandler();
void setFKeyFunction ( uint64_t key_number, functionKey f );
uint64_t getCurrent();
uint64_t bufferHasNext();

int64_t stdinRead ( uint8_t * buff, uint64_t amount );
PCB * getKeyboardBlocked();
void setKeyboardBlockedNull();

#endif