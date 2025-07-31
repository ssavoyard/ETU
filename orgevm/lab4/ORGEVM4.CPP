#include <dos.h>
#include <stdio.h>
#include <conio.h>

typedef struct {
    int x;
    int y;
} Point;

int my_getch() {
    union REGS in, out;
    in.h.ah = 0x07; // Read symbol
    int86(0x21, &in, &out);
    return out.h.al;
}

int my_kbhit() {
    union REGS in, out;
    in.h.ah = 0x0B; // Keyboard input
    int86(0x21, &in, &out);
    return out.h.al != 0;
}

void draw_window() {
    int i;
    for (i = 15; i <= 65; i++) { // Draw top and bottom borders
        gotoxy(i, 15);
        printf("-");
        gotoxy(i, 20);
        printf("-");
    }
    for (i = 15; i <= 20; i++) { // Draw left and right borders
        gotoxy(15, i);
        printf("|");
        gotoxy(65, i);
        printf("|");
    }
}

void render(Point p) {
    gotoxy(p.x, p.y);
    printf("*");
}

void clear(Point p) {
    gotoxy(p.x, p.y);
    printf(" ");
}

int update_position(Point *p) {
    int ch = my_getch();

    if (ch == 27) { // Exit on ESC
        return 0;
    }

    clear(*p);

    if (ch == 0) {
        ch = my_getch(); // Read second part of scan code
        if (ch == 75 && p->x > 16) { // Left arrow
            p->x--;
        } else if (ch == 77 && p->x < 64) { // Right arrow
            p->x++;
        }
    }

    render(*p);
    return 1;
}

int main() {
    Point point = {
        40,
        17 
    };

    clrscr();
    draw_window();
    render(point);

    while (1) {
        if (my_kbhit()) { // Check if a key is pressed
            if (!update_position(&point)) { // Exit on ESC
                break;
            }
        }
    }

    clrscr();
    return 0;
}
