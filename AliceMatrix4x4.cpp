#include "AliceMatrix4x4.h"
#include "AliceMatrix3x3.h"
#include "AliceVector3.h"
#include <stdio.h>
#include <string.h>
namespace Alice {
	Matrix3x3 Cof(const Matrix4x4&mat4, int row, int column) {
		Matrix3x3 ret;
		int j = 0;
		for (int r = 1; r <= 4; ++r) {
			for (int c = 1; c <= 4; ++c) {
				if (r != row && c != column) {
					ret.mData[j++] = mat4.Get(r, c);
				}
			}
		}
		return ret;
	}
	float RowxColumn(const Matrix4x4&l, const Matrix4x4&r, int row, int column) {
		return l.Get(row, 1)*r.Get(1, column) +
			l.Get(row, 2)*r.Get(2, column) +
			l.Get(row, 3)*r.Get(3, column) +
			l.Get(row, 4)*r.Get(4, column);
	}
	Matrix4x4::Matrix4x4() {
		memset(mData, 0, sizeof(float) * 16);
	}
	Matrix4x4::Matrix4x4(float *v) {
		memcpy(mData, v, sizeof(float) * 16);
	}
	void Matrix4x4::Set(int row, int column, float v) {
		mData[(row - 1) * 4 + (column - 1)] = v;
	}
	float Matrix4x4::Get(int row, int column) const {
		return mData[(row - 1) * 4 + (column - 1)];
	}
	float Matrix4x4::Determinant() {
		Matrix3x3 mat11 = Cof(*this, 1, 1);
		Matrix3x3 mat12 = Cof(*this, 1, 2);
		Matrix3x3 mat13 = Cof(*this, 1, 3);
		Matrix3x3 mat14 = Cof(*this, 1, 4);
		return Get(1, 1)*
			mat11.Determinant() - Get(1, 2)*
			mat12.Determinant() + Get(1, 3)*
			mat13.Determinant() - Get(1, 4)*
			mat14.Determinant();
	}
	void Matrix4x4::operator=(const Matrix4x4&r) {
		memcpy(mData, r.mData, sizeof(float) * 16);
	}
	Matrix4x4 Matrix4x4::operator+(const Matrix4x4&r) {
		Matrix4x4 ret;
		for (int i = 0; i < 16; ++i) {
			ret.mData[i] = mData[i] + r.mData[i];
		}
		return ret;
	}
	Matrix4x4 Matrix4x4::operator-(const Matrix4x4&r) {
		Matrix4x4 ret;
		for (int i = 0; i < 16; ++i) {
			ret.mData[i] = mData[i] - r.mData[i];
		}
		return ret;
	}
	Matrix4x4 Matrix4x4::operator*(float scalar) {
		Matrix4x4 ret;
		for (int i = 0; i < 16; ++i) {
			ret.mData[i] = mData[i] * scalar;
		}
		return ret;
	}
	Matrix4x4 Matrix4x4::operator*(const Matrix4x4&right) {
		Matrix4x4 ret;
		for (int r = 1; r <= 4; ++r) {
			for (int c = 1; c <= 4; ++c) {
				ret.Set(r, c, RowxColumn(*this, right, r, c));
			}
		}
		return ret;
	}
	Matrix4x4 Matrix4x4::Transpose() {
		Matrix4x4 ret;
		for (int r = 1; r <= 4; ++r) {
			for (int c = 1; c <= 4; ++c) {
				ret.Set(r, c, Get(c, r));
			}
		}
		return ret;
	}
	Matrix4x4 Matrix4x4::Inverse() {
		float det = Determinant();
		float det_abs = det > 0.0f ? det : -det;
		if (det_abs < 0.000001f) {
			return Matrix4x4();
		}
		Matrix4x4 ret;
		for (int r = 1; r <= 4; ++r) {
			for (int c = 1; c <= 4; ++c) {
				ret.Set(r, c, ((r + c) % 2 == 0 ? 1 : -1)*Cof(*this, r, c).Determinant());
			}
		}
		ret = ret.Transpose();
		ret = ret * (1.0f / det);
		return ret;
	}
	void Matrix4x4::LookAt(float posx, float posy, float posz,
		float viewcenterx, float viewcentery, float viewcenterz,
		float upx, float upy, float upz) {
		Vector3 eye(posx, posy, posz);
		Vector3 viewcenter(viewcenterx, viewcentery, viewcenterz);
		Vector3 up(upx, upy, upz);
		up.normalize();
		Vector3 z = eye - viewcenter;
		z.normalize();
		Vector3 x = up ^ z;
		Vector3 y = z ^ x;
		Matrix4x4 rotateMatrix;
		rotateMatrix.Set(1, 1, x.x);
		rotateMatrix.Set(1, 2, x.y);
		rotateMatrix.Set(1, 3, x.z);
		rotateMatrix.Set(2, 1, y.x);
		rotateMatrix.Set(2, 2, y.y);
		rotateMatrix.Set(2, 3, y.z);
		rotateMatrix.Set(3, 1, z.x);
		rotateMatrix.Set(3, 2, z.y);
		rotateMatrix.Set(3, 3, z.z);
		rotateMatrix.Set(4, 4, 1.0f);
		rotateMatrix = rotateMatrix.Transpose();
		Matrix4x4 translateMatrix;
		translateMatrix.Set(1, 1, 1.0f);
		translateMatrix.Set(2, 2, 1.0f);
		translateMatrix.Set(3, 3, 1.0f);
		translateMatrix.Set(4, 4, 1.0f);
		translateMatrix.Set(4, 1, eye.x);
		translateMatrix.Set(4, 2, eye.y);
		translateMatrix.Set(4, 3, eye.z);
		translateMatrix = translateMatrix.Inverse();
		*this = translateMatrix * rotateMatrix;
	}
	void Matrix4x4::Perspective(float fov, float aspect, float near, float far) {
		memset(mData, 0, 16 * sizeof(float));
		float deltaZ = near - far;
		float radian = (fov / 2.0f / 360.0f)*2.0f*3.1415f;
		float yscale = cos(radian) / sin(radian);
		float xscale = yscale / aspect;
		mData[0] = xscale;
		mData[5] = yscale;
		mData[10] = (near + far) / deltaZ;
		mData[11] = -1.0f;
		mData[14] = (2.0f*near*far) / deltaZ;
	}
	void Matrix4x4::Ortho(float left, float right, float bottom, float top, float near, float far) {
		memset(mData, 0, 16 * sizeof(float));
		float deltax = right - left;
		float deltay = top - bottom;
		float deltaz = near - far;
		Set(1, 1, 2.0f / deltax);
		Set(1, 4, -(right + left) / deltax);
		Set(2, 2, 2.0f / deltay);
		Set(2, 4, -(top + bottom) / deltay);
		Set(3, 3, -2.0f / deltaz);
		Set(3, 4, -(far + near) / deltaz);
		Set(4, 4, 1.0f);
	}
	void Matrix4x4::Dump() {
		printf("dump mat4:\n=====start\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n=====end\n",
			mData[0], mData[1], mData[2], mData[3],
			mData[4], mData[5], mData[6], mData[7],
			mData[8], mData[9], mData[10], mData[11],
			mData[12], mData[13], mData[14], mData[15]);
	}
}