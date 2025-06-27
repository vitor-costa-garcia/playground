#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <windows.h>

#define SCREEN_W 120
#define SCREEN_H 75
#define RADIUS 18
#define THICKNESS 2
#define K1 30
#define K2 48

float bottomHeartY(float x){return abs(x)-RADIUS*2;}
float leftSemiHeartY(float x){return sqrt((RADIUS*RADIUS) - ((x + RADIUS)*(x + RADIUS)));}
float rightSemiHeartY(float x){return sqrt((RADIUS*RADIUS) - ((x - RADIUS)*(x - RADIUS)));}

void calculatePoint(float i, float j, float k, float A, float B, float C, CHAR_INFO* output, float* zbuffer){
    float r_x = i*cos(A)*cos(B) + j*(cos(A)*sin(B)*sin(C)-sin(A)*cos(C)) + k*(cos(A)*sin(B)*cos(C)+sin(A)*sin(C));
    float r_y = i*sin(A)*cos(B) + j*(sin(A)*sin(B)*sin(C)+cos(A)*cos(C)) + k*(sin(A)*sin(B)*cos(C)-cos(A)*sin(C));
    float r_z = -i*sin(B) + j*cos(B)*sin(C) + k*cos(B)*cos(C) + K2;

    int x_p = (int)(SCREEN_W/2 + (K1*r_x)/(r_z));
    int y_p = (int)(SCREEN_H/2 + (K1*r_y)/(r_z));

    if (x_p >= 0 && x_p < SCREEN_W && y_p >= 0 && y_p < SCREEN_H){
        if (r_z < zbuffer[y_p * SCREEN_W + x_p]){
                zbuffer[y_p * SCREEN_W + x_p] = r_z;
                output[y_p * SCREEN_W + x_p].Char.AsciiChar = '.';
        }
    }
}

void calculateHeartPiece(float a, float b, float offset, float A, float B, float C, CHAR_INFO* output, float* zbuffer, float (*func)(float)){
    for(float i = a; i < b; i += offset){
        for(float j = func(i); j >= bottomHeartY(i); j -= offset){
            for(float k = -THICKNESS/2; k <= THICKNESS/2; k += 0.5){
                calculatePoint(i, j, k, A, B, C, output, zbuffer);
            }
        }
    }
}

void calculateHeart(float offset, float A, float B, float C, CHAR_INFO* output, float* zbuffer){
    calculateHeartPiece(-RADIUS*2, RADIUS*2, offset, A, B, C, output, zbuffer, leftSemiHeartY);
    calculateHeartPiece(-RADIUS*2, RADIUS*2, offset, A, B, C, output, zbuffer, rightSemiHeartY);
};

int main(){
    system("cls");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = {SCREEN_W, SCREEN_H};
    COORD bufferCoord = {0, 0};
    SMALL_RECT writeRegion = {0, 0, SCREEN_W - 1, SCREEN_H - 1};

    CHAR_INFO output[SCREEN_W * SCREEN_H]; //OUTPUT BUFFER
    for(int i = 0; i < SCREEN_W * SCREEN_H; i++){
        output[i].Char.AsciiChar = ' ';
        output[i].Attributes = FOREGROUND_RED;
    }

    float zbuffer[SCREEN_W * SCREEN_H];
    for(int i = 0; i < SCREEN_H*SCREEN_W; i++){
        zbuffer[i] = 1000;
    };

    float A = 3.14159; float B = 0; float C = 0;

    while(1){
        calculateHeart(0.3, A, B, C, output, zbuffer);

        A += 0.0;
        B += 0.07;
        C += 0.0;

        WriteConsoleOutput(hConsole, output, bufferSize, bufferCoord, &writeRegion);
        usleep(7000);

        for (int i = 0; i < SCREEN_W * SCREEN_H; i++) {
            output[i].Char.AsciiChar = ' ';
            zbuffer[i] = 1000;
        }
    }
}