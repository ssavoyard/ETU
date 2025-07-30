// shape.h
#ifndef SHAPE_H  // Защита от повторного включения
#define SHAPE_H

#include "screen.h"     // Для point и функций экрана
#include "exceptions.h" // Для классов исключений
#include <list>        // Для std::list

// Предварительное объявление (forward declaration) классов
class shape;
class rotatable;
class reflectable;
class line;
class rectangle;

// ==============================================
// Базовый класс фигуры (только объявление!)
// ==============================================
class shape {
protected:
	static std::list<shape*> shapes; // Статический список всех фигур

public:
	friend void shape_refresh(); // Даем доступ функции
	shape();
	virtual ~shape();

	// Чисто виртуальные методы (должны быть реализованы в потомках)
	virtual point north() const = 0;
	virtual point south() const = 0;
	virtual point east() const = 0;
	virtual point west() const = 0;
	virtual point neast() const = 0;
	virtual point seast() const = 0;
	virtual point nwest() const = 0;
	virtual point swest() const = 0;

	virtual void draw() = 0;
	virtual void move(int, int) = 0;
	virtual void resize(double) = 0;

	// Статический метод для перерисовки всех фигур
	static void refresh_all();
};

// ==============================================
// Класс для поворачиваемых фигур (объявление)
// ==============================================
class rotatable : virtual public shape {
protected:
	enum class rotated { left, none, right };
	rotated state;

public:
	rotatable(rotated r = rotated::none);
	virtual void rotate_left();
	virtual void rotate_right();
};

// ==============================================
// Класс для отражаемых фигур (объявление)
// ==============================================
class reflectable : virtual public shape {
protected:
	bool hor, vert;

public:
	reflectable(bool h = false, bool v = false);
	virtual void flip_horisontally();
	virtual void flip_vertically();
};

// ==============================================
// Конкретные классы фигур (только объявления!)
// ==============================================

// 1. Линия
class line : public shape {
	point w, e;
public:
	line(point a, point b);
	line(point a, int L);

	// Реализация виртуальных методов
	point north() const override;
	point south() const override;
	point east() const override;
	point west() const override;
	point neast() const override;
	point seast() const override;
	point nwest() const override;
	point swest() const override;
	
	void move(int a, int b) override;
	void draw() override;
	virtual void resize(double d) override;
};

// 2. Прямоугольник
class rectangle : public rotatable {
	point sw, ne;
public:
	rectangle(point a, point b);

	// Все обязательные методы shape
	point north() const override { return point((sw.x + ne.x) / 2, ne.y); }
	point south() const override { return point((sw.x + ne.x) / 2, sw.y); }
	point east() const override { return point(ne.x, (sw.y + ne.y) / 2); }
	point west() const override { return point(sw.x, (sw.y + ne.y) / 2); }
	point neast() const override { return ne; }
	point seast() const override { return point(ne.x, sw.y); }
	point nwest() const override { return point(sw.x, ne.y); }
	point swest() const override { return sw; }

	void rotate_left() override;
	void rotate_right() override;
	void move(int a, int b) override;
	void draw() override;
	virtual void resize(double d) override;
	void resize(int d);
};

// 3. Пользовательская фигура (лицо)
class myshape : public rectangle {
	int w, h;
	line l_eye;
	line r_eye;
	line mouth;
public:
	myshape(point, point);
	void draw();
	void move(int, int);
	void resize(double r);
	void rotate_left() {}
	void rotate_right() {}
};


// ==============================================
// Класс Cross (крест)
// ==============================================
class cross : public rotatable {
protected:
	point sw, ne;
public:
	cross(point a, point b);

	point north() const override;
	point south() const override;
	point east() const override;
	point west() const override;
	point neast() const override;
	point seast() const override;
	point nwest() const override;
	point swest() const override;

	void move(int a, int b) override;
	void draw() override;
	virtual void resize(double d) override;
	void resize(int d);
};

// ==============================================
// Класс Parallelogram (параллелограмм)
// ==============================================
class parallelogram : public rotatable, public reflectable {
protected:
	point sw, ne;
	bool rotated_flag = false;
	bool reflected_flag = false;
public:
	parallelogram(point a, point b);

	point north() const override;
	point south() const override;
	point east() const override;
	point west() const override;
	point neast() const override;
	point seast() const override;
	point nwest() const override;
	point swest() const override;

	void rotate_left() override;
	void rotate_right() override;
	void move(int a, int b);
	void draw() override;
	virtual void resize(double d) override;
	void resize(int d);
	void flip_horisontally() override;
	void flip_vertically() override;
};

// ==============================================
// Класс NewShape (комбинированная фигура)
// ==============================================
class new_shape : public parallelogram, public cross {
public:
	new_shape(point a, point b);

	// Явно указываем, методы какого родителя использовать
	point north() const override { return cross::north(); }
	point south() const override { return cross::south(); }
	point east() const override { return cross::east(); }
	point west() const override { return cross::west(); }
	point neast() const override { return cross::neast(); }
	point seast() const override { return cross::seast(); }
	point nwest() const override { return cross::nwest(); }
	point swest() const override { return cross::swest(); }

	void rotate_left() override;
	void rotate_right() override;
	void move(int a, int b) override;
	void draw() override;  // Только объявление, реализация в shape2.cpp
	virtual void resize(double d) override;
	void flip_horisontally() override;
	void flip_vertically() override;
};

// ==============================================
// Вспомогательные функции (объявления)
// ==============================================
void shape_refresh();
void up(shape& p, const shape& q);    // Разместить p над q
void down(shape& p, const shape& q);  // Разместить p под q
void left(shape& p, const shape& q);  // Разместить p слева от q
void right(shape& p, const shape& q); // Разместить p справа от q
void center(shape& p, const shape& q); // Центрировать p относительно q

#endif // SHAPE_H