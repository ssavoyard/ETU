#include <iostream>
#include "screen.h"
#include "exceptions.h"
#include "shape.h"


int main() {

    setlocale(LC_ALL, "Russian");

    try {
        screen_init();

        // 1. ������������ ��������� ������
        try {
            rectangle bad_rect(point(-10,-10), point(300,300));
        }
        catch (const NotEnoughSpace& e) {
            std::cerr << "������: " << e.what() << std::endl;
        }

        // 2. �������� ������� ���
        rectangle hat(point(0,0), point(14,5));
        line brim(point(20,9), 17);

        // 3. ������������ ������ �������������
        try {
            hat.rotate_left();
            hat.rotate_left(); // ��������� �������
        }
        catch (const AlreadyTransformed& e) {
            std::cerr << "��������������: " << e.what() << std::endl;
        }

        shape_refresh();
        std::cin.get();
    }
    catch (const std::exception& e) {
        std::cerr << "����������� ������: " << e.what() << std::endl;
        return 1;
    }


    screen_init();

    //== 1. ���������� ������ ����� ==

    rectangle hat(point(0, 0), point(14, 5));
    line brim(point(20, 9), 17);

    myshape face(point(15, 10), point(27, 18));
    new_shape tie(point(40, 10), point(55, 13));
    new_shape feather(point(40, 15), point(45, 16));
    new_shape left_sideburn(point(47, 15), point(55, 16));
    new_shape right_sideburn(point(57, 15), point(65, 16));

    shape_refresh();

    std::cout << "=== Generated... ===\n";
    std::cin.get(); //�������� �������� �����


    //== 2. ���������� � ������ ==

    hat.rotate_right();
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
    std::cin.get(); //�������� ��������� ���������/���������

    //== 3. ������ ����������� ==
    //	face.move(0, -10); // ���� � � �������� ��������� (���� �����!)

    up(brim, face);
    up(hat, brim);
    up(feather, hat);
    center(feather, hat);
    down(tie, face);
    left(left_sideburn, face);
    right(right_sideburn, face);

    std::cout << "=== Ready! ===\n";

    std::cin.get();       //�������� ���������
    screen_destroy();

    return 0;
}

