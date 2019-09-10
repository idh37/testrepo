#include "StdAfx.h"

#include <windows.h>
#include "GraphMod.h"

WORD	R[32][32];			// ���� ���̺�
WORD	G[64][32];
WORD	B[32][32];

WORD	LIGHT_R[32][32];	// ������ ���� ���̺�
WORD	LIGHT_G[64][64];
WORD	LIGHT_B[32][32];

WORD	DARK_R[32][32];		// Darken Effect�� ���� ���̺�
WORD	DARK_G[64][64];
WORD	DARK_B[32][32];

//int _CX1, _CY1, _CX2, _CY2;  // Ŭ���� ���� ��ǥ
//DWORD _LPITCH;               // ���ǽ�(Ȥ�� ����������)�� lPitch

WORD RGB565mix(int r,int g,int b)
{	
	return 	(WORD) (  (((WORD)(r<<11))) | (((WORD)(g<<5))) | ((WORD)b)  );
}

WORD RGB555mix(int r,int g,int b)
{	
	//g>>=1;
	return 	(WORD) (  (((WORD)(r<<10))) | (((WORD)(g<<5))) | ((WORD)b)  );
}

WORD BGR565mix(int r,int g,int b)
{	
	g<<=1;
	return 	(WORD) (  (((WORD)(b<<11))) | (((WORD)(g<<5))) | ((WORD)r)  );
}

// RGBȥ���� �ִ� �Լ� ������..
// �Է� RGB ���� RGB555 ��� ���� �ȼ� ���˿� ����..(RGB������ �ִ밪�� 31)
WORD	(*RGBmix  )(int r,int g,int b ) = RGB555mix;

// �Լ� ������
void (_fastcall *MemCpyHB) (WORD *tar, WORD *sor, int Len);
void (_fastcall *MemCpyAnti) (WORD *tar, WORD *sor, int Len);
void (_fastcall *MemCpyGlow) (WORD *tar, WORD *sor, int Len, WORD color, int alpha);
void (_fastcall *MemCpyGB) (WORD *tar, WORD *sor, int Len, int alpha);
void (_fastcall *RvrsCpyGB) (WORD *tar, WORD *sor, int Len, int alpha);
void (_fastcall *MemCpyAB) (WORD *tar, WORD *sor, int Len, int alpha);
void (_fastcall *MemCpyLE) (WORD *tar, WORD *sor, int Len);
void (_fastcall *MemCpyDE) (WORD *tar, WORD *sor, int Len);


/*---------------------------------------------------------------------------
   Ŭ���� ���� ���� �Լ�
---------------------------------------------------------------------------*/
/*
void SetClipArea(int x1, int y1, int x2, int y2)
{
    _CX1 = x1;
    _CY1 = y1;
    _CX2 = x2;
    _CY2 = y2;
}
*/


/*---------------------------------------------------------------------------
   �׷��� ���̺귯�� �ʱ�ȭ
---------------------------------------------------------------------------*/
BOOL InitGrpLib(int PixelFormat)
{
	if(PixelFormat==565)
	{
		RGBmix = RGB565mix;

		MemCpyHB = MemCpyHB565;
		MemCpyAnti = MemCpyAnti565;
		MemCpyGlow = MemCpyGlow565;
		MemCpyGB = MemCpyGB565;
		RvrsCpyGB = RvrsCpyGB565;
		MemCpyAB = MemCpyAB565;
		MemCpyLE = MemCpyLE565;
		MemCpyDE = MemCpyDE565;
	}
	else
	{
		RGBmix = RGB555mix;

		MemCpyHB = MemCpyHB555;
		MemCpyAnti = MemCpyAnti555;
		MemCpyGlow = MemCpyGlow555;
		MemCpyGB = MemCpyGB555;
		RvrsCpyGB = RvrsCpyGB555;
		MemCpyAB = MemCpyAB555;
		MemCpyLE = MemCpyLE555;
		MemCpyDE = MemCpyDE555;
	}

	return TRUE;
}

static BOOL g_bInitedAlphaTable = FALSE;

void InitAlphaTable(int PixelFormat)
{
	if(g_bInitedAlphaTable) return;

	g_bInitedAlphaTable = TRUE;

	int i,j;
	WORD val;

	if(PixelFormat==dRGB555)
	for(i=0; i<32; i++)
	{
		for(j=0; j<32; j++)
		{
			// ���� ���� ���̺� ��� (i=source color, j=alpha depth)
			R[i][j] = ((i * j)>>5)<<10;
			G[i][j] = ((i * j)>>5)<<5;
			B[i][j] = ((i * j)>>5);

			// ���� ���̺� ��� (i=source color, j=background color)
			val = min((j + (31-j)*i/32), 31);
			LIGHT_R[i][j] = val<<10;
			LIGHT_G[i][j] = val<<5;
			LIGHT_B[i][j] = val;

			// Darken Effect ���̺� ��� (i=source color, j=background color)
			val = max(j - j*(31-i)/32, 0);
			DARK_R[i][j] = val<<10;
			DARK_G[i][j] = val<<5;
			DARK_B[i][j] = val;
		}
	}
	else 
	for(i=0; i<64; i++) // Green�� ������ 0-63�̱� ����
	{
		for(j=0; j<64; j++)
		{
			// ���� ���� ���̺� ��� (i=source color, j=alpha depth)
			if(i<32 && j<32) R[i][j] = (i * j / 32)<<11;
			if(j<32) G[i][j] = ((i * j)>>5)<<5;
			if(i<32 && j<32) B[i][j] = (i * j / 32);

			// ���� ���̺� ��� (i=source color, j=background color)
			if(i<32 && j<32) 
			{
				val = min((j + (31-j)*i/32), 31);
				LIGHT_R[i][j] = val<<11;
				LIGHT_B[i][j] = val;
			}
			val = min((j + (63-j)*i/64), 63);
			LIGHT_G[i][j] = val<<5;

			// Darken Effect ���̺� ��� (i=source color, j=background color)
			if(i<32 && j<32) 
			{
				val = max(j- j*(31-i)/32, 0);
				DARK_R[i][j] = val<<11;
				DARK_B[i][j] = val;
			}
			val = max(j - j*(63-i)/64, 0);
			DARK_G[i][j] = val<<5;
		}
	}
}


/*---------------------------------------------------------------------------
   �׷��� ���̺귯�� ����
---------------------------------------------------------------------------*/
void RemoveGrpLib()
{
    //CloseDirectDraw();
	//if(PagePtr!=NULL) free(PagePtr);
}

///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////




/*---------------------------------------------------------------------------
   ���� �׸���(Ŭ����)
---------------------------------------------------------------------------*/
void DrawHLine( int x, int y, int len, WORD *tar, WORD color, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
    if(y < _CY1 || y > _CY2 || x+len-1 < _CX1 || x > _CX2) return;

    if(x<_CX1) x = _CX1;
    if(x + len -1 > _CX2) len = _CX2 - x +1;

    tar += _LPITCH * y + x;

	for(int i=0; i<len; i++)
	{
		*(tar+i) = color;
	}
}

/*---------------------------------------------------------------------------
   ������ �׸���(Ŭ����)
---------------------------------------------------------------------------*/
void DrawVLine( int x, int y, int len, WORD *tar, WORD color, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
    register int i;
    if(x < _CX1 || x > _CX2 || y+len-1 < _CY1 || y > _CY2) return;

    if(y<_CY1) y = _CY1;
    if(y + len -1 > _CY2) len = _CY2 - y +1;

    tar += _LPITCH * y + x;

    for(i=0; i<len; i++)
    {
        *tar = color;
        tar += _LPITCH;
    }
}

/*---------------------------------------------------------------------------
   XOR ���� �׸���(Ŭ����)
---------------------------------------------------------------------------*/
void DrawHLineXOR( int x, int y, int len, WORD *tar, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH)
{
	int i;
    if(y < _CY1 || y > _CY2 || x+len-1 < _CX1 || x > _CX2) return;

    if(x<_CX1) x = _CX1;
    if(x + len -1 > _CX2) len = _CX2 - x +1;

    tar += _LPITCH * y + x;

	for(i=1; i<len; i++) *(tar+i ) ^= 0xFFFF;
}

/*---------------------------------------------------------------------------
   XOR ������ �׸���(Ŭ����)
---------------------------------------------------------------------------*/
void DrawVLineXOR( int x, int y, int len, WORD *tar, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH)
{
    register int i;
    if(x < _CX1 || x > _CX2 || y+len-1 < _CY1 || y > _CY2) return;

    if(y<_CY1) y = _CY1;
    if(y + len -1 > _CY2) len = _CY2 - y +1;

    tar += _LPITCH * y + x;

    for(i=0; i<len; i++)
    {
        *tar ^= 0xFFFF;
        tar += _LPITCH;
    }
}


/*---------------------------------------------------------------------------
   ���� ä�� �ڽ� �׸���(Ŭ����)
---------------------------------------------------------------------------*/
void FillBox( int x1, int y1, int x2, int y2, WORD *tar, WORD color, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
    register int i,j;
    register int xl;

    if(x1<_CX1) { x1 = _CX1;}
    if(x2>_CX2) { x2 = _CX2;}
    if(y1<_CY1) { y1 = _CY1;}
    if(y2>_CY2) { y2 = _CY2;}

    xl = x2-(x1-1);

    if(xl<=0) return;

    tar += _LPITCH * y1 + x1;

    for(i=y1; i<=y2; i++)
    {
		for(j=0; j<xl; j++)
		{
			*(tar+j) = color;
		}
        tar += _LPITCH;
    }
}

