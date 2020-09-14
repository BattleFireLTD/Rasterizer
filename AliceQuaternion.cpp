#include "AliceQuaternion.h"
namespace Alice {
	Quaternion::Quaternion() {
	}
	Quaternion::Quaternion(float x, float y, float z, float w) :
		x(x), y(y), z(z), w(w) {
	}
	Quaternion EulerToQuaternion(const Vector3& someEulerAngles) {
		float cX(cos(someEulerAngles.x / 2.0f));
		float sX(sin(someEulerAngles.x / 2.0f));
		float cY(cos(someEulerAngles.y / 2.0f));
		float sY(sin(someEulerAngles.y / 2.0f));
		float cZ(cos(someEulerAngles.z / 2.0f));
		float sZ(sin(someEulerAngles.z / 2.0f));
		Quaternion qX(sX, 0.0f, 0.0f, cX);
		Quaternion qY(0.0f, sY, 0.0f, cY);
		Quaternion qZ(0.0f, 0.0f, sZ, cZ);
		Quaternion q = (qY * qX) * qZ;
		return q;
	}
	void QuaternionToMatrix(const Quaternion& q, Matrix3x3& m) {
		float x = q.x * 2.0f;
		float y = q.y * 2.0f;
		float z = q.z * 2.0f;
		float xx = q.x * x;
		float yy = q.y * y;
		float zz = q.z * z;
		float xy = q.x * y;
		float xz = q.x * z;
		float yz = q.y * z;
		float wx = q.w * x;
		float wy = q.w * y;
		float wz = q.w * z;
		m.mData[0] = 1.0f - (yy + zz);
		m.mData[1] = xy + wz;
		m.mData[2] = xz - wy;
		m.mData[3] = xy - wz;
		m.mData[4] = 1.0f - (xx + zz);
		m.mData[5] = yz + wx;
		m.mData[6] = xz + wy;
		m.mData[7] = yz - wx;
		m.mData[8] = 1.0f - (xx + yy);
	}
}