#include <videoDriver.h>
#include <font8x16.h>

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

static int fontSize = 1;

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

static int cursor[] = {0, 0};

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}


void drawChar(char c, uint32_t hexColor){
	int x = cursor[0];
	int y = cursor[1];
	unsigned char * bitMap = font8x16[c-FIRST_SYMBOL_MAPPED];
	int z=0;
	int w=0;
	for(int i=0; i < 16*fontSize; i+=fontSize){
		w=0;
		for(int j=0; j < 8*fontSize; j+=fontSize){
			if(bitMap[z] & (1 << (7 - w))){
				if(fontSize == 2){
					putPixel(hexColor, j+x, i+y);
					putPixel(hexColor, j+x+1, i+y);
					putPixel(hexColor, j+x, i+y+1);
					putPixel(hexColor, j+x+1, i+y+1);
				}else{
					putPixel(hexColor, j+x, i+y);
				}
			}
			w++;
		}
		z++;
	}
	cursor[0] += SYMBOL_WIDTH*fontSize;
}


void vdPrint(const char *buffer, int count, uint32_t hexColor){
	
	int i=0;
	while (buffer[i] != 0 && i<count) {
        switch (buffer[i]) {
            case '\n':
                newline();
                break;
            case '\b':
            	delete();
                break;
            case '\t':
                tab();
                break;
            default:
                if(buffer[i] >= FIRST_SYMBOL_MAPPED && buffer[i] <= LAST_SYMBOL_MAPPED){  
					//(PENSAR) Guardar todo lo escrito en un buffer?
                    drawChar(buffer[i], hexColor);
                }
                break;
        }
        i++;
    }

}

void newline(){
	cursor[1] += SYMBOL_LENGTH*fontSize;
	cursor[0] = 0;
}

void delete(){
	int x = cursor[0] - SYMBOL_WIDTH*fontSize;
	int y = cursor[1];
	for(int i=0; i < SYMBOL_LENGTH*fontSize; i++){
		for(int j=0; j < SYMBOL_WIDTH*fontSize; j++){
			putPixel(0x00000000, j+x, i+y);
		}
	}
	cursor[0]-=SYMBOL_WIDTH*fontSize;
}

void tab(){  //solución mala pero rápida
	vdPrint("    ", 4, 0);
	return;
}

void vdClearScreen(){
	for(int i=0; i < VBE_mode_info->width; i++){
		for(int j=0; j < VBE_mode_info->height; j++){
			putPixel(0x00000000,i,j);
		}
	}
	cursor[0]=0;
	cursor[1]=0;
}

int vdChangeFontSize(int op){
	if(op == -1 && fontSize == 2){
		fontSize = 1;
		return 1;
	}else if(op == 1 && fontSize == 1){
		fontSize = 2;
		return 1;
	}
	return 0;
}

void vdDrawRectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint64_t color){
	for(int i=0; i < width; i++){
		for(int j=0; j < height; j++){
			putPixel(color, x+i, y+j);
		}
	}
}

//Habría que pensar funciones para saber dónde está el cursor y para poder moverlo a gusto
//(Para escribir en distintas zonas de la pantalla)
