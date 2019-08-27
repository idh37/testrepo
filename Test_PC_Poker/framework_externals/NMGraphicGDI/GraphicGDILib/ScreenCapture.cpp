#include "stdafx.h"
#include "ScreenCapture.h"
#include "Graphic.h"
#include <mmsystem.h>

namespace NMBASE
{
	namespace GRAPHICGDI
	{

		// 스크린샷 저장에 대한것을 새로 작성한다. 
		BOOL ScreenCapture(HWND hwnd, LPCTSTR fname)
		{
			if(!hwnd) 
			{
				return FALSE;
			}

			// 마지막에 캡춰한 시간을 기록하여 너무 잦은 캡춰를 막는다
			static DWORD dwLastCapture = timeGetTime()-100;
			DWORD dwNowTick = timeGetTime();
			if(dwNowTick - dwLastCapture < 50) 
			{
				return FALSE;
			}

			CRect rc;
			GetWindowRect(hwnd, rc);

			if(rc.Width()==0 || rc.Height()==0)
			{
				return FALSE;
			}

			CPage page;
			page.Init(rc.Width(), rc.Height(), 16);

			// 스크린 DC를 읽어온다
			HDC hdc = ::GetDC(NULL); 
			if(!hdc) 
			{
				return FALSE;
			}

			HDC hdcMem = ::CreateCompatibleDC(hdc);
			HGDIOBJ hOldBmp = ::SelectObject(hdcMem, page.hBmp);
			::BitBlt(hdcMem, 0, 0, rc.Width(), rc.Height(), hdc, rc.left, rc.top, SRCCOPY);

			COleDateTime time = COleDateTime::GetCurrentTime();
			CString fullpath;

			// 내문서 폴더 얻기
			char lpszPath[MAX_PATH] = {0,};
			SHGetSpecialFolderPath(hwnd, lpszPath, CSIDL_PERSONAL, FALSE);
			fullpath = lpszPath;
			fullpath += "\\";
			fullpath += fname;

			// 스크린 캡춰된 이미지를 JPG로 저장
			if(!page.SaveToJpeg((char*)(LPCTSTR)fullpath, 88))
			{
				return FALSE;
			}

			::SelectObject(hdcMem, hOldBmp);
			::ReleaseDC(NULL, hdc);
			::DeleteDC(hdcMem);

			dwLastCapture = timeGetTime();

			return TRUE;
		}

	}//namespace GRAPHICGDI

}//namespace NMBASE
