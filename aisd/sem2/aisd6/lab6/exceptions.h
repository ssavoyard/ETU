#ifndef SHAPE_EXCEPTIONS_H  // ������ �� ���������� ���������
#define SHAPE_EXCEPTIONS_H

#include <exception>  // ������� ����� ����������
#include <string>     // ��� ������ �� ��������
#include <sstream>    // ��� �������������� ���������

// ==============================================
// ������� ����� ��� ���� ���������� �����
// ============================================
class ShapeError : public std::exception {
    std::string msg;
public:
    ShapeError(const std::string& message) : msg("������ ������: " + message) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

// ==============================================
// ���������� ���� ���������� (��� � ���������)
// ==============================================

// 1. ����� ��� ������
class PointOutOfScreen : public ShapeError {
public:
    PointOutOfScreen(int x, int y) : ShapeError(
        "����� (" + std::to_string(x) + ", " + std::to_string(y) + ") ����� �� ������� ������!"
    ) {}
};

// 2. ������������ ��������� ������
class InvalidShapeParameters : public ShapeError {
public:
    InvalidShapeParameters(const std::string& details) : ShapeError(
        "������������ ��������� ������: " + details
    ) {}
};

// 3. �� ������� ����� �� ������
class NotEnoughSpace : public ShapeError {
public:
    NotEnoughSpace(const std::string& shape_type) : ShapeError(
        "������������ ����� ��� ������ ����: " + shape_type
    ) {}
};

// 4. ��������� �������������
class AlreadyTransformed : public ShapeError {
public:
    AlreadyTransformed(const std::string& transform_type) : ShapeError(
        "������ ��� ���� �������������: " + transform_type
    ) {}
};

// 5. ������ ������������� ������ (�� �������)
class AttachmentError : public ShapeError {
public:
    AttachmentError(const std::string& side) : ShapeError(
        "���������� ������������ ������: �������� ������� " + side
    ) {}
};

// ==============================================
// �������������� �������� ����������
// ==============================================

// ��� ������ ���������
class DrawingError : public ShapeError {
public:
    DrawingError(const std::string& shape_type) : ShapeError(
        "������ ��� ��������� " + shape_type
    ) {}
};

// ��� ������ �����������
class MovementError : public ShapeError {
public:
    MovementError(int dx, int dy) : ShapeError(
        "���������� ����������� �� (" + std::to_string(dx) + ", " + std::to_string(dy) + ")"
    ) {}
};

#endif // SHAPE_EXCEPTIONS_H
