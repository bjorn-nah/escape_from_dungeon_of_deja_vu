#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <conio.h>
#include <joystick.h> 

extern unsigned char title01[];
extern unsigned char title02[];
extern unsigned char title03[];
extern unsigned char title04[];
extern unsigned char title05[];
extern unsigned char title06[];

unsigned char title_gross[6];

extern void std_functions();

unsigned int button_title, zoom;
//extern unsigned char title_screen_bg[];
unsigned int randomizator = 0;
unsigned int zoom_step1 = 0x0010;
unsigned int zoom_step2 = 0x0001;


SCB_REHV_PAL title_tule_01 = {
  BPP_4 | TYPE_NORMAL, 
  REHV,
  0x01,
  0x0000,
  title01,
  0, 0,
  0x0100, 0x0100,
  {0x0B,0xC3,0x45,0x67,0x89,0xAB,0xCD,0xEF}
};
SCB_REHV_PAL title_tule_02, title_tule_03, title_tule_04, title_tule_05, title_tule_06;
SCB_REHV_PAL *work_title; 

void screen_init(){
	unsigned int i;
	title_tule_02= title_tule_03= title_tule_04= title_tule_05= title_tule_06 = title_tule_01;
	
	title_tule_06.next = 0x0000;
	title_tule_05.next = (char *)&title_tule_06;
	title_tule_04.next = (char *)&title_tule_05;
	title_tule_03.next = (char *)&title_tule_04;
	title_tule_02.next = (char *)&title_tule_03;
	title_tule_01.next = (char *)&title_tule_02;
	
	
	title_tule_01.data = title01;
	title_tule_02.data = title02;
	title_tule_03.data = title03;
	title_tule_04.data = title04;
	title_tule_05.data = title05;
	title_tule_06.data = title06;
	
	//dont ask any question :v
	
	// title_tule_01.penpal = ;
	// title_tule_02.penpal = ;
	title_tule_03.penpal[0] = 0xCB;
	title_tule_03.penpal[1] = 0x03;
	// title_tule_04.penpal = ;
	title_tule_05.penpal[0] = 0xCB;
	title_tule_05.penpal[1] = 0x03;
	title_tule_06.penpal[0] = 0xC0;
	title_tule_06.penpal[1] = 0xB3;
	
	title_tule_01.hpos = 41;
	title_tule_01.vpos = 16;
	title_tule_02.hpos = 89;
	title_tule_02.vpos = 16;
	title_tule_03.hpos = 42;
	title_tule_03.vpos = 35;
	title_tule_04.hpos = 101;
	title_tule_04.vpos = 36;
	title_tule_05.hpos = 54;
	title_tule_05.vpos = 54;
	title_tule_06.hpos = 89;
	title_tule_06.vpos = 55;
	
	work_title = &title_tule_01;
	zoom = 0x0000;
	for(i=0;i<6;i++){
		work_title->hsize+=zoom;
		work_title->vsize+=zoom;
		work_title = (SCB_REHV_PAL *)work_title->next;
		zoom+=zoom_step1;
		title_gross[i]=1;
	}
}

void screen_update(){
	unsigned int i;
	tgi_clear();
	tgi_sprite(&title_tule_01);
	
	//do things
	work_title = &title_tule_01;
	for(i=0;i<6;i++){
		if(work_title->hsize>0x0140){
			title_gross[i]=0;
		}
		if(work_title->hsize<0x0100){
			title_gross[i]=1;
		}
		if(title_gross[i]==1){
			work_title->hsize+=zoom_step2;
			work_title->vsize+=zoom_step2;
		}else{
			work_title->hsize-=zoom_step2;
			work_title->vsize-=zoom_step2;
		}
			
		work_title = (SCB_REHV_PAL *)work_title->next;
	}
	
	//That's white, trust me.
	tgi_setcolor(COLOR_PURPLE);
	if(title_gross[1]){
		tgi_outtextxy( 32, 88, "Press A or B");
	}else{
		tgi_outtextxy( 32, 88, "#LYNXJAM2023");
	}
	
	tgi_updatedisplay();
}

void screen_logic(){
	unsigned char joy;
	//tgi_sprite(&screen_bg);


	joy = joy_read(JOY_1);
	if (JOY_BTN_1(joy) || JOY_BTN_2(joy) ) {
		button_title = 2;
	}
	else{
		if (button_title == 2) button_title = 0;
	}
	std_functions();
	
}


void title_screen(){
	
	button_title = 1;
	tgi_clear();
	/*
	tgi_setcolor(COLOR_RED);
	
	tgi_outtextxy( 8, 8, "ESCAPE");
	tgi_outtextxy( 8, 16, "FROM");
	tgi_outtextxy( 8, 24, "DUNGEON");
	tgi_outtextxy( 8, 32, "OF");
	tgi_outtextxy( 8, 40, "DEJA");
	tgi_outtextxy( 8, 48, "VU");
	tgi_outtextxy( 8, 88, "Press A or B");
	*/
	screen_init();
	
	tgi_updatedisplay();
	
	while(button_title){
		if (!tgi_busy()){
			screen_update();
			screen_logic();
			randomizator++;
		}
	}	
	srand(randomizator);
}