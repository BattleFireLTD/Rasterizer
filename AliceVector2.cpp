#include "AliceVector2.h"
#include <math.h>
namespace Alice {
	float Vector2::magnitude() {
		return sqrtf(x*x + y * y);
	}
	void Vector2::normalize() {
		float len = magnitude();
		if (len > 0.000001f) {
			x /= len;
			y /= len;
		}
		else {
			x = 0.0f;
			y = 0.0f;
		}
	}
	Vector2 Vector2::operator+(const Vector2&r) {
		return Vector2(x + r.x, y + r.y);
	}
	Vector2 Vector2::operator*(float scalar) {
		return Vector2(x*scalar, y*scalar);
	}
	float Vector2::operator*(const Vector2&r) {
		return x * r.x + y * r.y;
	}
	Vector2 Vector2::operator-(const Vector2&r) {
		return Vector2(x - r.x, y - r.y);
	}
	void Vector2::operator=(const Vector2&r) {
		x = r.x; y = r.y;
	}
	Vector2 operator-(const Vector2&r) {
		return Vector2(-r.x, -r.y);
	}

}