// �ٴܰ� ���������� �޸𸮸� ����(����: WORD���� ����)
void _fastcall MemSetGB555(WORD *tar, WORD color, int Len, int alpha)
{
	int HalfLen = Len>>1;
	int i;

	UINT	first,second,sc4,ic4;
	//WORD	sc2,ic2;
	int dest_alpha = 31-alpha;

	UINT sor = color;
	sor = (sor<<16)|color;
	UINT *p4_scr=(UINT *)tar;
	UINT *p4_spr= &sor;
	
	for (i=0; i<HalfLen; i++)
	{
		/*
		*p4_scr =( ((((*p4_scr & d4AM_RGB555_1)>>5)*dest_alpha) & d4AM_RGB555_1) | // Dest first
				  ((((*p4_scr & d4AM_RGB555_2)*dest_alpha)>>5) & d4AM_RGB555_2)  ) // Dest Second
				  +
				 ( ((((*p4_spr & d4AM_RGB555_1)>>5)*alpha) & d4AM_RGB555_1) |      // Source first
				  ((((*p4_spr &d4AM_RGB555_2)*alpha)>>5)&d4AM_RGB555_2)   );       // Source Second
				  */

		first	=	*p4_scr & d4AM_RGB555_1;
		first	>>=	5;
		first	*=	dest_alpha;
		first	&=	d4AM_RGB555_1;

		second	=	*p4_scr & d4AM_RGB555_2;
		second	*=	dest_alpha;
		second	>>=	5;
		second	&=	d4AM_RGB555_2;

		sc4		=	first | second;
		
		first	=	*p4_spr & d4AM_RGB555_1;
		first	>>=	5;
		first	*=	alpha;
		first	&=	d4AM_RGB555_1;

		second	=	*p4_spr &d4AM_RGB555_2;
		second	*=	alpha;
		second	>>=	5;
		second	&=	d4AM_RGB555_2;

		ic4		=	first | second;


		*p4_scr  =	sc4+ic4;
		
		
		p4_scr++; 
		//p4_spr++;
	}
	if(Len%2)
	{
		WORD* p2_scr = (WORD*)p4_scr;
		*p2_scr	= ( (WORD)((((*p4_scr & d4AM_RGB555_1) >> 5) * dest_alpha) & d4AM_RGB555_1) | \
					((((*p4_scr & d4AM_RGB555_2)*dest_alpha)>>5) & d4AM_RGB555_2) )\
					+ \
				  ( (WORD)((((*p4_spr & d4AM_RGB555_1)>>5)*alpha) & d4AM_RGB555_1) | \
					((((*p4_spr & d4AM_RGB555_2)*alpha)>>5) & d4AM_RGB555_2) );
	}
}


/*---------------------------------------------------------------------------
   �ٴܰ� ������ �ڽ� �׸���(Ŭ����)
---------------------------------------------------------------------------*/
void FillBoxGB( int x1, int y1, int x2, int y2, WORD *tar, WORD color, int grade, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH)
{
    register int i;
    register int xl;

    if(x1<_CX1) { x1 = _CX1;}
    if(x2>_CX2) { x2 = _CX2;}
    if(y1<_CY1) { y1 = _CY1;}
    if(y2>_CY2) { y2 = _CY2;}

    //xl = x2-(x1-1);
	xl = (x2-x1)+1;

    if(xl<=0) return;

    tar += _LPITCH * y1 + x1;

    for(i=y1; i<=y2; i++)
    {
		MemSetGB555(tar, color, xl, grade);
        tar += _LPITCH;
    }
}


/*---------------------------------------------------------------------------
   ���� �� �ڽ� �׸���(Ŭ����)
---------------------------------------------------------------------------*/
void DrawBox( int x1, int y1, int x2, int y2, WORD *tar, WORD color, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
    register int i,yp;
    register int xl;
    int x=0,y=0,h=0,t=0;

    if(x1>_CX2 || x2<_CX1 || y1>_CY2 || y2<_CY1) return;

    if(x1<_CX1) { x1 = _CX1; x=1;}
    if(x2>_CX2) { x2 = _CX2; y=1;}
    if(y1<_CY1) { y1 = _CY1; h=1;}
    if(y2>_CY2) { y2 = _CY2; t=1;}

    xl = x2-x1+1;
	WORD* target;

    if(!h)
	{
		target = tar + y1*_LPITCH + x1;
		for(i=0; i<xl; i++) *(target+i) = color;
	}
    if(!t)
	{
		target = tar + y2*_LPITCH + x1;
		for(i=0; i<xl; i++) *(target+i) = color;
	}

    yp = y1*_LPITCH;

    for(i=y1; i<=y2; i++)
    {
        if(!x) *(tar+yp+x1) = color;
        if(!y) *(tar+yp+x2) = color;
        yp+=_LPITCH;
    }
}

/*---------------------------------------------------------------------------
   ���� �� XOR �ڽ� �׸���(Ŭ����)
---------------------------------------------------------------------------*/
void DrawBoxXOR( int x1, int y1, int x2, int y2, WORD *tar, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH)
{
    register int i,yp;
    register int xl;
    int x=0,y=0,h=0,t=0;

    if(x1>_CX2 || x2<_CX1 || y1>_CY2 || y2<_CY1) return;

    if(x1<_CX1) { x1 = _CX1; x=1;}
    if(x2>_CX2) { x2 = _CX2; y=1;}
    if(y1<_CY1) { y1 = _CY1; h=1;}
    if(y2>_CY2) { y2 = _CY2; t=1;}

    xl = x2-x1+1;

	WORD* temp1 = tar + y1*_LPITCH + x1;
	WORD* temp2 = tar + y2*_LPITCH + x1;

    for(i=1; i<xl-1; i++)
	{
		if(!h) *( temp1 +i ) ^= 0xFFFF;
		if(!t) *( temp2 +i ) ^= 0xFFFF;
	}

    yp = y1*_LPITCH;

    for(i=y1; i<=y2; i++)
    {
        if(!x) *(tar+yp+x1) ^= 0xFFFF;
        if(!y) *(tar+yp+x2) ^= 0xFFFF;
        yp+=_LPITCH;
    }
}

/*---------------------------------------------------------------------------
   ���ùڽ� �׸���(���� ����)
---------------------------------------------------------------------------*/
void DrawSelectBox( int x1, int y1, int x2, int y2, WORD *tar, WORD color, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH)
{
	int dx1, dy1, dx2, dy2;
	dx1 = x1-1; dy1= y1-1; dx2 = x1+1; dy2 = y1+1;
	DrawBox(dx1, dy1, dx2, dy2, tar, color, _CX1, _CY1, _CX2, _CY2, _LPITCH);
	dx1 = x2-1; dy1= y1-1; dx2 = x2+1; dy2 = y1+1;
	DrawBox(dx1, dy1, dx2, dy2, tar, color, _CX1, _CY1, _CX2, _CY2, _LPITCH);
	dx1 = x2-1; dy1= y2-1; dx2 = x2+1; dy2 = y2+1;
	DrawBox(dx1, dy1, dx2, dy2, tar, color, _CX1, _CY1, _CX2, _CY2, _LPITCH);
	dx1 = x1-1; dy1= y2-1; dx2 = x1+1; dy2 = y2+1;
	DrawBox(dx1, dy1, dx2, dy2, tar, color, _CX1, _CY1, _CX2, _CY2, _LPITCH);
}

/*---------------------------------------------------------------------------
   ���ùڽ� �׸���(XOR)
---------------------------------------------------------------------------*/
void DrawXORSelectBox( int x1, int y1, int x2, int y2, WORD *tar, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH)
{
	int dx1, dy1, dx2, dy2;
	dx1 = x1-1; dy1= y1-1; dx2 = x1+1; dy2 = y1+1;
	DrawBoxXOR(dx1, dy1, dx2, dy2, tar, _CX1, _CY1, _CX2, _CY2, _LPITCH);
	dx1 = x2-1; dy1= y1-1; dx2 = x2+1; dy2 = y1+1;
	DrawBoxXOR(dx1, dy1, dx2, dy2, tar, _CX1, _CY1, _CX2, _CY2, _LPITCH);
	dx1 = x2-1; dy1= y2-1; dx2 = x2+1; dy2 = y2+1;
	DrawBoxXOR(dx1, dy1, dx2, dy2, tar, _CX1, _CY1, _CX2, _CY2, _LPITCH);
	dx1 = x1-1; dy1= y2-1; dx2 = x1+1; dy2 = y2+1;
	DrawBoxXOR(dx1, dy1, dx2, dy2, tar, _CX1, _CY1, _CX2, _CY2, _LPITCH);
}




/*---------------------------------------------------------------------------
   ���ǽ��� ������ ������ ü��
---------------------------------------------------------------------------*/
void FillPage( WORD *tar, WORD fillColor, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
	int i,j;
	/*
    for(i=0; i<_MYL; i++)
	{
		for(j=0; j<_MXL; j++) *(tar+j) = fillColor;
        tar += _LPITCH;
	}
	*/

	for(i=_CY1; i<=_CY2; i++)
	{
		for(j=_CX1; j<=_CX2; j++) *(tar+j) = fillColor;
        tar += _LPITCH;
	}
}


/*---------------------------------------------------------------------------
   ���ǽ��� �� ���
---------------------------------------------------------------------------*/
void PutPixel(int x, int y, WORD *tar, WORD color, DWORD _LPITCH )
{
    *(tar+_LPITCH * y + x)  = color;
}

