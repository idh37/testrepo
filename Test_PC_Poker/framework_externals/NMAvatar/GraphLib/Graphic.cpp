//
//                                   그래픽 처리 모듈
//
#include "StdAfx.h"
#include "Graphic.h"

//#include "GraphMod.cpp"
//#include "rsfman.cpp"
//#include "sprio.cpp"

CPage::CPage()
{
	lpBmpInfo = NULL;
	lpBit     = NULL;
	lPitch    = 0;
	lPitchHalf= 0;
	nBitNum   = 8;
	nBPD      = 1;
	nColorNum = 0;
	lpLogPal  = NULL;
	hPal      = NULL;
	hBmp      = NULL;

	////////////

	_cx1 = _cy1 = _cx2 = _cy2 = 0;

	TClipX1 = 0;
	TClipY1 = 0;
	TClipX2 = 0;
	TClipY2 = 0;

	m_iMirrorXBase = 0;

	////////////

	bAttached = FALSE;
	
	////////////

	xpFlip = 0;
	ypFlip = 0;
	iZoomX = 1;
}


CPage::~CPage()
{
	Destroy();
}

void CPage::Destroy()
{
	if(bAttached) Detach();

	nColorNum = 0;
	if(lpBmpInfo != NULL){ delete lpBmpInfo; lpBmpInfo=NULL;}
	//if(lpBit != NULL) { delete lpBit; lpBit=NULL; }
	if(lpLogPal) { free( lpLogPal );  lpLogPal=NULL; }
	if(hPal) { DeleteObject(hPal); hPal=NULL;}
	if(hBmp) { DeleteObject(hBmp); hBmp=NULL;}

}

void CPage::SetClipArea(int x1, int y1, int x2, int y2)
{
	_cx1 = min(max(0, x1), width-1);
	_cy1 = min(max(0, y1), height-1);
	_cx2 = min(max(0, x2), width-1);
	_cy2 = min(max(0, y2), height-1);
}

void CPage::SetFullClip()
{
	if(!lpBit) return;

	_cx1 = _cy1 = 0;
	_cx2 = width - 1;
	_cy2 = height - 1;
}

// 임시클리핑 영역을 설정한다
void CPage::SetTempClip(int x1, int y1, int x2, int y2)
{
	TClipX1 = min(max(0, x1), width-1);
	TClipY1 = min(max(0, y1), height-1);
	TClipX2 = min(max(0, x2), width-1);
	TClipY2 = min(max(0, y2), height-1);
}

// 미러찍기 기준 좌표를 설정한다
// 2009.02.27 미러 모드 오류 수정 By 허재민
void CPage::SetMirrorXEx(int iMultiXp, int iAdjust)
{
	m_iMirrorXBase = iMultiXp*2 + iAdjust;
}

void CPage::SetMirrorX(int x)
{
	// 2009.02.27 미러 모드 오류 수정 By 허재민
	SetMirrorXEx(x, 0);
	// MirrorX = x;
}

