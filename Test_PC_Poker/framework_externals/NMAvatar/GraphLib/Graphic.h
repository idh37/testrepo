
#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__


#include <windows.h>
#include <stdio.h>
#include <memory.h>

#include "GraphMod.h"
#include "xSprite.h"

// 스프라이트 출력효과 설정 플레그
#define LR		((DWORD)1)		// 0000000000000001(b) (좌우반전)
#define UD		((DWORD)2)		// 0000000000000010(b) (상하반전)
#define HB		((DWORD)4)		// 0000000000000100(b) (반투명효과)
#define GB		((DWORD)8)		// 0000000000001000(b) (다단계 반투명효과)
#define GLOW	((DWORD)16)		// 0000000000010000(b) (글로우필터효과)
#define ANTI	((DWORD)32)		// 0000000000100000(b) (안티효과)
#define LE		((DWORD)64)		// 0000000001000000(b) (광원효과)

#define RXY		((DWORD)128)	// 0000000010000000(b) (상대좌표 적용)
#define CENTER	((DWORD)256)	// 0000000100000000(b) (확대시 캐릭터를 좌표의 중심에 위치시킨다)
#define PERCENT	((DWORD)512)	// 0000001000000000(b) (확대시 비율에 따른 확대)
#define MIRROR ((DWORD)1024)	// 0000010000000000(b) (좌우 뒤집기-LR과 달리 상대좌표도 함께 반전)
#define TEMPCLIP ((DWORD)2048)	// 0000100000000000(b) (임시 클리핑 영역을 사용함)



class CPage
{
public:
	LPBITMAPINFO lpBmpInfo;     // 비트맵 정보
	LPLOGPALETTE lpLogPal;      // 로그 파레트 포인터
	HPALETTE	 hPal;	        // DIB형식의 팔레트 핸들
	HBITMAP      hBmp;			// 비트맵의 핸들(CreateDIBSection()으로 생성된 핸들)

	WORD* lpBit;                // 페이지 포인터
	LONG  lPitch;               // PITCH(4의배수) - 실제의 페이지의 가로 사이즈
	LONG  lPitchHalf;           // lPitch/2
	LONG  width, height;        // 페이지의 유효 범위 길이(최대 클리핑 지역)
	WORD  nBitNum;              // 비트수
	LONG  nBPD;                 // 도트당 바이트 수
	WORD  nColorNum;            // 칼라수
	
	CPage();
	~CPage();

	// 클리핑 영역
	int _cx1, _cy1, _cx2, _cy2;				// 클리핑 영역
	int TClipX1, TClipY1, TClipX2, TClipY2;	// 임시 클리핑 영역


	void SetClipArea(int x1, int y1, int x2, int y2);
	void SetFullClip();
	void SetTempClip(int x1, int y1, int x2, int y2);

	// 미러 찍기 정보
	// 2009.02.27 미러 모드 오류 수정 By 허재민
	//int MirrorX;
	//void SetMirrorX(int x);
	int m_iMirrorXBase;

	/**
	@brif - 미러 모드 찍기 설정 함수
	@param[in] x - 미러축이 될 중심 좌표.
	@example SetMirrorXEx(xp + (pRes->width/2));
	*/
	void SetMirrorX(int x);
	/**
	@brif - 미러 모드 찍기 설정 함수 (이미지 너비가 홀수 일 경우 중심점 오차제거 버전.)
	@param[in] xp - 이미지가 출력될 상대 기준 좌표.
	@param[in] iImageWidth - 이미지의 너비
	@remark 해당 함수를 호출할 경우, 해당 이미지의 너비의 가운데 중심이 미러축이 되어 출력된다.
	@example SetMirrorXEx(xp, pRes->width);
	*/
	void SetMirrorXEx(int xp, int iImageWidth);



	// Attach, Detach
	BOOL bAttached;
	//BOOL Attach(LONG xl, LONG yl, LPBITMAPINFO lpbmpinfo, WORD *lpbit, HBITMAP hbmp);
	BOOL Attach(LONG xl, LONG yl, void *lpbit, LONG lpitch);
	BOOL Detach();

	
	///////// 페이지 관련

	BOOL Init(LONG xl, LONG yl, INT bit, RGBQUAD *lpPal=NULL);
	void Destroy();
	BOOL ReCreateDIBSection();
	BOOL SetPal(RGBQUAD *lpPal);
	BOOL SetLogPal(LPLOGPALETTE lpPal);
	BOOL SetPalHandle(HPALETTE h);
	BOOL PutFromPage(LONG xp, LONG yp, CPage *pPage);
	BOOL PutFromPage(LONG xp, LONG yp, LONG ww, LONG hh, CPage *pPage);
	BOOL SmoothZoomFromPage(LONG xp, LONG yp, CPage *pPage);
	BOOL FillGDI(HDC hTarDC, int nXStart, int nYStart, COLORREF crColor);
	BOOL FlipToGDI(HWND hwnd, LONG xp=0, LONG yp=0, int ZoomX=1);
	BOOL FlipToGDI(HDC hdc, LONG xp=0, LONG yp=0, int ZoomX=1);
	void SetFlipPos(LONG xp, LONG yp, int zoom);
	BOOL Flip(HWND hwnd);
	BOOL Flip(HDC hdc);
	BOOL StretchFlip(HWND hwnd, RECT* rcDest, RECT* rcSrc);

	///////// 페이지 정보관련

	int  xpFlip, ypFlip;
	int  iZoomX;

	void ToPagePos(LONG xp, LONG yp, LONG* X, LONG* Y);

	///////// 드로잉 관련
	void HLine(int x, int y, int len, WORD color, BOOL flag);
	void VLine(int x, int y, int len, WORD color, BOOL flag);
	void DrawEmptyBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag=0);
	void DrawFillBox(int x1, int y1, int x2, int y2, WORD color);
	void DrawFillBoxGB(int x1, int y1, int x2, int y2, WORD color, int grade);
	void DrawSelectBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag=0);

	///////// 스프라이트 관련

	BOOL PutSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int opt=NULL, int grade=NULL , WORD color=NULL, int TargetPixelFormat = 555);
	BOOL PutSprAuto(int x, int y, SPRITE30 *pSpr, int opt=NULL, int grade=NULL , WORD color=NULL, int TargetPixelFormat = 555);
	BOOL ZoomSprAuto(int x, int y, SPRITE30 *pSpr, int tx, int ty, int opt=NULL, int grade=NULL, WORD color=NULL);
	BOOL PutImageAuto(int x, int y, SPRITE30 *pSpr, int opt=NULL, int grade=NULL, WORD color=NULL);

};


#endif