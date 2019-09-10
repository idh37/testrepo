//
//                                   �׷��� ó�� ���
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

// �ӽ�Ŭ���� ������ �����Ѵ�
void CPage::SetTempClip(int x1, int y1, int x2, int y2)
{
	TClipX1 = min(max(0, x1), width-1);
	TClipY1 = min(max(0, y1), height-1);
	TClipX2 = min(max(0, x2), width-1);
	TClipY2 = min(max(0, y2), height-1);
}

// �̷���� ���� ��ǥ�� �����Ѵ�
// 2009.02.27 �̷� ��� ���� ���� By �����
void CPage::SetMirrorXEx(int iMultiXp, int iAdjust)
{
	m_iMirrorXBase = iMultiXp*2 + iAdjust;
}

void CPage::SetMirrorX(int x)
{
	// 2009.02.27 �̷� ��� ���� ���� By �����
	SetMirrorXEx(x, 0);
	// MirrorX = x;
}

/*-----------------------------------------------------------------------------------------
   �������� �ʱ�ȭ �Ѵ�
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
	while(lPitch%4) lPitch++; // Pitch�� 4�� ����� �ǰ� �Ѵ�
	//lPitch/=2;                // 16��Ʈ Į�󿩼� ����� ���.
	                          // ���⼭�� lPitch�� ����Ʈ���̴�(���� ������ �ƴϴ�)
	lPitchHalf = lPitch/nBPD; // lPitch�� ����Ʈ���̹Ƿ� 8,16,23��Ʈ ����� ���� ������ ��´�.

	// ��Ʈ�� ���� ����ü�� ���� �޸� �Ҵ�
	DWORD nAllocSize = sizeof(BITMAPINFO) + sizeof(RGBQUAD) * nColorNum;
	if(nColorNum != 0) nAllocSize -= sizeof(RGBQUAD);
	lpBmpInfo = (LPBITMAPINFO)new char[nAllocSize];
	if(lpBmpInfo == NULL) return FALSE;

	// ���� ������ ����(��Ʈ�� �̹��� ������)
	//lpBit = (WORD*)new char[lPitch * yl];
	//if(lpBit == NULL) return FALSE;

	// ��Ʈ�� ���� �ش� ����
	lpBmpInfo->bmiHeader.biSize	         = sizeof (BITMAPINFOHEADER);
	lpBmpInfo->bmiHeader.biWidth         = width;      //width;
	lpBmpInfo->bmiHeader.biHeight        = -height;    // ������ �׸��� ������ ��µȴ�
	lpBmpInfo->bmiHeader.biPlanes        = 1;          // �׻�1
	lpBmpInfo->bmiHeader.biBitCount	     = nBitNum;    // ��Ʈ�� ��Ʈ��(16��Ʈ�� 5:6:5)
	lpBmpInfo->bmiHeader.biCompression	 = 0;          // BI_RGB�� ����(�����)
	lpBmpInfo->bmiHeader.biSizeImage     = lPitch*yl;  // lPitch; // �׻� 4�ǹ��
	lpBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	lpBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	lpBmpInfo->bmiHeader.biClrUsed	     = 0;          // ���� �� Į���(�׻� 0���� ����)
	lpBmpInfo->bmiHeader.biClrImportant	 = 0;          // �߿��� �����(�׻� 0���� ����)

	// DIB�� ����
	HWND ActiveWindow = GetActiveWindow();
	HDC ScreenDC = GetDC(ActiveWindow);
	hBmp = CreateDIBSection(ScreenDC, lpBmpInfo, DIB_RGB_COLORS, (VOID**)&lpBit, NULL, 0);
	ReleaseDC(ActiveWindow, ScreenDC);

	//InitGrpLib(xl, yl, lPitch/2, 555);
	InitGrpLib(555);
	InitAlphaTable(555);

	// Ŭ���� ���� ����
	_cx1 = 0;
	_cy1 = 0;
	_cx2 = xl - 1;
	_cy2 = yl - 1;

	// �ķ�Ʈ ����
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
	while(lPitch%4) lPitch++; // Pitch�� 4�� ����� �ǰ� �Ѵ�
	lPitchHalf = lPitch/nBPD; // lPitch�� ����Ʈ���̹Ƿ� 8,16,23��Ʈ ����� ���� ������ ��´�.
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
	//while(lPitch%4) lPitch++; // Pitch�� 4�� ����� �ǰ� �Ѵ�
	lPitchHalf = lPitch/nBPD; // lPitch�� ����Ʈ���̹Ƿ� 8,16,23��Ʈ ����� ���� ������ ��´�.
	lpBmpInfo = NULL;
	lpBit = (WORD*)lpbit;

	//InitGrpLib(xl, yl, lPitch/2, 555);
	InitGrpLib(555);

	// Ŭ���� ���� ����
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

	// DIB�� ����
	HWND ActiveWindow = GetActiveWindow();
	HDC ScreenDC = GetDC(ActiveWindow);
	hBmp = CreateDIBSection(ScreenDC, lpBmpInfo, DIB_RGB_COLORS, (VOID**)&lpBit, NULL, 0);
	ReleaseDC(ActiveWindow, ScreenDC);

	return (hBmp != NULL);
}

/*-----------------------------------------------------------------------------------------
   �������� �ķ�Ʈ�� �����Ѵ�

   - �μ� : RGBQUAD�� ������
------------------------------------------------------------------------------------------*/
BOOL CPage::SetPal(RGBQUAD *lpPal)
{
	if( nColorNum == 0)	return TRUE;        //�ķ�Ʈ�� �ʿ���ٸ�

	if(lpLogPal) { free( lpLogPal );  lpLogPal=NULL; } // �޸� ����
	lpLogPal = (LPLOGPALETTE)malloc( sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * nColorNum );

	lpLogPal->palVersion = 0x300;	        // �ȷ�Ʈ ���� �Է�
	lpLogPal->palNumEntries = nColorNum;    // �ȷ�Ʈ ����� �Է�
	
	int i;
	if(lpPal==NULL){             // �ķ�Ʈ �����Ͱ� NULL�̸� �⺻ �ķ�Ʈ�� �����Ѵ�
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
	if( nColorNum == 0)	//�ķ�Ʈ�� �ʿ���ٸ�
		return TRUE;
	
	if(lpLogPal!=NULL) { free( lpLogPal );  lpLogPal=NULL; } // �޸� ����

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
   GDI ǥ���� Ư�� Į��� ä���
------------------------------------------------------------------------------------------*/
BOOL CPage::FillGDI(HDC hTarDC, int nXStart, int nYStart, COLORREF crColor)
{
	BOOL check;
	check = FloodFill(hTarDC, nXStart, nYStart, crColor);
	if(check != 0)return TRUE;
	else return FALSE;
}

/*-----------------------------------------------------------------------------------------
   �ٸ� �������� ������ ���� ������ ǥ�鿡 �Ѹ���
------------------------------------------------------------------------------------------*/
BOOL CPage::PutFromPage(LONG xp, LONG yp, CPage *pPage)
{
	int w = pPage->lPitchHalf;	// �ҽ� �������� ���� ����� �ƴ϶� Half-Pitch�� ��´�
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
   �ٸ� �������� ������ ���� ������ ǥ�鿡 �Ѹ���
------------------------------------------------------------------------------------------*/
BOOL CPage::PutFromPage(LONG xp, LONG yp, LONG ww, LONG hh, CPage *pPage)
{
	int w = pPage->lPitchHalf;	// �ҽ� �������� ���� ����� �ƴ϶� Half-Pitch�� ��´�
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
   �ٸ� �������� ������ ���� ������ ǥ�鿡 �Ѹ���(������ ũ�⿡ �°� �ε巯�� Ȯ��/��Ұ� �����)
------------------------------------------------------------------------------------------*/
BOOL CPage::SmoothZoomFromPage(LONG xp, LONG yp, CPage *pPage)
{
	int w = pPage->lPitchHalf;	// �ҽ� �������� ���� ����� �ƴ϶� Half-Pitch�� ��´�
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
   �������� �ѷ��� Ŭ���̾�Ʈ�� Ư�� ��ǥ�� �������� ��ǥ�� ��ȯ��Ų��
------------------------------------------------------------------------------------------*/
void CPage::ToPagePos(LONG xp, LONG yp, LONG* X, LONG* Y)
{
	*X = (xp-xpFlip)/iZoomX;
	*Y = (yp-ypFlip)/iZoomX;
}

/*-----------------------------------------------------------------------------------------
    �������� �ø��� GDIǥ�� ��ġ(Ŭ���̾�Ʈ ����)�� �����Ѵ�
------------------------------------------------------------------------------------------*/
void CPage::SetFlipPos(LONG xp, LONG yp, int zoom)
{
	xpFlip = xp;
	ypFlip = yp;
	iZoomX = zoom;
}

/*-----------------------------------------------------------------------------------------
   �������� ������ GDI ǥ�鿡 ������� �Ѹ���(SetPagePos()�� ��ġ�� �ٲܼ� �ִ�)
------------------------------------------------------------------------------------------*/
BOOL CPage::Flip(HWND hwnd)
{
	// �������� �ʱ�ȭ ���� �ʾҴٸ�
	if(  lpBit == NULL ) return FALSE;
	HDC hDC = GetDC(hwnd);
	BOOL rtn = Flip(hDC);
	ReleaseDC(hwnd, hDC);
	return rtn;
}

BOOL CPage::Flip(HDC hdc)
{
	int rtn;

	// �������� �ʱ�ȭ ���� �ʾҴٸ�
	if(  lpBit == NULL ) return FALSE;

	HDC hDC = hdc;

	if( nColorNum != 0 && hPal != NULL)	//�ķ�Ʈ�� �ʿ��ϴٸ�
	{
		HPALETTE hOldPal;
		hOldPal = SelectPalette( hDC, hPal, TRUE ); //�ȷ�Ʈ ����
		if(hOldPal == NULL) return FALSE;
		rtn = RealizePalette( hDC );				 //�ȷ�Ʈ Ȱ��ȭ
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
   �������� ������ GDI ǥ�鿡 ������ Ȯ��� �Ѹ���(SetFlipPos()�� �̸�����)
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
   �������� ������ GDI ǥ�鿡 Stretch�� �Ѹ���
   
   - �ҽ��� Ÿ���� RECT�� NULL�̸� ��ü �������� �����ȴ�
------------------------------------------------------------------------------------------*/
BOOL CPage::StretchFlip(HWND hwnd, RECT* rcDest, RECT* rcSrc )
{
	int rtn;

	// �������� �ʱ�ȭ ���� �ʾҴٸ�
	if(  lpBit == NULL ) return FALSE;

	HDC hDC = GetDC(hwnd);

	if( nColorNum != 0 && hPal != NULL)	//�ķ�Ʈ�� �ʿ��ϴٸ�
	{
		HPALETTE hOldPal;
		hOldPal = SelectPalette( hDC, hPal, TRUE ); //�ȷ�Ʈ ����
		if(hOldPal == NULL) return FALSE;
		rtn = RealizePalette( hDC );				 //�ȷ�Ʈ Ȱ��ȭ
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

	sy = height-sy-syl; // �׸��� ������ �����Ƿ� ��ǥ�� �����´�

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
//  ����� ����
/////////////////////

/*-----------------------------------------------------------------------------------------
   �������� ������ �׸���(flag�� TRUE�̸� XOR�� �׸���)
------------------------------------------------------------------------------------------*/
void CPage::HLine(int x, int y, int len, WORD color, BOOL flag)
{
	if(lpBit==NULL) return;
	//InitGrpLib(555);

	if(flag==FALSE) DrawHLine( x, y, len, lpBit, color, _cx1, _cy1, _cx2, _cy2, lPitchHalf );    // �ܻ� Į�� �����
	else if(flag==TRUE) DrawHLineXOR( x, y, len, lpBit, _cx1, _cy1, _cx2, _cy2, lPitchHalf );           // XOR �����
}

/*-----------------------------------------------------------------------------------------
   �������� �������� �׸���(flag�� TRUE�̸� XOR�� �׸���)
------------------------------------------------------------------------------------------*/
void CPage::VLine(int x, int y, int len, WORD color, BOOL flag)
{
	if(lpBit==NULL) return;
	//InitGrpLib(555);

	if(flag==FALSE) DrawVLine( x, y, len, lpBit, color, _cx1, _cy1, _cx2, _cy2, lPitchHalf );    // �ܻ� Į�� �����
	else if(flag==TRUE) DrawVLineXOR( x, y, len, lpBit, _cx1, _cy1, _cx2, _cy2, lPitchHalf );           // XOR �����
}

/*-----------------------------------------------------------------------------------------
   �������� ���� �� �ڽ��� �׸���(flag�� TRUE�̸� XOR�� �׸���)
------------------------------------------------------------------------------------------*/
void CPage::DrawEmptyBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag)
{
	if(lpBit==NULL) return;
	//InitGrpLib(555);

	if(x1>x2) swap(x1,x2);
	if(y1>y2) swap(y1,y2);
	
	if(flag==FALSE) DrawBox( x1, y1, x2, y2, lpBit, color, _cx1, _cy1, _cx2, _cy2, lPitchHalf );    // �ܻ� Į�� �����
	else if(flag==TRUE) DrawBoxXOR( x1, y1, x2, y2, lpBit, _cx1, _cy1, _cx2, _cy2, lPitchHalf );           // XOR �����
}

/*-----------------------------------------------------------------------------------------
   �������� ���� �� �ڽ��� �׸���
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
   �������� ���� �� ������ �ڽ��� �׸���
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
   ���� �ڽ��� �׸���
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
//  ��������Ʈ ����
/////////////////////

/*--------------------------------------------------------------------------------------
   �ڵ� ��������Ʈ ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
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
   �ڵ� ��������Ʈ ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
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

	// ������ ����̸�(�����ǥ ����)
	if(opt & MIRROR)
	{
		opt = (~(opt&MIRROR))&opt;
		//opt &= ~MIRROR;
		opt |= LR;
		if(opt & RXY)
		{
			// 2009.02.27 �̷� ��� ���� ���� By �����
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
			// 2009.02.27 �̷� ��� ���� ���� By �����
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
   Zoom ��������Ʈ ��� �Լ�(����/����� ��� ��밡��, �¿�/�����ø� ����)
----------------------------------------------------------------------------------------*/
BOOL CPage::ZoomSprAuto(int x, int y, SPRITE30 *pSpr, int tx, int ty, int opt, int grade, WORD color)
{
	if(pSpr==NULL) return FALSE;
	if(lpBit==NULL) return FALSE;

	//InitGrpLib(555);
	SetFullClip();

	if(opt & TEMPCLIP) SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);

	if(opt & PERCENT) { tx = pSpr->xl*tx/100; ty = pSpr->yl*ty/100; }	// 100% �������� Ȯ��,���
	if(opt & RXY) { x+=pSpr->rx*tx/pSpr->xl; y+=pSpr->ry*ty/pSpr->yl; }	// �����ǥ�� ����
	if(opt & CENTER) { x -= (tx>>1); y -= (ty>>1); }					// ĳ���͸� ��ǥ�� �߽ɿ� ��ġ��Ų��
	
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
      �ڵ� �̹��� ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
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