/*-----------------------------------------------------------------------------------------
   페이지를 초기화 한다
------------------------------------------------------------------------------------------*/
BOOL CPage::Init(LONG xl, LONG yl, INT bit, RGBQUAD *lpPal)
{
	if(bAttached) Detach();

	//if(lpBit != NULL) { delete lpBit; lpBit=NULL; }
	if(lpBmpInfo != NULL){ delete lpBmpInfo; lpBmpInfo=NULL;}
	if(hBmp) { DeleteObject(hBmp); hBmp=NULL;}
	
	switch(bit)
	{
	case 4  : nBPD=1; nColorNum=16;  break;
	case 8  : nBPD=1; nColorNum=256; break;
	case 16 : nBPD=2; nColorNum=0;   break;
	case 24 : nBPD=3; nColorNum=0;   break;
	default : return FALSE;
	}

	nBitNum = bit;
	width = xl;
	lPitch = xl*nBPD;
	height = yl;
	while(lPitch%4) lPitch++; // Pitch가 4의 배수가 되게 한다
	//lPitch/=2;                // 16비트 칼라여서 편법을 썼다.
	                          // 여기서의 lPitch란 바이트수이다(점의 개수는 아니다)
	lPitchHalf = lPitch/nBPD; // lPitch는 바이트수이므로 8,16,23비트 모드의 점의 개수를 얻는다.

	// 비트맵 인포 구조체를 위한 메모리 할당
	DWORD nAllocSize = sizeof(BITMAPINFO) + sizeof(RGBQUAD) * nColorNum;
	if(nColorNum != 0) nAllocSize -= sizeof(RGBQUAD);
	lpBmpInfo = (LPBITMAPINFO)new char[nAllocSize];
	if(lpBmpInfo == NULL) return FALSE;

	// 가상 페이지 생성(비트맵 이미지 포인터)
	//lpBit = (WORD*)new char[lPitch * yl];
	//if(lpBit == NULL) return FALSE;

	// 비트맵 인포 해더 생성
	lpBmpInfo->bmiHeader.biSize	         = sizeof (BITMAPINFOHEADER);
	lpBmpInfo->bmiHeader.biWidth         = width;      //width;
	lpBmpInfo->bmiHeader.biHeight        = -height;    // 음수면 그림이 뒤집혀 출력된다
	lpBmpInfo->bmiHeader.biPlanes        = 1;          // 항상1
	lpBmpInfo->bmiHeader.biBitCount	     = nBitNum;    // 도트당 비트수(16비트는 5:6:5)
	lpBmpInfo->bmiHeader.biCompression	 = 0;          // BI_RGB와 같다(비압축)
	lpBmpInfo->bmiHeader.biSizeImage     = lPitch*yl;  // lPitch; // 항상 4의배수
	lpBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	lpBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	lpBmpInfo->bmiHeader.biClrUsed	     = 0;          // 사용된 총 칼라수(항상 0으로 고정)
	lpBmpInfo->bmiHeader.biClrImportant	 = 0;          // 중요한 색상수(항상 0으로 고정)

	// DIB를 생성
	HWND ActiveWindow = GetActiveWindow();
	HDC ScreenDC = GetDC(ActiveWindow);
	hBmp = CreateDIBSection(ScreenDC, lpBmpInfo, DIB_RGB_COLORS, (VOID**)&lpBit, NULL, 0);
	ReleaseDC(ActiveWindow, ScreenDC);

	//InitGrpLib(xl, yl, lPitch/2, 555);
	InitGrpLib(555);
	InitAlphaTable(555);

	// 클리핑 구역 설정
	_cx1 = 0;
	_cy1 = 0;
	_cx2 = xl - 1;
	_cy2 = yl - 1;

	// 파레트 세팅
	if( nColorNum != 0) SetPal(lpPal);

	return TRUE;
}

/*
BOOL CPage::Attach(LONG xl, LONG yl, LPBITMAPINFO lpbmpinfo, WORD *lpbit, HBITMAP hbmp)
{
	if(lpBmpInfo) return FALSE;
	if(hBmp) return FALSE;
	if(bAttached) return FALSE;
	
	nBPD=2; nColorNum=0;
	nBitNum = 16;
	width = xl;
	lPitch = xl*nBPD;
	height = yl;
	while(lPitch%4) lPitch++; // Pitch가 4의 배수가 되게 한다
	lPitchHalf = lPitch/nBPD; // lPitch는 바이트수이므로 8,16,23비트 모드의 점의 개수를 얻는다.
	lpBmpInfo = lpbmpinfo;
	lpBit = lpbit;
	hBmp = hbmp;

	InitGrpLib(xl, yl, lPitch/2, 555);
	//InitAlphaTable(555);

	bAttached = TRUE;
	return TRUE;
}
*/

BOOL CPage::Attach(LONG xl, LONG yl, void *lpbit, LONG lpitch)
{
	if(lpBmpInfo) return FALSE;
	if(hBmp) return FALSE;
	if(bAttached) return FALSE;
	
	nBPD=2; nColorNum=0;
	nBitNum = 16;
	width = xl;
	lPitch = lpitch;
	height = yl;
	//while(lPitch%4) lPitch++; // Pitch가 4의 배수가 되게 한다
	lPitchHalf = lPitch/nBPD; // lPitch는 바이트수이므로 8,16,23비트 모드의 점의 개수를 얻는다.
	lpBmpInfo = NULL;
	lpBit = (WORD*)lpbit;

	//InitGrpLib(xl, yl, lPitch/2, 555);
	InitGrpLib(555);

	// 클리핑 구역 설정
	_cx1 = 0;
	_cy1 = 0;
	_cx2 = xl - 1;
	_cy2 = yl - 1;

	bAttached = TRUE;
	return TRUE;
}

