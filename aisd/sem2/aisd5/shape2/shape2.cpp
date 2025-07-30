#include <locale.h>
#include <iostream>
#include "screen.h"
#include "shape.h"

class cross : public rotatable {
    protected:
        point sw, ne;
    public:
        cross(point a, point b) :  sw(a), ne(b) { }
        point north( ) const { return point((sw.x + ne.x) / 2, ne.y); }
        point south( ) const { return point((sw.x + ne.x) / 2, sw.y); }
        point east( ) const { return point(ne.x, (sw.y + ne.y) / 2); }
        point west( ) const { return point(sw.x, (sw.y + ne.y) / 2); }
        point neast( ) const { return ne; }
        point seast( ) const { return point(ne.x, sw.y); }
        point nwest( ) const { return point(sw.x, ne.y); }
        point swest( ) const { return sw; }
        void rotate_right( ){ 
            int w = ne.x - sw.x, h = ne.y - sw.y;
            sw.x = ne.x - h * 2; ne.y = sw.y + w / 2;	
        }
        void rotate_left() {
            int w = ne.x - sw.x, h = ne.y - sw.y; 
            ne.x = sw.x + h * 2; ne.y = sw.y + w / 2; 
        }
        void move(int a, int b)
            { sw.x += a; sw.y += b; ne.x += a; ne.y += b; }
        void resize(int d) {
            ne.x = sw.x + (ne.x - sw.x) * d; ne.y = sw.y + (ne.y - sw.y) * d; 
        }
        void resize(double d){
            ne.x = sw.x + (ne.x - sw.x) * d;    ne.y = sw.y + (ne.y - sw.y) * d;
        }
        void draw( ){ 
            put_line(sw, ne);
            put_line(seast(), nwest( ));
        }
};

class paralellogram : public rotatable, public reflectable {
    protected:
        point sw, ne;
        bool rotated = 0, reflected = 0;
    public:
        paralellogram(point a, point b) :  sw(a), ne(b) { }
        point north( ) const { return point((sw.x + ne.x) / 2, ne.y); }
        point south( ) const { return point((sw.x + ne.x) / 2, sw.y); }
        point east( ) const { return point(ne.x, (sw.y + ne.y) / 2); }
        point west( ) const { return point(sw.x, (sw.y + ne.y) / 2); }
        point neast( ) const { return ne; }
        point seast( ) const { return point(ne.x, sw.y); }
        point nwest( ) const { return point(sw.x, ne.y); }
        point swest( ) const { return sw; }
        void rotate_right( ){ 
            int w = ne.x - sw.x, h = ne.y - sw.y;
            sw.x = ne.x - h * 2; ne.y = sw.y + w / 2;	
            rotated = !rotated;
        }
        void rotate_left(){ 
            int w = ne.x - sw.x, h = ne.y - sw.y; 
            ne.x = sw.x + h * 2; ne.y = sw.y + w / 2; 
            rotated = !rotated;
        }
        void move(int a, int b){ 
            sw.x += a; sw.y += b; ne.x += a; ne.y += b; 
        }
        void resize(int d){ 
            ne.x = sw.x + (ne.x - sw.x) * d; ne.y = sw.y + (ne.y - sw.y) * d; 
        }
        void resize(double d){
            ne.x = sw.x + (ne.x - sw.x) * d;    ne.y = sw.y + (ne.y - sw.y) * d;
        }
        void flip_horisontally(){
            reflectable::flip_horisontally();
            reflected = !reflected;
        }
        
        void flip_vertically(){
            reflectable::flip_vertically();
            reflected = !reflected;
        }
        void draw( ) { 
            if (!rotated && !reflected){
                put_line(swest(), north());
                put_line(north(), neast());
                put_line(neast(), south());
                put_line(south(), swest());
            } else if (rotated && !reflected) {
                put_line(nwest(), east());
                put_line(east(), seast());
                put_line(seast(), west());
                put_line(west(), nwest());
            } else if (!rotated && reflected) {
                put_line(nwest(), north());
                put_line(north(), seast());
                put_line(seast(), south());
                put_line(south(), nwest());
            } else {
                put_line(west(), neast());
                put_line(neast(), east());
                put_line(east(), swest());
                put_line(swest(), west());
            }
        }
};

