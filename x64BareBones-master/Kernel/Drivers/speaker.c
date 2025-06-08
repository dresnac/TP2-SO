
#include <speaker.h>


static void playSound ( uint32_t n_frequence )
{
	uint32_t div;
	uint8_t tmp;

	//set the pit to the desired frequency
	div = 1193180 / n_frequence;
	outb ( 0x43, 0xb6 );
	outb ( 0x42, ( uint8_t ) ( div ) );
	outb ( 0x42, ( uint8_t ) ( div >> 8 ) );

	//and play the sound using the pc speaker
	tmp = inb ( 0x61 );
	if ( tmp != ( tmp | 3 ) ) {
		outb ( 0x61, tmp | 3 );
	}
}

//make it shut up
static void noSound()
{
	uint8_t tmp = inb ( 0x61 ) & 0xfc;

	outb ( 0x61, tmp );
}

//make a beep
void beep ( uint32_t freq, int time )
{
	playSound ( freq );
	ticksSleep ( time );
	noSound();
}