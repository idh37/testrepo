#pragma once
//#ifndef __GRAPHMOD_H__
//#define __GRAPHMOD_H__
//
//
//#include <windows.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <malloc.h>
//#include <dos.h>
//#include <conio.h>
//#include <string.h>
#include "GraphicDXDefine.h"


#define dRGB565			565 
#define dRGB555			555
#define dBGR565			(-565)

#define dGRAY			0
#define dGREEN			1
#define dRED			2
#define dBLUE			3
#define dYELLOW			4 
#define dCYAN			5

// RGB �������� ����
#define R555(r)			(((r)&0x7c00)>>10)
#define G555(g)			(((g)&0x03e0)>>5)
#define B555(b)			((b)&0x001f)
#define R565(r)			(((r)&0xf800)>>11)
#define G565(g)			(((g)&0x07e0)>>5)
#define B565(b)			((b)&0x001f)

#define	d2HM_RGB565				0xf7de
#define	d4HM_RGB565				0xf7def7de
#define	d4AM_RGB565_1			0xf81f07e0
#define	d4AM_RGB565_2			0x07e0f81f


#define	d2HM_RGB555				0x7bde
#define	d4HM_RGB555				0x7bde7bde
#define	d4AM_RGB555_1			0x7c1f03e0		// == 1111100000111110000001111100000b
#define	d4AM_RGB555_2			0x03e07c1f		// == 0000011111000000111110000011111b
#define	d2AM_RGB555_1			((WORD)0x03e0)	// == 0000001111100000b
#define	d2AM_RGB555_2			((WORD)0x7c1f)	// == 0111110000011111b

///////////////////////////// ����Ʈ ��ũ�� ///////////////////////////////

#define HBMIXW_555(tarw, srcw) ((((tarw) & d2HM_RGB555)>>1)+(((srcw) & d2HM_RGB555)>>1))

#define ALPHAMIXW_555(tarw, srcw, talpha, salpha) \
(\
	( (WORD)(((((tarw) & d2AM_RGB555_1) >> 5) * (talpha)) & d2AM_RGB555_1) | (WORD)(((((tarw) & d2AM_RGB555_2) * (talpha)) >> 5) & d2AM_RGB555_2) )\
	+ \
	( (WORD)(((((srcw) & d2AM_RGB555_1) >> 5) * (salpha)) & d2AM_RGB555_1) | (WORD)(((((srcw) & d2AM_RGB555_2) * (salpha)) >> 5) & d2AM_RGB555_2) )\
)

#define LEMIXW_555_FAST(tarw, srcw) (LIGHT_R[R555((srcw))][R555((tarw))] | LIGHT_G[G555((srcw))][G555((tarw))] | LIGHT_B[B555((srcw))][B555((tarw))])
#define LEMIXW_555(tarw, srcw, alpha) (LIGHT_R[ABGRADE[(alpha)][R555((srcw))]][R555((tarw))] | LIGHT_G[ABGRADE[(alpha)][G555((srcw))]][G555((tarw))] | LIGHT_B[ABGRADE[(alpha)][B555((srcw))]][B555((tarw))])

#define LEMIXW_565_FAST(tarw, srcw) (LIGHT_R[R555((srcw))][R565((tarw))] | LIGHT_G[G555((srcw))][G565((tarw))] | LIGHT_B[B555((srcw))][B565((tarw))])
#define LEMIXW_565(tarw, srcw, alpha) (LIGHT_R[ABGRADE[(alpha)][R555((srcw))]][R565((tarw))] | LIGHT_G[ABGRADE[(alpha)][G555((srcw))]][G565((tarw))] | LIGHT_B[ABGRADE[(alpha)][B555((srcw))]][B565((tarw))])

#define DEMIXW_555_FAST(tarw, srcw) (DARK_R[R555((srcw))][R555((tarw))] | DARK_G[G555((srcw))][G555((tarw))] | DARK_B[B555((srcw))][B555((tarw))])
#define DEMIXW_555(tarw, srcw, alpha) (DARK_R[RVS_ABGRADE[(alpha)][R555((srcw))]][R555((tarw))] | DARK_G[RVS_ABGRADE[(alpha)][G555((srcw))]][G555((tarw))] | DARK_B[RVS_ABGRADE[(alpha)][B555((srcw))]][B555((tarw))])