BOOL CPage::Detach()
{
	if(!bAttached) return FALSE;

	lpBmpInfo = NULL;
	hBmp = NULL;
	lpBit = NULL;
	width = height = 0;
	lPitch = lPitchHalf = 0;
	bAttached = FALSE;

	return TRUE;
}

BOOL CPage::ReCreateDIBSection()
{
	if(hBmp) DeleteObject(hBmp);

	// DIB를 생성
	HWND ActiveWindow = GetActiveWindow();
	HDC ScreenDC = GetDC(ActiveWindow);
	hBmp = CreateDIBSection(ScreenDC, lpBmpInfo, DIB_RGB_COLORS, (VOID**)&lpBit, NULL, 0);
	ReleaseDC(ActiveWindow, ScreenDC);

	return (hBmp != NULL);
}

/*-----------------------------------------------------------------------------------------
   페이지의 파레트를 설정한다

   - 인수 : RGBQUAD의 포인터
------------------------------------------------------------------------------------------*/
BOOL CPage::SetPal(RGBQUAD *lpPal)
{
	if( nColorNum == 0)	return TRUE;        //파레트가 필요없다면

	if(lpLogPal) { free( lpLogPal );  lpLogPal=NULL; } // 메모리 해제
	lpLogPal = (LPLOGPALETTE)malloc( sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * nColorNum );

	lpLogPal->palVersion = 0x300;	        // 팔레트 버젼 입력
	lpLogPal->palNumEntries = nColorNum;    // 팔레트 색상수 입력
	
	int i;
	if(lpPal==NULL){             // 파레트 포인터가 NULL이면 기본 파레트를 생성한다
	for(i=0; i<nColorNum; i++)
	{
		lpLogPal->palPalEntry[i].peRed   = lpBmpInfo->bmiColors[i].rgbRed      = (BYTE)(((i>>5) & 0x07) * 255/7);
		lpLogPal->palPalEntry[i].peGreen = lpBmpInfo->bmiColors[i].rgbGreen    = (BYTE)(((i>>2) & 0x07) * 255/7);
		lpLogPal->palPalEntry[i].peBlue  = lpBmpInfo->bmiColors[i].rgbBlue     = (BYTE)(((i>>0) & 0x03) * 255/3);
		lpLogPal->palPalEntry[i].peFlags = lpBmpInfo->bmiColors[i].rgbReserved = (BYTE)0;
	}}
	else{
	for(i=0; i<nColorNum; i++)
	{
		lpLogPal->palPalEntry[i].peRed   = lpBmpInfo->bmiColors[i].rgbRed   = lpPal[i].rgbRed;
		lpLogPal->palPalEntry[i].peGreen = lpBmpInfo->bmiColors[i].rgbGreen = lpPal[i].rgbGreen;
		lpLogPal->palPalEntry[i].peBlue  = lpBmpInfo->bmiColors[i].rgbBlue  = lpPal[i].rgbBlue;
		lpLogPal->palPalEntry[i].peFlags = lpBmpInfo->bmiColors[i].rgbReserved = 0;
	}}

	if(hPal) { DeleteObject(hPal); hPal=NULL;}
	hPal = CreatePalette( lpLogPal );		
	
	if(hPal == NULL) return FALSE;
	return TRUE;
}

BOOL CPage::SetLogPal(LPLOGPALETTE lpPal)
{
	if( nColorNum == 0)	//파레트가 필요없다면
		return TRUE;
	
	if(lpLogPal!=NULL) { free( lpLogPal );  lpLogPal=NULL; } // 메모리 해제

	LONG size = sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * nColorNum;
	lpLogPal = (LPLOGPALETTE)malloc( size );

	memcpy(lpLogPal, lpPal, size);

	if(hPal) { DeleteObject(hPal); hPal=NULL;}
	hPal = CreatePalette( lpLogPal );		
	
	if(hPal == NULL) return FALSE;
	return TRUE;
}


BOOL CPage::SetPalHandle(HPALETTE h)
{
	if(h==NULL) return FALSE;
	if(hPal) { DeleteObject(hPal); hPal=NULL;}
	hPal = h;
	return TRUE;
}

