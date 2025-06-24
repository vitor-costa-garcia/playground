#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

int main(){
    float A, B, C;
    float distanceScreen = 10;

    int x_p = 0; //2d projection
    int y_p = 0;

    float x = 0;
    float y = 0; //3d coordinates
    float z = 0;

    int cube_w = 30; int cube_h = 30;
    float k1 = 30;

    int screen_w = 50; int screen_h = 50;

    char* output[screen_h];
    for(int i = 0; i < screen_w; i++){
        output[i] = malloc(sizeof(char)*(screen_h + 2));
        for(int j = 0; j < screen_h; j++){
            output[i][j] = ' ';
        }
        output[i][screen_h] = '\n';
        output[i][screen_h + 1] = '\0';
    }

    float* zbuffer[screen_h];
    for(int i = 0; i < screen_w; i++){
        zbuffer[i] = malloc(sizeof(char)*screen_h);
        for(int j = 0; j < screen_h; j++){
            zbuffer[i][j] = 0;
        }
    }

    float calculatePointR(float x, float y, float z, char a){
        x = x*cos(A)*cos(B) + y*(cos(A)*sin(B)*sin(C)-sin(A)*cos(C)) + z*(cos(A)*sin(B)*cos(C)+sin(A)*sin(C));
        y = x*sin(A)*cos(B) + y*(sin(A)*sin(B)*sin(C)+cos(A)*cos(C)) + z*(sin(A)*sin(B)*cos(C)-cos(A)*sin(C));
        z = -x*sin(B) + y*cos(B)*sin(C) + z*cos(B)*cos(C) + distanceScreen;

        x_p = (int)();
        y_p = (int)();
    }

    while(1){
        for(float i = -cube_w/2; i < cube_w/2; i += 0.15){
            for(float j = -cube_h/2; j < cube_h/2; j += 0.15){
                x = 0;
                y = 0;
                z = 0;
            }
        }


        A += 0.08;
        B += 0.04;

        for(int i = 0; i < screen_w; i++){
            printf("%s", output[i]);
        }

        for (int i = 0; i < screen_h; i++) {
            for (int j = 0; j < screen_w; j++) {
                output[i][j] = ' ';
            }
        }

        usleep(10000);
    }
}