#include "ServerWindow.h"
#include "../include/Data.h"
LRESULT ServerWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (uMsg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(m_hwnd, &ps);
		EndPaint(m_hwnd, &ps);
		break;
	}
	case WM_TIMER:
	{
		int dataLen = 0;
		//char *data = GetScreenData(&dataLen);
		//pServer->SendAllData(data, dataLen);
		//delete[]data;
		break;
	}
	default:
		break;
	}

	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
	return LRESULT();
}

char * ServerWindow::GetScreenData(int * dataLen)
{
	HDC hdcScreen = GetDC(NULL);
	HDC hdcMemDC = CreateCompatibleDC(hdcScreen);
	//RECT rcClient;
	//GetClientRect(NULL,&rcClient);
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);
	SelectObject(hdcMemDC, hbmScreen);
	char* data = nullptr;

	if (!BitBlt(hdcMemDC,
		0, 0,
		width, height,
		hdcScreen,
		0, 0,
		SRCCOPY))
	{
		*dataLen = 0;
		return data;
	}
	BITMAP bmpScreen;
	GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

	BITMAPINFOHEADER   bi;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bmpScreen.bmWidth;
	bi.biHeight = bmpScreen.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

	HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
	char *lpbitmap = (char *)GlobalLock(hDIB);

	// Gets the "bits" from the bitmap and copies them into a buffer 
	// which is pointed to by lpbitmap.
	GetDIBits(hdcScreen, hbmScreen, 0,
		(UINT)bmpScreen.bmHeight,
		lpbitmap,
		(BITMAPINFO *)&bi, DIB_RGB_COLORS);

	int headSize = sizeof(DataHead);
	int infoSize = sizeof(BITMAPINFOHEADER);
	int totalSize = headSize + infoSize + dwBmpSize;

	DataHead head;
	head.m_type = DataHead::Image;
	head.m_dataSize = infoSize + dwBmpSize;

	data = new char[totalSize];
	memmove(data, &head, headSize);
	memmove(data + headSize, &bi, infoSize);
	memmove(data + headSize + infoSize, lpbitmap, dwBmpSize);
	*dataLen = totalSize;


	GlobalUnlock(hDIB);
	GlobalFree(hDIB);
	DeleteObject(hbmScreen);
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);

	return data;
}

int main()
{
	return 0;
}