class new_shape : public paralellogram, public cross {	
	public:
		new_shape(point a, point b): paralellogram(a, b), cross(a, b){};
        point north( ) const { return cross::north(); }
        point south( ) const { return cross::south(); }
        point east( ) const { return cross::east(); }
        point west( ) const { return cross::west(); }
        point neast( ) const { return cross::neast(); }
        point seast( ) const { return cross::seast(); }
        point nwest( ) const { return cross::nwest(); }
        point swest( ) const { return cross::swest(); }
        void rotate_right() { 
            paralellogram::rotate_right();
            cross::rotate_right();
        }
        void rotate_left() {
            paralellogram::rotate_left();
            cross::rotate_left();
        }
        void move(int a, int b){
            paralellogram::move(a,b);
            cross::move(a,b);
        }
        void resize(int d) { 
            paralellogram::resize(d);
            cross::resize(d); 
        }
        void resize(double d){
            paralellogram::resize(d);
            cross::resize(d);
        }
        void flip_horisontally(){
            paralellogram::flip_horisontally();
        }
        void flip_vertically(){
            paralellogram::flip_vertically();
        }
        void draw(){ 
            paralellogram::draw();
            cross::draw();
        }
};


void down(shape &p,  const shape &q){
	point n = q.south( );
    point s = p.north( );
    p.move(n.x - s.x, n.y - s.y - 1); 
}
void left(shape &p,  const shape &q){
	point n = q.swest( );
	point s = p.seast( );
	p.move(n.x - s.x - 1, n.y - s.y); 
}
void right(shape &p,  const shape &q){
	point n = q.seast( );
	point s = p.swest( );
	p.move(n.x - s.x + 1, n.y - s.y);
}

// void center(shape &p,  const shape &q){
// 	point n1 = q.north( );
// 	point n2 = p.north( );
//     point s1 = q.south();
//     point s2 = p.south();
// 	p.move(n1.x - n2.x, n1.y - n2.y - (n1.y - s1.y)/2 + (n2.y - s2.y)/2);
// }

// Cборная пользовательская фигура – физиономия
class myshape : public rectangle {   // Моя фигура ЯВЛЯЕТСЯ
     int w, h;			             //        прямоугольником
     line l_eye;    // левый глаз – моя фигура СОДЕРЖИТ линию
     line r_eye;   // правый глаз
     line mouth;  // рот
  public:
     myshape(point, point);
     void draw( );
     void move(int, int);
	 void resize(double r) { rectangle::resize(r); rectangle::move(w*(1-r)*0.5, h*(1-r)*0.5); }
	 void rotate_left() {}
	 void rotate_right() {}
};
myshape :: myshape(point a, point b)
	: rectangle(a, b),	//Инициализация базового класса
	  w(neast( ).x - swest( ).x + 1), // Инициализация данных
	  h(neast( ).y - swest( ).y + 1), // – строго в порядке объявления!
	  l_eye(point(swest( ).x + 2, swest( ).y + h * 3 / 4), 2),
	  r_eye(point(swest( ).x + w - 4, swest( ).y + h * 3 / 4), 2),
	  mouth(point(swest( ).x + 2, swest( ).y + h / 4), w - 4) 
{ }
void myshape :: draw( )
{
	 rectangle :: draw( );      //Контур лица (глаза и нос рисуются сами!) 
	 int a = (swest( ).x + neast( ).x) / 2;
	 int b = (swest( ).y + neast( ).y) / 2;
	 put_point(point(a, b));   // Нос – существует только на рисунке!
}
void myshape :: move(int a, int b)
{
	 rectangle :: move(a, b);
	 l_eye.move(a, b);  r_eye.move(a, b);
	 mouth.move(a, b);
}
int main( ) 
{   setlocale(LC_ALL, "Rus");
	screen_init( );
//== 1. Объявление набора фигур ==
	rectangle hat(point(0, 0), point(14, 5));
	line brim(point(20,9),17);
	myshape face(point(15,10), point(27,18));
	new_shape tie(point(40,10), point(55,13));
	new_shape feather(point(40,15), point(45,16));
	new_shape left_sideburn(point(47,15), point(55,16));
	new_shape right_sideburn(point(57,15), point(65,16));
	shape_refresh();
	std::cout << "=== Generated... ===\n";
	std::cin.get(); //Смотреть исходный набор
//== 2. Подготовка к сборке ==
	hat.rotate_right( );
	brim.resize(2.0);
	face.resize(1.2);
	tie.flip_horisontally();
	tie.rotate_right();
	feather.rotate_right();
	left_sideburn.rotate_left();
	right_sideburn.rotate_right();
	right_sideburn.flip_vertically();
	shape_refresh();
	std::cout << "=== Prepared... ===\n";
	std::cin.get(); //Смотреть результат поворотов/отражений
//== 3. Сборка изображения ==
//	face.move(0, -10); // Лицо – в исходное положение (если нужно!)
	up(brim, face);
	up(hat, brim);
    up(feather, hat);
	// center(feather, hat);
	down(tie, face);
	left(left_sideburn, face);
	right(right_sideburn, face);
	shape_refresh( );
	std::cout << "=== Ready! ===\n";
	std::cin.get();       //Смотреть результат
	screen_destroy( );
	return 0; 
}