#define DEMIXW_565_FAST(tarw, srcw) (DARK_R[R555((srcw))][R565((tarw))] | DARK_G[G555((srcw))][G565((tarw))] | DARK_B[B555((srcw))][B565((tarw))])
#define DEMIXW_565(tarw, srcw, alpha) (DARK_R[RVS_ABGRADE[(alpha)][R555((srcw))]][R565((tarw))] | DARK_G[RVS_ABGRADE[(alpha)][G555((srcw))]][G565((tarw))] | DARK_B[RVS_ABGRADE[(alpha)][B555((srcw))]][B565((tarw))])


////////////////////////////////////////////////////////////////////////////

extern	WORD	(*RGBmix)(int r,int g,int b);	// 16��Ʈ �÷� RGB ���� �Լ�
extern	WORD	RGB565mix(int r,int g,int b);
extern	WORD	RGB555mix(int r,int g,int b);
extern	WORD	BGR565mix(int r,int g,int b);

extern	WORD	R[32][32];						// ���� ���̺�
extern	WORD	G[64][32];
extern	WORD	B[32][32];

extern	WORD	LIGHT_R[32][32];				// ������ ���� ���̺�
extern	WORD	LIGHT_G[64][64];
extern	WORD	LIGHT_B[32][32];

extern	WORD	DARK_R[32][32];					// Darken Effect�� ���� ���̺�
extern	WORD	DARK_G[64][64];
extern	WORD	DARK_B[32][32];

extern	int		ABGRADE[32][32];				// ���� ���� �׷��̵� ���̺�
extern	int     RVS_ABGRADE[32][32];			// ������ ���� ���� �׷��̵� ���̺� (Darken Effect ���� ��ȯ� ���)

extern	int		_MXL, _MYL;						// ȭ���� X,Y����
extern	int		_CX1, _CY1, _CX2, _CY2;			// Ŭ���� ���� ��ǥ
extern	int		_CXL, _CYL;						// Ŭ���� ������ X,Y����
extern	DWORD	_LPITCH;						// ���ǽ�(Ȥ�� ����������)�� lPitch

/////////////////////////////////////////////////////////////////////////////////////////////////

#define GET_CSPLINEPTR(pCsp, line)  ((pCsp) + (*(long*)((pCsp)+((line)<<1))))
#define BEGINPUTSPR 1  // ���� ��������Ʈ Ŭ���ο� ���(������ ��½��ۺ� ��������� �˸�)
#define ENDPUTSPR   2  // ���� ��������Ʈ Ŭ���ο� ���(���� ��º� ó�� ������ �˸�)

/////////////////////////////////////////////////////////////////////////////////////////////////

// 4����Ʈ �޸� ���� ���� �Լ�(����: WORD���� ����, Len=WORD����)
extern void _fastcall RvrsCpy(WORD *tar, WORD *sor, int Len);

// ���������� �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall MemCpyHB555(WORD *tar, WORD *sor, int Len);
extern void _fastcall MemCpyHB565(WORD *tar, WORD *sor, int Len);

// ���������� �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall RvrsCpyHB555(WORD *tar, WORD *sor, int Len);
extern void _fastcall RvrsCpyHB565(WORD *tar, WORD *sor, int Len);

// ��Ƽ ���� �Լ�(����: WORD���� ����, Len=WORD����)
extern void _fastcall MemCpyAnti555(WORD *tar, WORD *sor, int Len);
extern void _fastcall MemCpyAnti565(WORD *tar, WORD *sor, int Len);

// �۷ο� ȿ�� ���� �Լ�(����: WORD���� ����, Len=WORD����)
extern void _fastcall MemCpyGlow555(WORD *tar, WORD *sor, int Len, WORD color, int grade);
extern void _fastcall MemCpyGlow565(WORD *tar, WORD *sor, int Len, WORD color, int grade);

// �۷ο� ȿ���� �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall RvrsCpyGlow555(WORD *tar, WORD *sor, int Len, WORD color, int grade);
extern void _fastcall RvrsCpyGlow565(WORD *tar, WORD *sor, int Len, WORD color, int grade);

// �׷��� ������ ȿ���� �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall MemCpyGrayscl555(WORD *tar, WORD *sor, int Len);
extern void _fastcall MemCpyGrayscl565(WORD *tar, WORD *sor, int Len);

// �׷��� ������ ȿ���� �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall RvrsCpyGrayscl555(WORD *tar, WORD *sor, int Len);
extern void _fastcall RvrsCpyGrayscl565(WORD *tar, WORD *sor, int Len);

