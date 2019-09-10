
#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

#include "GraphMod.h"	// 16��Ʈ ��������Ʈ ��� ���
#include "xSprite.h"	// 16 ��Ʈ ��������Ʈ Ŭ����
#include "GraphMod8.h"	// 8��Ʈ ��������Ʈ ��� ���
#include "Sprite8.h"	// 8 ��Ʈ ��������Ʈ Ŭ����


// ��������Ʈ ���ȿ�� ���� �÷���
#define LR		((DWORD)1)		// 0000000000000001(b) (�¿����)
#define UD		((DWORD)2)		// 0000000000000010(b) (���Ϲ���)
#define HB		((DWORD)4)		// 0000000000000100(b) (������ȿ��)
#define GB		((DWORD)8)		// 0000000000001000(b) (�ٴܰ� ������ȿ��)
#define GLOW	((DWORD)16)		// 0000000000010000(b) (�۷ο� ����ȿ�� - ���� ����)
#define GRAYSCL	((DWORD)32)		// 0000000000100000(b) (�׷��̽����� ����ȿ�� - ��� ���)
#define MONO	((DWORD)64)		// 0000000001000000(b) (��� ����ȿ�� - �ܻ� ���)
#define ANTI	((DWORD)128)	// 0000000010000000(b) (��Ƽȿ��)
#define LE		((DWORD)256)	// 0000000100000000(b) (����ȿ��)
#define DE		((DWORD)512)	// 0000001000000000(b) (Darken Effect)
#define RXY		((DWORD)1024)	// 0000010000000000(b) (�����ǥ ����)
#define CENTER	((DWORD)2048)	// 0000100000000000(b) (Ȯ��� ĳ���͸� ��ǥ�� �߽ɿ� ��ġ��Ų��)
#define PERCENT	((DWORD)4096)	// 0001000000000000(b) (Ȯ��� ������ ���� Ȯ��)
#define MIRROR  ((DWORD)8192)	// 0010000000000000(b) (�¿� ������-LR�� �޸� �����ǥ�� �Բ� ����)
#define TEMPCLIP ((DWORD)16384)	// 0100000000000000(b) (�ӽ� Ŭ���� ������ �����)
#define AB		((DWORD)32768)	// 1000000000000000(b) (���ĺ��� ȿ��)


// �� �׸��� ���� ����ü
struct DRAWINGCELLDESC
{
	float   xp;		// ��������Ʈ ��� X��ǥ
	float   yp;		// ��������Ʈ ��� Y��ǥ
	int   sprno;	// ��������Ʈ ��ȣ
	float   width;	// ��µ� �ʺ�
	float   height;	// ��µ� ���� 
	float angle;	// ����� ȸ������
	int   opt;		// ȿ�� �ɼ�
	int   grade;	// ȿ�� �ܰ�
	WORD  color;	// ȿ�� �÷�
};

// �� ȿ�� ����ü
struct SCNEFFECTDESC
{
	int   mx;		// ��������Ʈ ��� X��ǥ
	int   my;		// ��������Ʈ ��� Y��ǥ
	int   zoomx;	// ��µ� ���� �� ����
	int   zoomy;	// ��µ� ���� �� ���� 
	float angle;	// ����� ȸ������
	int   alpha;	// ���絵 �ܰ�
};

// ��������Ʈ ��Ʈ �׽�Ʈ
BOOL PointHitTest(xSprite *pSpr, int sprnum, int x, int y, DWORD opt=0);

// ���� ������ ��������Ʈ ��Ʈ �׽�Ʈ
BOOL PointHitTest_Zoom(xSprite *pSpr, int sprnum, int x, int y, int w, int h, DWORD opt=0);

// ȸ���� ���� ������ ��������Ʈ ��Ʈ �׽�Ʈ
BOOL PointHitTest_RotateZoom(xSprite *pSpr, int sprnum, int x, int y, int w, int h, float angle, DWORD opt=0);

// �׷��� ��¿� CPage Ŭ����
class CPage
{
public:
	LPBITMAPINFO	lpBmpInfo;			// ��Ʈ�� ����
	LPLOGPALETTE	lpLogPal;			// �α� �ķ�Ʈ ������
	HPALETTE		hPal;				// DIB������ �ȷ�Ʈ �ڵ�
	HBITMAP			hBmp;				// ��Ʈ���� �ڵ�(CreateDIBSection()���� ������ �ڵ�)
	DWORD			nAllocSize;

	void*			lpBit;				// ������ ������
	LONG			lPitch;				// PITCH(4�ǹ��) - ������ �������� ���� ������
	LONG			lPitchHalf;			// lPitch/2
	LONG			width, height;		// �������� ��ȿ ���� ����(�ִ� Ŭ���� ����)
	WORD			nBitNum;			// ��Ʈ��
	LONG			nBPD;				// ��Ʈ�� ����Ʈ ��
	WORD			nColorNum;			// Į���

