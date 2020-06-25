#include <iostream>
#include <curses.h>
#include <unistd.h>

struct Vec
{
    int x, y;
};
struct Uod
{
    int dx, dy;
    int size = 1;
    Vec tail[100];

    Uod()
    {
        tail[0] = { 10, 10 };
    }

    bool hit(int x, int y, int beginAt = 0)
    {
        for (int i = beginAt; i < size; i++)
            if (x == tail[i].x && y == tail[i].y) return true;
        return false;
    }

    int& x() { return tail[0].x; }
    int& y() { return tail[0].y; }

} uod;

Vec food;

bool gameover = false;
int key;
int W = 30, H = 30;
int score = 0;

void Init()
{
    uod.dx = 1;
    uod.dy = 0;
    food = { 20, 20 };
}

void Draw()
{
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            move(y,x);
            delch();

            if (x == 0 || x == W-1 || y == 0 || y == H-1)
                addch('#');
            else if (x == food.x && y == food.y)
                addch('f');
            else if (uod.hit(x, y))
                addch('o');
            else
                addch(' ');
        }
    }
    refresh();
}


void Update()
{
    for (int i = uod.size - 1; i > 0; i--)
    {
        uod.tail[i].x = uod.tail[i-1].x;
        uod.tail[i].y = uod.tail[i-1].y;
    }

    switch (key)
    {
    case KEY_UP:
        if (uod.dy != 1) {
            uod.dx = 0;
            uod.dy = -1;
        }
        break;
    case KEY_RIGHT:
        if (uod.dx != -1) {
            uod.dx = 1;
            uod.dy = 0;
        }
        break;
    case KEY_DOWN:
        if (uod.dy != -1) {
            uod.dx = 0;
            uod.dy = 1;
        }
        break;
    case KEY_LEFT:
        if (uod.dx != 1) {
            uod.dx = -1;
            uod.dy = 0;
        }
        break;
    }

    uod.tail[0].x += uod.dx;
    uod.tail[0].y += uod.dy;

    if (food.x == uod.x() && food.y == uod.y())
    {
        srand(time(NULL));
        food.x = rand() % (W-2) + 1;
        food.y = rand() % (H-2) + 1;
        uod.size++;
        score++;
    }

    if (uod.x() == 0 || uod.x() == W-1 
        || uod.y() == 0 || uod.y() == H-1 
        || uod.hit(uod.x(), uod.y(), 1)) 
    {
        gameover = true;
    }
}

int main()
{
    initscr();
    nodelay(stdscr, true);
    keypad(stdscr, true);
    cbreak();
    noecho();
    clear();
    refresh();

    Init();
    
    while (!gameover)
    {
	    key = getch();

        Draw();
        Update();
        
        if (key == 'q') break;

        usleep((1000/12) * 1000);
    }

    nodelay(stdscr, false);
    move(H/2,W/2);
    printw("Game Over!");
    getch();

    endwin();

    return 0;
}
