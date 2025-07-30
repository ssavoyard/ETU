#include "shape.h"
#include <iostream>
#include <locale.h>
#include "screen.h"
#include "exceptions.h"

// Инициализация статических членов
std::list<shape*> shape::shapes;

// ==============================================
// Реализация функций экрана
// ==============================================

char screen[YMAX][XMAX];

void screen_init() {
    for (int y = 0; y < YMAX; ++y)
        for (int x = 0; x < XMAX; ++x)
            screen[y][x] = white;
}

void screen_destroy() {
    for (int y = 0; y < YMAX; ++y)
        for (int x = 0; x < XMAX; ++x)
            screen[y][x] = black;
}

bool on_screen(int a, int b) {
    return 0 <= a && a < XMAX && 0 <= b && b < YMAX;
}

void put_point(int a, int b) {
    if (!on_screen(a, b))
        throw PointOutOfScreen(a, b);
    screen[b][a] = black;
}

void put_line(int x0, int y0, int x1, int y1) {
    if (!on_screen(x0, y0) || !on_screen(x1, y1))
        throw PointOutOfScreen(x0, y0);

    int dx = 1;
    int a = x1 - x0;
    if (a < 0) dx = -1, a = -a;

    int dy = 1;
    int b = y1 - y0;
    if (b < 0) dy = -1, b = -b;

    int two_a = 2 * a;
    int two_b = 2 * b;
    int xcrit = -b + two_a;
    int eps = 0;

    for (;;) {
        put_point(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        if (eps <= xcrit) x0 += dx, eps += two_b;
        if (eps >= a || a < b) y0 += dy, eps -= two_a;
    }
}

void screen_clear() {
    screen_init();
}

void screen_refresh() {
    for (int y = YMAX - 1; y >= 0; --y) {
        for (int x = 0; x < XMAX; ++x)
            std::cout << screen[y][x];
        std::cout << '\n';
    }
}

void shape_refresh() {
    screen_clear();
    for (auto p : shape::shapes) {
        try {
            p->draw();
        }
        catch (const ShapeError& e) {
            std::cerr << "Drawing error: " << e.what() << std::endl;
        }
    }
    screen_refresh();
}

// ==============================================
// Реализация методов shape
// ==============================================

shape::shape() {
    shapes.push_back(this);
}

shape::~shape() {
    shapes.remove(this);
}

void shape::refresh_all() {
    screen_clear();
    for (auto p : shapes) {
        try {
            p->draw();
        }
        catch (const ShapeError& e) {
            std::cerr << "Ошибка рисования: " << e.what() << std::endl;
        }
    }
    screen_refresh();
}

// ==============================================
// Реализация rotatable и reflectable
// ==============================================

//class rotatable : virtual public shape { 	//Фигуры, пригодные к повороту 
//protected:
//    enum class rotated { left, no, right };
//    rotated state;							//Текущее состояние поворота
//
//public:
//    rotatable(rotated r = rotated::no) : state(r) { }
//
//    void rotate_left() {
//        state = rotated::left;
//    }
//
//    void rotate_right() {
//        state = rotated::right;
//    }
//};


//class reflectable : virtual public shape { 	//Фигуры, пригодные к зеркальному отражению
//protected:
//    bool hor, vert;         //Текущее состояние отражения
//public:
//    reflectable(bool h = false, bool v = false) : hor(h), vert(v) { }
//
//    void flip_horisontally() { hor = !hor; }		//Отразить горизонтально
//    void flip_vertically() { vert = !vert; } 	//Отразить вертикально
//};


void rotatable::rotate_left() {
    if (state == rotated::left)
        throw AlreadyTransformed("left rotation");
    state = rotated::left;
}

void rotatable::rotate_right() {
    if (state == rotated::right)
        throw AlreadyTransformed("right rotation");
    state = rotated::right;
}

void reflectable::flip_horisontally() {
    hor = !hor;
}

void reflectable::flip_vertically() {
    vert = !vert;
}


// ==============================================
// Реализация конкретных фигур
// ==============================================

// ---- line ----

//class line : public shape {        // ==== Прямая линия ====
//protected:
//    point w, e;
//
//public:
//    line(point a, point b) : w(a), e(b) { }; //Произвольная линия (по двум точкам)
//    line(point a, int L) : w(point(a.x + L - 1, a.y)), e(a) {  }; //Горизонтальная линия
//    point north() const { return point((w.x + e.x) / 2, e.y < w.y ? w.y : e.y); }
//    point south() const { return point((w.x + e.x) / 2, e.y < w.y ? e.y : w.y); }
//    point east() const { return point(e.x < w.x ? w.x : e.x, (w.y + e.y) / 2); }
//    point west() const { return point(e.x < w.x ? e.x : w.x, (w.y + e.y) / 2); }
//    point neast() const { return point(w.x < e.x ? e.x : w.x, e.y < w.y ? w.y : e.y); }
//    point seast() const { return point(w.x < e.x ? e.x : w.x, e.y < w.y ? e.y : w.y); }
//    point nwest() const { return point(w.x < e.x ? w.x : e.x, e.y < w.y ? w.y : e.y); }
//    point swest() const { return point(w.x < e.x ? w.x : e.x, e.y < w.y ? e.y : w.y); }
//    void move(int a, int b) { w.x += a; w.y += b; e.x += a; e.y += b; }
//    void draw() { put_line(w, e); }
//    void resize(double d)                // Изменение длины линии в (d) раз
//    {
//        e.x = w.x + (e.x - w.x) * d; e.y = w.y + (e.y - w.y) * d;
//    }
//};

line::line(point a, int L) : w(point(a.x + L - 1, a.y)), e(a) {
    if (!on_screen(w.x, w.y) || !on_screen(e.x, e.y))
        throw NotEnoughSpace("horizontal line");
    if (L <= 0)
        throw InvalidShapeParameters("line length must be positive");
}

point line::north() const {
    return point((w.x + e.x) / 2, e.y < w.y ? w.y : e.y);
}

point line::south() const {
    return point((w.x + e.x) / 2, e.y < w.y ? e.y : w.y);
}

point line::east() const {
    return point(e.x < w.x ? w.x : e.x, (w.y + e.y) / 2);
}

point line::west() const {
    return point(e.x < w.x ? e.x : w.x, (w.y + e.y) / 2);
}

point line::neast() const {
    return point(w.x < e.x ? e.x : w.x, e.y < w.y ? w.y : e.y);
}

point line::seast() const {
    return point(w.x < e.x ? e.x : w.x, e.y < w.y ? e.y : w.y);
}

point line::nwest() const {
    return point(w.x < e.x ? w.x : e.x, e.y < w.y ? w.y : e.y);
}

point line::swest() const {
    return point(w.x < e.x ? w.x : e.x, e.y < w.y ? e.y : w.y);
}


void line::move(int a, int b) {
    if (!on_screen(w.x + a, w.y + b) || !on_screen(e.x + a, e.y + b))
        throw MovementError(a, b);

    w.x += a;
    w.y += b;
    e.x += a;
    e.y += b;
}

void line::draw() {
    try {
        put_line(w, e);
    }
    catch (const PointOutOfScreen& e) {
        std::cerr << "Ошибка рисования линии: " << e.what() << std::endl;
        throw DrawingError("line");
    }
}

void line::resize(double d) {
    if (d <= 0)
        throw InvalidShapeParameters("scale factor must be positive");

    int new_x = w.x + (e.x - w.x) * d;
    int new_y = w.y + (e.y - w.y) * d;

    if (!on_screen(new_x, new_y))
        throw NotEnoughSpace("resized line");

    e.x = new_x;
    e.y = new_y;
}



// ---- rectangle ----

//class rectangle : public rotatable {      // ==== Прямоугольник ====
//    /*    nw------n------ne
//       |		               |
//       |		               |
//       w	       c         e
//       |		               |
//       |		               |
//       sw ------- s ------ se  */
//    protected:
//        point sw, ne;
//    public:
//        rectangle(point a, point b) :  sw(a), ne(b) { }
//        point north( ) const { return point((sw.x + ne.x) / 2, ne.y); }
//        point south( ) const { return point((sw.x + ne.x) / 2, sw.y); }
//        point east( ) const { return point(ne.x, (sw.y + ne.y) / 2); }
//        point west( ) const { return point(sw.x, (sw.y + ne.y) / 2); }
//        point neast( ) const { return ne; }
//        point seast( ) const { return point(ne.x, sw.y); }
//        point nwest( ) const { return point(sw.x, ne.y); }
//        point swest( ) const { return sw; }
//
//        void rotate_right( ){           // Поворот вправо относительно se
//            int w = ne.x - sw.x, h = ne.y - sw.y; // (учитывается масштаб по осям)
//            sw.x = ne.x - h * 2; ne.y = sw.y + w / 2;
//        }
//
//        void rotate_left(){ // Поворот влево относительно sw
//            int w = ne.x - sw.x, h = ne.y - sw.y;
//            ne.x = sw.x + h * 2;
//            ne.y = sw.y + w / 2;
//        }
//
//        void move(int a, int b){
//            sw.x += a;
//            sw.y += b;
//            ne.x += a;
//            ne.y += b;
//        }
//        void resize(int d){
//            ne.x = sw.x + (ne.x - sw.x) * d;
//            ne.y = sw.y + (ne.y - sw.y) * d;
//        }
//
//        void resize(double d){
//            ne.x = sw.x + (ne.x - sw.x) * d;
//            ne.y = sw.y + (ne.y - sw.y) * d;
//        }
//
//        void draw( ){
//        put_line(nwest( ), ne);   put_line(ne, seast( ));
//        put_line(seast( ), sw);   put_line(sw, nwest( ));
//        }
//    };

rectangle::rectangle(point a, point b) : sw(a), ne(b) {
    if (a.x >= b.x || a.y >= b.y)
        throw InvalidShapeParameters("invalid rectangle coordinates");
    if (!on_screen(a.x, a.y) || !on_screen(b.x, b.y))
        throw NotEnoughSpace("rectangle");
}

void rectangle::draw() {
    put_line(nwest(), ne);
    put_line(ne, seast());
    put_line(seast(), sw);
    put_line(sw, nwest());
}


// ---- cross ----

//class cross : public rotatable {
//protected:
//    point sw, ne;
//public:
//    cross(point a, point b) : sw(a), ne(b) { }
//    point north() const { return point((sw.x + ne.x) / 2, ne.y); }
//    point south() const { return point((sw.x + ne.x) / 2, sw.y); }
//    point east() const { return point(ne.x, (sw.y + ne.y) / 2); }
//    point west() const { return point(sw.x, (sw.y + ne.y) / 2); }
//    point neast() const { return ne; }
//    point seast() const { return point(ne.x, sw.y); }
//    point nwest() const { return point(sw.x, ne.y); }
//    point swest() const { return sw; }
//    void rotate_right() {
//        int w = ne.x - sw.x, h = ne.y - sw.y;
//        sw.x = ne.x - h * 2; ne.y = sw.y + w / 2;
//    }
//    void rotate_left() {
//        int w = ne.x - sw.x, h = ne.y - sw.y;
//        ne.x = sw.x + h * 2; ne.y = sw.y + w / 2;
//    }
//    void move(int a, int b)
//    {
//        sw.x += a; sw.y += b; ne.x += a; ne.y += b;
//    }
//    void resize(int d) {
//        ne.x = sw.x + (ne.x - sw.x) * d; ne.y = sw.y + (ne.y - sw.y) * d;
//    }
//    void resize(double d) {
//        ne.x = sw.x + (ne.x - sw.x) * d;    ne.y = sw.y + (ne.y - sw.y) * d;
//    }
//    void draw() {
//        put_line(sw, ne);
//        put_line(seast(), nwest());
//    }
//};


cross::cross(point a, point b) : sw(a), ne(b) {
    if (!on_screen(a.x, a.y) || !on_screen(b.x, b.y))
        throw NotEnoughSpace("cross");
}

void cross::draw() {
    put_line(sw, ne);
    put_line(seast(), nwest());
}


// ---- parallelogram ----

//class paralellogram : public rotatable, public reflectable {
//protected:
//    point sw, ne;
//    bool rotated = 0, reflected = 0;
//public:
//    paralellogram(point a, point b) : sw(a), ne(b) { }
//    point north() const { return point((sw.x + ne.x) / 2, ne.y); }
//    point south() const { return point((sw.x + ne.x) / 2, sw.y); }
//    point east() const { return point(ne.x, (sw.y + ne.y) / 2); }
//    point west() const { return point(sw.x, (sw.y + ne.y) / 2); }
//    point neast() const { return ne; }
//    point seast() const { return point(ne.x, sw.y); }
//    point nwest() const { return point(sw.x, ne.y); }
//    point swest() const { return sw; }
//    void rotate_right() {
//        int w = ne.x - sw.x, h = ne.y - sw.y;
//        sw.x = ne.x - h * 2; ne.y = sw.y + w / 2;
//        rotated = !rotated;
//    }
//    void rotate_left() {
//        int w = ne.x - sw.x, h = ne.y - sw.y;
//        ne.x = sw.x + h * 2; ne.y = sw.y + w / 2;
//        rotated = !rotated;
//    }
//    void move(int a, int b) {
//        sw.x += a; sw.y += b; ne.x += a; ne.y += b;
//    }
//    void resize(int d) {
//        ne.x = sw.x + (ne.x - sw.x) * d; ne.y = sw.y + (ne.y - sw.y) * d;
//    }
//    void resize(double d) {
//        ne.x = sw.x + (ne.x - sw.x) * d;    ne.y = sw.y + (ne.y - sw.y) * d;
//    }
//    void flip_horisontally() {
//        reflectable::flip_horisontally();
//        reflected = !reflected;
//    }
//
//    void flip_vertically() {
//        reflectable::flip_vertically();
//        reflected = !reflected;
//    }
//    void draw() {
//        if (!rotated && !reflected) {
//            put_line(swest(), north());
//            put_line(north(), neast());
//            put_line(neast(), south());
//            put_line(south(), swest());
//        }
//        else if (rotated && !reflected) {
//            put_line(nwest(), east());
//            put_line(east(), seast());
//            put_line(seast(), west());
//            put_line(west(), nwest());
//        }
//        else if (!rotated && reflected) {
//            put_line(nwest(), north());
//            put_line(north(), seast());
//            put_line(seast(), south());
//            put_line(south(), nwest());
//        }
//        else {
//            put_line(west(), neast());
//            put_line(neast(), east());
//            put_line(east(), swest());
//            put_line(swest(), west());
//        }
//    }
//};

parallelogram::parallelogram(point a, point b) : sw(a), ne(b) {
    if (a.x == b.x || a.y == b.y)
        throw InvalidShapeParameters("invalid parallelogram points");
}

void parallelogram::draw() {
    if (!rotated_flag && !reflected_flag) {
        put_line(swest(), north());
        put_line(north(), neast());
        put_line(neast(), south());
        put_line(south(), swest());
    }
    // ... другие случаи ...
}

// ---- new_shape ----

new_shape::new_shape(point a, point b) :
    parallelogram(a, b), cross(a, b)
{
    if (!on_screen(a.x, a.y) || !on_screen(b.x, b.y))
        throw NotEnoughSpace("new_shape");
}

void new_shape::draw() {
    try {
        parallelogram::draw();
        cross::draw();
    }
    catch (const PointOutOfScreen& e) {
        std::cerr << "Ошибка в new_shape: " << e.what() << std::endl;
        throw DrawingError("new_shape");
    }
}

void new_shape::rotate_left() {
    parallelogram::rotate_left();
    cross::rotate_left();
}

void new_shape::rotate_right() {
    parallelogram::rotate_right();
    cross::rotate_right();
}

void new_shape::move(int a, int b) {
    parallelogram::move(a, b);
    cross::move(a, b);
}

void new_shape::resize(double d) {
    parallelogram::resize(d);
    cross::resize(d);
}

void new_shape::flip_horisontally() {
    parallelogram::flip_horisontally();
}

void new_shape::flip_vertically() {
    parallelogram::flip_vertically();
}

// ==============================================
// Реализация вспомогательных функций
// ==============================================

void up(shape& p, const shape& q) {
    point n = q.north();
    point s = p.south();
    p.move(n.x - s.x, n.y - s.y + 1);
}

void down(shape& p, const shape& q) {
    point n = q.south();
    point s = p.north();
    p.move(n.x - s.x, n.y - s.y - 1);
}

void left(shape& p, const shape& q) {
    point n = q.swest();
    point s = p.seast();
    p.move(n.x - s.x - 1, n.y - s.y);
}

void right(shape& p, const shape& q) {
    point n = q.seast();
    point s = p.swest();
    p.move(n.x - s.x + 1, n.y - s.y);
}

void center(shape& p, const shape& q) {
    point n1 = q.north();
    point n2 = p.north();
    point s1 = q.south();
    point s2 = p.south();
    p.move(n1.x - n2.x, n1.y - n2.y - (n1.y - s1.y) / 2 + (n2.y - s2.y) / 2);
}

// Cборная пользовательская фигура – физиономия

myshape::myshape(point a, point b) : rectangle(a, b),
w(neast().x - swest().x + 1),
h(neast().y - swest().y + 1),
l_eye(point(swest().x + 2, swest().y + h * 3 / 4), 2),
r_eye(point(swest().x + w - 4, swest().y + h * 3 / 4), 2),
mouth(point(swest().x + 2, swest().y + h / 4), w - 4)
{ }

void myshape::draw() {
    rectangle::draw();
    point nose((swest().x + neast().x) / 2,
        (swest().y + neast().y) / 2);
    put_point(nose.x, nose.y);
}

void myshape::move(int a, int b) {
    rectangle::move(a, b);
    l_eye.move(a, b);
    r_eye.move(a, b);
    mouth.move(a, b);
}

void myshape::resize(double r) {
    rectangle::resize(r);
    rectangle::move(static_cast<int>(w * (1 - r) * 0.5),
        static_cast<int>(h * (1 - r) * 0.5));
}

/****************************************************************************/
line::line(point a, point b) : w(a), e(b) {
    if (!on_screen(a.x, a.y) || !on_screen(b.x, b.y))
        throw NotEnoughSpace("line");
    if (a.x == b.x && a.y == b.y)
        throw InvalidShapeParameters("Line has zero length");
}

// Реализация методов rotatable
rotatable::rotatable(rotated r) : state(r) {}

// Реализация методов reflectable
reflectable::reflectable(bool h, bool v) : hor(h), vert(v) {}

// Реализация методов rectangle
void rectangle::rotate_left() {
    int w = ne.x - sw.x, h = ne.y - sw.y;
    ne.x = sw.x + h * 2;
    ne.y = sw.y + w / 2;
}

void rectangle::rotate_right() {
    int w = ne.x - sw.x, h = ne.y - sw.y;
    sw.x = ne.x - h * 2;
    ne.y = sw.y + w / 2;
}

void rectangle::move(int a, int b) {
    sw.x += a;
    sw.y += b;
    ne.x += a;
    ne.y += b;
}

void rectangle::resize(int d) {
    ne.x = sw.x + (ne.x - sw.x) * d;
    ne.y = sw.y + (ne.y - sw.y) * d;
}

void rectangle::resize(double d) {
    if (d <= 0)
        throw InvalidShapeParameters("scale factor must be positive");

    int new_x = sw.x + static_cast<int>((ne.x - sw.x) * d);
    int new_y = sw.y + static_cast<int>((ne.y - sw.y) * d);

    if (!on_screen(new_x, new_y))
        throw NotEnoughSpace("resized rectangle");

    ne.x = new_x;
    ne.y = new_y;
}

// Реализация методов cross
point cross::north() const { return point((sw.x + ne.x) / 2, ne.y); }
point cross::south() const { return point((sw.x + ne.x) / 2, sw.y); }
point cross::east() const { return point(ne.x, (sw.y + ne.y) / 2); }
point cross::west() const { return point(sw.x, (sw.y + ne.y) / 2); }
point cross::neast() const { return ne; }
point cross::seast() const { return point(ne.x, sw.y); }
point cross::nwest() const { return point(sw.x, ne.y); }
point cross::swest() const { return sw; }

void cross::move(int a, int b) {
    sw.x += a; sw.y += b;
    ne.x += a; ne.y += b;
}

void cross::resize(int d) {
    ne.x = sw.x + (ne.x - sw.x) * d;
    ne.y = sw.y + (ne.y - sw.y) * d;
}

void cross::resize(double d) {
    if (d <= 0)
        throw InvalidShapeParameters("scale factor must be positive");

    int new_x = sw.x + static_cast<int>((ne.x - sw.x) * d);
    int new_y = sw.y + static_cast<int>((ne.y - sw.y) * d);

    if (!on_screen(new_x, new_y))
        throw NotEnoughSpace("resized cross");

    ne.x = new_x;
    ne.y = new_y;
}

// Реализация методов parallelogram
point parallelogram::north() const { return point((sw.x + ne.x) / 2, ne.y); }
point parallelogram::south() const { return point((sw.x + ne.x) / 2, sw.y); }
point parallelogram::east() const { return point(ne.x, (sw.y + ne.y) / 2); }
point parallelogram::west() const { return point(sw.x, (sw.y + ne.y) / 2); }
point parallelogram::neast() const { return ne; }
point parallelogram::seast() const { return point(ne.x, sw.y); }
point parallelogram::nwest() const { return point(sw.x, ne.y); }
point parallelogram::swest() const { return sw; }

void parallelogram::rotate_left() {
    int w = ne.x - sw.x, h = ne.y - sw.y;
    ne.x = sw.x + h * 2;
    ne.y = sw.y + w / 2;
    rotated_flag = !rotated_flag;
}

void parallelogram::rotate_right() {
    int w = ne.x - sw.x, h = ne.y - sw.y;
    sw.x = ne.x - h * 2;
    ne.y = sw.y + w / 2;
    rotated_flag = !rotated_flag;
}

void parallelogram::move(int a, int b) {
    sw.x += a; sw.y += b;
    ne.x += a; ne.y += b;
}

void parallelogram::resize(int d) {
    ne.x = sw.x + (ne.x - sw.x) * d;
    ne.y = sw.y + (ne.y - sw.y) * d;
}

void parallelogram::resize(double d) {
    if (d <= 0)
        throw InvalidShapeParameters("scale factor must be positive");

    int new_x = sw.x + static_cast<int>((ne.x - sw.x) * d);
    int new_y = sw.y + static_cast<int>((ne.y - sw.y) * d);

    if (!on_screen(new_x, new_y))
        throw NotEnoughSpace("resized parallelogram");

    ne.x = new_x;
    ne.y = new_y;
}

void parallelogram::flip_horisontally() {
    reflectable::flip_horisontally();
    reflected_flag = !reflected_flag;
}

void parallelogram::flip_vertically() {
    reflectable::flip_vertically();
    reflected_flag = !reflected_flag;
}

//rectangle::rectangle(point a, point b) : sw(a), ne(b) {
//    if (a.x >= b.x || a.y >= b.y)
//        throw InvalidShapeParameters("Invalid rectangle coordinates");
//    if (!on_screen(a.x, a.y) || !on_screen(b.x, b.y))
//        throw NotEnoughSpace("rectangle");
//}
//
//void rotatable::rotate_left() {
//    if (state == rotated::left)
//        throw AlreadyTransformed("left rotated");
//    state = rotated::left;
//}
//
//void rotatable::rotate_right() {
//    if (state == rotated::right)
//        throw AlreadyTransformed("right rotated");
//    state = rotated::right;
//}

/****************************************************************************/






























//int main( ) {   
//    /*
//    // main.cpp
//#include "shape.h"
//#include <iostream>
//
//int main() {
//    setlocale(LC_ALL, "Russian");
//    
//    try {
//        screen_init();
//        
//        // 1. Демонстрация обработки ошибок
//        try {
//            rectangle bad_rect(point(-10,-10), point(300,300));
//        }
//        catch (const NotEnoughSpace& e) {
//            std::cerr << "Ошибка: " << e.what() << std::endl;
//        }
//        
//        // 2. Основной рабочий код
//        rectangle hat(point(0,0), point(14,5));
//        line brim(point(20,9), 17);
//        
//        // 3. Демонстрация ошибки трансформации
//        try {
//            hat.rotate_left();
//            hat.rotate_left(); // Повторный поворот
//        }
//        catch (const AlreadyTransformed& e) {
//            std::cerr << "Предупреждение: " << e.what() << std::endl;
//        }
//        
//        shape_refresh();
//        std::cin.get();
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
//        return 1;
//    }
//    
//    screen_destroy();
//    return 0;
//}
//    */
//
//    setlocale(LC_ALL, "Rus");
//	screen_init( );
//
////== 1. Объявление набора фигур ==
//
//	rectangle hat(point(0, 0), point(14, 5));
//	line brim(point(20,9),17);
//
//	myshape face(point(15,10), point(27,18));
//	new_shape tie(point(40,10), point(55,13));
//	new_shape feather(point(40,15), point(45,16));
//	new_shape left_sideburn(point(47,15), point(55,16));
//	new_shape right_sideburn(point(57,15), point(65,16));
//
//	shape_refresh();
//
//	std::cout << "=== Generated... ===\n";
//	std::cin.get(); //Смотреть исходный набор
//
//
////== 2. Подготовка к сборке ==
//
//	hat.rotate_right( );
//	brim.resize(2.0);
//	face.resize(1.2);
//	tie.flip_horisontally();
//	tie.rotate_right();
//	feather.rotate_right();
//	left_sideburn.rotate_left();
//	right_sideburn.rotate_right();
//	right_sideburn.flip_vertically();
//	shape_refresh();
//
//	std::cout << "=== Prepared... ===\n";
//	std::cin.get(); //Смотреть результат поворотов/отражений
//
////== 3. Сборка изображения ==
////	face.move(0, -10); // Лицо – в исходное положение (если нужно!)
//
//	up(brim, face);
//	up(hat, brim);
//    up(feather, hat);
//	center(feather, hat);
//	down(tie, face);
//	left(left_sideburn, face);
//	right(right_sideburn, face);
//	shape_refresh( );
//
//	std::cout << "=== Ready! ===\n";
//
//	std::cin.get();       //Смотреть результат
//	screen_destroy( );
//
//	return 0; 
//}
//