/*-----------------------------------------------------------------------------------------
   GDI 표면을 특정 칼라로 채운다
------------------------------------------------------------------------------------------*/
BOOL CPage::FillGDI(HDC hTarDC, int nXStart, int nYStart, COLORREF crColor)
{
	BOOL check;
	check = FloodFill(hTarDC, nXStart, nYStart, crColor);
	if(check != 0)return TRUE;
	else return FALSE;
}

/*-----------------------------------------------------------------------------------------
   다른 페이지의 내용을 현재 페이지 표면에 뿌린다
------------------------------------------------------------------------------------------*/
BOOL CPage::PutFromPage(LONG xp, LONG yp, CPage *pPage)
{
	int w = pPage->lPitchHalf;	// 소스 페이지의 가로 사이즈가 아니라 Half-Pitch를 얻는다
	int h = pPage->height;
	if(lpBit == NULL) return FALSE;
	if(pPage->lpBit == NULL) return FALSE;
	if(w==0 || h==0) return FALSE;
	
	//InitGrpLib(555);
	SetFullClip();

	_PutImage(xp, yp, w, h, lpBit, pPage->lpBit, _cx1, _cy1, _cx2, _cy2, lPitchHalf);

	return TRUE;
}

/*-----------------------------------------------------------------------------------------
   다른 페이지의 내용을 현재 페이지 표면에 뿌린다
------------------------------------------------------------------------------------------*/
BOOL CPage::PutFromPage(LONG xp, LONG yp, LONG ww, LONG hh, CPage *pPage)
{
	int w = pPage->lPitchHalf;	// 소스 페이지의 가로 사이즈가 아니라 Half-Pitch를 얻는다
	int h = pPage->height;
	if(lpBit == NULL) return FALSE;
	if(pPage->lpBit == NULL) return FALSE;
	if(w==0 || h==0) return FALSE;

	if(xp < 0      ) xp = 0;
	if(xp > width-1) xp = width-1;
	if(yp < 0      ) yp = 0;
	if(yp > width-1) yp = height-1;
	if(xp + ww > width-1) ww = width-1;
	if(yp + hh > height-1) hh = height-1;
	
	//InitGrpLib(555);
	SetClipArea(xp, yp, xp+ww-1, yp+hh-1);
	
	_PutImage(xp, yp, w, h, lpBit, pPage->lpBit, _cx1, _cy1, _cx2, _cy2, lPitchHalf);
	
	SetFullClip();

	return TRUE;
}

/*-----------------------------------------------------------------------------------------
   다른 페이지의 내용을 현재 페이지 표면에 뿌린다(페이지 크기에 맞게 부드러운 확대/축소가 적용됨)
------------------------------------------------------------------------------------------*/
BOOL CPage::SmoothZoomFromPage(LONG xp, LONG yp, CPage *pPage)
{
	int w = pPage->lPitchHalf;	// 소스 페이지의 가로 사이즈가 아니라 Half-Pitch를 얻는다
	int h = pPage->height;
	if(lpBit == NULL) return FALSE;
	if(pPage->lpBit == NULL) return FALSE;
	if(w==0 || h==0) return FALSE;

	int tx = width;
	int ty = height;

	_ZoomImage_Smooth(xp, yp, w, h, lpBit, tx, ty, pPage->lpBit, _cx1, _cy1, _cx2, _cy2, lPitchHalf);

	return TRUE;
}

/*-----------------------------------------------------------------------------------------
   페이지가 뿌려질 클라이언트의 특정 좌표를 페이지의 좌표로 변환시킨다
------------------------------------------------------------------------------------------*/
void CPage::ToPagePos(LONG xp, LONG yp, LONG* X, LONG* Y)
{
	*X = (xp-xpFlip)/iZoomX;
	*Y = (yp-ypFlip)/iZoomX;
}

/*-----------------------------------------------------------------------------------------
    페이지를 플립할 GDI표면 위치(클라이언트 영역)을 설정한다
------------------------------------------------------------------------------------------*/
void CPage::SetFlipPos(LONG xp, LONG yp, int zoom)
{
	xpFlip = xp;
	ypFlip = yp;
	iZoomX = zoom;
}

