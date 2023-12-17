#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <conio.h>
#include <joystick.h> 

#include "game.h"

extern void init_music();
extern void start_music();
extern void std_functions();

extern unsigned int level;

unsigned int playing, pause;
unsigned int x_map, y_map;
unsigned char game_status;
unsigned int randomizator2 = 0;

extern unsigned char palette[];

int map_h = 12;
int map_v = 3;
unsigned int level_map[36];
unsigned int row_current[3],row_last[3];
/*
unsigned int level_map[] = {
	1, 2, 3, 1, 2, 3, 1, 1, 4, 1, 2, 3,
	1, 1, 4, 1, 2, 3, 1, 1, 9, 1, 1, 1,
	1, 2, 3, 1, 2, 3, 1, 1, 4, 1, 2, 3
};

unsigned int level_map[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

unsigned int level_map[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2,2, 2, 2, 2,2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};*/


extern unsigned char knight_run0[];
extern unsigned char knight_run1[];
extern unsigned char knight_run2[];
extern unsigned char knight_run3[];
extern unsigned char knight_hit[];

extern unsigned char floor_1[];
extern unsigned char floor_2[];
extern unsigned char floor_3[];
extern unsigned char floor_4[];
extern unsigned char floor_spikes[];

unsigned int tule_tilt, tule_size, tule_v, start_v, knight_tics, knight_step, x_knight;
unsigned int current_tule, difficulty, debug;
unsigned char knight_status;
unsigned int tule_stretch = 0x0001;
unsigned int tule_tilt_const = 0x0008;
unsigned int tule_size_const = 0x0008;
unsigned int tule_v_const = 0x0001;
//unsigned int tule_v_const = 0x0003;

unsigned int distance =0;

SCB_REHV_PAL knight = {
  BPP_4 | TYPE_NORMAL, 
  REHV,
  0x01,
  0x0000,
  knight_run0,
  10, 40,
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
  {0x78,0x95,0xA1,0x67,0x89,0xAB,0xCD,0xEF}
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
		case 1:
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
		case 5:
			return floor_spikes;
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

void add_rnd_row(row_number){
	unsigned int i,j,spike_counter;
	
	spike_counter=0;
	//random the current row
	for(i=0;i<map_v;i++){
		row_current[i]=rand()%4+1;
		if(rand()<difficulty){
			row_current[i]=5;
		}
		if(row_last[i]>4){
			spike_counter++;
		}
	}	
	debug = row_last[1];
	//forbid to get full spike row
	if(row_current[0]>4 && row_current[1]>4 && row_current[2]>4){
		i=rand()%3;
		row_current[i]=1;
	} 
	//forbid no issue
	if(spike_counter>1){
		for(i=0;i<map_v;i++){
			if(row_last[i]<6){
				row_current[i]=1;
			}
		}
	}
	j=0;
	//copy current row to level map
	for(i=0;i<map_v;i++){
		level_map[row_number+j]=row_current[i];
		j+=map_h;
	}
	//copy current to last
	for(i=0;i<map_v;i++){
		row_last[i]=row_current[i];
	}
}

void check_collide(){
	unsigned int row_v, row_h,i;
	row_v = (knight.vpos-14)>>4;
	row_h =x_knight;
	//row_h = ((distance+24)>>4)%map_h;
	//row_h = ((distance+24+row_v)>>4)%map_h;	//rocket science!
	for(i=0;i<row_v;i++){
		row_h+=map_h;
	}
	current_tule = level_map[row_h];
}

void init_level(){
	unsigned int x, y;
	
	tule_6 = tule_5 = tule_4 = tule_3 = tule_2 = tule_1;
	tule_7 = tule_8 = tule_9 = tule_10 = tule_11 = tule_12 = tule_1;
	tule_13= tule_14= tule_15= tule_16= tule_17= tule_18 = tule_1;
	tule_19= tule_20= tule_21= tule_22= tule_23= tule_24 = tule_1;
	tule_25= tule_26= tule_27= tule_28= tule_29= tule_30 = tule_1;
	tule_31= tule_32= tule_33= tule_34= tule_35= tule_36 = tule_1;
	
	tule_36.next = 0x0000;
	tule_35.next = (char *)&tule_36;
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
			level_map[y*map_h+x]=1;
			//work_spr->vpos=40+y*16;
			//work_spr->hpos=x*16;
			//optimized!
			work_spr->vpos=40+(y<<4);
			work_spr->hpos=x<<4;
			work_spr->stretch=tule_stretch;
			work_spr->hsize=tule_size;

			work_spr->data=get_tule(level_map[y*map_h+x]);
			work_spr = (SCB_REHVST_PAL *)work_spr->next;
		}
		tule_size+=tule_size_const;
	}
	for(y=0;y<map_v;y++){
		row_current[y]=1;
		row_last[y]=1;
	}
	
	
	knight.data=knight_run0;
	knight.vpos=40;
	knight.hpos=10;
	
	
	
	pause = 0;
}



