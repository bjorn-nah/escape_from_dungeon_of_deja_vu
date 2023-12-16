#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <conio.h>
#include <joystick.h> 

extern unsigned int level;
extern void std_functions();

unsigned int button_result;

/*
void end_init(){
	char text[20];

	
	tgi_setcolor(COLOR_RED);
	tgi_outtextxy(8, 8, "GAME OVER");
	tgi_outtextxy(8, 16, "You are level");
	itoa(level, text, 10);
	tgi_outtextxy(8, 24, text);
	
	tgi_outtextxy(8, 40, "Press A or B");
}*/

void end_logic(){
	unsigned char joy;

	joy = joy_read(JOY_1);
	if (JOY_BTN_1(joy) || JOY_BTN_2(joy) ) {
		button_result = 2;
	}
	else{
		if (button_result == 2) button_result = 0;
	}
	std_functions();
	//tgi_updatedisplay();
}


void result_screen(){
	char text[20];
	
	button_result = 1;
	tgi_clear();
	tgi_setcolor(COLOR_PURPLE);
	tgi_outtextxy(40, 8, "GAME OVER");
	tgi_outtextxy(8, 24, "Number of runned");
	tgi_outtextxy(8, 32, "frames:");
	itoa(level, text, 10);
	tgi_outtextxy(72, 32, text);
	
	tgi_outtextxy(32, 88, "Press A or B");
	
	tgi_updatedisplay();
	
	while(button_result){
		if (!tgi_busy()){
			end_logic();
		}
	}
}