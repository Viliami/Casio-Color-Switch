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
#include "stdlib.h"


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
#define PARTICLE_COUNT  20

typedef enum {CIRCLE, DOUBLE_CIRCLE, LINE} Shape;
typedef enum {MENU, GAMEPLAY, GAMEOVER} State;

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


typedef struct Particle{
    int x, y, vel_x, vel_y, rad;
} Particle;

int BALL_DIAMETER = 4;
unsigned int key, key1, key2, unused = 0;
int ball_x = SCREEN_WIDTH/2, ball_y = SCREEN_HEIGHT/2;
int ball_speed = 5;
Star stars[10];
Obstacle obstacles[10];
Particle particles[PARTICLE_COUNT];
Camera cam;
State gamestate = MENU;
int angle = 0;
int score = 0, highscore = 0;
int ball_dead = FALSE;

void fill_screen(int color);
void fill_old_squares();
void ball_update();
void ball_jump();
void ball_draw();
void draw_stars();
void draw_menu();
void draw_score(int color);
void draw_obstacle(Obstacle obstacle);
void draw_obstacles();
void update_obstacles();
void collision_detection();
Obstacle new_obstacle(int x, int y, int COLOR);

int random_number(int min, int max){
    int range = max-min+1;
    return (int)(rand()%range+min);
}