// �ٴܰ� ���������� �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall MemCpyGB555(WORD *tar, WORD *sor, int Len, int grade);
extern void _fastcall MemCpyGB565(WORD *tar, WORD *sor, int Len, int grade);

// �ٴܰ� ���������� �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall RvrsCpyGB555(WORD *tar, WORD *sor, int Len, int grade);
extern void _fastcall RvrsCpyGB565(WORD *tar, WORD *sor, int Len, int grade);

// ���ĺ�������  �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall MemCpyAB555(WORD *tar, WORD *sor, int Len, BYTE* palpha, int grade);
extern void _fastcall MemCpyAB565(WORD *tar, WORD *sor, int Len, BYTE* palpha, int grade);

// ���ĺ�������  �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall RvrsCpyAB555(WORD *tar, WORD *sor, int Len, BYTE* palpha, int grade);
extern void _fastcall RvrsCpyAB565(WORD *tar, WORD *sor, int Len, BYTE* palpha, int grade);

// ����ȿ����  �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall MemCpyLE555(WORD *tar, WORD *sor, int Len, int grade);
extern void _fastcall MemCpyLE565(WORD *tar, WORD *sor, int Len, int grade);

// ����ȿ����  �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall RvrsCpyLE555(WORD *tar, WORD *sor, int Len, int grade);
extern void _fastcall RvrsCpyLE565(WORD *tar, WORD *sor, int Len, int grade);

// Darken ȿ����  �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall MemCpyDE555(WORD *tar, WORD *sor, int Len, int grade);
extern void _fastcall MemCpyDE565(WORD *tar, WORD *sor, int Len, int grade);

// Darken ȿ����  �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall RvrsCpyDE555(WORD *tar, WORD *sor, int Len, int grade);
extern void _fastcall RvrsCpyDE565(WORD *tar, WORD *sor, int Len, int grade);

// ����� �޸𸮸� �÷�Ű �����Ͽ� �ٴܰ� ���������� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall ColorKeyMemCpyGB555(WORD *tar, WORD *sor, WORD colorkey, int Len, int grade);


/////////////////////////////////////////////////////////////////////////////////////////////////

// �Լ� ������
extern void (_fastcall *MemCpyHB) (WORD *tar, WORD *sor, int Len);
extern void (_fastcall *RvrsCpyHB) (WORD *tar, WORD *sor, int Len);
extern void (_fastcall *MemCpyAnti) (WORD *tar, WORD *sor, int Len);
extern void (_fastcall *MemCpyGlow) (WORD *tar, WORD *sor, int Len, WORD color, int grade);
extern void (_fastcall *RvrsCpyGlow) (WORD *tar, WORD *sor, int Len, WORD color, int grade);
extern void (_fastcall *MemCpyGrayscl) (WORD *tar, WORD *sor, int Len);
extern void (_fastcall *RvrsCpyGrayscl) (WORD *tar, WORD *sor, int Len);
extern void (_fastcall *MemCpyGB) (WORD *tar, WORD *sor, int Len, int grade);
extern void (_fastcall *RvrsCpyGB) (WORD *tar, WORD *sor, int Len, int grade);
extern void (_fastcall *MemCpyAB) (WORD *tar, WORD *sor, int Len, BYTE* palpha, int grade);
extern void (_fastcall *RvrsCpyAB) (WORD *tar, WORD *sor, int Len, BYTE* palpha, int grade);
extern void (_fastcall *MemCpyLE) (WORD *tar, WORD *sor, int Len, int grade);
extern void (_fastcall *RvrsCpyLE) (WORD *tar, WORD *sor, int Len, int grade);
extern void (_fastcall *MemCpyDE) (WORD *tar, WORD *sor, int Len, int grade);
extern void (_fastcall *RvrsCpyDE) (WORD *tar, WORD *sor, int Len, int grade);
extern void (_fastcall *ColorKeyMemCpyGB) (WORD *tar, WORD *sor, WORD colorkey, int Len, int grade);

/////////////////////////////////////////////////////////////////////////////////////////////////

// �׷��� ���̺귯�� �ʱ�ȭ
BOOL InitGrpLib(int xl, int yl, DWORD lPitch, int PixelFormat);

// �������̺� �ʱ�ȭ
void InitAlphaTable(int PixelFormat);

// Ŭ���� ���� ����
void SetClipArea(int x1, int y1, int x2, int y2);


//============================ ������ ��� �Լ��� ==========================

