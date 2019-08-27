// DxGraphic.h: interface for the DxGraphic class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

// 아래 두 클래스는 구현이 안된 클래스 같음
class DXSprite;
class DXSPRITE30;

#include "GraphMod.h"		// 16비트 스프라이트 출력 모듈
#include "xSprite.h"		// 16 비트 스프라이트 클래스
#include "GraphMod8.h"		// 8비트 스프라이트 출력 모듈
#include "Sprite8.h"		// 8 비트 스프라이트 클래스
#include "DirectX.h"

// 스프라이트 출력효과 설정 플래그
#define LR		((DWORD)1)		// 0000000000000001(b) (좌우반전)
#define UD		((DWORD)2)		// 0000000000000010(b) (상하반전)
#define HB		((DWORD)4)		// 0000000000000100(b) (반투명효과)
#define GB		((DWORD)8)		// 0000000000001000(b) (다단계 반투명효과)
#define GLOW	((DWORD)16)		// 0000000000010000(b) (글로우 필터효과 - 색톤 변경)
#define GRAYSCL	((DWORD)32)		// 0000000000100000(b) (그레이스케일 필터효과 - 흑백 출력)
#define MONO	((DWORD)64)		// 0000000001000000(b) (모노 필터효과 - 단색 출력)
#define ANTI	((DWORD)128)	// 0000000010000000(b) (안티효과)
#define LE		((DWORD)256)	// 0000000100000000(b) (광원효과)
#define DE		((DWORD)512)	// 0000001000000000(b) (Darken Effect)
#define RXY		((DWORD)1024)	// 0000010000000000(b) (상대좌표 적용)
#define CENTER	((DWORD)2048)	// 0000100000000000(b) (확대시 캐릭터를 좌표의 중심에 위치시킨다)
#define PERCENT	((DWORD)4096)	// 0001000000000000(b) (확대시 비율에 따른 확대)
#define MIRROR  ((DWORD)8192)	// 0010000000000000(b) (좌우 뒤집기-LR과 달리 상대좌표도 함께 반전)
#define TEMPCLIP ((DWORD)16384)	// 0100000000000000(b) (임시 클리핑 영역을 사용함)
#define AB		((DWORD)32768)	// 1000000000000000(b) (알파블렌딩 효과)


// 셀 그리기 정보 구조체
struct DRAWINGCELLDESC
{
	float   xp;		// 스프라이트 출력 X좌표
	float   yp;		// 스프라이트 출력 Y좌표
	int   sprno;	// 스프라이트 번호
	float   width;	// 출력될 너비
	float   height;	// 출력될 높이 
	float angle;	// 출력할 회전각도
	int   opt;		// 효과 옵션
	int   grade;	// 효과 단계
	WORD  color;	// 효과 컬러
};

// 씬 효과 구조체
struct SCNEFFECTDESC
{
	int   mx;		// 스프라이트 출력 X좌표
	int   my;		// 스프라이트 출력 Y좌표
	int   zoomx;	// 출력될 가로 줌 비율
	int   zoomy;	// 출력될 세로 줌 비율 
	float angle;	// 출력할 회전각도
	int   alpha;	// 투며도 단계
};



extern DWORD _WordtoDwordColor(WORD data);
extern WORD  _DWordtoWordColor(DWORD data);


class CDxPage
{
public:
	CDxPage();
	~CDxPage();

public:
	void SetTempClip(int x1, int y1, int x2, int y2);	// 스프라이트를 그릴때 임시 클리핑 영역을 지정(EnableTempClip(TRUE)를 호출하거나 그리기 옵션에서 TMPCLIP을 적용해야함)
	void EnableTempClip(BOOL bEnable);					// 임시 클리핑 영역을 지정을 활성화 여부를 설정(한번만 사용하기 위해서라면 그리기 옵션에서 TMPCLIP을 사용)
	void SetMirrorX(int x);								// 스프라이트 그리기 옵션중 미러(MIRROR)를 적용하는 경우 뒤집는 중심 위치
	void SetClipArea(int x1, int y1, int x2, int y2);

