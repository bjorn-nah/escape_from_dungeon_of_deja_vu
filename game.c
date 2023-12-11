#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <conio.h>
#include <joystick.h> 

#include "game.h"

/*
extern void init_music();
extern void start_music();
*/
extern void std_functions();

extern unsigned int level;

unsigned int playing, pause;
unsigned int x_map, y_map;
unsigned char game_status;
unsigned int randomizator2 = 0;

extern unsigned char palette[];

int map_h = 12;
int map_v = 3;
unsigned int level_map[] = {
	1, 2, 3, 1, 2, 3, 1, 1, 4, 1, 2, 3,
	1, 1, 4, 1, 2, 3, 1, 1, 1, 1, 1, 1,
	1, 2, 3, 1, 2, 3, 1, 1, 4, 1, 2, 3
};

extern unsigned char knight_run0[];
extern unsigned char knight_run1[];
extern unsigned char knight_run2[];
extern unsigned char knight_run3[];

extern unsigned char floor_1[];
extern unsigned char floor_2[];
extern unsigned char floor_3[];
extern unsigned char floor_4[];

unsigned int tule_tilt, tule_size, tule_v, start_v, knight_tics, knight_step;
unsigned char knight_status;
unsigned int tule_stretch = 0x0001;
unsigned int tule_tilt_const = 0x0008;
unsigned int tule_size_const = 0x0008;
unsigned int tule_v_const = 0x0001;

unsigned int distance =0;

SCB_REHV_PAL knight = {
  BPP_4 | TYPE_NORMAL, 
  REHV,
  0x01,
  0x0000,
  knight_run0,
  40, 20,
  0x0100, 0x0100,
  // palette values can be found in .pal file provided by sprpck
  {0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF}  
};

SCB_REHVST_PAL tule_1 = {
  BPP_4 | TYPE_NORMAL, 
  REHVST ,
  0x01,
  0x0000,
  floor_3,
  40, 16,
  0x0100, 0x0100,
  //0x0020, 0xFF0F,
  //0x0004, 0x0040,
  0x0000, 0x0000,
  {0x78,0x93,0x01,0x67,0x89,0xAB,0xCD,0xEF}
};

SCB_REHVST_PAL tule_2, tule_3, tule_4, tule_5, tule_6;
SCB_REHVST_PAL tule_7, tule_8, tule_9, tule_10, tule_11, tule_12;
SCB_REHVST_PAL tule_13, tule_14, tule_15, tule_16, tule_17, tule_18;
SCB_REHVST_PAL tule_19, tule_20, tule_21, tule_22, tule_23, tule_24;
SCB_REHVST_PAL tule_25, tule_26, tule_27, tule_28, tule_29, tule_30;
SCB_REHVST_PAL tule_31, tule_32, tule_33, tule_34, tule_35, tule_36;
SCB_REHVST_PAL *work_spr; 

//functions goes here:
unsigned char *get_tule(int tule_index){
	switch(tule_index){
		case RUN:
			return floor_1;
			break;
		case 2:
			return floor_2;
			break;
		case 3:
			return floor_3;
			break;
		case 4:
			return floor_4;
			break;
		default:
			return floor_1;
	}
}
unsigned char *get_knight(){
	switch(knight_status){
		case RUN:
			switch(knight_tics){
				case 0:
					return knight_run0;
					break;
				case 1:
					return knight_run1;
					break;
				case 2:
					return knight_run2;
					break;
				case 3:
					return knight_run3;
					break;
			}
			break;
		case HIT:
			return knight_run1;
			break;
		default:
			return knight_run0;
	}
}

void init_level(){
	signed int x, y;
	tgi_install(&tgi_static_stddrv);
	tgi_init();
	
	tule_6 = tule_5 = tule_4 = tule_3 = tule_2 = tule_1;
	tule_7 = tule_8 = tule_9 = tule_10 = tule_11 = tule_12 = tule_1;
	tule_13= tule_14= tule_15= tule_16= tule_17= tule_18 = tule_1;
	tule_19= tule_20= tule_21= tule_22= tule_23= tule_24 = tule_1;
	tule_25= tule_26= tule_27= tule_28= tule_29= tule_30 = tule_1;
	tule_31= tule_32= tule_33= tule_34= tule_35= tule_36 = tule_1;
	
	tule_25.next = (char *)&tule_36;
	tule_34.next = (char *)&tule_35;
	tule_33.next = (char *)&tule_34;
	tule_32.next = (char *)&tule_33;
	tule_31.next = (char *)&tule_32;
	tule_30.next = (char *)&tule_31;
	tule_29.next = (char *)&tule_30;
	tule_28.next = (char *)&tule_29;
	tule_27.next = (char *)&tule_28;
	tule_26.next = (char *)&tule_27;
	tule_25.next = (char *)&tule_26;
	tule_24.next = (char *)&tule_25;
	tule_23.next = (char *)&tule_24;
	tule_22.next = (char *)&tule_23;
	tule_21.next = (char *)&tule_22;
	tule_20.next = (char *)&tule_21;
	tule_19.next = (char *)&tule_20;
	tule_18.next = (char *)&tule_19;
	tule_17.next = (char *)&tule_18;
	tule_16.next = (char *)&tule_17;
	tule_15.next = (char *)&tule_16;
	tule_14.next = (char *)&tule_15;
	tule_13.next = (char *)&tule_14;
	tule_12.next = (char *)&tule_13;
	tule_11.next = (char *)&tule_12;
	tule_10.next = (char *)&tule_11;
	tule_9.next = (char *)&tule_10;
	tule_8.next = (char *)&tule_9;
	tule_7.next = (char *)&tule_8;
	tule_6.next = (char *)&tule_7;
	tule_5.next = (char *)&tule_6;
	tule_4.next = (char *)&tule_5;
	tule_3.next = (char *)&tule_4;
	tule_2.next = (char *)&tule_3;
	tule_1.next = (char *)&tule_2;
	
	tule_size = 0x0100;
	work_spr = &tule_1;
	for(y=0;y<map_v;y++){
		for(x=0;x<map_h;x++){
			
			work_spr->vpos=40+y*16;
			work_spr->hpos=x*16;
			work_spr->stretch=tule_stretch;
			work_spr->hsize=tule_size;

			work_spr->data=get_tule(level_map[y*map_h+x]);
			work_spr = (SCB_REHVST_PAL *)work_spr->next;
		}
		tule_size+=tule_size_const;
	}
	
	CLI();
	while (tgi_busy()) ;
	tgi_setpalette(palette);
	tgi_setcolor(COLOR_WHITE);
	tgi_setbgcolor(COLOR_BLACK);
	
	/*
	tgi_sprite(&tule_1);
	tgi_updatedisplay();
	*/
	
	pause = 0;
}



