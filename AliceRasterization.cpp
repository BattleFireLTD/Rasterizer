#include "AliceRasterization.h"
#include "AliceVector3.h"
void DrawLine(OnePoint &p0, OnePoint &p1) {
	int i = int(p0.mPosition.x);
	int end_x = int(p1.mPosition.x);
	float deltaX = p1.mPosition.x - p0.mPosition.x;
	float k = (p1.mPosition.y - p0.mPosition.y) / (p1.mPosition.x - p0.mPosition.x);
	for (; i < end_x; ++i) {
		float t = (i - p0.mPosition.x) / deltaX;
		float y = p0.mPosition.y + k * (i - p0.mPosition.x);
		float z0_inverse = 1.0f / p0.mPosition.z;
		float z1_inverse = 1.0f / p1.mPosition.z;
		float z_inverse = z0_inverse * (1.0f - t) + z1_inverse*t;
		float target_z = 1.0f / z_inverse;
		if (target_z > -1.0f&&target_z <= 1.0) {
			Abyte target_r = Abyte(target_z * (float(p0.mColor[0]) / 255.0f * z0_inverse*(1.0f - t) + float(p1.mColor[0]) / 255.0f * z1_inverse*t)*255.0f);
			Abyte target_g = Abyte(target_z * (float(p0.mColor[1]) / 255.0f * z0_inverse*(1.0f - t) + float(p1.mColor[1]) / 255.0f * z1_inverse*t)*255.0f);
			Abyte target_b = Abyte(target_z * (float(p0.mColor[2]) / 255.0f * z0_inverse*(1.0f - t) + float(p1.mColor[2]) / 255.0f * z1_inverse*t)*255.0f);
			Abyte target_a = Abyte(target_z * (float(p0.mColor[3]) / 255.0f * z0_inverse*(1.0f - t) + float(p1.mColor[3]) / 255.0f * z1_inverse*t)*255.0f);
			target_r = target_r > 255 ? 255 : target_r;
			target_g = target_g > 255 ? 255 : target_g;
			target_b = target_b > 255 ? 255 : target_b;
			target_a = target_a > 255 ? 255 : target_a;
			SetPixel(i, int(y), target_r, target_g, target_b, target_a);
		}
	}
}
#define ALICE_MIN(a,b) (a>b?b:a)
#define ALICE_MAX(a,b) (a>b?a:b)
bool rayTriangleIntersect(
	Alice::Vector3 &orig, Alice::Vector3 &dir,
	Alice::Vector3 &v0, Alice::Vector3 &v1, Alice::Vector3 &v2,
	float &t, float &u, float &v)
{
	// compute plane's normal
	Alice::Vector3 v0v1 = v1 - v0;
	Alice::Vector3 v0v2 = v2 - v0;
	// no need to normalize
	Alice::Vector3 N = v0v1^(v0v2); // N 
	float area2 = N.magnitude();

	// Step 1: finding P

	// check if ray and plane are parallel ?
	float NdotRayDirection = N*(dir);
	if (fabs(NdotRayDirection) < 0.000001f) // almost 0 
		return false; // they are parallel so they don't intersect ! 

	// compute d parameter using equation 2
	float d = N*(v0);

	// compute t (equation 3)
	t = (N*(orig) + d) / NdotRayDirection;
	// check if the triangle is in behind the ray
	//if (t < 0) return false; // the triangle is behind 

	// compute the intersection point using equation 1
	Alice::Vector3 P = orig + t * dir;

	// Step 2: inside-outside test
	Alice::Vector3 C; // vector perpendicular to triangle's plane 

	// edge 0
	Alice::Vector3 edge0 = v1 - v0;
	Alice::Vector3 vp0 = P - v0;
	C = edge0^vp0;
	if (N*C < 0) return false; // P is on the right side 

	// edge 1
	Alice::Vector3 edge1 = v2 - v1;
	Alice::Vector3 vp1 = P - v1;
	C = edge1^vp1;
	u = C.magnitude() / area2;
	if (N*C < 0)  return false; // P is on the right side 

	// edge 2
	Alice::Vector3 edge2 = v0 - v2;
	Alice::Vector3 vp2 = P - v2;
	C = edge2^vp2;
	v = C.magnitude() / area2;
	if (N*C < 0) return false; // P is on the right side; 
	return true; // this ray hits the triangle 
}
ATexture*ptr;
void GetTextureColor(float u, float v, Abyte &r, Abyte&g, Abyte&b, Abyte&a) {
	u = u > 1.0f ? 1.0f : u;
	v = v > 1.0f ? 1.0f : v;
	u = u < 0.0f ? 0.0f : u;
	v = v < 0.0f ? 0.0f : v;
	int row = (int)(v * ptr->mHeight);
	int col = (int)(u * ptr->mWidth);
	r = ptr->mPixel[(row*ptr->mWidth + col) * 3];
	g = ptr->mPixel[(row*ptr->mWidth + col) * 3+1];
	b = ptr->mPixel[(row*ptr->mWidth + col) * 3+2];
	a = 255;
}
void DrawTriangle(OnePoint &A, OnePoint &B, OnePoint &C) {
	//P = u * A + v * B + w * C
	/*
	    C
	A		B
	*/
	int xMin = (int)ALICE_MIN(ALICE_MIN(A.mPosition.x, B.mPosition.x), C.mPosition.x);
	int xMax = (int)ALICE_MAX(ALICE_MAX(A.mPosition.x, B .mPosition.x), C.mPosition.x);
	int yMin = (int)ALICE_MIN(ALICE_MIN(A.mPosition.y, B.mPosition.y), C.mPosition.y);
	int yMax = (int)ALICE_MAX(ALICE_MAX(A.mPosition.y, B.mPosition.y), C.mPosition.y);
	A.u = 0.0f; A.v = 0.0f;
	B.u = 1.0f; B.v = 0.0f;
	C.u = 0.5f; C.v = 1.0f;
	for (int y=yMin;y<yMax;++y){
		for (int x=xMin;x<xMax;++x){
			float t, u, v;
			if (rayTriangleIntersect(Alice::Vector3(x, y, 0.0f), Alice::Vector3(0, 0, -1.0f), Alice::Vector3(A.mPosition.x, A.mPosition.y, A.mPosition.z),
				Alice::Vector3(B.mPosition.x, B.mPosition.y, B.mPosition.z),
				Alice::Vector3(C.mPosition.x, C.mPosition.y, C.mPosition.z), t, u, v)) {
				float w = 1.0f - v - u;
				Abyte target_r = Abyte((u * (float(A.mColor[0]) / 255.0f) + v * (float(B.mColor[0]) / 255.0f) + float(C.mColor[0]) / 255.0f * w)*255.0f);
				Abyte target_g = Abyte((u * (float(A.mColor[1]) / 255.0f) + v * (float(B.mColor[1]) / 255.0f) + float(C.mColor[1]) / 255.0f * w)*255.0f);
				Abyte target_b = Abyte((u * (float(A.mColor[2]) / 255.0f) + v * (float(B.mColor[2]) / 255.0f) + float(C.mColor[2]) / 255.0f * w)*255.0f);
				Abyte target_a = Abyte((u * (float(A.mColor[3]) / 255.0f) + v * (float(B.mColor[3]) / 255.0f) + float(C.mColor[3]) / 255.0f * w)*255.0f);
				GetTextureColor(u * A.u + v * B.u + w * C.u, u * A.v + v * B.v + w * C.v, target_r, target_g, target_b, target_a);
				target_r = target_r > 255 ? 255 : target_r;
				target_g = target_g > 255 ? 255 : target_g;
				target_b = target_b > 255 ? 255 : target_b;
				target_a = target_a > 255 ? 255 : target_a;
				SetPixel(x, y, target_r, target_g, target_b, target_a);
			}
		}
	}
}