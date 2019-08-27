
#pragma once

#include "GraphicGDIDefine.h"
#include "ddraw.h"
#include "wchar.h"

namespace NMBASE
{
	namespace GRAPHICGDI
	{

		class NMGRAPHICGDI_CLASS CDisplay
		{
		public:
			LPDIRECTDRAW2        lpDD;
			LPDIRECTDRAWSURFACE3 lpPrimary;
			//LPDIRECTDRAWSURFACE3 lpBack;

			///// 내부적 변수
			HWND winHandle;
			BOOL DDRAWINIT;		// 다이렉트 드로우 초기화 성공 여부

			CDisplay();
			~CDisplay();
			BOOL InitDirectDraw(HWND hwnd, int xl, int yl, int cBit);
			void CloseDirectDraw();

			LPSTR LockPrimary(DWORD *lPitch);
			void UnlockPrimary();
		};

	}//namespace GRAPHICGDI

}//namespace NMBASE