void physics(){
	// do physicals stuff here ;)
	unsigned int x, y, y_multi;
	tule_v = 0x0000;

	work_spr = &tule_1;
	for(y=0;y<map_v;y++){
		y_multi =0;
		for(x=0;x<map_h;x++){
			work_spr->hpos=y_multi-(distance%16);	//optimized!
			
			work_spr = (SCB_REHVST_PAL *)work_spr->next;
			y_multi+=16+tule_v;
		}
		tule_v+=tule_v_const;
		
	}
	
	if(distance%16==0){
		x_map = (distance>>4)%map_h;
		
		
		work_spr = &tule_1;
		y_multi =0;
		for(y=0;y<map_v;y++){
			for(x=x_map;x<map_h;x++){
				//work_spr->data=get_tule(level_map[y*map_h+x]);	//optimized!
				work_spr->data=get_tule(level_map[y_multi+x]);
				
				work_spr = (SCB_REHVST_PAL *)work_spr->next;
			}
			for(x=0;x<x_map;x++){
				//work_spr->data=get_tule(level_map[y*map_h+x]);	//optimized!
				work_spr->data=get_tule(level_map[y_multi+x]);
				
				work_spr = (SCB_REHVST_PAL *)work_spr->next;
			}
			y_multi+=map_h;
		}
		work_spr = &tule_1;
		for(y=0;y<map_v;y++){
			tule_tilt =0x0000;

			for(x=x_map;x<map_h;x++){
				work_spr->tilt=tule_tilt;
				tule_tilt+=tule_tilt_const;
				
				work_spr = (SCB_REHVST_PAL *)work_spr->next;
			}
			for(x=0;x<x_map;x++){
				work_spr->tilt=tule_tilt;
				tule_tilt+=tule_tilt_const;
				
				work_spr = (SCB_REHVST_PAL *)work_spr->next;
			}
		}
		//random add map_h
		if(x_map==0){
			add_rnd_row(map_h);
		}else{
			add_rnd_row(x_map-1);
		}
		
	}
	if (distance%16==8){
		//no shame hack:
		x_knight = ((distance+24)>>4)%map_h;
	}
	
	
	check_collide();
	if(current_tule == 5 && knight_status == RUN){
		knight_status = HIT;
		//triky hack
		knight_step = 1;
		knight.data=knight_hit;
	}
	
}

void game_logic(){
	unsigned char joy;
	
	//tgi_clear();
	/*tgi_sprite(&tule_1);
	tgi_sprite(&knight);*/
	
	joy = joy_read(JOY_1);
	
	if(knight_status == RUN){
		if (JOY_UP(joy) && knight.vpos > 20) {
			knight.vpos--;
		}
		if (JOY_DOWN(joy) && knight.vpos < 58) {
			knight.vpos++;
		}
		distance++;
		difficulty+=2;
	}

}

void player_logic(){
	if(knight_step>8 && knight_status == RUN){
		knight_tics++;
		knight_step=0;
		if(knight_tics>3){
			knight_tics=0;
		}
		knight.data=get_knight();
	}
	if(knight_status == HIT){
		
		//knight_step++;
		if(knight_step%32==0){
			knight.sprctl0 = BPP_4 | TYPE_NORMAL;
			knight.hpos-=16;
		}else{
			if(knight_step%16==0){
				knight.sprctl0 = BPP_4 | TYPE_NORMAL | HFLIP;
				knight.hpos+=16;
			}
		}
		if(knight_step>100){
			game_status = LEVEL_UP;
		}
		
	}
	
	knight_step++;
}


void game(){
	char text[20];
	
	playing = 1;
	level = 0;
	current_tule =0;
	debug =0;
	difficulty = 0;
	
	game_status = NORMAL;

	tgi_clear();

	init_level();
	
	// chiper init + start
	init_music();
	start_music();
	
	
	knight_step = 0;
	knight_tics = 0;
	knight_status = RUN;
	
	distance = 0;
	
	tgi_updatedisplay();
	
	
	while(playing){
		if (!tgi_busy())
		{
			tgi_clear();
			if(game_status == LEVEL_UP){
				level=distance;
				game_status = NORMAL;
				playing = 0;
			}
			if(!pause){
				tgi_sprite(&tule_1);
				tgi_sprite(&knight);
				game_logic();
				player_logic();
				physics();
			} else{
				
				tgi_outtextxy(36, 48, "GAME PAUSED");
			}
			
			//debug
			/*
			tgi_setcolor(COLOR_RED);
			itoa(debug, text, 10);
			tgi_outtextxy(64, 8, text);
			*/
			
			
			std_functions();
			tgi_updatedisplay();
			randomizator2++;
		}
	}	
	// chiper stop
	asm("jsr SndStopAll");

}