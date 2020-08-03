#include "Vector2D.h"
#include <iostream>

Vector2D::Vector2D() {
	x = 0;
	y = 0;
}

Vector2D::Vector2D(float x, float y) {
	this->x = x;
	this->y = y;
}

Vector2D& Vector2D::add(const Vector2D& vec) {
	this->x += vec.x;
	this->y += vec.y;
	return *this;
}

Vector2D& Vector2D::subtract(const Vector2D& vec) {
	this->x -= vec.x;
	this->y -= vec.y;
	return *this;
}
Vector2D& Vector2D::multiply(const Vector2D& vec) {
	this->x *= vec.x;
	this->y *= vec.y;
	return *this;
}
Vector2D& Vector2D::divide(const Vector2D& vec) {
	this->x /= vec.x;
	this->y /= vec.y;
	return *this;
}
//Operator overloading
Vector2D& operator+(Vector2D& vec1, const Vector2D& vec2) {
	return vec1.add(vec2);
}
Vector2D& operator-(Vector2D& vec1, const Vector2D& vec2) {
	return vec1.subtract(vec2);
}
Vector2D& operator*(Vector2D& vec1, const Vector2D& vec2) {
	return vec1.multiply(vec2);
}
Vector2D& operator/(Vector2D& vec1, const Vector2D& vec2) {
	return vec1.divide(vec2);
}

Vector2D& Vector2D::operator+=(const Vector2D& vec) {
	return this->add(vec);
}
Vector2D& Vector2D::operator-=(const Vector2D& vec) {
	return this->subtract(vec);

}
Vector2D& Vector2D::operator*=(const Vector2D& vec) {
	return this->multiply(vec);
}
Vector2D& Vector2D::operator/=(const Vector2D& vec) {
	return this->divide(vec);
}
Vector2D& Vector2D::operator*(const int& i) {
	this->x *= i;
	this->y *= i;
	return *this;
}
Vector2D& Vector2D::zero() {
	this->x = 0;
	this->y = 0;
	return *this;
}
std::ostream& operator<<(std::ostream& stream, const Vector2D& vec) {
	stream << "(" << vec.x << "," << vec.y << ")";
	return stream;
}