/*---------------------------------------------------------------------------
   Ư�� ���������� �̹����� ����
---------------------------------------------------------------------------*/
void GetImage(int x, int y, int xl, int yl, WORD *page, WORD *tar, DWORD _LPITCH )
{
    int i;

    page += _LPITCH * y + x;

    for(i=0; i<yl; i++)
    {
        memcpy( tar , page, xl<<1);
        page += _LPITCH;
        tar += xl;
    }
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------------------
�̹��� ��� �Լ�
---------------------------------------------------------------------------*/
void _PutImage( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
    int x=0, y=0, h=0, t=0;  // �ʰ�ġ
    register int i;

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                            // Ŭ���� ���� ������ ������ ������� ����

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp; 
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
    {
        int xLimit = xLen -x - h;
        int yLimit = yLen -y - t;

        tar += (yp+y) * _LPITCH + (xp+x) ;
        sor += y * xLen + x;

		int copybyte = xLimit<<1;
        for(i=0; i<yLimit; i++)
        {
            memcpy( tar , sor, copybyte);
			//fastcpy( tar , sor, copybyte);
            tar += _LPITCH;
            sor += xLen;
        }
    }
    else                    // Ŭ������ �߻����� �ʾ��� ���
    {
        tar += yp * _LPITCH + xp; 
        int copybyte = xLen<<1;
		for(i=0; i<yLen; i++)
        {
            memcpy( tar , sor, copybyte);
			//fastcpy( tar , sor, copybyte);
            tar += _LPITCH;
            sor += xLen;
        }
    }
}


/*--------------------------------------------------------------------------
 �̹��� ��� �Լ�(���� ����)
--------------------------------------------------------------------------*/
void _PutImageUD( int xp, int yp, short xLen, short yLen, WORD *tar, WORD *sor, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
    int x=0, y=0, h=0, t=0;  // �ʰ�ġ
    int rx, ry;              // ���� ���� ��ǥ
    register int i;
    register int xLimit, yLimit;

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                            // Ŭ���� ���� ������ ������ ������� ����

    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;
    if( xp < _CX1 ) { x = _CX1 - xp; rx = _CX1; } else rx = xp;
    if( yp < _CY1 ) { y = _CY1 - yp; ry=_CY1; } else ry = yp;


    if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
    {

        xLimit = xLen -x - h;
        yLimit = yLen -y - t;

        tar += ry * _LPITCH + rx;
        sor += xLen * (yLen -1 -y) + x;

		int copybyte = xLimit<<1;
        for(i=0; i<yLimit; i++)
        {
            memcpy( tar , sor, copybyte);
            tar += _LPITCH;
            sor -= xLen;
        }
    }
    else                      // Ŭ������ �߻����� �ʾ��� ���
    {
        tar += ry * _LPITCH + rx;
        sor += xLen * (yLen -1);

		int copybyte = xLen<<1;
        for(i=0; i<yLen; i++)
        {
            memcpy( tar , sor, copybyte);
            tar += _LPITCH;
            sor -= xLen;
        }
    }
}

void _PutImage_GB( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH)
{
    int x=0, y=0, h=0, t=0;  // �ʰ�ġ
    register int i;

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                            // Ŭ���� ���� ������ ������ ������� ����

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp; 
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
    {
        int xLimit = xLen -x - h;
        int yLimit = yLen -y - t;

        tar += (yp+y) * _LPITCH + (xp+x) ;
        sor += y * xLen + x;

        for(i=0; i<yLimit; i++)
        {
            MemCpyGB( tar , sor, xLimit, grade);
            tar += _LPITCH;
            sor += xLen;
        }
    }
    else                    // Ŭ������ �߻����� �ʾ��� ���
    {
        tar += yp * _LPITCH + xp; 
		for(i=0; i<yLen; i++)
        {
            MemCpyGB( tar , sor, xLen, grade);
            tar += _LPITCH;
            sor += xLen;
        }
    }
}

/*---------------------------------------------------------------------------
   ��������Ʈ�� ����ϴ� �Լ�
---------------------------------------------------------------------------*/
void _PutSpr( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
    int x=0, y=0, h=0, t=0;  // �ʰ�ġ
	int i,j;

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // Ŭ���� ���� ������ ������ ������� ����

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || y || h || t)               // Ŭ������ �߻��Ǹ�
    {
        int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
        int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
        int skip = _LPITCH - xLimit;
		int skip2 = x + h;

        tar += (yp+y) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
		sor += y * xLen + x;

        for(i=0; i<yLimit; i++)
        {
            //memcpy( tar , sor, xLimit);
			for(j=0; j<xLimit; j++)
			{
				if(*sor != colorkey) *tar = *sor;
				tar++; sor++;
			}
            tar += skip;
            sor += skip2;
        }


    }
    else                    // Ŭ������ �߻����� �ʾ��� ���
    {
        int skip = _LPITCH - xLen;
        tar += yp * _LPITCH + xp;

		for(i=0; i<yLen; i++)
        {
            
			for(j=0; j<xLen; j++)
			{
				if(*sor!=colorkey) *tar = *sor;
				tar++; sor++;
			}
            tar += skip;
        }
	}
}