	// Attach, Detach	
	BOOL Attach(LONG xl, LONG yl, void *lpbit, LONG lpitch);
	BOOL Detach();

	///////// 페이지 관련

	BOOL Init(CDirectX* pdirectx, LONG xl, LONG yl, INT bit, RGBQUAD *lpPal=NULL, LPCTSTR palfname=NULL);				// 페이지 초기화
	void Destroy();																					// 페이지 파괴
	BOOL SaveToBitmap(LPSTR fname);																	// 페이지의 내용을 비트맵으로 저장
	BOOL SaveToJpeg(LPSTR fname, int quality);	// quality = 0-100									// 페이지의 내용을 JPG로 저장
	BOOL ReCreateDIBSection();
	BOOL OpenPal(LPCTSTR filename);
	BOOL OpenAlphaTable8(LPCTSTR filename);
	BOOL SetPal(RGBQUAD *lpPal);
	BOOL SetLogPal(LPLOGPALETTE lpPal);
	BOOL SetPalHandle(HPALETTE h);
	BOOL PutFromPage(LONG xp, LONG yp, CDxPage *pPage, int blendgrade=248);							// 현재의 페이지에 다른 페이지의 이미지를 다단계 반투명으로 그림
	BOOL PutFromPageWithColorKey(LONG xp, LONG yp, CDxPage *pPage, WORD colorkey, int blendgrade=248);	// 현재의 페이지에 다른 페이지의 이미지를 다단계 반투명으로 그림(컬러키 적용)
	BOOL FillGDI(HDC hTarDC, int nXStart, int nYStart, COLORREF crColor);							// DC의 특정영역을 특정 컬러로 체워줌
	BOOL FlipToGDI(HWND hwnd, LONG xp=0, LONG yp=0, int ZoomX=1);									// 페이지의 내용을 윈도우 클라이언트 영역에 그려줌
	BOOL FlipToGDI(HDC hdc, LONG xp=0, LONG yp=0, int ZoomX=1);										// 페이지의 내용을 GDI에 그려줌
	void SetFlipPos(LONG xp, LONG yp, int zoom);													// Flip()함수 사용시 페이지가 그려질 위치 지정
	BOOL Flip(HWND hwnd);																			// 특정 윈도우의 클라이언트 영역에 페이지 내용을 그려줌
	BOOL Flip(HDC hdc);																				// 특정 DC에 페이지 내용을 그려줌
	BOOL StretchFlip(HWND hwnd, RECT* rcDest, RECT* rcSrc);											// 특정 윈도우의 클라이언트 영역에 페이지 내용을 타겟 영역에 맞게 늘려서 그려줌

	///////// 페이지 정보관련
	void ToPagePos(LONG xp, LONG yp, LONG* X, LONG* Y);

