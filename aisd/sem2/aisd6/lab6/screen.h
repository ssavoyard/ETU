#ifndef SCREEN_H
#define SCREEN_H

const int XMAX = 120;
const int YMAX = 50;

class point {
public:
    int x, y;
    point(int a = 0, int b = 0) : x(a), y(b) {}
};

extern char screen[YMAX][XMAX];

enum color { black = '*', white = '.' };

// Объявления функций
void screen_init();
void screen_destroy();
void screen_refresh();
void screen_clear();
bool on_screen(int a, int b);
void put_point(int a, int b);
void put_line(int x0, int y0, int x1, int y1);
inline void put_line(point a, point b) { put_line(a.x, a.y, b.x, b.y); }

#endif