	BOOL			bCreateAlphaTable8;	// 8��Ʈ ���� ���̺��� �����Ͽ��°�?
	BOOL			bEnableTempClip;	// �ӽ� Ŭ���� ������ Ȱ��ȭ ����
	int				TClipX1, TClipY1, TClipX2, TClipY2;	// �ӽ� Ŭ���� ����
	int				MirrorX;			// �̷� ��� ����
	
	CPage();
	~CPage();

	void SetTempClip(int x1, int y1, int x2, int y2);	// ��������Ʈ�� �׸��� �ӽ� Ŭ���� ������ ����(EnableTempClip(TRUE)�� ȣ���ϰų� �׸��� �ɼǿ��� TMPCLIP�� �����ؾ���)
	void EnableTempClip(BOOL bEnable);					// �ӽ� Ŭ���� ������ ������ Ȱ��ȭ ���θ� ����(�ѹ��� ����ϱ� ���ؼ���� �׸��� �ɼǿ��� TMPCLIP�� ���)
	void SetMirrorX(int x);								// ��������Ʈ �׸��� �ɼ��� �̷�(MIRROR)�� �����ϴ� ��� ������ �߽� ��ġ

	// Attach, Detach
	BOOL bAttached;						// Page�� Attach �Ǿ��°�?
	BOOL Attach(LONG xl, LONG yl, void *lpbit, LONG lpitch);
	BOOL Detach();

	///////// ������ ����

	BOOL Init(LONG xl, LONG yl, INT bit, RGBQUAD *lpPal=NULL, LPCTSTR palfname=NULL);				// ������ �ʱ�ȭ
	void Destroy();																					// ������ �ı�
	BOOL SaveToBitmap(LPSTR fname);																	// �������� ������ ��Ʈ������ ����
	BOOL SaveToJpeg(LPSTR fname, int quality);	// quality = 0-100									// �������� ������ JPG�� ����
	BOOL ReCreateDIBSection();
	BOOL OpenPal(LPCTSTR filename);
	BOOL OpenAlphaTable8(LPCTSTR filename);
	BOOL SetPal(RGBQUAD *lpPal);
	BOOL SetLogPal(LPLOGPALETTE lpPal);
	BOOL SetPalHandle(HPALETTE h);
	BOOL PutFromPage(LONG xp, LONG yp, CPage *pPage, int blendgrade=31);							// ������ �������� �ٸ� �������� �̹����� �ٴܰ� ���������� �׸�
	BOOL PutFromPageWithColorKey(LONG xp, LONG yp, CPage *pPage, WORD colorkey, int blendgrade=31);	// ������ �������� �ٸ� �������� �̹����� �ٴܰ� ���������� �׸�(�÷�Ű ����)
	BOOL FillGDI(HDC hTarDC, int nXStart, int nYStart, COLORREF crColor);							// DC�� Ư�������� Ư�� �÷��� ü����
	BOOL FlipToGDI(HWND hwnd, LONG xp=0, LONG yp=0, int ZoomX=1);									// �������� ������ ������ Ŭ���̾�Ʈ ������ �׷���
	BOOL FlipToGDI(HDC hdc, LONG xp=0, LONG yp=0, int ZoomX=1);										// �������� ������ GDI�� �׷���
	void SetFlipPos(LONG xp, LONG yp, int zoom);													// Flip()�Լ� ���� �������� �׷��� ��ġ ����
	BOOL Flip(HWND hwnd);																			// Ư�� �������� Ŭ���̾�Ʈ ������ ������ ������ �׷���
	BOOL Flip(HDC hdc);																				// Ư�� DC�� ������ ������ �׷���
	BOOL StretchFlip(HWND hwnd, RECT* rcDest, RECT* rcSrc);											// Ư�� �������� Ŭ���̾�Ʈ ������ ������ ������ Ÿ�� ������ �°� �÷��� �׷���

	///////// ������ ��������

	int  xpFlip, ypFlip;
	int  iZoomX;

	void ToPagePos(LONG xp, LONG yp, LONG* X, LONG* Y);