	///////// 드로잉 관련
	void HLine(int x, int y, int len, WORD color, BOOL flag);						// 수평선을 그림(빠름)
	void VLine(int x, int y, int len, WORD color, BOOL flag);						// 수직선을 그림(빠름)
	void DrawLine(int x1, int y1, int x2, int y2, WORD color);						// 선을 그림(브레제남 알고리즘)
	void DrawLineXOR(int x1, int y1, int x2, int y2);								// XOR효과로 선을 그림(브레제남 알고리즘)
	void DrawLineGB(int x1, int y1, int x2, int y2, WORD color, int grade);			// 다단계 반투명 효과로 선을 그림(브레제남 알고리즘)
	void DrawEmptyBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag=0);		// 속이 빈 박스를 그림
	void DrawFillBox(int x1, int y1, int x2, int y2, WORD color,int opt);					// 속을 채운 박스를 그림
	void DrawFillBoxGB(int x1, int y1, int x2, int y2, WORD color, int grade);		// 속을 채운 박스를 그림(다단계 반투명)
	void DrawSelectBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag=0);	// 선택 상자를 그림
	void DrawRotateXBox(int x1, int y1, int x2, int y2, float angle, WORD color , int opt,int rx,int ry);	// 회전 X-상자를 그림
	void DrawFillBox(int x1, int y1, int x2, int y2, DWORD color,int opt);
	void DrawRotateXBox(int x1, int y1, int x2, int y2, float angle, DWORD color , int opt,int rx,int ry);

	///////// 스프라이트 관련

	///////// 3D스프라이트 관련

	BOOL PutSprAuto(int x, int y, DXSprite *pXSpr, int sprnum, int opt=NULL, int grade=248 , WORD color=0);
	BOOL PutSprAuto(int x, int y, DXSPRITE30 *pSpr, int opt=NULL, int grade=248 , WORD color=0);
	BOOL ZoomSprAuto(int x, int y, DXSprite *pXSpr, int sprnum, int tx, int ty, int opt=NULL, int grade=248, WORD color=0);
	BOOL ZoomSprAuto(int x, int y, DXSPRITE30 *pSpr, int tx, int ty, int opt=NULL, int grade=248, WORD color=0);
	BOOL PutImageAuto(int x, int y, DXSprite *pXSpr, int sprnum, int opt=NULL, int grade=248, WORD color=0);
	BOOL PutImageAuto(int x, int y, DXSPRITE30 *pSpr, int opt=NULL, int grade=0, WORD color=0);

	BOOL PutSprAuto(float x, float y, xSprite *pXSpr, int sprnum, int opt=NULL, int grade=31 , WORD color=0);
	BOOL ZoomSprAuto(float x, float y, xSprite *pXSpr, int sprnum, int tx, int ty, int opt=NULL, int grade=31, WORD color=0);
	//	BOOL RotateZoomSprAuto(float x, float y, xSprite *pXSpr, int sprnum, int tx, int ty, float anglex, float angley, float anglez, int opt=NULL, int grade=31, WORD color=0);
	BOOL RotateZoomSprAuto(float x, float y, xSprite *pXSpr, int sprnum, int tx, int ty, float angle, int opt=NULL, int grade=31, WORD color=0);

	// 16비트 스프라이트
	BOOL PutSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int opt=NULL, int grade=248 , WORD color=0);
	BOOL PutSprAuto(int x, int y, SPRITE30 *pSpr, int opt=NULL, int grade=248 , WORD color=0);
	BOOL ZoomSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int tx, int ty, int opt=NULL, int grade=248, WORD color=0);
	BOOL ZoomSprAuto(int x, int y, SPRITE30 *pSpr, int tx, int ty, int opt=NULL, int grade=248, WORD color=0);
	BOOL RotateZoomSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int tx, int ty, float angle, int opt=NULL, int grade=248, WORD color=0);
	BOOL RotateZoomSprAuto(int x, int y, SPRITE30 *pSpr, int tx, int ty, float angle, int opt=NULL, int grade=248, WORD color=0);
	BOOL PutImageAuto(int x, int y, xSprite *pXSpr, int sprnum, int opt=NULL, int grade=248, WORD color=0);
	BOOL PutImageAuto(int x, int y, SPRITE30 *pSpr, int opt=NULL, int grade=0, WORD color=0);

	// 8비트 스프라이트
	BOOL PutSprAuto(int x, int y, CSprite8 *pSpr8, int sprnum, int opt=NULL, int grade=248 , BYTE color=0);
	BOOL PutSprAuto(int x, int y, SPRITE8 *pSpr, int opt=NULL, int grade=248 , BYTE color=0);
	BOOL ZoomSprAuto(int x, int y, CSprite8 *pSpr8, int sprnum, int tx, int ty, int opt=NULL, int grade=248, BYTE color=0);
	BOOL ZoomSprAuto(int x, int y, SPRITE8 *pSpr, int tx, int ty, int opt=NULL, int grade=248, BYTE color=0);
	BOOL PutImageAuto(int x, int y, CSprite8 *pSpr8, int sprnum, int opt=NULL, int grade=248, BYTE color=0);
	BOOL PutImageAuto(int x, int y, SPRITE8 *pSpr, int opt=NULL, int grade=248, BYTE color=0);

	///////// 씬(장면) 관련

	// 씬 그리기
	BOOL DrawScene(int x, int y, xSprite *pXSpr, int scn, DWORD time, BOOL loop=TRUE, int n_opt=NULL, int n_grade=248, WORD n_color=0);
	BOOL DrawFrameOfScene(int x, int y, xSprite *pXSpr, int scn, int frame, float fdelta=0.0f, int n_opt=NULL, int n_grade=248, WORD n_color=0);
	BOOL GetDrawingCellInfo(float x, float y, xSprite *pXSpr, int scn, int frame, int cell, float fdelta, DRAWINGCELLDESC* pDesc);

	///////// 멀티씬 관련

	// 씬 그리기 효과 설정
	void SetSceneEffect(SCNEFFECTDESC* pEffect);

	// 멀티씬에서 그려질 트랙의 범위 설정(모든 멀티씬이 영향 받음)
	void SetDrawingTrackScope(int begin=-1, int end=-1);	// -1은 처음과 끝을 의미

	// 멀티씬 그리기
	BOOL DrawMultiScene(int x, int y, xSprite *pXSpr, int multiscn, DWORD time, BOOL loop=TRUE, int n_opt=NULL, int n_grade=248, WORD n_color=0);

	// 멀티씬의 특정 씬 트랙의 특정 셀의 출력 정보를 얻는 함수
	BOOL GetMSceneDrawingCellInfo(int x, int y, xSprite *pXSpr, int multiscn, int track, int cell, DWORD time, BOOL loop, DRAWINGCELLDESC* pDesc);

	BOOL PutSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int opt , DWORD dwColor );
	void SetSprBlend( int PixelFormat , int opt , int grade);

	BOOL RotateZoomSprAuto2(int x, int y, xSprite *pXSpr, int sprnum, int tx, int ty, float angle, int opt, int *ilpCenterPosX, int *ilpCenterPosY);


