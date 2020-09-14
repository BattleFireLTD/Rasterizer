#include "AliceVector3.h"
#include <math.h>
namespace Alice {
	Vector3 Vector3::operator+(const Vector3&r) {
		return Vector3(x + r.x, y + r.y, z + r.z);
	}
	Vector3 Vector3::operator-(const Vector3&r) {
		return Vector3(x - r.x, y - r.y, z - r.z);
	}
	Vector3 Vector3::operator*(float scalar) {
		return Vector3(x*scalar, y*scalar, z*scalar);
	}
	float Vector3::operator*(const Vector3&r) {
		return x * r.x + y * r.y + z * r.z;
	}
	Vector3 Vector3::operator^(const Vector3&r) const {
		return Vector3(y*r.z - r.y*z, z*r.x - r.z*x, x*r.y - r.x*y);
	}
	float Vector3::magnitude() {
		return sqrtf(x*x + y * y + z * z);
	}
	void Vector3::normalize() {
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
	Vector3 Vector3::ProjectTo(Vector3&v) {
		float lenSquared = v.LengthSquared();
		return (*this)*v*v* (1.0f/lenSquared);
	}
	Vector3 Vector3::PerpendicularTo(Vector3&v) {
		Vector3 projP2Q = ProjectTo(v);
		return (*this) - projP2Q;
	}
	float Vector3::LengthSquared() {
		float len = x * x + y * y + z * z;
		return len != 0.0f ? len : 1.0f;
	}
	Vector3 operator*(float scalar,Vector3&r) {
		return r * scalar;
	}
}