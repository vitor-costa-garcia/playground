#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <windows.h>

void calculatePointR(float x, float y, float z, char a);

int main(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    float A = 0; float B = 0; float C = 0;
    float distanceScreen = 55;

    int x_p = 0; //2d projection
    int y_p = 0;

    float x = 0;
    float y = 0; //3d coordinates
    float z = 0;

    int cube_w = 30; int cube_h = 30; int cube_l = 30;
    float k1 = 28;

    int screen_w = 50; int screen_h = 50;

    char* output[screen_h];
    for(int i = 0; i < screen_h; i++){
        output[i] = malloc(sizeof(char)*(screen_w + 2));
        for(int j = 0; j < screen_w; j++){
            output[i][j] = ' ';
        }
        output[i][screen_w] = '\n';
        output[i][screen_w + 1] = '\0';
    }

    float* zbuffer[screen_h];
    for(int i = 0; i < screen_h; i++){
        zbuffer[i] = malloc(sizeof(float)*screen_w);
        for(int j = 0; j < screen_w; j++){
            zbuffer[i][j] = 1000;
        };
    };

    void calculatePointR(float x, float y, float z, char a){
        float r_x = x*cos(A)*cos(B) + y*(cos(A)*sin(B)*sin(C)-sin(A)*cos(C)) + z*(cos(A)*sin(B)*cos(C)+sin(A)*sin(C));
        float r_y = x*sin(A)*cos(B) + y*(sin(A)*sin(B)*sin(C)+cos(A)*cos(C)) + z*(sin(A)*sin(B)*cos(C)-cos(A)*sin(C));
        float r_z = -x*sin(B) + y*cos(B)*sin(C) + z*cos(B)*cos(C) + distanceScreen;

        x_p = (int)(screen_w/2 + (k1*r_x)/(r_z + 5));
        y_p = (int)(screen_h/2 + (k1*r_y)/(r_z + 5));

        if (x_p >= 0 && x_p < screen_w && y_p >= 0 && y_p < screen_h) {
            if (r_z < zbuffer[y_p][x_p]) {
                zbuffer[y_p][x_p] = r_z;
                output[y_p][x_p] = a;
            }
        }
    }

    while(1){
        for(float i = -cube_l/2; i < cube_l/2; i += 0.26){
            for(float j = -cube_h/2; j < cube_h/2; j += 0.26){
                calculatePointR(i, cube_w/2, j, '#');
                calculatePointR(i, -cube_w/2, j, '%');
            }
        }

        for(float i = -cube_w/2; i < cube_w/2; i += 0.26){
            for(float j = -cube_h/2; j < cube_h/2; j += 0.26){
                calculatePointR(cube_l/2, i, j, '*');
                calculatePointR(-cube_l/2, i, j, 'o');
            }
        }

        for(float i = -cube_w/2; i < cube_w/2; i += 0.26){
            for(float j = -cube_l/2; j < cube_l/2; j += 0.26){
                calculatePointR(i, j, cube_h/2, '&');
                calculatePointR(i, j, -cube_h/2, '$');
            }
        }

        A += 0.11;
        B += 0.07;
        C += 0.03;


        //COLORED FACES - SLOWER
        // for(int i = 0; i < screen_w; i++){
        //     for(int j = 0; j < screen_w+2; j++){
        //         char ch = output[i][j];
        //         if(ch == '#'){
        //             SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
        //         }   
        //         else if((ch == '*')){
        //             SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        //         }
        //         else if((ch == '&')){
        //             SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        //         }
        //         else if((ch == '%')){
        //             SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);
        //         }
        //         else if((ch == 'o')){
        //             SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE);
        //         }
        //         else if((ch == '$')){
        //             SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED);
        //         }
        //         printf("%c", ch);
        //     }
        // }

        //UNCOLORED FACES - Faster
        for(int i = 0; i < screen_w; i++){
            printf("%s", output[i]);
        }

        for (int i = 0; i < screen_h; i++) {
            for (int j = 0; j < screen_w; j++) {
                output[i][j] = ' ';
                zbuffer[i][j] = 1000;
            }
        }
    };
};