public:
	LPBITMAPINFO	lpBmpInfo;			// 비트맵 정보
	LPLOGPALETTE	lpLogPal;			// 로그 파레트 포인터
	HPALETTE		hPal;				// DIB형식의 팔레트 핸들
	HBITMAP			hBmp;				// 비트맵의 핸들(CreateDIBSection()으로 생성된 핸들)
	DWORD			nAllocSize;

	void*			lpBit;				// 페이지 포인터
	LONG			lPitch;				// PITCH(4의배수) - 실제의 페이지의 가로 사이즈
	LONG			lPitchHalf;			// lPitch/2
	LONG			width, height;		// 페이지의 유효 범위 길이(최대 클리핑 지역)
	WORD			nBitNum;			// 비트수
	LONG			nBPD;				// 도트당 바이트 수
	WORD			nColorNum;			// 칼라수

	BOOL			bCreateAlphaTable8;	// 8비트 알파 테이블을 생성하였는가?
	BOOL			bEnableTempClip;	// 임시 클리핑 영역의 활성화 여부
	int				TClipX1, TClipY1, TClipX2, TClipY2;	// 임시 클리핑 영역
	int				MirrorX;			// 미러 찍기 정보

	// DX용
	CDirectX*       pDirectx;

	// 씬 그리기 효과 설정
	BOOL			bScnEffect;
	SCNEFFECTDESC	ScnEffect;

	// 멀티씬 관련
	int  iDrawingTrackBegin;
	int  iDrawingTrackEnd;

	// 페이지 정보관련
	int  xpFlip, ypFlip;
	float  iZoomX;

	// Attach, Detach	
	BOOL bAttached;						// Page가 Attach 되었는가?
};