// ���� �׸���(Ŭ����)
void DrawHLine(int x, int y, int len, WORD *tar, WORD color );

// ������ �׸���(Ŭ����)
void DrawVLine(int x, int y, int len, WORD *tar, WORD color );

// XOR ���� �׸���(Ŭ����)
void DrawHLineXOR(int x, int y, int len, WORD *tar);

// XOR ������ �׸���(Ŭ����)
void DrawVLineXOR(int x, int y, int len, WORD *tar);

// �� �׸���(�극���� �˰���)
void DrawBresLine(int xs, int ys, int xe, int ye, WORD *tar, WORD color );
void DrawBresLineXOR(int xs, int ys, int xe, int ye, WORD *tar);
void DrawBresLineGB(int xs, int ys, int xe, int ye, WORD *tar, WORD color, int alpha );

// ���� ä�� �ڽ� �׸���(Ŭ����)
void FillBox(int x1, int y1, int x2, int y2, WORD *tar, WORD color );

// �ٴܰ� ������ �ڽ� �׸���(Ŭ����)
void FillBoxGB(int x1, int y1, int x2, int y2, WORD *tar, WORD color, int grade);

// ���� �� �ڽ� �׸���(Ŭ����)
void DrawBox(int x1, int y1, int x2, int y2, WORD *tar, WORD color );

// ���� �� XOR�ڽ� �׸���(Ŭ����)
void DrawBoxXOR(int x1, int y1, int x2, int y2, WORD *tar);

//   ���ùڽ� �׸���(���� ����)
void DrawSelectBox(int x1, int y1, int x2, int y2, WORD *tar, WORD color);

//   ���ùڽ� �׸���(XOR)
void DrawXORSelectBox(int x1, int y1, int x2, int y2, WORD *tar);

// ���ǽ��� ������ ������ ü��
void FillPage(WORD *tar, WORD fillColor );

// ���ǽ��� �� ���
void PutPixel(int x, int y, WORD *tar, WORD color );

// Ư�� ���������� �̹����� ����
void GetImage(int x, int y, int xl, int yl, WORD *page, WORD *tar );

// �̹��� ��� �Լ�
void _PutImage(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor );
void _PutImageUD(int xp, int yp, short xLen, short yLen, WORD *tar, WORD *sor );
void _PutImage_GB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade);

// �Ϲ� ��������Ʈ�� ����ϴ� �Լ�
void _PutSpr(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey);
void _PutSprUD(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey );
void _PutSpr_GB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey, int grade);
void _PutSpr_LE(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey, int grade);
void _PutSpr_DE(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey, int grade);
void _PutSpr_AB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey, BYTE* alpha_table, int grade);

//////////////////////////// ���� ��������Ʈ ��� �Լ� ////////////////////////
void _PutCsp(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor );
void _PutCspLR(int xp, int yp, int xLen, int yLen, WORD* tar, WORD* sor );
void _PutCspUD(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);

//////////////////////////////// ������ ��� �Լ� /////////////////////////////
void _PutCsp_HB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor );
void _PutCspLR_HB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor );
void _PutCspUD_HB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor );

////////////////////////////// ��Ƽ �˶��̽� ��� �Լ� ////////////////////////
void _PutCsp_Anti(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor );

/////////////////////////////// �۷ο� ȿ�� ��� �Լ� /////////////////////////
void _PutCsp_Glow(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color, int grade );
void _PutCspLR_Glow(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color, int grade );
void _PutCspUD_Glow(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color, int grade );

/////////////////////////////// �׷��� ������ ȿ�� ��� �Լ� /////////////////////////
void _PutCsp_Grayscl(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);
void _PutCspLR_Grayscl(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);
void _PutCspUD_Grayscl(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);

/////////////////////////////// ��� ���� ȿ�� ��� �Լ� /////////////////////////
void _PutCsp_Mono(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color);
void _PutCspLR_Mono(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color);
void _PutCspUD_Mono(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color);

////////////////////////// ��Ƽ �׷��̵� ���� ��� �Լ� //////////////////////
void _PutCsp_GB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor ,int grade);
void _PutCspLR_GB(int xp, int yp, int xLen, int yLen, WORD* tar, WORD* sor, int grade);
void _PutCspUD_GB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade);

////////////////////////////// ���� ���� ��� �Լ� ///////////////////////////
void _PutCsp_AB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, BYTE* alpha_table, int grade);
void _PutCspLR_AB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, BYTE* alpha_table, int grade);
void _PutCspUD_AB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, BYTE* alpha_table, int grade);

