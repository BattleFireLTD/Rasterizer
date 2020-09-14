#pragma once
#include "AliceVector3.h"
namespace Alice {
	class Matrix3x3 {
	public:
		float mData[9];
		Matrix3x3();
		Matrix3x3(float*v);
	public:
		void Set(int row, int column, float v);
		float Get(int row, int column);
		float Determinant();
		void operator=(const Matrix3x3&r);
		Matrix3x3 operator+(const Matrix3x3&r);
		Matrix3x3 operator-(const Matrix3x3&r);
		Matrix3x3 operator*(float scalar);
		Matrix3x3 operator*(const Matrix3x3&r);
		Matrix3x3 Transpose();
		Matrix3x3 Inverse();
		void Dump();
	};
	inline Vector3 operator*(const Vector3&l, const Matrix3x3&r) {
		Vector3 ret;
		ret.x = l.x*r.mData[0] + l.y*r.mData[3] + l.z*r.mData[6];
		ret.y = l.x*r.mData[1] + l.y*r.mData[4] + l.z*r.mData[7];
		ret.z = l.x*r.mData[2] + l.y*r.mData[5] + l.z*r.mData[8];
		return ret;
	}
}