#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <conio.h>
#include <joystick.h> 
#include <peekpoke.h>

extern int title_screen();
extern int game();
extern int result_screen();

extern unsigned int pause;

// palette values can be found in .pal file provided by sprpck
unsigned char palette[] = {
	0x0F,0x02,0x04,0x08,0x0D,0x0F,0x07,0x02,0x03,0x05,0x09,0x00,0x00,0x00,0x00,0x00,
	0xFF,0x22,0x7D,0x9F,0xC7,0xEF,0x84,0x22,0x34,0x57,0x93,0x00,0x00,0x00,0x00,0x00};

unsigned int level;

/*
void setpalette(const int* palette)
{
  char index;
  for (index = 0; index < 0x10; index++)
  {
    POKE(0xFDA0 + index, palette[index] >> 8);
    POKE(0xFDB0 + index, palette[index] & 0xFF);
  }
}*/

//chiper functions
/*
void init_music(void)
{
	asm("jsr SndInit");
}

void start_music(void)
{
	asm("jsr SndPauseOn");
	asm("lda musicptr+0");
	asm("ldy musicptr+1");
	asm("ldx #0");
	asm("jsr SndStartSound");
	asm("lda musicptr+2");
	asm("ldy musicptr+3");
	asm("ldx #1");
	asm("jsr SndStartSound");
	asm("lda musicptr+4");
	asm("ldy musicptr+5");
	asm("ldx #2");
	asm("jsr SndStartSound");
	asm("lda musicptr+6");
	asm("ldy musicptr+7");
	asm("ldx #3");
	asm("jsr SndStartSound");
	asm("jsr SndPauseOff");
}*/

void initialize()
{
	tgi_install(&tgi_static_stddrv);
	joy_install(&joy_static_stddrv);
	tgi_init();

	CLI();

	while (tgi_busy()) ;
	
	//tgi_setpalette(tgi_getdefpalette());
	tgi_setpalette(palette);
	//setpalette(palette);
	tgi_setcolor(COLOR_WHITE);
	tgi_setbgcolor(COLOR_BLACK);

	tgi_clear();
}

void reboot()
{
	asm("sei");
	asm("stz $FFF9");
	asm("jmp ($FFFC)");
}

void std_functions()
{
	if (kbhit()){
		switch (cgetc())
			{
			case 'F':
				tgi_flip();
				break;
			case 'R':
				reboot();
				break;
			case 'P':
				pause = !pause;
				break;

			default:
				break;
			}
	}
}



void main(void)
{
	initialize();

	while (1)
	{
		title_screen();
		game();
		result_screen();
	}
	return;
}