/*-----------------------------------------------------------------------------------------
   페이지의 내용을 GDI 표면에 정수배로 뿌린다(SetPagePos()로 위치를 바꿀수 있다)
------------------------------------------------------------------------------------------*/
BOOL CPage::Flip(HWND hwnd)
{
	// 페이지가 초기화 되지 않았다면
	if(  lpBit == NULL ) return FALSE;
	HDC hDC = GetDC(hwnd);
	BOOL rtn = Flip(hDC);
	ReleaseDC(hwnd, hDC);
	return rtn;
}

BOOL CPage::Flip(HDC hdc)
{
	int rtn;

	// 페이지가 초기화 되지 않았다면
	if(  lpBit == NULL ) return FALSE;

	HDC hDC = hdc;

	if( nColorNum != 0 && hPal != NULL)	//파레트가 필요하다면
	{
		HPALETTE hOldPal;
		hOldPal = SelectPalette( hDC, hPal, TRUE ); //팔레트 선택
		if(hOldPal == NULL) return FALSE;
		rtn = RealizePalette( hDC );				 //팔레트 활성화
		if(rtn==GDI_ERROR) return FALSE;             // GDI_ERROR = -1
	}

	int xl = width * iZoomX;
	int yl = height * iZoomX;
	
	rtn = StretchDIBits( hDC,
				   xpFlip,
				   ypFlip,
				   xl,
				   yl,
				   0,
				   0,
				   width,
				   height,
				   lpBit,
				   lpBmpInfo,
				   DIB_RGB_COLORS,
				   SRCCOPY );

	if(rtn == GDI_ERROR) return FALSE;
	return TRUE;
}

/*-----------------------------------------------------------------------------------------
   페이지의 내용을 GDI 표면에 정수배 확대로 뿌린다(SetFlipPos()를 미리실행)
------------------------------------------------------------------------------------------*/
BOOL CPage::FlipToGDI( HDC hdc, LONG xp, LONG yp, int zoom)
{
	SetFlipPos(xp, yp, zoom);
	return Flip(hdc);
}

BOOL CPage::FlipToGDI( HWND hwnd, LONG xp, LONG yp, int zoom)
{
	SetFlipPos(xp, yp, zoom);
	return Flip(hwnd);
}

/*-----------------------------------------------------------------------------------------
   페이지의 내용을 GDI 표면에 Stretch로 뿌린다
   
   - 소스나 타겟의 RECT가 NULL이면 전체 영역으로 설정된다
------------------------------------------------------------------------------------------*/
BOOL CPage::StretchFlip(HWND hwnd, RECT* rcDest, RECT* rcSrc )
{
	int rtn;

	// 페이지가 초기화 되지 않았다면
	if(  lpBit == NULL ) return FALSE;

	HDC hDC = GetDC(hwnd);

	if( nColorNum != 0 && hPal != NULL)	//파레트가 필요하다면
	{
		HPALETTE hOldPal;
		hOldPal = SelectPalette( hDC, hPal, TRUE ); //팔레트 선택
		if(hOldPal == NULL) return FALSE;
		rtn = RealizePalette( hDC );				 //팔레트 활성화
		if(rtn==GDI_ERROR) return FALSE;             // GDI_ERROR = -1
	}

	//int xl = width * iZoomX;
	//int yl = height * iZoomX;
	
	int sx, sy, sxl, syl;
	int dx, dy, dxl, dyl;
	
	if(rcDest==NULL) 
	{ 
		RECT rect;
		GetClientRect(hwnd, &rect);
		dx=0; dy=0; dxl=rect.right-rect.left+1; dyl=rect.bottom-rect.top+1;
	}
	else
	{
		dx = rcDest->left; dy = rcDest->top;
		dxl = rcDest->right - rcDest->left + 1; dyl = rcDest->bottom - rcDest->top + 1;
	}

	if(rcSrc==NULL) 
	{ 
		sx=0; sy=0; sxl=width; syl=height;
	}
	else
	{
		sx = rcSrc->left; sy = rcSrc->top;
		sxl = rcSrc->right - rcSrc->left + 1; syl = rcSrc->bottom - rcSrc->top + 1;
	}

	sy = height-sy-syl; // 그림이 뒤집혀 있으므로 좌표를 뒤집는다

	rtn = StretchDIBits( hDC,
				   dx, dy, dxl, dyl,
				   sx, sy, sxl, syl,
				   lpBit,
				   lpBmpInfo,
				   DIB_RGB_COLORS,
				   SRCCOPY );

	ReleaseDC(hwnd, hDC);

	if(rtn == GDI_ERROR) return FALSE;
	return TRUE;
}


