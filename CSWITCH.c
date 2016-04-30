/*****************************************************************/
/*                                                               */
/*   CASIO fx-9860G SDK Library                                  */
/*                                                               */
/*   File name : CSWITCH.c                                       */
/*                                                               */
/*   Copyright (c) 2016 CASIO COMPUTER CO., LTD.                 */
/*                                                               */
/*****************************************************************/

#include "drawlib.h"


//****************************************************************************
//  AddIn_main (Sample program main function)
//
//  param   :   isAppli   : 1 = This application is launched by MAIN MENU.
//                        : 0 = This application is launched by a strip in eACT application.
//
//              OptionNum : Strip number (0~3)
//                         (This parameter is only used when isAppli parameter is 0.)
//
//  retval  :   1 = No error / 0 = Error
//
//****************************************************************************

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define BLACK           1
#define WHITE           0
#define TRUE            1
#define FALSE           0

typedef enum {CIRCLE, LINE} Shape;

typedef struct Star{
    int x,y,color;
} Star;

typedef struct Obstacle{
    int x, y, color, rad, thickness;
    Shape shape;    
} Obstacle;

typedef struct Camera{
    int x, y;
} Camera;

unsigned int key, key1, key2, unused = 0;
int ball_x = SCREEN_WIDTH/2, ball_y = SCREEN_HEIGHT/2;
int ball_speed = 5;
int BALL_RADIUS = 4;
Star stars[10];
Obstacle obstacles[10];
Camera cam;
int angle = 0;

void fill_screen(int color);
void ball_update();
void ball_jump();
void ball_draw();
void draw_stars();
void collision_detection();

int handle_keys(){
    if(key1 == 3 && key2 == 2){ //EXE key
        ball_jump();
    }else if(key1 == 4 && key2 == 8){ //EXIT key
        return FALSE;
    }else if(key1 == 4 && key2 == 9){ //MENU key
        return FALSE;
    }
    key1 = key2 = 0;
    return TRUE;
}

void draw_obstacle(Obstacle obstacle){
    int i;
    for(i = obstacle.rad-obstacle.thickness; i <= obstacle.rad; i+=2){
            draw_arc(obstacle.x-cam.x, obstacle.y, i, angle, angle+90, obstacle.color);
            draw_arc(obstacle.x-cam.x, obstacle.y, i, angle, angle+90, obstacle.color);
        }
    draw_circle(obstacle.x-cam.x, obstacle.y, obstacle.rad-obstacle.thickness, obstacle.color);
    draw_circle(obstacle.x-cam.x, obstacle.y, obstacle.rad, obstacle.color);
}

void gen_obstacles(){
    int i;
    for(i = 0; i <= 9; i++){
        Star star;
        Obstacle obstacle;
        star.x = i*150+150;
        star.y = SCREEN_HEIGHT/2;
        star.color = WHITE;
        obstacle.x = star.x;
        obstacle.y = star.y;
        obstacle.rad = 25;
        obstacle.thickness = 5;
        obstacle.color = WHITE;
        stars[i] = star;
        obstacles[i] = obstacle;
    }
}

void fill_old_squares(){
    int i;
    draw_filled_rectangle(ball_x-BALL_RADIUS-cam.x, ball_y-BALL_RADIUS, BALL_RADIUS*2, BALL_RADIUS*2, BLACK);
    for(i = 0; i <= 9; i++){
        Star star = stars[i];
        if(star.x-cam.x <= SCREEN_WIDTH && star.x-cam.x >= 0){
            draw_filled_star(star.x-cam.x, star.y, !star.color);
            draw_filled_circle(star.x-cam.x, star.y, 30, !star.color);
        }
        
    }
    
}

int AddIn_main(int isAppli, unsigned short OptionNum){
    int i;
    Bdisp_AllClr_DDVRAM();
    fill_screen(BLACK);
    
    gen_obstacles();
    cam.x = 0; cam.y = 0;
    while(handle_keys()){
        //Sleep(20);
        
        Bkey_GetKeyWait(&key1, &key2, KEYWAIT_HALTOFF_TIMEROFF, 0, 1, &unused);
        
        fill_old_squares();      
        angle+=3;
        if(angle >= 360)
            angle = 0;
        for(i = 0; i < sizeof(obstacles)/sizeof(obstacles[0]); i++){
            Obstacle obstacle = obstacles[i];
            if(obstacle.x-cam.x > -obstacle.rad/2 && obstacle.x-cam.x <= SCREEN_WIDTH+obstacle.rad/2)
                draw_obstacle(obstacle);
        }
        ball_update();
        ball_draw(WHITE);
        draw_stars();
        Bdisp_PutDisp_DD();
    }

    return 1;
}

void draw_stars(){
    int i;
    for(i = 0; i <= 9; i++){
        Star star = stars[i];
        if(star.x-cam.x <= SCREEN_WIDTH && star.x-cam.x >= 0)
            draw_filled_star(star.x-cam.x, star.y, star.color);
    }
}

void ball_jump(){
    ball_speed = 4;
}

void ball_update(){
    ball_x+=ball_speed;
    if(cam.x <= ball_x-SCREEN_WIDTH*0.4)
        cam.x = ball_x-SCREEN_WIDTH*0.4;
    ball_speed-=1;
    collision_detection();
}

void ball_draw(int color){
    if(ball_x-cam.x >= 0){
        draw_filled_circle(ball_x-cam.x, ball_y, BALL_RADIUS, color);
    }
}

void fill_screen(int color){
    int x, y;
    for(x = 0; x < SCREEN_WIDTH; x++){
        for(y = 0; y < SCREEN_HEIGHT; y++){
            Bdisp_SetPoint_VRAM(x,y,color);
        }
    }
}



void collision_detection(){
    
}

//****************************************************************************
//**************                                              ****************
//**************                 Notice!                      ****************
//**************                                              ****************
//**************  Please do not change the following source.  ****************
//**************                                              ****************
//****************************************************************************


#pragma section _BR_Size
unsigned long BR_Size;
#pragma section


#pragma section _TOP

//****************************************************************************
//  InitializeSystem
//
//  param   :   isAppli   : 1 = Application / 0 = eActivity
//              OptionNum : Option Number (only eActivity)
//
//  retval  :   1 = No error / 0 = Error
//
//****************************************************************************
int InitializeSystem(int isAppli, unsigned short OptionNum)
{
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}

#pragma section