/*---------------------------------------------------------------------------
   ��������Ʈ�� ����ϴ� �Լ�(���� ����)
---------------------------------------------------------------------------*/
void _PutSprUD( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH)
{
    int x=0, y=0, h=0, t=0;  // �ʰ�ġ
	int i,j;

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                            // Ŭ���� ���� ������ ������ ������� ����
	if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
	if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;
    
    if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
    {
        int xLimit = xLen -x - h;
        int yLimit = yLen -y - t;
		int skip = _LPITCH + xLimit;
        int skip2 = x + h;

		tar += (yp+y+yLimit-1) * _LPITCH + (xp+x);
        sor += xLen * t + x;

        for(i=0; i<yLimit; i++)
        {
			for(j=0; j<xLimit; j++)
			{
				if(*sor!=colorkey) *tar = *sor;
				sor++;
				tar++;
			}
            tar -= skip;
            sor += skip2;
        }

    }
    else                      // Ŭ������ �߻����� �ʾ��� ���
    {
        int skip = _LPITCH + xLen;
        tar += (yp+yLen-1) * _LPITCH + xp;

		for(i=0; i<yLen; i++)
        {
			for(j=0; j<xLen; j++)
			{
				if(*sor!=colorkey) *tar = *sor;
				sor++;
				tar++;
			}
            tar -= skip;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
// Compress Sprite Drawing Function
////////////////////////////////////////////////////////////////////////////////////

// 4����Ʈ �޸� ���� ����(����: WORD���� ����, Len=WORD����)
void _fastcall RvrsCpy(WORD *tar, WORD *sor, int Len)
{
	int i;
	tar += Len-1;
	for(i=0; i<Len; i++) *tar-- = *sor++;
}


/*---------------------------------------------------------------------------
   ���� ��������Ʈ ���
---------------------------------------------------------------------------*/
void _PutCsp( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
    int x=0, y=0, h=0, t=0;  // �ʰ�ġ

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // Ŭ���� ���� ������ ������ ������� ����

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // Ŭ������ �߻��Ǹ�
    {
		int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
        int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
		sor += (*(long*)(sor+(y<<1)));    // ��� ������ �������� �̵�

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // ���ϼ��� ����
			sbyte=0;
			doing=0;

			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // ����� ���۵Ǵ� ���� ���
				{
					if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
					else if(sbyte+pbyte > x) 
					{ 
						sor += x-sbyte; 
						pbyte = pbyte - (x-sbyte);
						sbyte = 0;
						doing = BEGINPUTSPR;
					}
				}

				if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						memcpy(tar+sbyte, sor, (xLimit-sbyte)<<1);
						doing = ENDPUTSPR;
					}
					else memcpy(tar+sbyte, sor, pbyte<<1);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else  // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // ��� ������ �������� �̵�

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++; // ����� ����(���� ����)
				pbyte = *sor++;  // �̹��� ����(���� ����)
				
				memcpy(tar+sbyte, sor, pbyte<<1);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}


/*---------------------------------------------------------------------------
   ���� ��������Ʈ�� ����ϴ� �Լ�(�¿����)
---------------------------------------------------------------------------*/
void _PutCspLR( int xp, int yp, int xLen, int yLen, WORD* tar, WORD* sor, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
    int x=0, y=0, h=0, t=0;  // �ʰ�ġ

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // Ŭ���� ���� ������ ������ ������� ����
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
    {
        int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
        int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
			
		int i,cnt,doing;
	    int sbyte,pbyte;
		int temp;

        tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // ���� ��ǥ ���(�ʰ�ġ ����) - ����
		sor += *((long*)(sor+(y<<1)));  // ��� ������ �������� �̵� - ����

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt=*sor++;  // ���ϼ��� ����

			sbyte=0;
			doing=0;

			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;
				pbyte =  *sor++;
								
				if(doing==0)  // ����� ���۵Ǵ� ���� ���
				{
					if(sbyte>=h) { sbyte-=h; doing=BEGINPUTSPR;}
					else if(sbyte+pbyte > h) 
					{ 
						sor += h-sbyte; 
						pbyte = (pbyte - (h-sbyte));
						sbyte = 0;
						doing = BEGINPUTSPR;
					}
				}

				if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						temp = xLimit-sbyte; // ���� ��Ʈ��
						RvrsCpy(tar-sbyte-temp+1, sor, temp);
						doing = ENDPUTSPR;
					}
					else RvrsCpy(tar-sbyte-pbyte+1, sor, pbyte);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else    // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���
        
		tar += (yp+y) * _LPITCH + xp + (xLen);   // ���� ��ǥ���(�ѵ�Ʈ �ʰ�)
		sor += *((long*)(sor+(y<<1)));  // ��� ������ �������� �̵�

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++; // ����� ����(���� ����)
				pbyte = *sor++;  // �̹��� ����(���� ����)
				
				RvrsCpy(tar-sbyte-pbyte+1, sor, pbyte); // ���� ����

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}


/*---------------------------------------------------------------------------
   ���� ��������Ʈ�� ����ϴ� �Լ�(���Ϲ���)
---------------------------------------------------------------------------*/
void _PutCspUD( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
    int x=0, y=0, h=0, t=0;  // �ʰ�ġ

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // Ŭ���� ���� ������ ������ ������� ����
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
    {
        int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
        int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y+yLimit-1) * _LPITCH + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
		sor += (*(long*)(sor+(t<<1)));    // ��� ������ �������� �̵�

		for(i=0;i<yLimit;i++,tar-=_LPITCH)
		{
			cnt= *sor++;  // ���ϼ��� ����

			sbyte=0;
			doing=0;

			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // ����� ���۵Ǵ� ���� ���
				{
					if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
					else if(sbyte+pbyte > x) 
					{ 
						sor += x-sbyte; 
						pbyte = pbyte - (x-sbyte);
						sbyte = 0;
						doing = BEGINPUTSPR;
					}
				}

				if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						memcpy(tar+sbyte, sor, (xLimit-sbyte)<<1);
						doing = ENDPUTSPR;
					}
					else memcpy(tar+sbyte, sor, pbyte<<1);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else    // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���

		int i,cnt;
		int sbyte,pbyte;
        
		tar += (yp+y+yLimit-1) * _LPITCH + xp;   // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
		sor += (*(long*)(sor+(t<<1)));  // ��� ������ �������� �̵�

		for(i=0; i<yLimit; i++,tar-=_LPITCH)
		{
			cnt = *sor++;

			sbyte=0;
			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;
				pbyte = *sor++;
				
				memcpy(tar+sbyte, sor, pbyte<<1);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
// Half Blending Drawing Function
////////////////////////////////////////////////////////////////////////////////////

// ���������� �޸𸮸� �����Ѵ�(����: ���̴� WORD����)
void _fastcall MemCpyHB555(WORD *tar, WORD *sor, int Len)
{
	int i;
	DWORD *p4_scr=(DWORD *)tar;
	DWORD *p4_spr=(DWORD *)sor;
	int HalfLen	= Len/2;
	
	for (i=0; i<HalfLen; i++, p4_scr++, p4_spr++)
		*p4_scr = (((*p4_scr & d4HM_RGB555)>>1)+((*p4_spr & d4HM_RGB555)>>1));

	if(Len%2)
	{
		tar += Len-1;
		sor += Len-1;
		*tar = (((*tar & d2HM_RGB555)>>1)+((*sor & d2HM_RGB555)>>1));
	}
}

void _fastcall MemCpyHB565(WORD *tar, WORD *sor, int Len)
{
	int i;
	DWORD *p4_scr=(DWORD *)tar;
	DWORD *p4_spr=(DWORD *)sor;
	int HalfLen	= Len/2;
	
	for (i=0; i<HalfLen; i++, p4_scr++, p4_spr++)
		*p4_scr = (((*p4_scr & d4HM_RGB565)>>1)+((*p4_spr & d4HM_RGB565)>>1));

	if(Len%2)
	{
		tar += Len-1;
		sor += Len-1;
		*tar = (((*tar & d2HM_RGB565)>>1)+((*sor & d2HM_RGB565)>>1));
	}
}

/*---------------------------------------------------------------------------
   ���� ��������Ʈ�� ���������� ���
---------------------------------------------------------------------------*/
void _PutCsp_HB( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
    int x=0, y=0, h=0, t=0;  // �ʰ�ġ

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // Ŭ���� ���� ������ ������ ������� ����

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // Ŭ������ �߻��Ǹ�
    {
		int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
        int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
		sor += (*(long*)(sor+(y<<1)));    // ��� ������ �������� �̵�

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // ���ϼ��� ����
			sbyte=0;
			doing=0;

			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // ����� ���۵Ǵ� ���� ���
				{
					if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
					else if(sbyte+pbyte > x) 
					{ 
						sor += x-sbyte; 
						pbyte = pbyte - (x-sbyte);
						sbyte = 0;
						doing = BEGINPUTSPR;
					}
				}

				if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpyHB(tar+sbyte, sor, (xLimit-sbyte));
						doing = ENDPUTSPR;
					}
					else MemCpyHB(tar+sbyte, sor, pbyte);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else  // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // ��� ������ �������� �̵�

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++; // ����� ����(���� ����)
				pbyte = *sor++;  // �̹��� ����(���� ����)
				
				MemCpyHB(tar+sbyte, sor, pbyte);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////
// Anti Drawing Function
////////////////////////////////////////////////////////////////////////////////////

// ��Ƽ ����(����: WORD���� ����, Len=WORD����)
void _fastcall MemCpyAnti555(WORD *tar, WORD *sor, int Len)
{
	if(Len<1) return;
	*tar = (((*tar & d2HM_RGB555)>>1)+((*sor & d2HM_RGB555)>>1));
	tar++; sor++;

	if(Len>2)
	{
		Len-=2;
		if(Len>0)
		{
			memcpy(tar, sor, Len<<1);
			tar+=Len;
			sor+=Len;
		}
		*tar = (((*tar & d2HM_RGB555)>>1)+((*sor & d2HM_RGB555)>>1));
	}
}

// ��Ƽ ����(����: WORD���� ����, Len=WORD����)
void _fastcall MemCpyAnti565(WORD *tar, WORD *sor, int Len)
{
	if(Len<1) return;
	*tar = (((*tar & d2HM_RGB565)>>1)+((*sor & d2HM_RGB565)>>1));
	tar++; sor++;

	if(Len>2)
	{
		Len-=2;
		if(Len>0)
		{
			memcpy(tar, sor, Len<<1);
			tar+=Len;
			sor+=Len;
		}
		*tar = (((*tar & d2HM_RGB565)>>1)+((*sor & d2HM_RGB565)>>1));
	}
}

/*---------------------------------------------------------------------------
   ���� ��������Ʈ�� ��Ƽ-�˶��̽� �ؼ� ���
---------------------------------------------------------------------------*/
void _PutCsp_Anti( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
    int x=0, y=0, h=0, t=0;  // �ʰ�ġ

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // Ŭ���� ���� ������ ������ ������� ����

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // Ŭ������ �߻��Ǹ�
    {
		int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
        int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
		sor += (*(long*)(sor+(y<<1)));    // ��� ������ �������� �̵�

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // ���ϼ��� ����
			sbyte=0;
			doing=0;

			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // ����� ���۵Ǵ� ���� ���
				{
					if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
					else if(sbyte+pbyte > x) 
					{ 
						sor += x-sbyte; 
						pbyte = pbyte - (x-sbyte);
						sbyte = 0;
						doing = BEGINPUTSPR;
					}
				}

				if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpyAnti(tar+sbyte, sor, (xLimit-sbyte));
						doing = ENDPUTSPR;
					}
					else MemCpyAnti(tar+sbyte, sor, pbyte);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else  // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // ��� ������ �������� �̵�

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++; // ����� ����(���� ����)
				pbyte = *sor++;  // �̹��� ����(���� ����)
				
				MemCpyAnti(tar+sbyte, sor, pbyte);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
// Glow Drawing Function
////////////////////////////////////////////////////////////////////////////////////

// �۷ο�ȿ�� ����(����: WORD���� ����, Len=WORD����)
void _fastcall MemCpyGlow555(WORD *tar, WORD *sor, int Len, WORD color, int alpha)
{
	int HalfLen = Len>>1;
	int i;

	UINT	first,second,sc4,ic4;
	int dest_alpha = 31-alpha;

	UINT *p4_scr=(UINT *)tar;
	UINT *p4_spr=(UINT *)sor;
	UINT color4 = (UINT)(color<<16)|(UINT)color;
	
	first	=	color4 & d4AM_RGB555_1;
	first	>>=	5;
	first	*=	dest_alpha;
	first	&=	d4AM_RGB555_1;

	second	=	color4 & d4AM_RGB555_2;
	second	*=	dest_alpha;
	second	>>=	5;
	second	&=	d4AM_RGB555_2;

	sc4		=	first | second;
	
	for (i=0; i<HalfLen; i++)
	{		
		first	=	*p4_spr & d4AM_RGB555_1;
		first	>>=	5;
		first	*=	alpha;
		first	&=	d4AM_RGB555_1;

		second	=	*p4_spr &d4AM_RGB555_2;
		second	*=	alpha;
		second	>>=	5;
		second	&=	d4AM_RGB555_2;

		ic4		=	first | second;
		
		*p4_scr  =	sc4+ic4;
		
		p4_scr++; 
		p4_spr++;
	}
	if(Len%2)
	{
		WORD* p2_scr = (WORD*)p4_scr;
		*p2_scr	= (WORD)(sc4)+
				  ((WORD)((((*p4_spr & d4AM_RGB555_1)>>5)*alpha) & d4AM_RGB555_1) | \
					((((*p4_spr & d4AM_RGB555_2)*alpha)>>5) & d4AM_RGB555_2) );
	}
}

void _fastcall MemCpyGlow565(WORD *tar, WORD *sor, int Len, WORD color, int alpha)
{
	int HalfLen = Len>>1;
	int i;

	UINT	first,second,sc4,ic4;
	int dest_alpha = 31-alpha;

	UINT *p4_scr=(UINT *)tar;
	UINT *p4_spr=(UINT *)sor;
	UINT color4 = (UINT)(color<<16)|(UINT)color;
	
	for (i=0; i<HalfLen; i++)
	{
		first	=	color4 & d4AM_RGB565_1;
		first	>>=	5;
		first	*=	dest_alpha;
		first	&=	d4AM_RGB565_1;

		second	=	color4 & d4AM_RGB565_2;
		second	*=	dest_alpha;
		second	>>=	5;
		second	&=	d4AM_RGB565_2;

		sc4		=	first | second;
		
		first	=	*p4_spr & d4AM_RGB565_1;
		first	>>=	5;
		first	*=	alpha;
		first	&=	d4AM_RGB565_1;

		second	=	*p4_spr &d4AM_RGB565_2;
		second	*=	alpha;
		second	>>=	5;
		second	&=	d4AM_RGB565_2;

		ic4		=	first | second;
		
		*p4_scr  =	sc4+ic4;
		
		p4_scr++; 
		p4_spr++;
	}
	if(Len%2)
	{
		WORD* p2_scr = (WORD*)p4_scr;
		*p2_scr	= ( (WORD)((((color4 & d4AM_RGB565_1) >> 5) * dest_alpha) & d4AM_RGB565_1) | \
					((((color4 & d4AM_RGB565_2)*dest_alpha)>>5) & d4AM_RGB565_2) )\
					+ \
				  ( (WORD)((((*p4_spr & d4AM_RGB565_1)>>5)*alpha) & d4AM_RGB565_1) | \
					((((*p4_spr & d4AM_RGB565_2)*alpha)>>5) & d4AM_RGB565_2) );
	}
}

/*---------------------------------------------------------------------------
   ���� ��������Ʈ�� �۷ο� ȿ�� ���
---------------------------------------------------------------------------*/
void _PutCsp_Glow( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color, int grade, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH)
{
    int x=0, y=0, h=0, t=0;  // �ʰ�ġ

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // Ŭ���� ���� ������ ������ ������� ����

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // Ŭ������ �߻��Ǹ�
    {
		int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
        int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
		sor += (*(long*)(sor+(y<<1)));    // ��� ������ �������� �̵�

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // ���ϼ��� ����
			sbyte=0;
			doing=0;

			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // ����� ���۵Ǵ� ���� ���
				{
					if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
					else if(sbyte+pbyte > x) 
					{ 
						sor += x-sbyte; 
						pbyte = pbyte - (x-sbyte);
						sbyte = 0;
						doing = BEGINPUTSPR;
					}
				}

				if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpyGlow(tar+sbyte, sor, (xLimit-sbyte), color, grade);
						doing = ENDPUTSPR;
					}
					else MemCpyGlow(tar+sbyte, sor, pbyte, color, grade);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else  // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // ��� ������ �������� �̵�

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++; // ����� ����(���� ����)
				pbyte = *sor++;  // �̹��� ����(���� ����)
				
				MemCpyGlow(tar+sbyte, sor, pbyte, color, grade);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}



////////////////////////////////////////////////////////////////////////////////////
//  Multi Grade Blandigs Function
////////////////////////////////////////////////////////////////////////////////////

// �ٴܰ� ���������� �޸𸮸� ����(����: WORD���� ����)
void _fastcall MemCpyGB555(WORD *tar, WORD *sor, int Len, int alpha)
{
	int HalfLen = Len>>1;
	int i;

	UINT	first,second,sc4,ic4;
	//WORD	sc2,ic2;
	int dest_alpha = 31-alpha;

	UINT *p4_scr=(UINT *)tar;
	UINT *p4_spr=(UINT *)sor;
	
	for (i=0; i<HalfLen; i++)
	{
		/*
		*p4_scr =( ((((*p4_scr & d4AM_RGB555_1)>>5)*dest_alpha) & d4AM_RGB555_1) | // Dest first
				  ((((*p4_scr & d4AM_RGB555_2)*dest_alpha)>>5) & d4AM_RGB555_2)  ) // Dest Second
				  +
				 ( ((((*p4_spr & d4AM_RGB555_1)>>5)*alpha) & d4AM_RGB555_1) |      // Source first
				  ((((*p4_spr &d4AM_RGB555_2)*alpha)>>5)&d4AM_RGB555_2)   );       // Source Second
				  */

		first	=	*p4_scr & d4AM_RGB555_1;
		first	>>=	5;
		first	*=	dest_alpha;
		first	&=	d4AM_RGB555_1;

		second	=	*p4_scr & d4AM_RGB555_2;
		second	*=	dest_alpha;
		second	>>=	5;
		second	&=	d4AM_RGB555_2;

		sc4		=	first | second;
		
		first	=	*p4_spr & d4AM_RGB555_1;
		first	>>=	5;
		first	*=	alpha;
		first	&=	d4AM_RGB555_1;

		second	=	*p4_spr &d4AM_RGB555_2;
		second	*=	alpha;
		second	>>=	5;
		second	&=	d4AM_RGB555_2;

		ic4		=	first | second;


		*p4_scr  =	sc4+ic4;
		
		
		p4_scr++; 
		p4_spr++;
	}
	if(Len%2)
	{
		WORD* p2_scr = (WORD*)p4_scr;
		*p2_scr	= ( (WORD)((((*p4_scr & d4AM_RGB555_1) >> 5) * dest_alpha) & d4AM_RGB555_1) | \
					((((*p4_scr & d4AM_RGB555_2)*dest_alpha)>>5) & d4AM_RGB555_2) )\
					+ \
				  ( (WORD)((((*p4_spr & d4AM_RGB555_1)>>5)*alpha) & d4AM_RGB555_1) | \
					((((*p4_spr & d4AM_RGB555_2)*alpha)>>5) & d4AM_RGB555_2) );
	}
}



// ���������� �޸𸮸� ����(����: WORD���� ����)
void _fastcall MemCpyGB565(WORD *tar, WORD *sor, int Len, int alpha)
{
	int HalfLen = Len>>1;
	int i;

	UINT	first,second,sc4,ic4;
	//WORD	sc2,ic2;
	int dest_alpha = 31-alpha;

	UINT *p4_scr=(UINT *)tar;
	UINT *p4_spr=(UINT *)sor;
	
	for (i=0; i<HalfLen; i++) // 2��Ʈ�� ó��
	{
		/*
		*p4_scr =( ((((*p4_scr & d4AM_RGB565_1)>>5)*dest_alpha) & d4AM_RGB565_1) | // Dest first
				  ((((*p4_scr & d4AM_RGB565_2)*dest_alpha)>>5) & d4AM_RGB565_2)  ) // Dest Second
				  +
				 ( ((((*p4_spr & d4AM_RGB565_1)>>5)*alpha) & d4AM_RGB565_1) |      // Source first
				  ((((*p4_spr &d4AM_RGB565_2)*alpha)>>5)&d4AM_RGB565_2)   );       // Source Second
				  */

		first	=	*p4_scr & d4AM_RGB565_1;
		first	>>=	5;
		first	*=	dest_alpha;
		first	&=	d4AM_RGB565_1;

		second	=	*p4_scr & d4AM_RGB565_2;
		second	*=	dest_alpha;
		second	>>=	5;
		second	&=	d4AM_RGB565_2;

		sc4		=	first | second;
		
		first	=	*p4_spr & d4AM_RGB565_1;
		first	>>=	5;
		first	*=	alpha;
		first	&=	d4AM_RGB565_1;

		second	=	*p4_spr &d4AM_RGB565_2;
		second	*=	alpha;
		second	>>=	5;
		second	&=	d4AM_RGB565_2;

		ic4		=	first | second;

		*p4_scr  =	sc4+ic4;
		
		
		p4_scr++; 
		p4_spr++;
	}
	if(Len%2) // ���� ��Ʈ ó��
	{
		WORD* p2_scr = (WORD*)p4_scr;
		*p2_scr	= ( (WORD)((((*p4_scr & d4AM_RGB565_1) >> 5) * dest_alpha) & d4AM_RGB565_1) | \
					((((*p4_scr & d4AM_RGB565_2)*dest_alpha)>>5) & d4AM_RGB565_2) )\
					+ \
				  ( (WORD)((((*p4_spr & d4AM_RGB565_1)>>5)*alpha) & d4AM_RGB565_1) | \
					((((*p4_spr & d4AM_RGB565_2)*alpha)>>5) & d4AM_RGB565_2) );
	}
}

// ���������� �޸𸮸� ������ ����(����: WORD���� ����)
void _fastcall RvrsCpyGB555(WORD *tar, WORD *sor, int Len, int alpha)
{
	int HalfLen = Len>>1;
	int i;

	UINT	first,second,sc4,ic4;
	int dest_alpha = 31-alpha;

	tar += Len-1;

	UINT* p4_scr=(UINT *)tar; // Ÿ���� ���� ����
	UINT* p4_spr=(UINT *)sor; // �ҽ��� ���� ����

	if(Len%2)
	{
		WORD* p2_scr = (WORD*)p4_scr;
		*p2_scr	= ( (WORD)((((*p4_scr & d4AM_RGB555_1) >> 5) * dest_alpha) & d4AM_RGB555_1) | \
					((((*p4_scr & d4AM_RGB555_2)*dest_alpha)>>5) & d4AM_RGB555_2) )\
					+ \
				  ( (WORD)((((*p4_spr & d4AM_RGB555_1)>>5)*alpha) & d4AM_RGB555_1) | \
					((((*p4_spr & d4AM_RGB555_2)*alpha)>>5) & d4AM_RGB555_2) );
		tar--;
	}

	p4_scr=(UINT *)(tar-1); // ��������̱� ������ ���� ����� ��Ʈ�� �����
	
	for (i=0; i<HalfLen; i++)
	{
		first	=	*p4_scr & d4AM_RGB555_1;
		first	>>=	5;
		first	*=	dest_alpha;
		first	&=	d4AM_RGB555_1;

		second	=	*p4_scr & d4AM_RGB555_2;
		second	*=	dest_alpha;
		second	>>=	5;
		second	&=	d4AM_RGB555_2;

		sc4		=	first | second;
		
		first	=	((*p4_spr<<16)|(*p4_spr>>16)) & d4AM_RGB555_1; // �ҽ� ���� ������
		first	>>=	5;
		first	*=	alpha;
		first	&=	d4AM_RGB555_1;

		second	=	((*p4_spr<<16)|(*p4_spr>>16)) &d4AM_RGB555_2; // �ҽ� ���� ������
		second	*=	alpha;
		second	>>=	5;
		second	&=	d4AM_RGB555_2;

		ic4		=	first | second;


		*p4_scr  =	sc4+ic4;
		
		
		p4_scr--; 
		p4_spr++;
	}
}

// ���������� �޸𸮸� ������ ����(����: WORD���� ����)
void _fastcall RvrsCpyGB565(WORD *tar, WORD *sor, int Len, int alpha)
{
	int HalfLen = Len>>1;
	int i;

	UINT	first,second,sc4,ic4;
	//WORD	sc2,ic2;
	int dest_alpha = 31-alpha;

	UINT *p4_scr=(UINT *)(tar+Len); // Ÿ���� ���� ����
	UINT *p4_spr=(UINT *)sor;       // �ҽ��� ���� ����
	
	for (i=0; i<HalfLen; i++)
	{
		first	=	*p4_scr & d4AM_RGB565_1;
		first	>>=	5;
		first	*=	dest_alpha;
		first	&=	d4AM_RGB565_1;

		second	=	*p4_scr & d4AM_RGB565_2;
		second	*=	dest_alpha;
		second	>>=	5;
		second	&=	d4AM_RGB565_2;

		sc4		=	first | second;
		
		first	=	((*p4_spr<<16)|(*p4_spr>>16)) & d4AM_RGB565_1; // �ҽ� ���� ������
		first	>>=	5;
		first	*=	alpha;
		first	&=	d4AM_RGB565_1;

		second	=	((*p4_spr<<16)|(*p4_spr>>16)) &d4AM_RGB565_2; // �ҽ� ���� ������
		second	*=	alpha;
		second	>>=	5;
		second	&=	d4AM_RGB565_2;

		ic4		=	first | second;


		*p4_scr  =	sc4+ic4;
		
		
		p4_scr--; 
		p4_spr++;
	}
	if(Len%2)
	{
		WORD* p2_scr = (WORD*)p4_scr;
		*p2_scr	= ( (WORD)((((*p4_scr & d4AM_RGB565_1) >> 5) * dest_alpha) & d4AM_RGB565_1) | \
					((((*p4_scr & d4AM_RGB565_2)*dest_alpha)>>5) & d4AM_RGB565_2) )\
					+ \
				  ( (WORD)((((*p4_spr & d4AM_RGB565_1)>>5)*alpha) & d4AM_RGB565_1) | \
					((((*p4_spr & d4AM_RGB565_2)*alpha)>>5) & d4AM_RGB565_2) );
	}
}


/*---------------------------------------------------------------------------
   ���� ��������Ʈ�� ��Ƽ�׷��̵� ���������� ����ϴ� �Լ�
---------------------------------------------------------------------------*/
void _PutCsp_GB( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
	int x=0, y=0, h=0, t=0;  // �ʰ�ġ

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // Ŭ���� ���� ������ ������ ������� ����
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // Ŭ������ �߻��Ǹ�
    {
		int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
        int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
		sor += (*(long*)(sor+(y<<1)));    // ��� ������ �������� �̵�

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // ���ϼ��� ����
			sbyte=0;
			doing=0;

			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // ����� ���۵Ǵ� ���� ���
				{
					if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
					else if(sbyte+pbyte > x) 
					{ 
						sor += x-sbyte; 
						pbyte = pbyte - (x-sbyte);
						sbyte = 0;
						doing = BEGINPUTSPR;
					}
				}

				if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpyGB(tar+sbyte, sor, xLimit-sbyte, grade);
						doing = ENDPUTSPR;
					}
					else MemCpyGB(tar+sbyte, sor, pbyte, grade);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else  // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // ��� ������ �������� �̵�

		int i,cnt;
	    int sbyte,pbyte;

		//sor+=yLen<<1;   // Skip Line-Distance-Table(yLen*2)
		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++; // ����� ����(���� ����)
				pbyte = *sor++;  // �̹��� ����(���� ����)
				
				MemCpyGB(tar+sbyte, sor, pbyte, grade);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

/*---------------------------------------------------------------------------
   ���� ��������Ʈ�� ��Ƽ�׷��̵� ���������� ����ϴ� �Լ�(�¿����)
---------------------------------------------------------------------------*/
void _PutCspLR_GB( int xp, int yp, int xLen, int yLen, WORD* tar, WORD* sor, int grade, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
    int x=0, y=0, h=0, t=0;  // �ʰ�ġ

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // Ŭ���� ���� ������ ������ ������� ����
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
    {
        int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
        int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
			
		int i,cnt,doing;
	    int sbyte,pbyte;
		int temp;

        tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // ���� ��ǥ ���(�ʰ�ġ ����) - ����
		sor += *((long*)(sor+(y<<1)));  // ��� ������ �������� �̵� - ����

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt=*sor++;  // ���ϼ��� ����

			sbyte=0;
			doing=0;

			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;
				pbyte =  *sor++;
								
				if(doing==0)  // ����� ���۵Ǵ� ���� ���
				{
					if(sbyte>=h) { sbyte-=h; doing=BEGINPUTSPR;}
					else if(sbyte+pbyte > h) 
					{ 
						sor += h-sbyte; 
						pbyte = (pbyte - (h-sbyte));
						sbyte = 0;
						doing = BEGINPUTSPR;
					}
				}

				if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						temp = xLimit-sbyte; // ���� ��Ʈ��
						RvrsCpyGB(tar-sbyte-temp+1, sor, temp, grade);
						doing = ENDPUTSPR;
					}
					else RvrsCpyGB(tar-sbyte-pbyte+1, sor, pbyte, grade);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else    // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���
        
		tar += (yp+y) * _LPITCH + xp + (xLen);   // ���� ��ǥ���(�ѵ�Ʈ �ʰ�)
		sor += *((long*)(sor+(y<<1)));  // ��� ������ �������� �̵�

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++; // ����� ����(���� ����)
				pbyte = *sor++;  // �̹��� ����(���� ����)
				
				RvrsCpyGB(tar-sbyte-pbyte+1, sor, pbyte, grade); // ���� ����

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////
//  Alpha Blandigs Function
////////////////////////////////////////////////////////////////////////////////////

// ���� �������� �޸𸮸� ����(����: WORD���� ����)
void _fastcall MemCpyAB555(WORD *tar, WORD *sor, int Len, int alpha)
{
	int i;
	int dest_alpha = 31-alpha;

	for(i=0; i<Len; i++)
	{
		*tar = (R[R555(*sor)][alpha] | G[G555(*sor)][alpha] | B[B555(*sor)][alpha]) +
			   (R[R555(*tar)][dest_alpha] | G[G555(*tar)][dest_alpha] | B[B555(*tar)][dest_alpha]);
		sor++;
		tar++;
	}
}

// ���� �������� �޸𸮸� ����(����: WORD���� ����)
void _fastcall MemCpyAB565(WORD *tar, WORD *sor, int Len, int alpha)
{
	int i;
	int dest_alpha = 31-alpha;

	for(i=0; i<Len; i++)
	{
		*tar = (R[R565(*sor)][alpha] | G[G565(*sor)][alpha] | B[B565(*sor)][alpha]) +
			   (R[R565(*tar)][dest_alpha] | G[G565(*tar)][dest_alpha] | B[B565(*tar)][dest_alpha]);
		sor++;
		tar++;
	}
}


/*---------------------------------------------------------------------------
   ���� ��������Ʈ�� ���� �������� ����ϴ� �Լ�
---------------------------------------------------------------------------*/
void _PutCsp_AB( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
	int x=0, y=0, h=0, t=0;  // �ʰ�ġ

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // Ŭ���� ���� ������ ������ ������� ����
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // Ŭ������ �߻��Ǹ�
    {
		int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
        int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
		sor += (*(long*)(sor+(y<<1)));    // ��� ������ �������� �̵�

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // ���ϼ��� ����
			sbyte=0;
			doing=0;

			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // ����� ���۵Ǵ� ���� ���
				{
					if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
					else if(sbyte+pbyte > x) 
					{ 
						sor += x-sbyte; 
						pbyte = pbyte - (x-sbyte);
						sbyte = 0;
						doing = BEGINPUTSPR;
					}
				}

				if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpyAB(tar+sbyte, sor, xLimit-sbyte, grade);
						doing = ENDPUTSPR;
					}
					else MemCpyAB(tar+sbyte, sor, pbyte, grade);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else  // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // ��� ������ �������� �̵�

		int i,cnt;
	    int sbyte,pbyte;

		int alpha = grade;
		int dest_alpha = 31-alpha;
		//if(alpha>31) return;

		for(i=0; i<yLimit; i++, tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;  // ����� ����(���� ����)
				pbyte = *sor++;   // �̹��� ����(���� ����)
				
				MemCpyAB(tar+sbyte, sor, pbyte, grade);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
//  Lightning Effect Function
////////////////////////////////////////////////////////////////////////////////////

// Lightning Effect�� �޸𸮸� ����(����: WORD���� ����)
void _fastcall MemCpyLE555(WORD *tar, WORD *sor, int Len)
{
	int i;
	for(i=0; i<Len; i++)
	{
		*tar = (LIGHT_R[R555(*sor)][R555(*tar)] | LIGHT_G[G555(*sor)][G555(*tar)] | LIGHT_B[B555(*sor)][B555(*tar)]);
		sor++;
		tar++;
	}
}

void _fastcall MemCpyLE565(WORD *tar, WORD *sor, int Len)
{
	int i;
	for(i=0; i<Len; i++)
	{
		*tar = (LIGHT_R[R565(*sor)][R565(*tar)] | LIGHT_G[G565(*sor)][G565(*tar)] | LIGHT_B[B565(*sor)][B565(*tar)]);
		sor++;
		tar++;
	}
}

/*---------------------------------------------------------------------------
   ���� ��������Ʈ�� Lightning Effect�� ����ϴ� �Լ�
---------------------------------------------------------------------------*/
void _PutCsp_LE( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
	int x=0, y=0, h=0, t=0;  // �ʰ�ġ

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // Ŭ���� ���� ������ ������ ������� ����
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // Ŭ������ �߻��Ǹ�
    {
		int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
        int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
		sor += (*(long*)(sor+(y<<1)));    // ��� ������ �������� �̵�

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // ���ϼ��� ����
			sbyte=0;
			doing=0;

			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // ����� ���۵Ǵ� ���� ���
				{
					if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
					else if(sbyte+pbyte > x) 
					{ 
						sor += x-sbyte; 
						pbyte = pbyte - (x-sbyte);
						sbyte = 0;
						doing = BEGINPUTSPR;
					}
				}

				if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpyLE(tar+sbyte, sor, xLimit-sbyte);
						doing = ENDPUTSPR;
					}
					else MemCpyLE(tar+sbyte, sor, pbyte);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else  // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // ��� ������ �������� �̵�

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++, tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;  // ����� ����(���� ����)
				pbyte = *sor++;   // �̹��� ����(���� ����)
				
				MemCpyLE(tar+sbyte, sor, pbyte);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
//  Darken Effect Function
////////////////////////////////////////////////////////////////////////////////////

// Darken Effect�� �޸𸮸� ����(����: WORD���� ����)
void _fastcall MemCpyDE555(WORD *tar, WORD *sor, int Len)
{
	int i;
	for(i=0; i<Len; i++)
	{
		*tar = (DARK_R[R555(*sor)][R555(*tar)] | DARK_G[G555(*sor)][G555(*tar)] | DARK_B[B555(*sor)][B555(*tar)]);
		sor++;
		tar++;
	}
}

void _fastcall MemCpyDE565(WORD *tar, WORD *sor, int Len)
{
	int i;
	for(i=0; i<Len; i++)
	{
		*tar = (DARK_R[R565(*sor)][R565(*tar)] | DARK_G[G565(*sor)][G565(*tar)] | DARK_B[B565(*sor)][B565(*tar)]);
		sor++;
		tar++;
	}
}

/*---------------------------------------------------------------------------
   ���� ��������Ʈ�� Darken Effect�� ����ϴ� �Լ�
---------------------------------------------------------------------------*/
void _PutCsp_DE( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
	int x=0, y=0, h=0, t=0;  // �ʰ�ġ

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // Ŭ���� ���� ������ ������ ������� ����
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // Ŭ������ �߻��Ǹ�
    {
		int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
        int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
		sor += (*(long*)(sor+(y<<1)));    // ��� ������ �������� �̵�

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // ���ϼ��� ����
			sbyte=0;
			doing=0;

			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // ����� ���۵Ǵ� ���� ���
				{
					if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
					else if(sbyte+pbyte > x) 
					{ 
						sor += x-sbyte; 
						pbyte = pbyte - (x-sbyte);
						sbyte = 0;
						doing = BEGINPUTSPR;
					}
				}

				if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpyDE(tar+sbyte, sor, xLimit-sbyte);
						doing = ENDPUTSPR;
					}
					else MemCpyDE(tar+sbyte, sor, pbyte);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else  // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // ��� ������ �������� �̵�

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++, tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;  // ����� ����(���� ����)
				pbyte = *sor++;   // �̹��� ����(���� ����)
				
				MemCpyDE(tar+sbyte, sor, pbyte);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
//  change pixel format Function
////////////////////////////////////////////////////////////////////////////////////

// RGB555������ RGB565�� �޸𸮸� ����(����: WORD���� ����)
void _fastcall MemCpy555TO565(WORD *tar, WORD *sor, int Len)
{
	for(register int i=0; i<Len; i++) {
		*tar = ((*sor)&(WORD)0x001F) | ((*sor)&(WORD)0xFFE0)<<1;
		sor++;
		tar++;
	}
}

/*---------------------------------------------------------------------------
   RGB555���� ��������Ʈ�� RGB565�� ����ϴ� �Լ�
---------------------------------------------------------------------------*/
void _PutCsp_RGB555TORGB565( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
	int x=0, y=0, h=0, t=0;  // �ʰ�ġ

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // Ŭ���� ���� ������ ������ ������� ����
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // Ŭ������ �߻��Ǹ�
    {
		int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
        int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
		sor += (*(long*)(sor+(y<<1)));    // ��� ������ �������� �̵�

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // ���ϼ��� ����
			sbyte=0;
			doing=0;

			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // ����� ���۵Ǵ� ���� ���
				{
					if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
					else if(sbyte+pbyte > x) 
					{ 
						sor += x-sbyte; 
						pbyte = pbyte - (x-sbyte);
						sbyte = 0;
						doing = BEGINPUTSPR;
					}
				}

				if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpy555TO565(tar+sbyte, sor, xLimit-sbyte);
						doing = ENDPUTSPR;
					}
					else MemCpy555TO565(tar+sbyte, sor, pbyte);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else  // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // ��� ������ �������� �̵�

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++, tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;  // ����� ����(���� ����)
				pbyte = *sor++;   // �̹��� ����(���� ����)
				
				MemCpy555TO565(tar+sbyte, sor, pbyte);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

// RGB555������ RGB24�� �޸𸮸� ����(����: WORD���� ����)
void _fastcall MemCpy555TORGB24(BYTE *tar, WORD *sor, int Len)
{
	for(register int i=0; i<Len; i++) {
		// 24��Ʈ�� BGR���̴�
		tar[2] = (BYTE)(((*sor)&(WORD)0x7C00) >> 7);	// r
		tar[1] = (BYTE)(((*sor)&(WORD)0x03E0) >> 2);	// g
		tar[0] = (BYTE)(((*sor)&(WORD)0x001F) << 3);	// b
		sor++;
		tar+=3;
	}
}

/*---------------------------------------------------------------------------
   RGB555���� ��������Ʈ�� RGB24�� ����ϴ� �Լ�
---------------------------------------------------------------------------*/
void _PutCsp_RGB555TORGB24( int xp, int yp, int xLen, int yLen, BYTE *tar, int lpitch, WORD *sor, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
	int x=0, y=0, h=0, t=0;  // �ʰ�ġ

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // Ŭ���� ���� ������ ������ ������� ����
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

	int bytepitch = lpitch;

    if( x || h )               // Ŭ������ �߻��Ǹ�
    {
		int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
        int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += ((yp+y) * bytepitch) + (xp+x)*3; // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
		sor += (*(long*)(sor+(y<<1)));    // ��� ������ �������� �̵�

		for(i=0; i<yLimit; i++, tar+=bytepitch)
		{
			cnt = *sor++;  // ���ϼ��� ����
			sbyte=0;
			doing=0;

			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // ����� ���۵Ǵ� ���� ���
				{
					if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
					else if(sbyte+pbyte > x) 
					{ 
						sor += x-sbyte; 
						pbyte = pbyte - (x-sbyte);
						sbyte = 0;
						doing = BEGINPUTSPR;
					}
				}

				if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpy555TORGB24(tar+sbyte*3, sor, xLimit-sbyte);
						doing = ENDPUTSPR;
					}
					else MemCpy555TORGB24(tar+sbyte*3, sor, pbyte);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else  // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
    {
        //int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
        tar += (yp+y) * bytepitch + xp*3;
		sor += *((long*)(sor+(y<<1)));  // ��� ������ �������� �̵�

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++, tar+=bytepitch)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;  // ����� ����(���� ����)
				pbyte = *sor++;   // �̹��� ����(���� ����)
				
				MemCpy555TORGB24(tar+sbyte*3, sor, pbyte);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

// RGB555������ RGB32�� �޸𸮸� ����(����: WORD���� ����)
void _fastcall MemCpy555TORGB32(DWORD *tar, WORD *sor, int Len)
{
	for(register int i=0; i<Len; i++) {
		*tar = ((DWORD)(*sor)&(DWORD)0x00007C00) << 9		// r
				| ((DWORD)(*sor)&(DWORD)0x000003E0) << 6	// g
				| ((DWORD)(*sor)&(DWORD)0x0000001F) << 3;	// b
		sor++;
		tar++;
	}
}

/*---------------------------------------------------------------------------
   RGB555���� ��������Ʈ�� RGB32�� ����ϴ� �Լ�
---------------------------------------------------------------------------*/
void _PutCsp_RGB555TORGB32( int xp, int yp, int xLen, int yLen, DWORD *tar, int lpitch, WORD *sor, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
	int x=0, y=0, h=0, t=0;  // �ʰ�ġ

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // Ŭ���� ���� ������ ������ ������� ����
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

	int qurdpitch = lpitch/4;

    if( x || h )               // Ŭ������ �߻��Ǹ�
    {
		int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
        int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * qurdpitch + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
		sor += (*(long*)(sor+(y<<1)));    // ��� ������ �������� �̵�

		for(i=0;i<yLimit;i++,tar+=qurdpitch)
		{
			cnt = *sor++;  // ���ϼ��� ����
			sbyte=0;
			doing=0;

			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // ����� ���۵Ǵ� ���� ���
				{
					if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
					else if(sbyte+pbyte > x) 
					{ 
						sor += x-sbyte; 
						pbyte = pbyte - (x-sbyte);
						sbyte = 0;
						doing = BEGINPUTSPR;
					}
				}

				if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpy555TORGB32(tar+sbyte, sor, xLimit-sbyte);
						doing = ENDPUTSPR;
					}
					else MemCpy555TORGB32(tar+sbyte, sor, pbyte);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else  // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
    {
        //int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
        tar += (yp+y) * qurdpitch + xp;
		sor += *((long*)(sor+(y<<1)));  // ��� ������ �������� �̵�

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++, tar+=qurdpitch)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // ���ϼ� ��ŭ �ݺ�
			{
				sbyte += *sor++;  // ����� ����(���� ����)
				pbyte = *sor++;   // �̹��� ����(���� ����)
				
				MemCpy555TORGB32(tar+sbyte, sor, pbyte);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////                             /////////////////////////////////
////////////////////////////  ���ེ������Ʈ ����/�ƿ�   /////////////////////////////////
////////////////////////////                             /////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  Sprite Zoom In/Out Function
////////////////////////////////////////////////////////////////////////////////////

void _ZoomSpr(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
	int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ
	
	int xErt,yErt=0;
	register int xCnt, yCnt;
	WORD *dest, *src;
	int sorXL, sorYL, tarXL, tarYL;

	if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;
    
	if( xp < _CX1 )         x = _CX1 - xp;
    if( yp < _CY1 )         y = _CY1 - yp;
    if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
    if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

	if(x||y||t||h)
	{
		tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
		tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
		tar += (yp+y) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
		
		if(x) x=x*xLen/txl;
		if(y) y=y*yLen/tyl; 
		if(h) h=h*xLen/txl;
		if(t) t=t*yLen/tyl; 

		sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
		sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
		sor += y * xLen + x;
	}
	else 
	{
		tarXL = txl;
		tarYL = tyl;
		sorXL = xLen;
		sorYL = yLen;
		tar += yp * _LPITCH + xp; 
	}

	yErt=0;
	yCnt = sorYL;
	for(; yCnt; yCnt--)
	{
		yErt += tarYL;
		while(yErt >= sorYL)
		{
			yErt -= sorYL;
			dest = tar;
			src = sor;
			xCnt = sorXL;
			xErt=0;

			for(; xCnt; xCnt--)
			{
				xErt += tarXL;
				while(xErt >= sorXL)
				{
					xErt-=sorXL;
					if(*src != colorkey) *dest = *src;
					dest++;
				}
				src++;
			}
			tar+=_LPITCH;
		}
		sor+= xLen;
	}
}



////////////////////////////////////////////////////////////////////////////////////
//  Compress Sprite Zoom In/Out Function
////////////////////////////////////////////////////////////////////////////////////

void _ZoomCsp( int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
	int x=0, y=0, h=0, t=0;         // Ÿ���ʰ�ġ
	int sx=0, sy=0, sh=0, st=0;     // �ҽ��ʰ�ġ
	
	int xErt,yErt=0;                // X,Y ������
	WORD *dest, *src;
	int sorXL, sorYL, tarXL, tarYL; // �ҽ��� Ÿ���� ��µ� X,Y����

	if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;
    
	if( xp < _CX1 )         x = _CX1 - xp;
    if( yp < _CY1 )         y = _CY1 - yp;
    if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
    if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

	if(x||y||t||h)
	{
		tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
		tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
		
		if(x) sx=x*xLen/txl;          // �ҽ��� �ʰ�ġ
		if(y) sy=y*yLen/tyl; 
		if(h) sh=h*xLen/txl;
		if(t) st=t*yLen/tyl; 

		sorXL = xLen -sx - sh;        // X������ ���� �ҽ��� ��Ʈ�� ���
		sorYL = yLen -sy - st;        // Y������ ���� �ҽ��� ��Ʈ�� ���
	}
	else 
	{
		tarXL = txl;
		tarYL = tyl;
		sorXL = xLen;
		sorYL = yLen;
	}

	tar += (yp+y) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

    if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
    {
		int i,j,ptnCnt;
	    int sbyte,pbyte;
		int tbyte;
		int first,temp;
		
		yErt=0;
		for(i=0; i<sorYL; i++)
		{
			yErt += tarYL;

			while(yErt >= sorYL)
			{
				yErt -= sorYL;
				dest = tar;
				xErt=0;

				first=TRUE;
				tbyte=0;

				src = GET_CSPLINEPTR(sor,sy+i); // ���� �ҽ��� �������� �̵�
				ptnCnt = *src++;

				while(ptnCnt--)					// ���ϼ� ��ŭ �ݺ�
				{
					sbyte = *src++;				// 0������ ���� �б�
					pbyte = *src++;				// ����Ÿ�� ���� �б�
					
					tbyte+=sbyte+pbyte;
					
					if(first==TRUE)				// ����� ���۵Ǵ� ���� ���
					{
						if(tbyte>=sx) 
						{
							if(tbyte-pbyte >= sx) { sbyte=tbyte-sx-pbyte;}
							else 
							{ 
								sbyte=0; 
								src+=pbyte-(tbyte-sx);
								pbyte=tbyte-sx; 
							}
							first=FALSE;
						}
						else { src+=pbyte; continue;}
					}

					temp = sorXL+sx;
					if(tbyte>=temp)		// ����� �������� ���
					{
						if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // ���� �ʿ䰡 �����Ƿ�
						else pbyte = temp-(tbyte-pbyte);
						ptnCnt=0;		// ���̻� ������ ���� ���ϰ� �Ѵ�
					}
					
					xErt += tarXL*sbyte;

					while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
					{
						xErt-=sorXL;
						dest++;
					}
					//dest += xErt/sorXL;
					//xErt = xErt%sorXL;

					for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							*dest++ = *src;
							//dest++;
						}
						src++;
					}
				}
				tar+=_LPITCH;
			}
		}
    }
    else    // Ŭ������ �߻����� �ʾҰų� ���� Ŭ���θ� �߻����� ���
    {
		
		int i,j,ptnCnt;
	    int sbyte,pbyte;
		
		yErt=0;
		for(i=0; i<sorYL; i++)
		{
			yErt += tarYL;

			while(yErt >= sorYL)
			{
				yErt -= sorYL;
				dest = tar;
				xErt=0;

				src = GET_CSPLINEPTR(sor,sy+i); // ���� �ҽ��� �������� �̵�
				ptnCnt = *src++;

				while(ptnCnt--)					// ���ϼ� ��ŭ �ݺ�
				{
					sbyte = *src++;				// 0������ ���� �б�
					pbyte = *src++;				// ����Ÿ�� ���� �б�
					
					
					xErt += tarXL*sbyte;
					
					while(xErt >= sorXL)		// 0���� ��ŵ(Ȯ��/����ϸ鼭)
					{
						xErt-=sorXL;
						dest++;
					}
					//dest += xErt/sorXL;
					//xErt = xErt%sorXL;

					for(j=0; j<pbyte; j++)		// ����Ÿ ���(Ȯ��/����ϸ鼭)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							*dest++ = *src;
						}
						src++;
					}
				}
				tar+=_LPITCH;
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////
//  Sprite Zoom In/Out Function
////////////////////////////////////////////////////////////////////////////////////


/*
void _ZoomImage_Smooth(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
	int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ
	
	int xErt,yErt=0;
	register int xCnt, yCnt;
	WORD *dest, *src;
	int sorXL, sorYL, tarXL, tarYL;

	if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;
    
	if( xp < _CX1 )         x = _CX1 - xp;
    if( yp < _CY1 )         y = _CY1 - yp;
    if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
    if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

	if(x||y||t||h)
	{
		tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
		tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
		tar += (yp+y) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
		
		if(x) x=x*xLen/txl;
		if(y) y=y*yLen/tyl; 
		if(h) h=h*xLen/txl;
		if(t) t=t*yLen/tyl; 

		sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
		sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
		sor += y * xLen + x;
	}
	else 
	{
		tarXL = txl;
		tarYL = tyl;
		sorXL = xLen;
		sorYL = yLen;
		tar += yp * _LPITCH + xp; 
	}

	yErt=0;
	yCnt = sorYL;
	
	WORD srccolor;
	int skiplinecnt = 0;

	for(; yCnt; yCnt--)
	{
		yErt += tarYL;

		while(yErt >= sorYL)
		{
			yErt -= sorYL;
			dest = tar;
			src = sor;
			xCnt = sorXL;
			xErt = 0;

			srccolor = *src;

			for(; xCnt; xCnt--)
			{
				xErt += tarXL;
				while(xErt >= sorXL)
				{
					xErt-=sorXL;
					
					//*dest = srccolor;
					
					if(skiplinecnt > 0) {
						
						WORD mixcolor = srccolor;
						for(int i=1; i<=skiplinecnt; ++i) {
							mixcolor = (((mixcolor & d2HM_RGB555)>>1)+((*(src - (xLen*i)) & d2HM_RGB555)>>1));
						}

						*dest = mixcolor;
					}
					else {
						*dest = srccolor;
					}

					++dest;
				}

				srccolor = (((srccolor & d2HM_RGB555)>>1)+((*src & d2HM_RGB555)>>1));
				++src;
			}

			skiplinecnt = 0;

			tar += _LPITCH;
		}

		++skiplinecnt;
		sor += xLen;
	}
}
*/

void _ZoomImage_Smooth(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, int _CX1, int _CY1, int _CX2, int _CY2, DWORD _LPITCH )
{
	int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ
	
	int xErt,yErt=0;
	register int xCnt, yCnt;
	WORD *dest, *src;
	int sorXL, sorYL, tarXL, tarYL;

	if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;
    
	if( xp < _CX1 )         x = _CX1 - xp;
    if( yp < _CY1 )         y = _CY1 - yp;
    if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
    if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

	if(x||y||t||h)
	{
		tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
		tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
		tar += (yp+y) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
		
		if(x) x=x*xLen/txl;
		if(y) y=y*yLen/tyl; 
		if(h) h=h*xLen/txl;
		if(t) t=t*yLen/tyl; 

		sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
		sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
		sor += y * xLen + x;
	}
	else 
	{
		tarXL = txl;
		tarYL = tyl;
		sorXL = xLen;
		sorYL = yLen;
		tar += yp * _LPITCH + xp; 
	}

	yErt=0;
	yCnt = sorYL;
	
	WORD srccolor;
	int skiplinecnt = 0;

	for(; yCnt; yCnt--)
	{
		yErt += tarYL;

		while(yErt >= sorYL)
		{
			yErt -= sorYL;
			dest = tar;
			src = sor;
			xCnt = sorXL;
			xErt = 0;

			srccolor = *src;

			for(; xCnt; xCnt--)
			{
				xErt += tarXL;
				while(xErt >= sorXL)
				{
					xErt-=sorXL;
					
					*dest = srccolor;
					
					/*
					if(skiplinecnt > 0) {
						
						WORD mixcolor = srccolor;
						for(int i=1; i<=skiplinecnt; ++i) {
							mixcolor = (((mixcolor & d2HM_RGB555)>>1)+((*(src - (xLen*i)) & d2HM_RGB555)>>1));
						}

						*dest = mixcolor;
					}
					else {
						*dest = srccolor;
					}
					*/

					++dest;
				}

				srccolor = (((srccolor & d2HM_RGB555)>>1)+((*src & d2HM_RGB555)>>1));
				//srccolor = *src;
				++src;
			}

			skiplinecnt = 0;

			tar += _LPITCH;
		}

		++skiplinecnt;
		sor += xLen;
	}
}
