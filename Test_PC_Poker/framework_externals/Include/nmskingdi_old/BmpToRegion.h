// 비트맵의 외각선을 따서 윈도우 모양을 생성시켜 준다


/* // 사용 예

BITMAP bm;
GetObject(background, sizeof(bm), &bm); // BMP의 가로 세로 사이즈를 얻는데만 씀
hwnd = CreateWindowEx(WS_EX_TOPMOST,szAppName,szAppName,WS_POPUP | WS_OVERLAPPED,0,0,bm.bmWidth,bm.bmHeight,NULL,NULL, hInst, NULL);

// API
HRGN h = BitmapToRegion(background);
if(h) SetWindowRgn(hwnd,h,TRUE);

// MFC
HRGN h = BitmapToRegion(Back.operator HBITMAP(), RGB(255,0,255));
if(h) SetWindowRgn(h,TRUE);

// 할당받은 HRGN 핸들은 윈도우가 종료되기전에 DeleteObject()로 해제해야한다


*/

#ifndef __BMPTOREGION_H__
#define __BMPTOREGION_H__

#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{
		NMSKINGDI_API HRGN BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor = 0, COLORREF cTolerance = 0x101010);
		NMSKINGDI_API BOOL WindowResize2Bitmap(HWND hwnd, HGDIOBJ hbmp);
		NMSKINGDI_API BOOL WindowResize2Bitmap(HWND hwnd, LPCTSTR bmpfilename);

		class NMSKINGDI_CLASS CBmpRegion
		{
		public:
			HRGN hRgn;

			CBmpRegion() 
			{
				hRgn = NULL;
			}

			~CBmpRegion() 
			{
				if(hRgn) ::DeleteObject(hRgn);
				hRgn = NULL;
			}

			BOOL SetWindowRgn(HWND hwnd, HBITMAP hbmp,  COLORREF cTransColor=0, COLORREF cTolerance=RGB(16,16,16));
		};

	}//namespace SKINGDI

}//namespace NMBASE


#endif