void physics(){
	// do physicals stuff here ;)
	signed int x, y;
	tule_v = 0x0000;
	//tule_v = parallax_magic;
	//start_v = 0x0000;
	work_spr = &tule_1;
	for(y=0;y<map_v;y++){
		for(x=0;x<map_h;x++){
			work_spr->hpos=x*(16+tule_v)-distance%16;//+start_v;
			//work_spr->hpos=x*20-distance%16;
			
			work_spr = (SCB_REHVST_PAL *)work_spr->next;
		}
		tule_v+=tule_v_const;
		//start_v+=parallax_magic_v;
	}
	
	//tule_tilt =0x0000;
	if(distance%16==0){
		x_map = (distance/16)%map_h;
		
		work_spr = &tule_1;
		for(y=0;y<map_v;y++){
			//tule_tilt =0x0000;
			for(x=x_map;x<map_h;x++){
				work_spr->data=get_tule(level_map[y*map_h+x]);
				//work_spr->tilt=tule_tilt;
				//tule_tilt+=tule_tilt_const;
				
				work_spr = (SCB_REHVST_PAL *)work_spr->next;
			}
			for(x=0;x<x_map;x++){
				work_spr->data=get_tule(level_map[y*map_h+x]);
				//work_spr->tilt=tule_tilt;
				//tule_tilt+=tule_tilt_const;
				
				work_spr = (SCB_REHVST_PAL *)work_spr->next;
			}
		}
			work_spr = &tule_1;
		for(y=0;y<map_v;y++){
			//tule_tilt =0xFFA0;
			tule_tilt =0x0000;
			//tule_tilt = parallax_magic;
			for(x=x_map;x<map_h;x++){
				work_spr->tilt=tule_tilt;
				tule_tilt+=tule_tilt_const;
				//tule_tilt+=x_map;
				
				work_spr = (SCB_REHVST_PAL *)work_spr->next;
			}
			for(x=0;x<x_map;x++){
				work_spr->tilt=tule_tilt;
				tule_tilt+=tule_tilt_const;
				//tule_tilt+=x_map;
				
				work_spr = (SCB_REHVST_PAL *)work_spr->next;
			}
		}
	}
	distance++;
	
}

void game_logic(){
	unsigned char joy;
	
	tgi_clear();
	tgi_sprite(&tule_1);
	tgi_sprite(&knight);
	
	joy = joy_read(JOY_1);
	
	
	if (JOY_BTN_1(joy) || JOY_BTN_2(joy) ) {
		game_status = EXIT;
	}
	else{
		if (game_status == EXIT ) game_status = LEVEL_UP;
	}

}

void player_logic(){
	
	if(knight_step>8){
		knight_tics++;
		knight_step=0;
		if(knight_tics>3){
			knight_tics=0;
		}
		knight.data=get_knight();
	}

	knight_step++;
}


void game(){
	//char text[20];
	
	playing = 1;
	level = 1;
	
	
	
	game_status = NORMAL;
	tgi_clear();
	init_level();
	
	// chiper init + start
	/*
	init_music();
	start_music();
	*/
	
	knight_step = 0;
	knight_tics = 0;
	knight_status = RUN;

	
	while(playing){
		if (!tgi_busy())
		{
			if(game_status == LEVEL_UP){
				level++;
				game_status = NORMAL;
				//init_level();
				//srand(randomizator2);
				{playing = 0;}
			}
			if(!pause){
				game_logic();
				player_logic();
				physics();
			} else{
				tgi_clear();
				
				tgi_outtextxy(36, 48, "GAME PAUSED");
			}
			/*
			itoa(level, text, 10);
			tgi_outtextxy(8, 2, text);
			*/
			std_functions();
			tgi_updatedisplay();
			randomizator2++;
		}
	}	
	// chiper stop
	//asm("jsr SndStopAll");

}