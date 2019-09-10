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
extern void ANALYSIS_RGB555(WORD color, WORD* r, WORD* g, WORD* b); // 16��Ʈ(555)�� RGB�� �м��Ѵ�
extern void ANALYSIS_RGB565(WORD color, WORD* r, WORD* g, WORD* b); // 16��Ʈ(565)�� RGB�� �м��Ѵ�

//================================= ��������Ʈ ���� 3.0 ======================================//


typedef struct         // ��������Ʈ�� ���� ���
{
    char  Identifier[16];      // ��������Ʈ �νĹ���
    short Version;             // ��������Ʈ ����
	short BPD;                 // ��Ʈ�� ����Ʈ��
	short PixelFormat;         // �ȼ� ����(��= 555, 565)
    short TotalSprNum;         // �� ��������Ʈ ����
	long  CharacterNum;        // �ɸ����� ����
} SPRFILEHEAD;
typedef SPRFILEHEAD* LPSPRFILEHEAD;

typedef struct         // �浹, ������ǥ ����ü
{
	short attrib;              // ��ǥ�� �Ӽ�
    short x1,y1;               // �»��� ��ǥ
	short x2,y2;               // ������ ��ǥ
	char reserved[2];
} SPRPOS;
typedef SPRPOS* LPSPRPOS;

typedef struct         // �� ��������Ʈ�� ���
{
    BYTE  compress;            // ���࿩��
	WORD  colorkey;            // �÷�Ű
	short rx, ry;              // ���� �����ǥ
    short xl, yl;              // X, Y�� ����
    short mx, my;              // �߽���ǥ
	short hitNum;              // �浹��ǥ�� ����
	short atckNum;             // ������ǥ�� ����
    unsigned long dsize;       // ��������Ʈ ����Ÿ�� Byte ������(����� �ʿ�)
	short PixelFormat;         // �ȼ� ����(��= 555, 565, 4444)
	char  SprFormat;		   // ��������Ʈ ����(0:��������Ʈ 1:JPG)
	char  Dithering;		   // JPG�����ΰ�� ����� ����(0:��������� 1:�����)
	BYTE  byUserVal;		  // ����� ���� ����
} SPRHEAD30;
typedef SPRHEAD30* LPSPRHEAD30;

typedef struct         // �׼� ���
{
    BYTE  id;                  // �׼� ��ȣ
	BYTE  attrib;              // �׼��� �Ӽ�
	short frameNum;            // �׼� �������� ����
} ACTIONHEAD;
typedef ACTIONHEAD* LPACTIONHEAD;

typedef struct         // �� �׼� ������ ����ü
{
    short frame;               // ������ ��ȣ
	long  delay;               // ���� ������ ī��Ʈ
	short rx, ry;              // �������� �����ǥ
	short flag;                // �������� �Ӽ� �÷���
} ACTIONFRAME;
typedef ACTIONFRAME* LPACTIONFRAME;

//------------------------------------------------------------------------------

typedef struct          // ��������Ʈ ����Ÿ ����ü(�޸𸮿�)
{
    int compress;            // ���࿩��
	WORD colorkey;           // �÷�Ű
	int xl, yl;              // X, Y�� ����
    int rx, ry;              // �����ǥ
    int mx, my;              // �߽���ǥ
	int hitNum;              // �浹��ǥ�� ����
	int atckNum;             // ������ǥ�� ����
	unsigned long  dsize;    // ��������Ʈ ����Ÿ�� ������(����� �ʿ�)
	short PixelFormat;       // �ȼ� ����(��= 555, 565, 4444)
	char SprFormat;			 // ���� ����(0:��������Ʈ 1:JPG)
	char Dithering;			 // JPG�����ΰ�� ����� ����(0:��������� 1:�����)
	BYTE  byUserVal;		 // ����� ���� ����
	SPRPOS* hit;             // �浹��ǥ ����ü�� ������
	SPRPOS* atck;            // ������ǥ ����ü�� ������
    WORD*   data;            // ��������Ʈ ����Ÿ ������
	//////////////////////
	unsigned long  jpgsize;  // JPG�����ΰ�� �̹��� ������
	BYTE*	pJpgData;		 // JPG�����ΰ�� ������ ������
	//////////////////////
} SPRITE30;


class xSprite
{
public:

	///// ���� ���� ����
	SPRFILEHEAD sfhead;                  // ��������Ʈ ���� ���
	SPRITE30 *spr;                       // ��������Ʈ ���� ����ü
	xAniFile ani;                        // ���ϸ��̼� ������ Ŭ����

	///// ������, �Ҹ���
	xSprite();
	~xSprite();

	//// ��������Ʈ �ε�,���̺�,�����Լ�
	BOOL Load(char *name);
	BOOL Load(char *name,  int ScreenRGBMode);
	BOOL Save(char *name);
	void Remove();

	//// Ư����� �Լ�
	BOOL Compress(LONG snum);                       // ��������Ʈ ������ ����
	BOOL DeCompress(LONG snum);                     // ��������Ʈ ������ ���� ����
	BOOL ChangePixelFormat(int NewForm);            // �ȼ� ������ �ٲ۴�(555,565,-565)
	void OptimizeSize(int snum, BOOL fixPos=TRUE);	// ��������Ʈ ����� ����ȭ�Ѵ�
	BOOL TransToSpr(int xl, int yl, WORD *pImage, WORD wColorKey);  // Ư�� �̹����� ��������Ʈ�� �����
	BOOL NewSpace(int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress);
	BOOL Insert(LONG snum, int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress);  // ��������Ʈ �� ����
	BOOL InsertByCopy(LONG snum, int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress);
};



#endif

