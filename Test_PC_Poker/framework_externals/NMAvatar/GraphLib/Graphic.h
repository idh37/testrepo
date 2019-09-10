
#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__


#include <windows.h>
#include <stdio.h>
#include <memory.h>

#include "GraphMod.h"
#include "xSprite.h"

// ��������Ʈ ���ȿ�� ���� �÷���
#define LR		((DWORD)1)		// 0000000000000001(b) (�¿����)
#define UD		((DWORD)2)		// 0000000000000010(b) (���Ϲ���)
#define HB		((DWORD)4)		// 0000000000000100(b) (������ȿ��)
#define GB		((DWORD)8)		// 0000000000001000(b) (�ٴܰ� ������ȿ��)
#define GLOW	((DWORD)16)		// 0000000000010000(b) (�۷ο�����ȿ��)
#define ANTI	((DWORD)32)		// 0000000000100000(b) (��Ƽȿ��)
#define LE		((DWORD)64)		// 0000000001000000(b) (����ȿ��)

#define RXY		((DWORD)128)	// 0000000010000000(b) (�����ǥ ����)
#define CENTER	((DWORD)256)	// 0000000100000000(b) (Ȯ��� ĳ���͸� ��ǥ�� �߽ɿ� ��ġ��Ų��)
#define PERCENT	((DWORD)512)	// 0000001000000000(b) (Ȯ��� ������ ���� Ȯ��)
#define MIRROR ((DWORD)1024)	// 0000010000000000(b) (�¿� ������-LR�� �޸� �����ǥ�� �Բ� ����)
#define TEMPCLIP ((DWORD)2048)	// 0000100000000000(b) (�ӽ� Ŭ���� ������ �����)



class CPage
{
public:
	LPBITMAPINFO lpBmpInfo;     // ��Ʈ�� ����
	LPLOGPALETTE lpLogPal;      // �α� �ķ�Ʈ ������
	HPALETTE	 hPal;	        // DIB������ �ȷ�Ʈ �ڵ�
	HBITMAP      hBmp;			// ��Ʈ���� �ڵ�(CreateDIBSection()���� ������ �ڵ�)

	WORD* lpBit;                // ������ ������
	LONG  lPitch;               // PITCH(4�ǹ��) - ������ �������� ���� ������
	LONG  lPitchHalf;           // lPitch/2
	LONG  width, height;        // �������� ��ȿ ���� ����(�ִ� Ŭ���� ����)
	WORD  nBitNum;              // ��Ʈ��
	LONG  nBPD;                 // ��Ʈ�� ����Ʈ ��
	WORD  nColorNum;            // Į���
	
	CPage();
	~CPage();

	// Ŭ���� ����
	int _cx1, _cy1, _cx2, _cy2;				// Ŭ���� ����
	int TClipX1, TClipY1, TClipX2, TClipY2;	// �ӽ� Ŭ���� ����


	void SetClipArea(int x1, int y1, int x2, int y2);
	void SetFullClip();
	void SetTempClip(int x1, int y1, int x2, int y2);

	// �̷� ��� ����
	// 2009.02.27 �̷� ��� ���� ���� By �����
	//int MirrorX;
	//void SetMirrorX(int x);
	int m_iMirrorXBase;

	/**
	@brif - �̷� ��� ��� ���� �Լ�
	@param[in] x - �̷����� �� �߽� ��ǥ.
	@example SetMirrorXEx(xp + (pRes->width/2));
	*/
	void SetMirrorX(int x);
	/**
	@brif - �̷� ��� ��� ���� �Լ� (�̹��� �ʺ� Ȧ�� �� ��� �߽��� �������� ����.)
	@param[in] xp - �̹����� ��µ� ��� ���� ��ǥ.
	@param[in] iImageWidth - �̹����� �ʺ�
	@remark �ش� �Լ��� ȣ���� ���, �ش� �̹����� �ʺ��� ��� �߽��� �̷����� �Ǿ� ��µȴ�.
	@example SetMirrorXEx(xp, pRes->width);
	*/
	void SetMirrorXEx(int xp, int iImageWidth);



	// Attach, Detach
	BOOL bAttached;
	//BOOL Attach(LONG xl, LONG yl, LPBITMAPINFO lpbmpinfo, WORD *lpbit, HBITMAP hbmp);
	BOOL Attach(LONG xl, LONG yl, void *lpbit, LONG lpitch);
	BOOL Detach();

	
	///////// ������ ����

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

	///////// ������ ��������

	int  xpFlip, ypFlip;
	int  iZoomX;

	void ToPagePos(LONG xp, LONG yp, LONG* X, LONG* Y);

	///////// ����� ����
	void HLine(int x, int y, int len, WORD color, BOOL flag);
	void VLine(int x, int y, int len, WORD color, BOOL flag);
	void DrawEmptyBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag=0);
	void DrawFillBox(int x1, int y1, int x2, int y2, WORD color);
	void DrawFillBoxGB(int x1, int y1, int x2, int y2, WORD color, int grade);
	void DrawSelectBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag=0);

	///////// ��������Ʈ ����

	BOOL PutSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int opt=NULL, int grade=NULL , WORD color=NULL, int TargetPixelFormat = 555);
	BOOL PutSprAuto(int x, int y, SPRITE30 *pSpr, int opt=NULL, int grade=NULL , WORD color=NULL, int TargetPixelFormat = 555);
	BOOL ZoomSprAuto(int x, int y, SPRITE30 *pSpr, int tx, int ty, int opt=NULL, int grade=NULL, WORD color=NULL);
	BOOL PutImageAuto(int x, int y, SPRITE30 *pSpr, int opt=NULL, int grade=NULL, WORD color=NULL);

};


#endif