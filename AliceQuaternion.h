#pragma once
#include <math.h>
#include "AliceVector3.h"
#include "AliceMatrix3x3.h"
namespace Alice {
	class Quaternion {
	public:
		float x, y, z, w;
		Quaternion();
		Quaternion(float x, float y, float z, float w);
		inline friend Quaternion operator * (const Quaternion& lhs, const Quaternion& rhs) {
			return Quaternion(
				lhs.w*rhs.x + lhs.x*rhs.w + lhs.y*rhs.z - lhs.z*rhs.y,
				lhs.w*rhs.y + lhs.y*rhs.w + lhs.z*rhs.x - lhs.x*rhs.z,
				lhs.w*rhs.z + lhs.z*rhs.w + lhs.x*rhs.y - lhs.y*rhs.x,
				lhs.w*rhs.w - lhs.x*rhs.x - lhs.y*rhs.y - lhs.z*rhs.z);
		}
	};
	Quaternion EulerToQuaternion(const Vector3& euler);
	void QuaternionToMatrix(const Quaternion& q, Matrix3x3& m);
}