#include "AliceVector4.h"
#include <math.h>
namespace Alice {
	Vector4 Vector4::operator+(const Vector4&r) {
		return Vector4(x + r.x, y + r.y, z + r.z);
	}
	Vector4 Vector4::operator-(const Vector4&r) {
		return Vector4(x - r.x, y - r.y, z - r.z);
	}
	Vector4 Vector4::operator*(float scalar) {
		return Vector4(x*scalar, y*scalar, z*scalar);
	}
	float Vector4::operator*(const Vector4&r) {
		return x * r.x + y * r.y + z * r.z;
	}
	Vector4 Vector4::operator^(const Vector4&r) const {
		return Vector4(y*r.z - r.y*z, z*r.x - r.z*x, x*r.y - r.x*y);
	}
	float Vector4::magnitude() {
		return sqrtf(x*x + y * y + z * z);
	}
	void Vector4::normalize() {
		float len = magnitude();
		if (len > 0.000001f) {
			x /= len;
			y /= len;
			z /= len;
		}
		else {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}
	}
}