/////////////////////
//  드로잉 관련
/////////////////////

/*-----------------------------------------------------------------------------------------
   페이지에 수평선을 그린다(flag가 TRUE이면 XOR로 그린다)
------------------------------------------------------------------------------------------*/
void CPage::HLine(int x, int y, int len, WORD color, BOOL flag)
{
	if(lpBit==NULL) return;
	//InitGrpLib(555);

	if(flag==FALSE) DrawHLine( x, y, len, lpBit, color, _cx1, _cy1, _cx2, _cy2, lPitchHalf );    // 단색 칼라 드로잉
	else if(flag==TRUE) DrawHLineXOR( x, y, len, lpBit, _cx1, _cy1, _cx2, _cy2, lPitchHalf );           // XOR 드로잉
}

/*-----------------------------------------------------------------------------------------
   페이지에 수직선을 그린다(flag가 TRUE이면 XOR로 그린다)
------------------------------------------------------------------------------------------*/
void CPage::VLine(int x, int y, int len, WORD color, BOOL flag)
{
	if(lpBit==NULL) return;
	//InitGrpLib(555);

	if(flag==FALSE) DrawVLine( x, y, len, lpBit, color, _cx1, _cy1, _cx2, _cy2, lPitchHalf );    // 단색 칼라 드로잉
	else if(flag==TRUE) DrawVLineXOR( x, y, len, lpBit, _cx1, _cy1, _cx2, _cy2, lPitchHalf );           // XOR 드로잉
}

/*-----------------------------------------------------------------------------------------
   페이지에 속이 빈 박스를 그린다(flag가 TRUE이면 XOR로 그린다)
------------------------------------------------------------------------------------------*/
void CPage::DrawEmptyBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag)
{
	if(lpBit==NULL) return;
	//InitGrpLib(555);

	if(x1>x2) swap(x1,x2);
	if(y1>y2) swap(y1,y2);
	
	if(flag==FALSE) DrawBox( x1, y1, x2, y2, lpBit, color, _cx1, _cy1, _cx2, _cy2, lPitchHalf );    // 단색 칼라 드로잉
	else if(flag==TRUE) DrawBoxXOR( x1, y1, x2, y2, lpBit, _cx1, _cy1, _cx2, _cy2, lPitchHalf );           // XOR 드로잉
}

/*-----------------------------------------------------------------------------------------
   페이지에 속이 찬 박스를 그린다
------------------------------------------------------------------------------------------*/
void CPage::DrawFillBox(int x1, int y1, int x2, int y2, WORD color)
{
	if(lpBit==NULL) return;
	//InitGrpLib(555);

	if(x1>x2) swap(x1,x2);
	if(y1>y2) swap(y1,y2);
	
	FillBox( x1, y1, x2, y2, lpBit, color, _cx1, _cy1, _cx2, _cy2, lPitchHalf );
}

/*-----------------------------------------------------------------------------------------
   페이지에 속이 찬 반투명 박스를 그린다
------------------------------------------------------------------------------------------*/
void CPage::DrawFillBoxGB(int x1, int y1, int x2, int y2, WORD color, int grade)
{
	if(lpBit==NULL) return;
	//InitGrpLib(555);

	if(x1>x2) swap(x1,x2);
	if(y1>y2) swap(y1,y2);
	
	FillBoxGB( x1, y1, x2, y2, lpBit, color, grade, _cx1, _cy1, _cx2, _cy2, lPitchHalf );
}

/*-----------------------------------------------------------------------------------------
   선택 박스를 그린다
------------------------------------------------------------------------------------------*/
void CPage::DrawSelectBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag)
{
	if(lpBit==NULL) return;
	//InitGrpLib(555);

	if(x1>x2) swap(x1,x2);
	if(y1>y2) swap(y1,y2);

	if(flag) ::DrawXORSelectBox(x1, y1, x2, y2, lpBit, _cx1, _cy1, _cx2, _cy2, lPitchHalf );
	else ::DrawSelectBox(x1, y1, x2, y2, lpBit, color, _cx1, _cy1, _cx2, _cy2, lPitchHalf );
}




