#pragma once
namespace Alice {
	class Vector4 {
	public:
		float x, y, z ,w;
	public:
		Vector4() :x(0), y(0), z(0) ,w(0.0f) {}
		Vector4(float x, float y, float z,float w=1.0f) :x(x), y(y), z(z) ,w(w) {}
		Vector4 operator+(const Vector4&r);
		Vector4 operator-(const Vector4&r);
		Vector4 operator*(float scalar);
		float operator*(const Vector4&r);
		Vector4 operator^(const Vector4&r) const;
		float magnitude();
		void normalize();
	};
}