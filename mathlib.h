#include <math.h>

int abs(int x){
    if(x < 0)
        return x*-1;
    return x;
}

int sgn(int x){
    if(x == 0)
        return 0;
    return (x / abs(x));
}

int degrees(double radians){
    return radians*(180/3.141592653);
}

double radians(int degrees){
    double pi = 3.141592653;
    return degrees*(pi/180);
}