/////////////////////
//  스프라이트 관련
/////////////////////

/*--------------------------------------------------------------------------------------
   자동 스프라이트 출력 함수(압축/비압축 자동구분, 반투명/플립/단색출력 가능)
----------------------------------------------------------------------------------------*/
BOOL CPage::PutSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int opt, int grade, WORD color, int TargetPixelFormat)
{
	if(pXSpr==NULL) return FALSE;
	if(pXSpr->spr==NULL) return FALSE;
	if(sprnum<0 || sprnum >= pXSpr->sfhead.TotalSprNum) return FALSE;
	PutSprAuto(x, y, &pXSpr->spr[sprnum], opt, grade, color, TargetPixelFormat);
	return TRUE;
}

/*--------------------------------------------------------------------------------------
   자동 스프라이트 출력 함수(압축/비압축 자동구분, 반투명/플립/단색출력 가능)
----------------------------------------------------------------------------------------*/
BOOL CPage::PutSprAuto(int x, int y, SPRITE30 *pSpr, int opt, int grade, WORD color, int TargetPixelFormat)
{
	if(pSpr==NULL) return FALSE;
	if(lpBit==NULL) return FALSE;

	//InitGrpLib(555);

	SetFullClip();
	
	if(opt & TEMPCLIP)
	{
		opt = (~(opt&TEMPCLIP))&opt;
		SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
	}

	// 뒤집기 모드이면(상대좌표 포함)
	if(opt & MIRROR)
	{
		opt = (~(opt&MIRROR))&opt;
		//opt &= ~MIRROR;
		opt |= LR;
		if(opt & RXY)
		{
			// 2009.02.27 미러 모드 오류 수정 By 허재민
			x = m_iMirrorXBase - (x + pSpr->rx + pSpr->xl);
			//x = MirrorX + (MirrorX - (x + pSpr->rx + pSpr->xl));
			//x = MirrorX+MirrorX - (x+pSpr->rx) - pSpr->xl;
			opt = (~(opt&RXY))&opt;
			//opt &= ~RXY;

			/*
			static bool be = 0;
			if(!be) {
				char str[256]={0,};
				wsprintf(str, "x=%d, MirrorX=%d, rx=%d, xl=%d", x, MirrorX, pSpr->rx, pSpr->xl);
				be = true;
				::MessageBox(NULL, str, "ff", MB_OK);
				//Sleep(100000);
				be = false;
			}
			*/
		}
		else {
			// 2009.02.27 미러 모드 오류 수정 By 허재민
			x = m_iMirrorXBase - (x + pSpr->xl);
			//x = MirrorX + (MirrorX - (x + pSpr->xl));
		}
		y += pSpr->ry;
	}
	else if(opt & RXY) { x+=pSpr->rx; y+=pSpr->ry; opt = (~(opt&RXY))&opt; }

	if(pSpr->compress)
	{
		if(TargetPixelFormat == 555) 
		{
			if(opt&LR)
			{
				opt = (~(opt&LR))&opt;
				_PutCspLR(x, y, pSpr->xl, pSpr->yl, lpBit, pSpr->data, _cx1, _cy1, _cx2, _cy2, lPitchHalf);
			}
			else
			{
				if(opt==NULL) _PutCsp(x, y, pSpr->xl, pSpr->yl, lpBit, pSpr->data, _cx1, _cy1, _cx2, _cy2, lPitchHalf);
				else if(opt&ANTI) _PutCsp_Anti(x, y, pSpr->xl, pSpr->yl, lpBit, pSpr->data, _cx1, _cy1, _cx2, _cy2, lPitchHalf);
				else if(opt&HB) _PutCsp_HB(x, y, pSpr->xl, pSpr->yl, lpBit, pSpr->data, _cx1, _cy1, _cx2, _cy2, lPitchHalf);
				else if(opt&GB) _PutCsp_GB(x, y, pSpr->xl, pSpr->yl, lpBit, pSpr->data, grade, _cx1, _cy1, _cx2, _cy2, lPitchHalf);
				else if(opt&LE) _PutCsp_LE(x, y, pSpr->xl, pSpr->yl, lpBit, pSpr->data, _cx1, _cy1, _cx2, _cy2, lPitchHalf);
				else if(opt&GLOW) _PutCsp_Glow(x, y, pSpr->xl, pSpr->yl, lpBit, pSpr->data, color, grade, _cx1, _cy1, _cx2, _cy2, lPitchHalf);
			}
		}
		else if(TargetPixelFormat == 565) _PutCsp_RGB555TORGB565(x, y, pSpr->xl, pSpr->yl, lpBit, pSpr->data, _cx1, _cy1, _cx2, _cy2, lPitchHalf);
		else if(TargetPixelFormat == 24) _PutCsp_RGB555TORGB24(x, y, pSpr->xl, pSpr->yl, (BYTE*)lpBit, lPitch, pSpr->data, _cx1, _cy1, _cx2, _cy2, lPitchHalf);
		else if(TargetPixelFormat == 32) _PutCsp_RGB555TORGB32(x, y, pSpr->xl, pSpr->yl, (DWORD*)lpBit, lPitch, pSpr->data, _cx1, _cy1, _cx2, _cy2, lPitchHalf);
	}
	else
	{
		_PutSpr(x, y, pSpr->xl, pSpr->yl, lpBit, pSpr->data, pSpr->colorkey, _cx1, _cy1, _cx2, _cy2, lPitchHalf);
	}

	return TRUE;
}

