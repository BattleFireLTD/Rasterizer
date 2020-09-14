#include "AliceMatrix3x3.h"
#include <stdio.h>
#include <string.h>
namespace Alice {
	Matrix3x3::Matrix3x3() {
		memset(mData, 0, sizeof(float) * 9);
	}
	Matrix3x3::Matrix3x3(float *v) {
		memcpy(mData, v, sizeof(float) * 9);
	}
	void Matrix3x3::Set(int row, int column, float v) {
		mData[(row - 1) * 3 + (column - 1)] = v;
	}
	float Matrix3x3::Get(int row, int column) {
		return mData[(row - 1) * 3 + (column - 1)];
	}
	float Matrix3x3::Determinant() {
		return mData[0] * mData[4] * mData[8] + mData[1] * mData[5] * mData[6] + mData[2] * mData[3] * mData[7]
			- mData[2] * mData[4] * mData[6] - mData[0] * mData[5] * mData[7] - mData[1] * mData[3] * mData[8];
	}
	void Matrix3x3::operator=(const Matrix3x3&r) {
		memcpy(mData, r.mData, sizeof(float) * 9);
	}
	Matrix3x3 Matrix3x3::operator+(const Matrix3x3&r) {
		Matrix3x3 ret;
		for (int i = 0; i < 9; ++i) {
			ret.mData[i] = mData[i] + r.mData[i];
		}
		return ret;
	}
	Matrix3x3 Matrix3x3::operator-(const Matrix3x3&r) {
		Matrix3x3 ret;
		for (int i = 0; i < 9; ++i) {
			ret.mData[i] = mData[i] - r.mData[i];
		}
		return ret;
	}
	Matrix3x3 Matrix3x3::operator*(float scalar) {
		Matrix3x3 ret;
		for (int i = 0; i < 9; ++i) {
			ret.mData[i] = mData[i] * scalar;
		}
		return ret;
	}
	Matrix3x3 Matrix3x3::operator*(const Matrix3x3&r) {
		Matrix3x3 ret;
		ret.mData[0] = mData[0] * r.mData[0] + mData[1] * r.mData[3] + mData[2] * r.mData[6];
		ret.mData[1] = mData[0] * r.mData[1] + mData[1] * r.mData[4] + mData[2] * r.mData[7];
		ret.mData[2] = mData[0] * r.mData[2] + mData[1] * r.mData[5] + mData[2] * r.mData[8];
		ret.mData[3] = mData[3] * r.mData[0] + mData[4] * r.mData[3] + mData[5] * r.mData[6];
		ret.mData[4] = mData[3] * r.mData[1] + mData[4] * r.mData[4] + mData[5] * r.mData[7];
		ret.mData[5] = mData[3] * r.mData[2] + mData[4] * r.mData[5] + mData[5] * r.mData[8];
		ret.mData[6] = mData[6] * r.mData[0] + mData[7] * r.mData[3] + mData[8] * r.mData[6];
		ret.mData[7] = mData[6] * r.mData[1] + mData[7] * r.mData[4] + mData[8] * r.mData[7];
		ret.mData[8] = mData[6] * r.mData[2] + mData[7] * r.mData[5] + mData[8] * r.mData[8];
		return ret;
	}
	Matrix3x3 Matrix3x3::Transpose() {
		Matrix3x3 ret;
		ret.mData[0] = mData[0];
		ret.mData[1] = mData[3];
		ret.mData[2] = mData[6];
		ret.mData[3] = mData[1];
		ret.mData[4] = mData[4];
		ret.mData[5] = mData[7];
		ret.mData[6] = mData[2];
		ret.mData[7] = mData[5];
		ret.mData[8] = mData[8];
		return ret;
	}
	Matrix3x3 Matrix3x3::Inverse() {
		float det = Determinant();
		float det_abs = det > 0.0f ? det : -det;
		if (det_abs < 0.000001f) {
			return Matrix3x3();
		}
		Matrix3x3 ret;
		ret.mData[0] = (Get(2, 2) * Get(3, 3) - Get(2, 3) * Get(3, 2)) / det;
		ret.mData[1] = (Get(1, 3) * Get(3, 2) - Get(1, 2) * Get(3, 3)) / det;
		ret.mData[2] = (Get(1, 2) * Get(2, 3) - Get(1, 3) * Get(2, 2)) / det;
		ret.mData[3] = (Get(2, 3) * Get(3, 1) - Get(2, 1) * Get(3, 3)) / det;
		ret.mData[4] = (Get(1, 1) * Get(3, 3) - Get(1, 3) * Get(3, 1)) / det;
		ret.mData[5] = (Get(1, 3) * Get(2, 1) - Get(1, 1) * Get(2, 3)) / det;
		ret.mData[6] = (Get(2, 1) * Get(3, 2) - Get(2, 2) * Get(3, 1)) / det;
		ret.mData[7] = (Get(1, 2) * Get(3, 1) - Get(1, 1) * Get(3, 2)) / det;
		ret.mData[8] = (Get(1, 1) * Get(2, 2) - Get(1, 2) * Get(2, 1)) / det;
		return ret;
	}
	void Matrix3x3::Dump() {
		printf("dump mat3:\n=====start\n%f,%f,%f\n%f,%f,%f\n%f,%f,%f\n=====end\n",
			mData[0], mData[1], mData[2],
			mData[3], mData[4], mData[5],
			mData[6], mData[7], mData[8]);
	}
}