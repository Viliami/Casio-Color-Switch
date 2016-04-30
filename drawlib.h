#include "fxlib.h"
#include "mathlib.h"

void draw_pixel(int x, int y, int color){
    if(x >= 0 && x <= 127)
        Bdisp_SetPoint_VRAM(x,y,color);
}

void draw_line(int x0, int y0, int x1, int y1, int color){
    int dX = x1-x0, dY = y1-y0, dErr = 0, x = 0, y = 0, error = 0;
    
    if(dX == 0){
        for(y = y0; y <= y1; y++){
            draw_pixel(x0,y, color);
        }
        return;
    }else if(dY == 0){
        for(x = x0; x <= x1; x++){
            draw_pixel(x,y0, color);
        }
        return;
    }else{
        dErr = dY/dX;
        if(dErr < 0)
            dErr = dErr*-1;
    }
    //for x in range(x0), x1)){
    for(x = x0; x <= x1; x++){
        draw_pixel(x,y, color);
        error+=dErr;
        while(error >= 0.5){
            draw_pixel(x,y, color);
            y += sgn(dY);
            error -= 1;
        }
    }
}

void draw_rectangle(int x, int y, int w, int h, int color){
    
}

void draw_filled_rectangle(int x, int y, int w, int h, int color){
    int ix, iy;
    for(ix=x; ix <= x+w; ix++){
        for(iy=y; iy <= y+h; iy++){
            draw_pixel(ix,iy,color);
        }
    }
}

void draw_circle(int cX, int cY, int rad, int color){
    int x = rad, y = 0, decOver = 1-rad;  // Decision criterion divided by 2 evaluated at x=r, y=0

    while(y <= x){
        draw_pixel( x + cX,  y + cY, color); //Octant 1
        draw_pixel(-x + cX,  y + cY, color); //Octant 4
        draw_pixel( y + cX,  x + cY, color); //Octant 2
        draw_pixel(-y + cX,  x + cY, color); //Octant 3
        draw_pixel( x + cX, -y + cY, color); //Octant 7
        draw_pixel(-x + cX, -y + cY, color); //Octant 5
        draw_pixel( y + cX, -x + cY, color); //Octant 8
        draw_pixel(-y + cX, -x + cY, color); //Octant 6
        y+=1;
        if (decOver<=0){
            decOver += 2 * y + 1;   // Change in decision criterion for y -> y+1
        }else{
            x-=1;
            decOver += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
        }
    }
}

void draw_filled_circle(int cX, int cY, int rad, int color){
    int x = rad, y = 0, decOver = 1 - x;
    while(y<=x){
        
        draw_line(-x+cX, y+cY, x+cX, y+cY, color);
        draw_line(-y+cX, -x+cY, y+cX, -x+cY, color);
        draw_line(-x+cX, -y+cY, x+cX, -y+cY, color);
        draw_line(-y+cX, x+cY, y+cX, x+cY, color);
        
        draw_pixel( x + cX,  y + cY, color); //Octant 1
        draw_pixel(-x + cX,  y + cY, color); //Octant 4
        draw_pixel( y + cX,  x + cY, color); //Octant 2
        draw_pixel(-y + cX,  x + cY, color); //Octant 3
        draw_pixel( x + cX, -y + cY, color); //Octant 7
        draw_pixel(-x + cX, -y + cY, color); //Octant 5
        draw_pixel( y + cX, -x + cY, color); //Octant 8
        draw_pixel(-y + cX, -x + cY, color); //Octant 6
        y+=1;
        if (decOver<=0){
            decOver += 2 * y + 1;   // Change in decision criterion for y -> y+1
        }else{
            x-=1;
            decOver += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
        }
    }
}

void draw_star(int x, int y, int color){

    draw_pixel(x+4,y-1,color);
    draw_pixel(x+4,y+1,color);
    draw_pixel(x+5,y-1,color);
    draw_pixel(x+5,y+1,color);
    
    draw_pixel(x+6,y,color);
    
    draw_pixel(x+3, y-2,color);
    draw_pixel(x+3,y+2,color);
    
    draw_line(x+2,y-6,x+2,y-3,color);
    draw_line(x+2,y+3,x+2,y+6,color);
    draw_pixel(x+2,y-6,color);
    draw_pixel(x+2,y+6,color);
    
    draw_pixel(x+1,y-6,color);
    draw_pixel(x+1,y+6,color);
    
    draw_pixel(x,y-5,color);
    draw_pixel(x,y+5,color);
    
    draw_pixel(x-1,y-4,color);
    draw_pixel(x-1,y+4,color);
    
    draw_pixel(x-2,y-3,color);
    draw_pixel(x-2,y+3,color);
    
    draw_pixel(x-3,y-4,color);
    draw_pixel(x-3,y+4,color);
    
    draw_pixel(x-4,y-4,color);
    draw_pixel(x-4,y+4,color);
    draw_pixel(x-4,y,color);
    
    draw_pixel(x-5,y-4,color);
    draw_pixel(x-5,y-1,color);
    draw_pixel(x-5,y-2,color);
    
    draw_pixel(x-5,y+1,color);
    draw_pixel(x-5,y+4,color);
    draw_pixel(x-5,y+2,color);
    
    draw_pixel(x-6,y+3,color);
    draw_pixel(x-6,y+4,color);
    draw_pixel(x-6,y-3,color);
    draw_pixel(x-6,y-4,color);
}

