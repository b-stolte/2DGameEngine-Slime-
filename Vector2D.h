#pragma once
#include <iostream>

class Vector2D {

public:
	float x;
	float y;

	Vector2D();
	Vector2D(float x, float y);

	Vector2D& add(const Vector2D& vec);
	Vector2D& subtract(const Vector2D& vec);
	Vector2D& multiply(const Vector2D& vec);
	Vector2D& divide(const Vector2D& vec);

	friend Vector2D& operator+(Vector2D& vec1, const Vector2D& vec2);
	friend Vector2D& operator-(Vector2D& vec1, const Vector2D& vec2);
	friend Vector2D& operator*(Vector2D& vec1, const Vector2D& vec2);
	friend Vector2D& operator/(Vector2D& vec1, const Vector2D& vec2);

	Vector2D& operator+=(const Vector2D& vec);
	Vector2D& operator-=(const Vector2D& vec);
	Vector2D& operator*=(const Vector2D& vec);
	Vector2D& operator/=(const Vector2D& vec);


	Vector2D& operator*(const int& i);
	Vector2D& zero();

	//Output overload
	friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);
};