int handle_keys(){
    if(key1 == 3 && key2 == 2){ //EXE key
        switch(gamestate){
            case MENU:
                fill_screen(BLACK);
                gamestate = GAMEPLAY;
            case GAMEPLAY:
                ball_jump();
                break;
            case GAMEOVER:
                fill_screen(BLACK);
                gamestate = GAMEPLAY;
                break;
        }
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
    if(obstacle.shape == CIRCLE){
        for(i = obstacle.rad-obstacle.thickness; i <= obstacle.rad; i+=2){
            draw_arc(obstacle.x-cam.x, obstacle.y, i, angle, angle+90, obstacle.color);
        }
        draw_circle(obstacle.x-cam.x, obstacle.y, obstacle.rad-obstacle.thickness, obstacle.color);
        draw_circle(obstacle.x-cam.x, obstacle.y, obstacle.rad, obstacle.color);
    }else if(obstacle.shape == DOUBLE_CIRCLE){
        for(i = obstacle.rad-obstacle.thickness; i <= obstacle.rad; i+=2){
            draw_arc(obstacle.x-cam.x, obstacle.y-obstacle.rad, i, angle, angle+90, obstacle.color);
            draw_arc(obstacle.x-cam.x, obstacle.y+obstacle.rad, i, 270-angle, 270-angle+90, obstacle.color);
        }
        draw_circle(obstacle.x-cam.x, obstacle.y-obstacle.rad, obstacle.rad-obstacle.thickness, obstacle.color);
        draw_circle(obstacle.x-cam.x, obstacle.y-obstacle.rad, obstacle.rad, obstacle.color);
        draw_circle(obstacle.x-cam.x, obstacle.y+obstacle.rad, obstacle.rad-obstacle.thickness, obstacle.color);
        draw_circle(obstacle.x-cam.x, obstacle.y+obstacle.rad, obstacle.rad, obstacle.color);
    }
}

void gen_obstacles(){
    int i;
    for(i = 0; i <= 9; i++){
        int rand = random_number(0,1);
        Star star;
        Obstacle obstacle;
        //obstacle = new_obstacle(star.x, star.y, WHITE);
        star.x = i*150+150;
        star.y = SCREEN_HEIGHT/2;
        star.color = WHITE;
        obstacle.x = star.x;
        obstacle.y = star.y;
        obstacle.color = WHITE;
        if(rand == 0){
            obstacle.rad = 25;
            obstacle.thickness = 5;
            obstacle.shape = CIRCLE;
        }else if(rand == 1){
            obstacle.rad = 15;
            obstacle.thickness = 3;
            obstacle.shape = DOUBLE_CIRCLE;
        }
        stars[i] = star;
        obstacles[i] = obstacle;
    }
}

void fill_old_squares(){
    int i;
    switch(gamestate){
        case MENU:
            draw_filled_rectangle(SCREEN_WIDTH/2-46, SCREEN_HEIGHT/2-26, 51, 51, BLACK);
            break;
        case GAMEPLAY:
            if(ball_dead){
                for(i = 0; i <= PARTICLE_COUNT-1; i++){
                    Particle p = particles[i];
                    draw_filled_circle(p.x-cam.x, p.y, p.rad, BLACK);
                }
            }else{
                draw_filled_rectangle(ball_x-BALL_DIAMETER-cam.x, ball_y-BALL_DIAMETER, BALL_DIAMETER*2, BALL_DIAMETER*2, BLACK);
            }
            for(i = 0; i <= 9; i++){
                Star star = stars[i];
                Obstacle obstacle = obstacles[i];
                if(star.x-cam.x <= SCREEN_WIDTH && star.x-cam.x >= 0){
                    draw_filled_star(star.x-cam.x, star.y, !star.color);
                }
                if(obstacle.x-cam.x < SCREEN_WIDTH+obstacle.rad && obstacle.x-cam.x > -obstacle.rad){
                    if(obstacle.shape == CIRCLE){
                        draw_filled_circle(obstacle.x-cam.x, obstacle.y, 29, !obstacle.color);
                    }else if(obstacle.shape == DOUBLE_CIRCLE){
                        draw_filled_circle(obstacle.x-cam.x, obstacle.y, 30, !obstacle.color);
                    }
                }
            }
            break;
        case GAMEOVER:
            break;
    }
}

int AddIn_main(int isAppli, unsigned short OptionNum){
    Bdisp_AllClr_DDVRAM();
    fill_screen(BLACK);
    
    gen_obstacles();
    cam.x = 0; cam.y = 0;
    Sleep(50);
    while(handle_keys()){
        Bkey_GetKeyWait(&key1, &key2, KEYWAIT_HALTOFF_TIMEROFF, 0, 1, &unused);
        
        fill_old_squares();

        //updating
        switch(gamestate){
            case MENU:
                update_obstacles();
                break;
            case GAMEPLAY:
                update_obstacles();
                ball_update();
                break;
            case GAMEOVER:
                break;
        }
        
        //drawing
        switch(gamestate){
            case MENU:
                draw_menu();
                break;
            case GAMEPLAY:
                draw_score(WHITE);
                ball_draw(WHITE);
                draw_stars();
                draw_obstacles();
                break;
            case GAMEOVER:
                break;
        }

        Bdisp_PutDisp_DD();
        
    }

    return 1;
}

void draw_play_button(int x, int y, int color){
    draw_filled_circle(x,y,12,color);
    x-=7;
    y-=5;
    draw_line(x+1,y,x+13,y,!color);
    draw_line(x+1,y+1,x+12,y+1,!color);
    draw_line(x+2,y+2,x+12,y+2,!color);
    draw_line(x+3,y+3,x+11,y+3,!color);
    draw_line(x+3,y+4,x+11,y+4,!color);
    draw_line(x+4,y+5,x+10,y+5,!color);
    draw_line(x+4,y+6,x+10,y+6,!color);
    draw_line(x+5,y+7,x+9,y+7,!color);
    draw_line(x+5,y+8,x+9,y+8,!color);
    draw_line(x+6,y+9,x+8,y+9,!color);
    draw_line(x+6,y+10,x+8,y+10,!color);
    draw_pixel(x+7,y+11,!color);
}

void draw_menu(){
    int i;
    for(i = 16; i <= 20; i++){
        draw_arc(SCREEN_WIDTH/2-20, SCREEN_HEIGHT/2, i, angle, angle+90, WHITE);
        draw_arc(SCREEN_WIDTH/2-20, SCREEN_HEIGHT/2, i+6, 270-angle, 270-angle+90, WHITE);    
    }
    draw_circle(SCREEN_WIDTH/2-20, SCREEN_HEIGHT/2, 20, WHITE);
    draw_circle(SCREEN_WIDTH/2-20, SCREEN_HEIGHT/2, 16, WHITE);
    draw_circle(SCREEN_WIDTH/2-20, SCREEN_HEIGHT/2, 26, WHITE);
    draw_circle(SCREEN_WIDTH/2-20, SCREEN_HEIGHT/2, 22, WHITE);
    draw_play_button(SCREEN_WIDTH/2-20, SCREEN_HEIGHT/2, WHITE);
    draw_vertical_text("Color",100,10,WHITE);
    draw_vertical_text("Switch",90,10,WHITE);
}

void draw_obstacles(){
    int i;
    for(i = 0; i < 9; i++){
        Obstacle obstacle = obstacles[i];
        if(obstacle.x-cam.x > -obstacle.rad/2 && obstacle.x-cam.x <= SCREEN_WIDTH+obstacle.rad/2)
            draw_obstacle(obstacle);
    }
}

void update_obstacles(){
    angle+=5;
    if(angle >= 360)
        angle = 0;
    if(gamestate == MENU)
        angle+=2;
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
    int i;
    if(!ball_dead){
        ball_x+=ball_speed;
        if(cam.x <= ball_x-SCREEN_WIDTH*0.4)
            cam.x = ball_x-SCREEN_WIDTH*0.4;
        ball_speed-=1;
        collision_detection();
    }else{
        for(i = 0; i <= PARTICLE_COUNT-1; i++){
            particles[i].x+=particles[i].vel_x;
            particles[i].y+=particles[i].vel_y;
           particles[i].vel_x--;
        }
    }
}

void ball_draw(int color){
    int i;
    if(ball_x-cam.x >= 0 && !ball_dead){
        draw_filled_circle(ball_x-cam.x, ball_y, BALL_DIAMETER, color);
    }
    if(ball_dead){
        for(i = 0; i < PARTICLE_COUNT-1; i++){
            Particle p = particles[i];
            draw_pixel(p.x-cam.x, p.y, color);
            draw_filled_circle(p.x-cam.x, p.y, p.rad, color);
        }
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

void draw_score(int color){
    int ones, tens, hundreds;
    if(score < 10){
        draw_filled_rectangle(122,1,5,4,!color);
        draw_number(score, 126, 1, color);
    }else if(score < 100){
        tens = (int)score/10;
        ones = score-(tens*10);
        draw_filled_rectangle(122,1,5,9,!color);
        draw_number(tens, 126, 1, color);
        draw_number(ones, 126, 6, color);
    }else if(score < 1000){
        hundreds = (int)score/100;
        tens = (int)((score-(hundreds*100))/10);
        ones = score-hundreds*100-tens*10;
        draw_filled_rectangle(122,1,5,15,!color);
        draw_number(hundreds, 126, 1, color);
        draw_number(tens, 126, 6, color);
        draw_number(ones, 126, 10, color);
    }
}

void ball_explode(){
    int i;
    ball_dead = TRUE;
    for(i = 0; i <= PARTICLE_COUNT-1; i++){
        Particle particle;
        particle.x = ball_x;
        particle.y = ball_y;
        particle.vel_x = (int)rand()%15+(-5);
        particle.vel_y = (int)rand()%10+(-5);
        particle.rad = 1;
        particles[i] = particle;
    }
}

Obstacle new_obstacle(int x, int y, int color){
   // int random;
    Obstacle obstacle;
    obstacle.x = x;
    obstacle.y = y;
    obstacle.color = color;
    /*random = random_number(0,1);
    if(random == 0){
        obstacle.shape = CIRCLE;
    }else{
        obstacle.shape = DOUBLE_CIRCLE;
    }*/
    return obstacle;
}

void collision_detection(){
    int i;
    Star star;
    Obstacle obstacle;
    if(ball_x >= stars[0].x){
        score++;
        for(i = 0; i <= 8; i++){
            stars[i] = stars[i+1];
        }
        star.x = stars[8].x+150;
        star.y = stars[8].y;
        star.color = WHITE;
        stars[9] = star;
        new_obstacle(10,10,WHITE);
    }
    for(i = 0; i <= 9; i++){
        obstacle = obstacles[i];
        if(obstacle.x -cam.x > 0 && obstacle.x-cam.x < SCREEN_WIDTH){
            if(obstacle.shape == CIRCLE){
                if((ball_x+BALL_DIAMETER/2 >= obstacle.x+obstacles[i].rad-obstacles[i].thickness && ball_x-BALL_DIAMETER/2 <= obstacles[i].x+obstacles[i].rad) && !(angle >= 90 && angle <= 180)){
                    ball_explode();
                }else if((ball_x+BALL_DIAMETER/2 >= obstacle.x-obstacle.rad && ball_x-BALL_DIAMETER/2 <= obstacle.x-obstacle.rad+obstacles[i].thickness) && !(angle >= 270 && angle <= 360)){
                    ball_explode();
                }
            }else if(obstacle.shape == DOUBLE_CIRCLE){
                if(ball_x+BALL_DIAMETER/2 >= obstacle.x-7 && ball_x-BALL_DIAMETER/2 <= obstacle.x-7 && !(angle >= 180 && angle <= 270)){
                    ball_explode();
                }
            }
        }
    }
    if(ball_x-cam.x <= -BALL_DIAMETER/2){
        ball_explode();
    }
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

