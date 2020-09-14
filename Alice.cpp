#include "Alice.h"
#include "AliceFragmentOperation.h"
#include "AliceRasterization.h"
#include <stdio.h>
#include <unordered_map>
#include "AliceMatrix4x4.h"
#include "AliceVector3.h"
using namespace Alice;
static HDC sBKGDC;
static HBITMAP sBKGBMP;
struct RenderContext
{
	HDC mDC;
	//color buffer
	HBITMAP mColorBitmap;
	Abyte*mColorBuffer;
	int mColorBufferWidth, mColorBufferHeight;
	//depth buffer
	Aushort*mDepthBuffer;
	//stencil buffer
	//acc buffer
	int mCurrentPrimitive;
	bool mbEnableBlend;
	bool mbEnableDepthTest;
	Abyte mColor[4];
	Auint mSRCBlendOption, mDSTBlendOption;
	Matrix4x4 mProjectionMatrix;
	Matrix4x4 mViewMatrix;
	std::vector<OnePoint> mPoints;
	ATexture *mCurrentTexture;
};

static std::unordered_map<HDC,RenderContext*> sDC2RCMap;
static RenderContext* sCurrentRC;
static HDC sCurrentDC;
static int sViewportWidth = 0, sViewportHeight = 0;
static Auint sClearColor = 0;

HARC aCreateRenderContext(HDC dc){
	HBITMAP hBmp = (HBITMAP)GetCurrentObject(dc, OBJ_BITMAP);
	BITMAP bmp;
	GetObject(hBmp, sizeof(bmp), &bmp);
	RenderContext*rc = new RenderContext;
	rc->mDC = CreateCompatibleDC(dc);
	rc->mColorBitmap = nullptr;
	rc->mCurrentPrimitive = 0;
	rc->mbEnableBlend = false;
	rc->mbEnableDepthTest = false;
	rc->mColor[0] = 255;
	rc->mColor[1] = 255;
	rc->mColor[2] = 255;
	rc->mColor[3] = 255;
	rc->mSRCBlendOption = A_SRC_ALPHA;
	rc->mDSTBlendOption = A_ONE_MINUS_SRC_ALPHA;
	sDC2RCMap.insert(std::pair<HDC, RenderContext*>(dc, rc));
	return rc;
}

int aMakeCurrent(HDC dc, HARC rc)
{
	if (sDC2RCMap.find(dc)!= sDC2RCMap.end()){
		sCurrentDC = dc;
		sCurrentRC = sDC2RCMap[dc];
	}
	return 0;
}

void aViewport(int x, int y, int width, int height){
	sViewportWidth = width;
	sViewportHeight = height;
	BITMAPINFO	bmpInfor;
	bmpInfor.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfor.bmiHeader.biWidth = width;
	bmpInfor.bmiHeader.biHeight = height;
	bmpInfor.bmiHeader.biPlanes = 1;
	bmpInfor.bmiHeader.biBitCount = 32;
	bmpInfor.bmiHeader.biCompression = BI_RGB;
	bmpInfor.bmiHeader.biSizeImage = 0;
	bmpInfor.bmiHeader.biXPelsPerMeter = 0;
	bmpInfor.bmiHeader.biYPelsPerMeter = 0;
	bmpInfor.bmiHeader.biClrUsed = 0;
	bmpInfor.bmiHeader.biClrImportant = 0;

	sCurrentRC->mColorBufferWidth = width;
	sCurrentRC->mColorBufferHeight = height;
	if (sCurrentRC->mColorBitmap!=nullptr)
	{
		DeleteObject(sCurrentRC->mColorBitmap);
		delete[] sCurrentRC->mDepthBuffer;
	}
	sCurrentRC->mColorBitmap = CreateDIBSection(sCurrentDC, &bmpInfor, DIB_RGB_COLORS, (void**)&sCurrentRC->mColorBuffer, 0, 0);
	SelectObject(sCurrentRC->mDC, sCurrentRC->mColorBitmap);
	sCurrentRC->mDepthBuffer = new Aushort[width*height];
}
void aPersperctive(float fov, float aspect, float n, float f) {
	sCurrentRC->mProjectionMatrix.Perspective(fov, aspect, n, f);
}