////////////////////////////// ����ȿ�� ��� �Լ� ///////////////////////////
void _PutCsp_LE(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade);
void _PutCspLR_LE(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade);
void _PutCspUD_LE(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade);

////////////////////////////// Darken ȿ�� ��� �Լ� ///////////////////////////
void _PutCsp_DE(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade);
void _PutCspLR_DE(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade);
void _PutCspUD_DE(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade);

////////////////////////////// �ȼ����� ��ȯ ��� �Լ� ///////////////////////////
void _PutCsp_RGB555TORGB565(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);
void _PutCsp_RGB555TORGB24(int xp, int yp, int xLen, int yLen, BYTE *tar, int lpitch, WORD *sor);
void _PutCsp_RGB555TORGB32(int xp, int yp, int xLen, int yLen, DWORD *tar, int lpitch, WORD *sor);

//////////////////////// �Ϲ� ��������Ʈ ����Ʈ �浹 �˻� �Լ� /////////////////
BOOL _PointHitTestSpr(int xLen, int yLen, WORD *sor, WORD colorkey, int txp, int typ );

//////////////////////// ���� ��������Ʈ ����Ʈ �浹 �˻� �Լ� /////////////////
BOOL _PointHitTestCsp(int xLen, int yLen, WORD *sor, int txp, int typ );



//================================== GraphMod_Zoom.cpp ===================================


//////////////////////// �Ϲ� ��������Ʈ ����,�ƿ� ��� �Լ� /////////////////
void _ZoomSpr(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey);
void _ZoomSpr_GB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, int grade);
void _ZoomSpr_LE(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, int grade);
void _ZoomSpr_DE(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, int grade);
void _ZoomSpr_AB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, BYTE* alpha_table, int grade);

//////////////////////// ���� ��������Ʈ ����,�ƿ� ��� �Լ� /////////////////
void _ZoomCsp(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor);
void _ZoomCsp_HB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor);
void _ZoomCsp_GB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, int alpha);
void _ZoomCsp_LE(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, int grade);
void _ZoomCsp_DE(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, int grade);
void _ZoomCsp_AB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, BYTE* alpha_table, int grade);

//////////////////////// ���� ��������Ʈ ȸ�� & ����,�ƿ� ��� �Լ� /////////////////
//void _RotateZoomCsp(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, float angle, WORD *sor);



//=============================== GraphMod_RotateZoom.cpp ================================



/////////////////////////// Types

// ������ �ؽ��� ��ǥ(2�� 16������ ������ ��)
struct TEXCOORDi
{
	int u, v, oz;
};

// �Ǽ��� �ؽ��� ��ǥ
struct TEXCOORDf
{
	union {
		float u;
		float uz; 
	};
	union {
		float v;
		float vz;
	};
	float oz;
};

// �ؽ��� ���� ���� �׸��� �Լ� ������
typedef void (_fastcall *PFN_HLINETEXTURE)(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2);

/////////////////////////// ���� ��� ���� �Լ� ������ ����

void _fastcall HLineTexture(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2);
void _fastcall HLineTexture_HB(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2);
void _fastcall HLineTexture_GB(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2);
void _fastcall HLineTexture_LE(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2);
void _fastcall HLineTexture_DE(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2);
void _fastcall HLineTexture_AB(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2);

/////////////////////////// Util Function

float _GetAngle(int x1, int y1, int x2, int y2);
POINT _GetMovePoint(int sx, int sy, float angle, float distance);
void _RotatePoint(POINT* pPoint, float angle);
void _RotatePoint(float* pxp, float* pyp, float angle);
void _AxisRotatePoint(POINT* pPoint, float angle, POINT axis);
void _AxisRotatePoint(float* pxp, float* pyp, float angle, float axis_x, float axis_y);

/////////////////////////// Function proto types

void _SetRenderTarget(WORD* bits);
void _SetTexture(WORD* bits, int width, int height, BYTE* alphatable);
void _SetTextureAlpha(int alpha);
void _SetTextureColorKey(WORD color);
void _SetBlendingFunc_HLineTexture(PFN_HLINETEXTURE pfnNew);

//////////////////////// �Ϲ� ��������Ʈ ȸ�� & ����,�ƿ� ��� �Լ� /////////////////
void _RotateZoomSpr( int xp,int yp, int txl, int tyl, float angle);
