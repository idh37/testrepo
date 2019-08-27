#ifndef __XSPRITE_H__
#define __XSPRITE_H__

#ifndef __AFXWIN_H__
#include <windows.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "xAniFile.h"
//#include "xCanvas.h"
//#include "xAniMan.h"

//#include "Graphic.h"

#define swap(a,b) {(a)^=(b); (b)^=(a); (a)^=(b);}

extern WORD RGB555_TO_RGB565(WORD color); // RGB555 --> RGB565
extern WORD RGB565_TO_RGB555(WORD color); // RGB565 --> RGB555
extern WORD RGB555_TO_BGR565(WORD color); // RGB555 --> BGR565
extern WORD RGB565_TO_BGR565(WORD color); // RGB565 --> BGR565
extern void ANALYSIS_RGB555(WORD color, WORD* r, WORD* g, WORD* b); // 16비트(555)의 RGB를 분석한다
extern void ANALYSIS_RGB565(WORD color, WORD* r, WORD* g, WORD* b); // 16비트(565)의 RGB를 분석한다

//================================= 스프라이트 버젼 3.0 ======================================//


typedef struct         // 스프라이트의 파일 헤더
{
    char  Identifier[16];      // 스프라이트 인식문자
    short Version;             // 스프라이트 버젼
	short BPD;                 // 도트당 바이트수
	short PixelFormat;         // 픽셀 포멧(값= 555, 565)
    short TotalSprNum;         // 총 스프라이트 개수
	long  CharacterNum;        // 케릭터의 개수
} SPRFILEHEAD;
typedef SPRFILEHEAD* LPSPRFILEHEAD;

typedef struct         // 충돌, 공격좌표 구조체
{
	short attrib;              // 좌표의 속성
    short x1,y1;               // 좌상의 좌표
	short x2,y2;               // 우하의 좌표
	char reserved[2];
} SPRPOS;
typedef SPRPOS* LPSPRPOS;

typedef struct         // 각 스프라이트의 헤더
{
    BYTE  compress;            // 압축여부
	WORD  colorkey;            // 컬러키
	short rx, ry;              // 고유 상대좌표
    short xl, yl;              // X, Y축 길이
    short mx, my;              // 중심좌표
	short hitNum;              // 충돌좌표의 개수
	short atckNum;             // 공격좌표의 개수
    unsigned long dsize;       // 스프라이트 데이타의 Byte 사이즈(압축시 필요)
	short PixelFormat;         // 픽셀 포멧(값= 555, 565, 4444)
	char  SprFormat;		   // 스프라이트 포멧(0:스프라이트 1:JPG)
	char  Dithering;		   // JPG포멧인경우 디더링 여부(0:디더링없음 1:디더링)
	BYTE  byUserVal;		  // 사용자 정의 변수
} SPRHEAD30;
typedef SPRHEAD30* LPSPRHEAD30;

typedef struct         // 액션 헤더
{
    BYTE  id;                  // 액션 번호
	BYTE  attrib;              // 액션의 속성
	short frameNum;            // 액션 프레임의 개수
} ACTIONHEAD;
typedef ACTIONHEAD* LPACTIONHEAD;

typedef struct         // 각 액션 프레임 구조체
{
    short frame;               // 프레임 번호
	long  delay;               // 지연 프레임 카운트
	short rx, ry;              // 프레임의 상대좌표
	short flag;                // 프레임의 속성 플레그
} ACTIONFRAME;
typedef ACTIONFRAME* LPACTIONFRAME;

//------------------------------------------------------------------------------

typedef struct          // 스프라이트 데이타 구조체(메모리용)
{
    int compress;            // 압축여부
	WORD colorkey;           // 컬러키
	int xl, yl;              // X, Y축 길이
    int rx, ry;              // 상대좌표
    int mx, my;              // 중심좌표
	int hitNum;              // 충돌좌표의 개수
	int atckNum;             // 공격좌표의 개수
	unsigned long  dsize;    // 스프라이트 데이타의 사이즈(압축시 필요)
	short PixelFormat;       // 픽셀 포멧(값= 555, 565, 4444)
	char SprFormat;			 // 파일 포멧(0:스프라이트 1:JPG)
	char Dithering;			 // JPG포멧인경우 디더링 여부(0:디더링없음 1:디더링)
	BYTE  byUserVal;		 // 사용자 정의 변수
	SPRPOS* hit;             // 충돌좌표 구조체의 포인터
	SPRPOS* atck;            // 공격좌표 구조체의 포인터
    WORD*   data;            // 스프라이트 데이타 포인터
	//////////////////////
	unsigned long  jpgsize;  // JPG포맷인경우 이미지 사이즈
	BYTE*	pJpgData;		 // JPG포맷인경우 데이터 포인터
	//////////////////////
} SPRITE30;


class xSprite
{
public:

	///// 공통 변수 선언
	SPRFILEHEAD sfhead;                  // 스프라이트 파일 헤더
	SPRITE30 *spr;                       // 스프라이트 정보 구조체
	xAniFile ani;                        // 에니메이션 데이터 클레스

	///// 생성자, 소멸자
	xSprite();
	~xSprite();

	//// 스프라이트 로드,세이브,제거함수
	BOOL Load(char *name);
	BOOL Load(char *name,  int ScreenRGBMode);
	BOOL Save(char *name);
	void Remove();

	//// 특수기능 함수
	BOOL Compress(LONG snum);                       // 스프라이트 프레임 압축
	BOOL DeCompress(LONG snum);                     // 스프라이트 프레임 압축 해제
	BOOL ChangePixelFormat(int NewForm);            // 픽셀 포맷을 바꾼다(555,565,-565)
	void OptimizeSize(int snum, BOOL fixPos=TRUE);	// 스프라이트 사이즈를 최적화한다
	BOOL TransToSpr(int xl, int yl, WORD *pImage, WORD wColorKey);  // 특정 이미지를 스프라이트로 만든다
	BOOL NewSpace(int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress);
	BOOL Insert(LONG snum, int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress);  // 스프라이트 컷 삽입
	BOOL InsertByCopy(LONG snum, int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress);
};



#endif

