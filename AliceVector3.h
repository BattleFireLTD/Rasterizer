#pragma once
#include <math.h>
#include <algorithm>
namespace Alice {
	class Vector3 {
	public:
		float x, y, z;
	public:
		Vector3() :x(0), y(0), z(0) {}
		Vector3(float x, float y, float z) :x(x), y(y), z(z) {}
		Vector3 operator+(const Vector3&r);
		Vector3 operator-(const Vector3&r);
		Vector3 operator*(float scalar);
		float operator*(const Vector3&r);
		Vector3 operator^(const Vector3&r) const;
		float magnitude();
		void normalize();
		Vector3 ProjectTo(Vector3&v);
		Vector3 PerpendicularTo(Vector3&v);
		float LengthSquared();
	};
	Vector3 operator*(float scalar,Vector3&v);
}