#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <conio.h>
#include <joystick.h> 

extern unsigned int level;
extern void std_functions();

unsigned int button_result;

void end_logic(){
	unsigned char joy;
	char text[20];
	
	tgi_clear();
	
	tgi_outtextxy(8, 8, "GAME OVER");
	tgi_outtextxy(8, 16, "You are level");
	itoa(level, text, 10);
	tgi_outtextxy(8, 24, text);
	
	tgi_outtextxy(8, 40, "Press A or B");

	joy = joy_read(JOY_1);
	if (JOY_BTN_1(joy) || JOY_BTN_2(joy) ) {
		button_result = 2;
	}
	else{
		if (button_result == 2) button_result = 0;
	}
	std_functions();
	tgi_updatedisplay();
}


void result_screen(){
	
	button_result = 1;
	tgi_clear();	
	
	while(button_result){
		if (!tgi_busy()){
			end_logic();
		}
	}
}