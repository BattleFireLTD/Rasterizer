#pragma once
namespace Alice {
	class Matrix4x4 {
	public:
		float mData[16];
		Matrix4x4();
		Matrix4x4(float*v);
	public:
		void Set(int row, int column, float v);
		float Get(int row, int column) const;
		float Determinant();
		void operator=(const Matrix4x4&r);
		Matrix4x4 operator+(const Matrix4x4&r);
		Matrix4x4 operator-(const Matrix4x4&r);
		Matrix4x4 operator*(float scalar);
		Matrix4x4 operator*(const Matrix4x4&right);
		Matrix4x4 Transpose();
		Matrix4x4 Inverse();
		void LookAt(float posx, float posy, float posz,
			float viewcenterx, float viewcentery, float viewcenterz,
			float upx, float upy, float upz);
		void Perspective(float fov, float aspect, float near, float far);
		void Ortho(float left, float right, float bottom, float top, float near, float far);
		void Dump();
	};
	inline void Vector4MultiplyMatrix4x4(float * v, const Matrix4x4&r, float * out) {
		out[0] = v[0] * r.Get(1, 1) + v[1] * r.Get(2, 1) + v[2] * r.Get(3, 1) + v[3] * r.Get(4, 1);
		out[1] = v[0] * r.Get(1, 2) + v[1] * r.Get(2, 2) + v[2] * r.Get(3, 2) + v[3] * r.Get(4, 2);
		out[2] = v[0] * r.Get(1, 3) + v[1] * r.Get(2, 3) + v[2] * r.Get(3, 3) + v[3] * r.Get(4, 3);
		out[3] = v[0] * r.Get(1, 4) + v[1] * r.Get(2, 4) + v[2] * r.Get(3, 4) + v[3] * r.Get(4, 4);
	}
}