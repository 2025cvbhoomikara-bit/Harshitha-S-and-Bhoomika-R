#include <windows.h>
#include <stdio.h>
#include <conio.h>

#define WIDTH 60
#define HEIGHT 25
#define PADDLE_SIZE 8
#define BRICK_ROWS 4
#define BRICK_COLS 10
#define TOTAL_LIVES 3

int ballX, ballY;
int ballDX, ballDY;
int paddleX;
int score, lives;
int bricks[BRICK_ROWS][BRICK_COLS];
int gameOver, win;

void gotoxy(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO ci;
    ci.dwSize = 1;
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}

void initGame() {
    ballX = WIDTH / 2;
    ballY = HEIGHT - 3;
    ballDX = 1;
    ballDY = -1;
    paddleX = WIDTH / 2 - PADDLE_SIZE / 2;
    score = 0;
    lives = TOTAL_LIVES;
    gameOver = 0;
    win = 0;
    int r, c;
    for (r = 0; r < BRICK_ROWS; r++)
        for (c = 0; c < BRICK_COLS; c++)
            bricks[r][c] = 1;
}

void drawGame() {
    gotoxy(0, 0);
    int r, c, x;

    // Top wall
    for (x = 0; x < WIDTH; x++) printf("#");
    printf("\n");

    // Bricks
    for (r = 0; r < BRICK_ROWS; r++) {
        printf("#");
        for (c = 0; c < BRICK_COLS; c++) {
            if (bricks[r][c])
                printf("[==]");
            else
                printf("    ");
        }
        printf("#\n");
    }

    // Empty space
    int row;
    for (row = BRICK_ROWS + 1; row < HEIGHT - 2; row++) {
        printf("#");
        for (x = 1; x < WIDTH - 1; x++) {
            if (x == ballX && row == ballY)
                printf("O");
            else
                printf(" ");
        }
        printf("#\n");
    }

    // Paddle row
    printf("#");
    for (x = 1; x < WIDTH - 1; x++) {
        if (x >= paddleX && x < paddleX + PADDLE_SIZE)
            printf("=");
        else
            printf(" ");
    }
    printf("#\n");

    // Bottom
    for (x = 0; x < WIDTH; x++) printf("#");
    printf("\n");

    // HUD
    printf("Score: %d   Lives: %d   (Left/Right arrows, ESC=quit)\n",
           score, lives);
}

void update() {
    ballX += ballDX;
    ballY += ballDY;

    // Wall bounce
    if (ballX <= 1 || ballX >= WIDTH - 2) ballDX = -ballDX;
    if (ballY <= 1) ballDY = -ballDY;

    // Fell down
    if (ballY >= HEIGHT - 2) {
        lives--;
        if (lives <= 0) gameOver = 1;
        else {
            ballX = WIDTH / 2;
            ballY = HEIGHT - 3;
            ballDY = -1;
        }
    }

    // Paddle collision
    if (ballY == HEIGHT - 3 &&
        ballX >= paddleX &&
        ballX < paddleX + PADDLE_SIZE) {
        ballDY = -1;
    }

    // Brick collision
    int r, c;
    int allGone = 1;
    for (r = 0; r < BRICK_ROWS; r++) {
        for (c = 0; c < BRICK_COLS; c++) {
            if (!bricks[r][c]) continue;
            allGone = 0;
            int bx = c * 4 + 1;
            int by = r + 1;
            if (ballY == by &&
                ballX >= bx &&
                ballX < bx + 4) {
                bricks[r][c] = 0;
                score += 10;
                ballDY = -ballDY;
            }
        }
    }
    if (allGone) win = 1;
}

void handleInput() {
    if (kbhit()) {
        int k = getch();
        if (k == 224 || k == 0) {
            k = getch();
            if (k == 75 && paddleX > 1)
                paddleX -= 2;
            if (k == 77 && paddleX + PADDLE_SIZE < WIDTH - 1)
                paddleX += 2;
        }
        if (k == 27) gameOver = 1;
    }
}

int main() {
    hideCursor();
    initGame();
    system("cls");

    while (!gameOver && !win) {
        drawGame();
        handleInput();
        update();
        Sleep(80);
    }

    system("cls");
    if (win) {
        printf("\n\n   YOU WIN! Final Score: %d\n", score);
    } else {
        printf("\n\n   GAME OVER! Final Score: %d\n", score);
    }
    printf("   Press any key to exit...\n");
    getch();
    return 0;
}