void draw_filled_star(int x,int y,int color){
    int i;
    draw_pixel(x,y,color);
    draw_pixel(x+1,y+1,color);
    draw_pixel(x-1,y-1,color);
    draw_pixel(x+1,y,color);
    draw_pixel(x-1,y,color);
    draw_pixel(x+1,y-1,color);
    draw_pixel(x-1,y+1,color);
    draw_pixel(x,y+1,color);
    draw_pixel(x,y-1,color);
    
    for(i = 2; i <= 6; i++){
        draw_pixel(x+i,y,color);
        draw_pixel(x+i,y-1,color);
        draw_pixel(x+i,y+1,color);
    }
    draw_pixel(x+6,y,color);
    
    draw_pixel(x+3, y-2,color);
    draw_pixel(x+3,y+2,color);
    
    draw_line(x+2,y-6,x+2,y+6,color);
    draw_pixel(x+2,y-6,color);
    draw_pixel(x+2,y+6,color);
    
    draw_line(x+1,y-6,x+1,y+6,color);
    draw_line(x,y-5,x,y+5,color);
    draw_line(x-1,y-4,x-1,y+4,color);
    draw_line(x-2,y-3,x-2,y+3,color);
    draw_line(x-3,y-4,x-3,y+4,color);
    draw_line(x-4,y-4,x-4,y+4,color);
    draw_line(x-5,y-4,x-5,y-1,color);
    draw_line(x-5,y+1,x-5,y+4,color);
    draw_pixel(x-5,y+1,color);
    draw_pixel(x-5,y+4,color);
    draw_pixel(x-6,y+3,color);
    draw_pixel(x-6,y+4,color);
    draw_pixel(x-6,y-3,color);
    draw_pixel(x-6,y-4,color);
}

void draw_circle_points(int cX, int cY, int x, int y, int sAngle, int eAngle, int color){
    
    double angle;
    //Calculate the angle the current point makes with the circle center
    angle = atan2(y, x);
    angle = degrees(angle);
    //draw the circle points as long as they lie in the range specified
    if (x < y){
        //draw point in range 0 to 45 degrees
        if ((90 - angle >= sAngle || sAngle > eAngle) && 90 - angle <= eAngle){
            draw_pixel(cX - y, cY - x, color);
        }
        
        //draw point in range 45 to 90 degrees
        if ((angle >= sAngle || sAngle > eAngle) && angle <= eAngle){
            draw_pixel(cX - x, cY - y, color);
        }

        //draw point in range 90 to 135 degrees
        if (180 - angle >= sAngle && 180 - angle <= eAngle){
            draw_pixel(cX + x, cY - y, color);
        }

        //draw point in range 135 to 180 degrees
        if (angle + 90 >= sAngle && angle + 90 <= eAngle){
            draw_pixel(cX + y, cY - x, color);
        }
        
        //draw point in range 180 to 225 degrees
        if (270 - angle >= sAngle && 270 - angle <= eAngle){
            draw_pixel(cX + y, cY + x, color);
        }
        
        //draw point in range 225 to 270 degrees
        if (angle + 180 >= sAngle && angle + 180 <= eAngle){
            draw_pixel(cX + x, cY + y, color);
        }
        
        //draw point in range 270 to 315 degrees
        if (360 - angle >= sAngle && (360 - angle <= eAngle || sAngle > eAngle)){
            draw_pixel(cX - x, cY + y, color);
        }
        //draw point in range 315 to 360 degrees
        if (angle + 270 >= sAngle && (angle + 270 <= eAngle || sAngle > eAngle)){
            draw_pixel(cX - y, cY + x, color);
        }
    }
}

void draw_arc(int cX, int cY, int rad, int sAngle, int eAngle, int color){
    int p, x, y;
    if(sAngle < 0){
        sAngle+=360;
    }
    if(eAngle < 0){
        eAngle+=360;
    }
    if(sAngle > 360){
        sAngle-=360;
    }
    if(eAngle > 360){
        eAngle-=360;
    }

    //Standard Midpoint Circle algorithm
    p = (5 - rad * 4) / 4;
    x = 0;
    y = rad;
    draw_circle_points(cX, cY, x, y, sAngle, eAngle, color);
    while(x <= y){
        x+=1;
        if (p < 0){
            p += 2 * x + 1;
        }else{
            y-=1;
            p += 2 * (x - y) + 1;
        }
        draw_circle_points(cX, cY, x, y, sAngle, eAngle, color);
    }
}