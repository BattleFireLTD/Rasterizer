#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Alice.h"
#include "utils.h"
extern ATexture*ptr;
LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	//register window
	WNDCLASSEX wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = NULL;
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndclass.hIcon = NULL;
	wndclass.hIconSm = NULL;
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = GLWindowProc;
	wndclass.lpszClassName = L"AWindow";
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	ATOM atom = RegisterClassEx(&wndclass);
	if (!atom)
	{
		return 0;
	}
	RECT rect = { 0,0,1280,720 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	int viewportWidth = rect.right - rect.left;
	int viewportHeight = rect.bottom - rect.top;
	//create window
	HWND hwnd = CreateWindowEx(NULL, L"AWindow", L"Alice Rendering Window", WS_OVERLAPPEDWINDOW,
		100, 100, viewportWidth, viewportHeight, NULL, NULL, hInstance, NULL);
	viewportWidth = 1280;
	viewportHeight = 720;
	//show window
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	HDC dc = GetDC(hwnd); 
	HARC rc=aCreateRenderContext(dc);
	aMakeCurrent(dc, rc);
	aViewport(0, 0, viewportWidth, viewportHeight);
	aClearColor(41, 77, 121, 255);
	aPersperctive(60.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
	int filesize = 0;
	unsigned char * filecontent = LoadFileContent("Res/test.bmp", filesize);
	ATexture texture;
	texture.mPixel = DecodeBMP(filecontent, texture.mWidth, texture.mHeight);
	aBindTexture(&texture);
	ptr = &texture;
	MSG msg;
	aClear(A_COLOR_BUFFER_BIT);
	aEnable(A_BLEND);
	aBegin(A_TRIANGLES);
	aColor4ub(200, 0, 0, 255);
	aVertex3f(-0.2f-0.4f, -0.2f, -10.6f);
	aColor4ub(0, 0, 200, 255);
	aVertex3f(0.2f - 0.4f, -0.2f, -0.6f);
	aColor4ub(0, 200, 0, 255);
	aVertex3f(0.0f -0.4f , 0.2f, -0.6f);

	aColor4ub(200, 0, 0, 255);
	aVertex3f(-0.2f + 0.25f, -0.2f, -0.6f);
	aColor4ub(0, 0, 200, 255);
	aVertex3f(0.2f + 0.2f, -0.2f, -0.6f);
	aColor4ub(0, 200, 0, 255);
	aVertex3f(0.0f + 0.2f, 0.2f, -0.6f);
	aEnd();
	aSwapBuffers(dc);
	while (true)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		/*aBegin(A_LINES);
		aColor4ub(255, 255, 255, 255);
		aVertex3f(-0.2f, -0.2f, -0.6f);
		aColor4ub(255, 0, 255, 255);
		aVertex3f(0.2f, -0.2f, -0.6f);

		aColor4ub(255, 255, 255, 255);
		aVertex3f(-0.2f, -0.2f, -0.6f);
		aColor4ub(255, 0, 255, 255);
		aVertex3f(0.0f, 0.2f, -0.6f);

		aColor4ub(255, 0, 255, 255);
		aVertex3f(0.0f, 0.2f, -0.6f);
		aColor4ub(255, 0, 255, 255);
		aVertex3f(0.2f, -0.2f, -0.6f);
		aEnd();*/
		Sleep(15);
	}
	return 0;
}