/*--------------------------------------------------------------------------------------
   Zoom 스프라이트 출력 함수(압축/비압축 모두 사용가능, 좌우/상하플립 가능)
----------------------------------------------------------------------------------------*/
BOOL CPage::ZoomSprAuto(int x, int y, SPRITE30 *pSpr, int tx, int ty, int opt, int grade, WORD color)
{
	if(pSpr==NULL) return FALSE;
	if(lpBit==NULL) return FALSE;

	//InitGrpLib(555);
	SetFullClip();

	if(opt & TEMPCLIP) SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);

	if(opt & PERCENT) { tx = pSpr->xl*tx/100; ty = pSpr->yl*ty/100; }	// 100% 기준으로 확대,축소
	if(opt & RXY) { x+=pSpr->rx*tx/pSpr->xl; y+=pSpr->ry*ty/pSpr->yl; }	// 상대좌표를 적용
	if(opt & CENTER) { x -= (tx>>1); y -= (ty>>1); }					// 캐릭터를 좌표의 중심에 위치시킨다
	
	// 1x Zoom
	if(tx == pSpr->xl && ty == pSpr->yl) { PutSprAuto(x, y, pSpr, opt, grade, color); return TRUE; }
	
	if(pSpr->compress)
	{
		//if(opt==NULL) _ZoomCsp(x, y, pSpr->xl, pSpr->yl, lpBit, tx, ty, pSpr->data);
		_ZoomCsp(x, y, pSpr->xl, pSpr->yl, lpBit, tx, ty, pSpr->data, _cx1, _cy1, _cx2, _cy2, lPitchHalf);
	}
	else
	{
		//if(opt==NULL) _ZoomSpr(x, y, pSpr->xl, pSpr->yl, lpBit, tx, ty, pSpr->data, pSpr->colorkey);
		//_PutSpr(x, y, pSpr->xl, pSpr->yl, lpBit, pSpr->data, pSpr->colorkey);
	}
	return TRUE;
}


/*--------------------------------------------------------------------------------------
      자동 이미지 출력 함수(압축/비압축 자동구분, 반투명/플립/단색출력 가능)
----------------------------------------------------------------------------------------*/
BOOL CPage::PutImageAuto(int x, int y, SPRITE30 *pSpr, int opt, int grade, WORD color)
{
	if(pSpr==NULL) return FALSE;
	if(lpBit==NULL) return FALSE;

	//InitGrpLib(555);
	SetFullClip();

	if(opt & TEMPCLIP) SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);

	if(opt & RXY) { x+=pSpr->rx; y+=pSpr->ry; opt = (~(opt&RXY))&opt; }

	if(pSpr->compress)
	{
		//if(opt==NULL) _PutCsp(x, y, pSpr->xl, pSpr->yl, lpBit, pSpr->data);
	}
	else
	{
		_PutImage(x, y, pSpr->xl, pSpr->yl, lpBit, pSpr->data, _cx1, _cy1, _cx2, _cy2, lPitchHalf);
	}

	
	return TRUE;
}
