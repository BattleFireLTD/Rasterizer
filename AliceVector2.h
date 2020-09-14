#pragma once
namespace Alice {
	class Vector2 {
	public:
		float x, y;
	public:
		Vector2() :x(0), y(0) {}
		Vector2(float x, float y) :x(x), y(y) {}
		Vector2 operator+(const Vector2&r);
		Vector2 operator*(float scalar);
		float operator*(const Vector2&r);
		Vector2 operator-(const Vector2&r);
		void operator=(const Vector2&r);
		float magnitude();
		void normalize();
	};
	Vector2 operator-(const Vector2&r);
}