	///////// ����� ����
	void HLine(int x, int y, int len, WORD color, BOOL flag);						// ������ �׸�(����)
	void VLine(int x, int y, int len, WORD color, BOOL flag);						// �������� �׸�(����)
	void DrawLine(int x1, int y1, int x2, int y2, WORD color);						// ���� �׸�(�극���� �˰���)
	void DrawLineXOR(int x1, int y1, int x2, int y2);								// XORȿ���� ���� �׸�(�극���� �˰���)
	void DrawLineGB(int x1, int y1, int x2, int y2, WORD color, int grade);			// �ٴܰ� ������ ȿ���� ���� �׸�(�극���� �˰���)
	void DrawEmptyBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag=0);		// ���� �� �ڽ��� �׸�
	void DrawFillBox(int x1, int y1, int x2, int y2, WORD color);					// ���� ä�� �ڽ��� �׸�
	void DrawFillBoxGB(int x1, int y1, int x2, int y2, WORD color, int grade);		// ���� ä�� �ڽ��� �׸�(�ٴܰ� ������)
	void DrawSelectBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag=0);	// ���� ���ڸ� �׸�
	void DrawRotateXBox(int x1, int y1, int x2, int y2, float angle, WORD color);	// ȸ�� X-���ڸ� �׸�

	///////// ��������Ʈ ����

	// 16��Ʈ ��������Ʈ
	BOOL PutSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int opt=NULL, int grade=31 , WORD color=0);
	BOOL PutSprAuto(int x, int y, SPRITE30 *pSpr, int opt=NULL, int grade=31 , WORD color=0);
	BOOL ZoomSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int tx, int ty, int opt=NULL, int grade=31, WORD color=0);
	BOOL ZoomSprAuto(int x, int y, SPRITE30 *pSpr, int tx, int ty, int opt=NULL, int grade=31, WORD color=0);
	BOOL RotateZoomSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int tx, int ty, float angle, int opt=NULL, int grade=31, WORD color=0);
	BOOL RotateZoomSprAuto(int x, int y, SPRITE30 *pSpr, int tx, int ty, float angle, int opt=NULL, int grade=31, WORD color=0);
	BOOL PutImageAuto(int x, int y, xSprite *pXSpr, int sprnum, int opt=NULL, int grade=31, WORD color=0);
	BOOL PutImageAuto(int x, int y, SPRITE30 *pSpr, int opt=NULL, int grade=0, WORD color=0);

	// 8��Ʈ ��������Ʈ
	BOOL PutSprAuto(int x, int y, CSprite8 *pSpr8, int sprnum, int opt=NULL, int grade=31 , BYTE color=0);
	BOOL PutSprAuto(int x, int y, SPRITE8 *pSpr, int opt=NULL, int grade=31 , BYTE color=0);
	BOOL ZoomSprAuto(int x, int y, CSprite8 *pSpr8, int sprnum, int tx, int ty, int opt=NULL, int grade=31, BYTE color=0);
	BOOL ZoomSprAuto(int x, int y, SPRITE8 *pSpr, int tx, int ty, int opt=NULL, int grade=31, BYTE color=0);
	BOOL PutImageAuto(int x, int y, CSprite8 *pSpr8, int sprnum, int opt=NULL, int grade=31, BYTE color=0);
	BOOL PutImageAuto(int x, int y, SPRITE8 *pSpr, int opt=NULL, int grade=31, BYTE color=0);

	///////// ��(���) ����

	// �� �׸���
	BOOL DrawScene(int x, int y, xSprite *pXSpr, int scn, DWORD time, BOOL loop=TRUE, int n_opt=NULL, int n_grade=31, WORD n_color=0);
	BOOL DrawFrameOfScene(int x, int y, xSprite *pXSpr, int scn, int frame, float fdelta=0.0f, int n_opt=NULL, int n_grade=31, WORD n_color=0);
	BOOL GetDrawingCellInfo(int x, int y, xSprite *pXSpr, int scn, int frame, int cell, float fdelta, DRAWINGCELLDESC* pDesc);

	// �� �׸��� ȿ�� ����
	BOOL			bScnEffect;
	SCNEFFECTDESC	ScnEffect;

	void SetSceneEffect(SCNEFFECTDESC* pEffect);

	///////// ��Ƽ�� ����

	int  iDrawingTrackBegin;
	int  iDrawingTrackEnd;

	// ��Ƽ������ �׷��� Ʈ���� ���� ����(��� ��Ƽ���� ���� ����)
	void SetDrawingTrackScope(int begin=-1, int end=-1);	// -1�� ó���� ���� �ǹ�

	// ��Ƽ�� �׸���
	BOOL DrawMultiScene(int x, int y, xSprite *pXSpr, int multiscn, DWORD time, BOOL loop=TRUE, int n_opt=NULL, int n_grade=31, WORD n_color=0);

	// ��Ƽ���� Ư�� �� Ʈ���� Ư�� ���� ��� ������ ��� �Լ�
	BOOL GetMSceneDrawingCellInfo(int x, int y, xSprite *pXSpr, int multiscn, int track, int cell, DWORD time, BOOL loop, DRAWINGCELLDESC* pDesc);

};


#endif