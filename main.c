#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <process.h>
#include <limits.h>

#define N 5
#define CURSOR 223 // ▀
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

void generate_grid(int grid[N][N])
{
    int all_pipes[] = {PIPE_KNEE_0, PIPE_KNEE_1, PIPE_KNEE_2, PIPE_KNEE_3,
                       PIPE_0, PIPE_1, PIPE_2, PIPE_3, PIPE_PLUS,
                       PIPE_VERTICAL, PIPE_HORIZONTAL, EMPTY_CHAR};

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; j++) {
            grid[j][i] = all_pipes[rand() % 12];
            printf("%c", grid[j][i]);
        }
        printf("\n");
    }
}

int rotate(int pipe) {
    switch (pipe) {
        case PIPE_KNEE_0:
            return PIPE_KNEE_1;
        case PIPE_KNEE_1:
            return PIPE_KNEE_2;
        case PIPE_KNEE_2:
            return PIPE_KNEE_3;
        case PIPE_KNEE_3:
            return PIPE_KNEE_0;
        case PIPE_0:
            return PIPE_2;
        case PIPE_1:
            return PIPE_0;
        case PIPE_2:
            return PIPE_3;
        case PIPE_3:
            return PIPE_1;
        case PIPE_VERTICAL:
            return PIPE_HORIZONTAL;
        case PIPE_HORIZONTAL:
            return PIPE_VERTICAL;
        default:
            return pipe;
    }
}

void update_grid_current_pos(int pipe) {
    printf("\b%c", pipe);
}

void update_cursor_position(int x, int y) {
    SetConsoleCursorPosition(g_StdOutHandle, (COORD){(SHORT)x, (SHORT)y});
}

int main() {
    g_StdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    int grid[N][N], x = 0, y = N - 1;
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
                grid[x][y] = rotate(grid[x][y]);
                update_cursor_position(x + 1, y);
                update_grid_current_pos(grid[x][y]);
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
            default:
                continue;
        }
        // todo check if is solved
    }

    return 0;
}
