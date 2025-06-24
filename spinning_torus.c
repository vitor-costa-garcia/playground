#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

int main(){
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

    float x = 0;
    float y = 0; //3d coordinates
    float z = 0;

    int x_p = 0; //2d projection
    int y_p = 0;

    float nx = 0;
    float ny = 0; // surface normal vector for lightning
    float nz = 0;

    int brightness = 0;
    float lb = 0;
    float lx = 0;
    float ly = 0; // light source
    float lz = 1;
    float llen = sqrt(lx*lx + ly*ly + lz*lz);
    lx /= llen;
    ly /= llen;
    lz /= llen;

    float r1 = 1; float r2 = 2; // r1 torus radius r2 distance x center of torus circle to 0
    float theta_step = 0.16; float phi_step = 0.16;
    float k1 = 30;

    float A = 0; float B = 0;

    while(1){
        for(float phi = 0; phi < 6.28; phi += phi_step){
            for(float theta = 0; theta < 6.28; theta += theta_step){
                x = cos(B) * cos(phi) * (r2 + r1*cos(theta)) - sin(B) * (cos(A)*r1*sin(theta) + sin(A)*sin(phi)*(r2 + r1*cos(theta)));
                y = cos(phi) * (r2 + r1*cos(theta)) * sin(B) + cos(B) * (cos(A)*r1*sin(theta) + sin(A)*sin(phi)*(r2 + r1*cos(theta)));
                z = sin(A)*r1*sin(theta) - sin(phi) * (r2 + r1*cos(theta)) * cos(A);

                x_p = (int)(screen_w/2 + (k1*x)/(z + 5));
                y_p = (int)(screen_h/2 + (k1*y)/(z + 5));

                nx = cos(phi) * cos(theta) * cos(B) - (sin(theta) * cos(A) - (-sin(phi) * cos(theta)) * sin(A)) * sin(B);
                ny = cos(phi) * cos(theta) * sin(B) + (sin(theta) * cos(A) - (-sin(phi) * cos(theta)) * sin(A)) * cos(B);
                nz = sin(theta) * sin(A) - sin(phi) * cos(theta) * cos(A);

                lb = nx * lx + ny * ly + nz * lz;
                brightness = (int)((lb + 1) / 2 * 10);

                if(x_p >= 0 && x_p < screen_w && y_p >= 0 && y_p < screen_h){
                    output[y_p][x_p] = ".,-~:;=!*#$@"[brightness];
                }
            };
        };

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