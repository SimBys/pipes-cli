#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <process.h>
#include <limits.h>

#define N 5
#define PIPE_VERTICAL 179 // │
#define PIPE_HORIZONTAL 196 // ─
#define PIPE_KNEE_0 192 // └
#define PIPE_KNEE_1 218 // ┌
#define PIPE_KNEE_2 191 // ┐
#define PIPE_KNEE_3 217 // ┘
#define PIPE_0 195 // ├
#define PIPE_1 193 // ┴
#define PIPE_2 194 // ┬
#define PIPE_3 180 // ┤
#define PIPE_PLUS 197 // ┼
#define EMPTY_CHAR 32 // ' '

HANDLE g_StdOutHandle;

void generate_grid(unsigned char grid[N][N])
{
    char all_pipes[] = {PIPE_KNEE_0, PIPE_KNEE_1, PIPE_KNEE_2, PIPE_KNEE_3,
                       PIPE_0, PIPE_1, PIPE_2, PIPE_3, PIPE_PLUS,
                       PIPE_VERTICAL, PIPE_HORIZONTAL, EMPTY_CHAR};

    for (char i = 0; i < N; ++i) {
        for (char j = 0; j < N; j++) {
            grid[j][i] = all_pipes[rand() % 12];
            printf("%c", grid[j][i]);
        }
        printf("\n");
    }
}

void rotate_pipe(unsigned char *pipe) {
    switch (*pipe) {
        case PIPE_KNEE_0:
            *pipe = PIPE_KNEE_1;
            break;
        case PIPE_KNEE_1:
            *pipe = PIPE_KNEE_2;
            break;
        case PIPE_KNEE_2:
            *pipe = PIPE_KNEE_3;
            break;
        case PIPE_KNEE_3:
            *pipe = PIPE_KNEE_0;
            break;
        case PIPE_0:
            *pipe = PIPE_2;
            break;
        case PIPE_1:
            *pipe = PIPE_0;
            break;
        case PIPE_2:
            *pipe = PIPE_3;
            break;
        case PIPE_3:
            *pipe = PIPE_1;
            break;
        case PIPE_VERTICAL:
            *pipe = PIPE_HORIZONTAL;
            break;
        case PIPE_HORIZONTAL:
            *pipe = PIPE_VERTICAL;
            break;
    }
}

void update_cursor_position(char x, char y) {
    SetConsoleCursorPosition(g_StdOutHandle, (COORD){(SHORT)x, (SHORT)y});
}

void rerender_pipe(char x, char y, unsigned char pipe) {
    update_cursor_position(++x, y);
    printf("\b%c", pipe);
}

int main() {
    g_StdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    unsigned char grid[N][N];
    char x = 0, y = N - 1;
    srand(_getpid() % INT_MAX); // randomize
    printf("\033[H\033[J"); // clear console
    SetConsoleTextAttribute(g_StdOutHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY); // set text color
    generate_grid(grid);

    while (1) {
        update_cursor_position(x, y);
        // handle input
        switch (getch()) {
            case 'q': // quit
                return 0;
            case ' ': // rotate
                rotate_pipe(&grid[x][y]);
                rerender_pipe(x, y, grid[x][y]);
                break;
            case 'w': // move up
                if (y > 0)
                    y--;
                break;
            case 's': // move down
                if (y < N - 1)
                    y++;
                break;
            case 'a': // move left
                if (x > 0)
                    x--;
                break;
            case 'd': // move right
                if (x < N - 1)
                    x++;
                break;
        }
        // todo check if is solved
    }
}
