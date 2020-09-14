#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "AliceVector4.h"
#define A_COLOR_BUFFER_BIT 0x01
#define A_DEPTH_BUFFER_BIT 0x02
#define A_POINTS 0x01
#define A_LINES 0x02
#define A_TRIANGLES 0x03
#define A_BLEND 0x01
#define A_DEPTH_TEST 0x02
//blend option
#define A_SRC_ALPHA 0x01
#define A_ONE_MINUS_SRC_ALPHA 0x10
typedef unsigned int Auint;
typedef unsigned char Abyte;
typedef unsigned short Aushort;
typedef void* AHANDLE;
typedef HANDLE HARC;
struct OnePoint {
	Alice::Vector4 mPosition;
	Abyte mColor[4];
	float u,v;
	OnePoint(float x, float y, float z,float w, Abyte r, Abyte g, Abyte b, Abyte a):mPosition(x,y,z,w) {
		mColor[0] = r; mColor[1] = g; mColor[2] = b; mColor[3] = a;
	}
};
struct ATexture{
	unsigned char * mPixel;
	int mWidth;
	int mHeight;
};
HARC aCreateRenderContext(HDC dc);
int aMakeCurrent(HDC dc, HARC rc);
void aSwapBuffers(HDC dc);
void aViewport(int x, int y, int width, int height);
void aPersperctive(float fov, float aspect, float n, float f);

void aClearColor(Abyte r,Abyte g,Abyte b,Abyte a);
void aClear(Auint mask);
void SetPixel(int x, int y, Abyte r, Abyte g, Abyte b, Abyte a);
void aBindTexture(ATexture*texture);
Auint MakeColor(Abyte r, Abyte g, Abyte b, Abyte a);
void aEnable(Auint state);
void aDisable(Auint state);
void aBegin(int primitive);
void aEnd();
void aColor4ub(Abyte r, Abyte g, Abyte b, Abyte a);
void aVertex3f(float x,float y,float z);