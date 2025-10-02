#include <math.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

float coordinates[8][3] = {
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 1.0f},
    {1.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 1.0f}
};

float angleY = 0.05f;   // Rotation um Y
float angleX = 0.02f;   // konstante Rotation um X

void printScreen(){
    clear();
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int cx = cols / 2;
    int cy = rows / 2;
    float scale_x = 20.0f;  // horizontale Skalierung
    float scale_y = 10.0f;  // vertikale Skalierung

    for(int i = 0; i < 8; i++){
        int sx = (int)((coordinates[i][0] - 0.5f) * scale_x + cx);
        int sy = (int)((coordinates[i][1] - 0.5f) * scale_y + cy);

        if (sy >= 0 && sy < rows && sx >= 0 && sx < cols) {
            mvprintw(sy, sx, "#");
        }
    }
    mvprintw(0, 0, "q = quit");
    refresh();
}

void rotateY(float *x, float *z, float a){
    float ox = *x, oz = *z;
    float c = cosf(a), s = sinf(a);
    *x = c * ox + s * oz;
    *z = -s * ox + c * oz;
}

void rotateX(float *y, float *z, float a){
    float oy = *y, oz = *z;
    float c = cosf(a), s = sinf(a);
    *y = c * oy - s * oz;
    *z = s * oy + c * oz;
}

void calculateCoordinates(){
    float cx = 0.5f, cy = 0.5f, cz = 0.5f;
    for(int i = 0; i < 8; i++){
        float x = coordinates[i][0] - cx;
        float y = coordinates[i][1] - cy;
        float z = coordinates[i][2] - cz;

        // Erst um Y, dann um X rotieren
        rotateY(&x, &z, angleY);
        rotateX(&y, &z, angleX);

        coordinates[i][0] = x + cx;
        coordinates[i][1] = y + cy;
        coordinates[i][2] = z + cz;
    }
}

int main(){
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);

    while(1){
        int ch = getch();
        if (ch == 'q' || ch == 'Q') break;

        printScreen();
        usleep(30000); // ~30 FPS
        calculateCoordinates();
    }

    endwin();
    return 0;
}
