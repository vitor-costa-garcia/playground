#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <windows.h>

void calculatePointR(float x, float y, float z, char a);

int main(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int screen_w = 50; int screen_h = 50;
    COORD bufferSize = {screen_w, screen_h};
    COORD bufferCoord = {0, 0};
    SMALL_RECT writeRegion = {0, 0, screen_w - 1, screen_h - 1};

    float A = 0; float B = 0; float C = 0;
    float distanceScreen = 55;

    int x_p = 0; //2d projection
    int y_p = 0;

    float x = 0;
    float y = 0; //3d coordinates
    float z = 0;

    int cube_w = 35; int cube_h = 7; int cube_l = 40;
    float k1 = 28;

    CHAR_INFO output[screen_w * screen_h]; //OUTPUT BUFFER
    for(int i = 0; i < screen_w * screen_h; i++){
        output[i].Char.AsciiChar = ' ';
        output[i].Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
    }

    float zbuffer[screen_w * screen_h];
    for(int i = 0; i < screen_h; i++){
        zbuffer[i] = 1000;
    };

    void calculatePointR(float x, float y, float z, char a){
        float r_x = x*cos(A)*cos(B) + y*(cos(A)*sin(B)*sin(C)-sin(A)*cos(C)) + z*(cos(A)*sin(B)*cos(C)+sin(A)*sin(C));
        float r_y = x*sin(A)*cos(B) + y*(sin(A)*sin(B)*sin(C)+cos(A)*cos(C)) + z*(sin(A)*sin(B)*cos(C)-cos(A)*sin(C));
        float r_z = -x*sin(B) + y*cos(B)*sin(C) + z*cos(B)*cos(C) + distanceScreen;

        x_p = (int)(screen_w/2 + (k1*r_x)/(r_z + 5));
        y_p = (int)(screen_h/2 + (k1*r_y)/(r_z + 5));

        if (x_p >= 0 && x_p < screen_w && y_p >= 0 && y_p < screen_h) {
            if (r_z < zbuffer[y_p * screen_w + x_p]) {
                zbuffer[y_p * screen_w + x_p] = r_z;
                output[y_p * screen_w + x_p].Char.AsciiChar = a;
                switch (a){
                    case '#':
                        output[y_p * screen_w + x_p].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
                    case '*':
                        output[y_p * screen_w + x_p].Attributes = FOREGROUND_RED; break;
                    case '&':
                        output[y_p * screen_w + x_p].Attributes = FOREGROUND_GREEN; break;
                    case '%':
                        output[y_p * screen_w + x_p].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN; break;
                    case 'o':
                        output[y_p * screen_w + x_p].Attributes = FOREGROUND_RED | FOREGROUND_BLUE; break;
                    case '$':
                        output[y_p * screen_w + x_p].Attributes = FOREGROUND_GREEN | FOREGROUND_RED; break;
                }
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

        for(float i = -cube_l/2; i < cube_l/2; i += 0.26){
            for(float j = -cube_w/2; j < cube_w/2; j += 0.26){
                calculatePointR(i, j, cube_h/2, '&');
                calculatePointR(i, j, -cube_h/2, '$');
            }
        }

        A += 0.11;
        B += 0.07;
        C += 0.04;

        WriteConsoleOutput(hConsole, output, bufferSize, bufferCoord, &writeRegion);
        usleep(7000);

        for (int i = 0; i < screen_w * screen_h; i++) {
            output[i].Char.AsciiChar = ' ';
            output[i].Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
            zbuffer[i] = 1000;
        }
    };
};