Auint MakeColor(Abyte r, Abyte g, Abyte b, Abyte a) {
	return (a << 24) + (r << 16) + (g << 8) + b;
}
void SetPixel(int x, int y, Abyte r,Abyte g,Abyte b,Abyte a) {
	if (x < 0) { return; }
	if (y < 0) { return; }
	if (x > sCurrentRC->mColorBufferWidth-1) { return; }
	if (y > sCurrentRC->mColorBufferHeight-1) { return; }
	Auint*pixel = (Auint*)sCurrentRC->mColorBuffer;
	if (sCurrentRC->mbEnableBlend){
		Auint dst_color=pixel[x + y * sCurrentRC->mColorBufferWidth];
		Abyte dst_r = ((dst_color << 8) >> 24);
		Abyte dst_g = ((dst_color << 16) >> 24);
		Abyte dst_b = ((dst_color << 24) >> 24);
		Abyte dst_a = dst_color>> 24;
		pixel[x + y * sCurrentRC->mColorBufferWidth] = BlendColor(r, g, b, a,sCurrentRC->mSRCBlendOption,
			dst_r,dst_g,dst_b,dst_a,sCurrentRC->mDSTBlendOption);
	}
	else {
		pixel[x + y * sCurrentRC->mColorBufferWidth] = MakeColor(r, g, b, a);
	}
}

void aClearColor(Abyte r, Abyte g, Abyte b, Abyte a){
	sClearColor = (a << 24)+(r << 16)+(g<<8)+b;
}
void aBindTexture(ATexture*texture) {
	sCurrentRC->mCurrentTexture = texture;
}
void aClear(Auint mask){
	if ((mask&A_COLOR_BUFFER_BIT)==A_COLOR_BUFFER_BIT){
		int nPixelCount = sViewportWidth*sViewportHeight;
		Auint*pixel = (Auint*)sCurrentRC->mColorBuffer;
		for (int i = 0; i < nPixelCount; ++i) {
			pixel[i] = sClearColor;
		}
	}
}

void aSwapBuffers(HDC dc)
{
	BitBlt(dc, 0, 0, sViewportWidth, sViewportHeight, sCurrentRC->mDC, 0, 0, SRCCOPY);
}
void aColor4ub(Abyte r, Abyte g, Abyte b, Abyte a) {
	sCurrentRC->mColor[0] = r;
	sCurrentRC->mColor[1] = g;
	sCurrentRC->mColor[2] = b;
	sCurrentRC->mColor[3] = a;
}
void aEnable(Auint state) {
	if (state==A_BLEND){
		sCurrentRC->mbEnableBlend = true;
	}
	else if (state == A_DEPTH_TEST) {
		sCurrentRC->mbEnableDepthTest = true;
	}
}
void aDisable(Auint state) {
	if (state == A_BLEND) {
		sCurrentRC->mbEnableBlend = false;
	}
	else if (state == A_DEPTH_TEST) {
		sCurrentRC->mbEnableDepthTest = false;
	}
}
void aBegin(int primitive) {
	sCurrentRC->mCurrentPrimitive = primitive;
}
void aEnd() {
	sCurrentRC->mCurrentPrimitive = 0;
}
static OnePoint RasterPoint(float x, float y, float z) {
	float vo[] = { x,y,z,1.0f };
	float vx[4];
	Vector4MultiplyMatrix4x4(vo, sCurrentRC->mProjectionMatrix, vx);
	vx[0] /= vx[3];
	vx[1] /= vx[3];
	vx[2] /= vx[3];
	float view_x = vx[0] * sCurrentRC->mColorBufferWidth/2 + sCurrentRC->mColorBufferWidth / 2;
	float view_y = vx[1] * sCurrentRC->mColorBufferHeight/2 + sCurrentRC->mColorBufferHeight / 2;
	return OnePoint(view_x, view_y, vx[2],vx[3], sCurrentRC->mColor[0], sCurrentRC->mColor[1], sCurrentRC->mColor[2], sCurrentRC->mColor[3]);
}
void aVertex3f(float x, float y, float z){
	if (sCurrentRC->mCurrentPrimitive==A_POINTS){
		OnePoint p = RasterPoint(x, y, z);
		SetPixel(int(p.mPosition.x), int(p.mPosition.y), p.mColor[0], p.mColor[1], p.mColor[2], p.mColor[3]);
	}
	else if (sCurrentRC->mCurrentPrimitive == A_LINES) {
		if (sCurrentRC->mPoints.size()==0){
			OnePoint point=RasterPoint(x,y,z);
			sCurrentRC->mPoints.push_back(point);
		}
		else {
			OnePoint point = RasterPoint(x, y, z);
			DrawLine(sCurrentRC->mPoints[0], point);
			sCurrentRC->mPoints.clear();
		}
	}else if (sCurrentRC->mCurrentPrimitive==A_TRIANGLES){
		if (sCurrentRC->mPoints.size() != 2) {
			OnePoint point = RasterPoint(x, y, z);
			sCurrentRC->mPoints.push_back(point);
		}
		else {
			OnePoint point = RasterPoint(x, y, z);
			DrawTriangle(sCurrentRC->mPoints[0], sCurrentRC->mPoints[1], point);
			sCurrentRC->mPoints.clear();
		}
	}
}