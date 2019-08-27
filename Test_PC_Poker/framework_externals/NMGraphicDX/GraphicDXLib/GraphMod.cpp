#include "stdafx.h"
#include "GraphMod.h"

//////////////////////////////////////////////////////////////////////////////////

#define LERP(alpha, v1, v2) ((v1)+(alpha)*((v2)-(v1)))

//////////////////////////////////////////////////////////////////////////////////

WORD	R[32][32]={0,};				// 알파 테이블
WORD	G[64][32]={0,};
WORD	B[32][32]={0,};

WORD	LIGHT_R[32][32]={0,};		// 광원용 알파 테이블
WORD	LIGHT_G[64][64]={0,};
WORD	LIGHT_B[32][32]={0,};

WORD	DARK_R[32][32]={0,};		// Darken Effect용 알파 테이블
WORD	DARK_G[64][64]={0,};
WORD	DARK_B[32][32]={0,};

int		ABGRADE[32][32]={0,};		// 알파 블렌딩 그레이드 테이블
int     RVS_ABGRADE[32][32]={0,};	// 리버스 알파 블렌딩 그레이드 테이블 (Darken Effect 색상 변환등에 사용)

int _MXL, _MYL;					// 화면의 X,Y길이
int _CX1, _CY1, _CX2, _CY2;		// 클리핑 영역 좌표
int _CXL, _CYL;					// 클리핑 영역의 X,Y길이
DWORD _LPITCH;					// 서피스(혹은 가상페이지)의 lPitch


//////////////////////////////////////////////////////////////////////////////////


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

// RGB혼합해 주는 함수 포인터..
// 입력 RGB 값은 RGB555 출력 값은 픽셀 포맷에 따라..(RGB각각의 최대값은 31)
WORD	(*RGBmix)(int r,int g,int b ) = RGB555mix;

// 함수 포인터
void (_fastcall *MemCpyHB) (WORD *tar, WORD *sor, int Len);
void (_fastcall *RvrsCpyHB) (WORD *tar, WORD *sor, int Len);
void (_fastcall *MemCpyAnti) (WORD *tar, WORD *sor, int Len);
void (_fastcall *MemCpyGlow) (WORD *tar, WORD *sor, int Len, WORD color, int grade);
void (_fastcall *RvrsCpyGlow) (WORD *tar, WORD *sor, int Len, WORD color, int grade);
void (_fastcall *MemCpyGrayscl) (WORD *tar, WORD *sor, int Len);
void (_fastcall *RvrsCpyGrayscl) (WORD *tar, WORD *sor, int Len);
void (_fastcall *MemCpyGB) (WORD *tar, WORD *sor, int Len, int grade);
void (_fastcall *RvrsCpyGB) (WORD *tar, WORD *sor, int Len, int grade);
void (_fastcall *MemCpyAB) (WORD *tar, WORD *sor, int Len, BYTE* palpha, int grade);
void (_fastcall *RvrsCpyAB) (WORD *tar, WORD *sor, int Len, BYTE* palpha, int grade);
void (_fastcall *MemCpyLE) (WORD *tar, WORD *sor, int Len, int grade);
void (_fastcall *RvrsCpyLE) (WORD *tar, WORD *sor, int Len, int grade);
void (_fastcall *MemCpyDE) (WORD *tar, WORD *sor, int Len, int grade);
void (_fastcall *RvrsCpyDE) (WORD *tar, WORD *sor, int Len, int grade);
void (_fastcall *ColorKeyMemCpyGB) (WORD *tar, WORD *sor, WORD colorkey, int Len, int grade);


/*---------------------------------------------------------------------------
   클리핑 영역 설정 함수
---------------------------------------------------------------------------*/
void SetClipArea(int x1, int y1, int x2, int y2)
{
    _CX1 = x1;
    _CY1 = y1;
    _CX2 = x2;
    _CY2 = y2;
    _CXL = x2 - x1 +1;
    _CYL = y2 - y1 +1;
}


/*---------------------------------------------------------------------------
   그래픽 라이브러리 초기화
---------------------------------------------------------------------------*/
BOOL InitGrpLib(int xl, int yl, DWORD lPitch, int PixelFormat)
{
	_LPITCH = lPitch;

    _MXL = xl;
    _MYL = yl;
    SetClipArea(0, 0, xl-1, yl-1);

	if(PixelFormat==565)
	{
		RGBmix = RGB565mix;

		MemCpyHB = MemCpyHB565;
		RvrsCpyHB = RvrsCpyHB565;
		MemCpyAnti = MemCpyAnti565;
		MemCpyGlow = MemCpyGlow565;
		RvrsCpyGlow = RvrsCpyGlow565;
		MemCpyGrayscl = MemCpyGrayscl565;
		RvrsCpyGrayscl = RvrsCpyGrayscl565;
		MemCpyGB = MemCpyGB565;
		RvrsCpyGB = RvrsCpyGB565;
		MemCpyAB = MemCpyAB565;
		RvrsCpyAB = RvrsCpyAB565;
		MemCpyLE = MemCpyLE565;
		RvrsCpyLE = RvrsCpyLE565;
		MemCpyDE = MemCpyDE565;
		RvrsCpyDE = RvrsCpyDE565;
	}
	else
	{
		RGBmix = RGB555mix;

		MemCpyHB = MemCpyHB555;
		RvrsCpyHB = RvrsCpyHB555;
		MemCpyAnti = MemCpyAnti555;
		MemCpyGlow = MemCpyGlow555;
		RvrsCpyGlow = RvrsCpyGlow555;
		MemCpyGrayscl = MemCpyGrayscl555;
		RvrsCpyGrayscl = RvrsCpyGrayscl555;
		MemCpyGB = MemCpyGB555;
		RvrsCpyGB = RvrsCpyGB555;
		MemCpyAB = MemCpyAB555;
		RvrsCpyAB = RvrsCpyAB555;
		MemCpyLE = MemCpyLE555;
		RvrsCpyLE = RvrsCpyLE555;
		MemCpyDE = MemCpyDE555;
		RvrsCpyDE = RvrsCpyDE555;
	}

	// 아래의 함수는 RGB555만 적용한다
	ColorKeyMemCpyGB = ColorKeyMemCpyGB555;

	return TRUE;
}

static BOOL g_bInitedAlphaTable = FALSE;

void InitAlphaTable(int PixelFormat)
{
	int i,j;
	WORD val;

	// 알파 테이블이 초기화 된 적이 있다면 리턴
	if(g_bInitedAlphaTable) return;
	g_bInitedAlphaTable = TRUE;

	if(PixelFormat==dRGB555)
	for(i=0; i<32; i++)
	{
		for(j=0; j<32; j++)
		{
			// 알파 블렌딩 테이블 계산 (i=source color, j=alpha depth)
			R[i][j] = ((i * j)/31)<<10;
			G[i][j] = ((i * j)/31)<<5;
			B[i][j] = ((i * j)/31);

			// 광원 테이블 계산 (i=source color, j=background color)
			val = min((j + (31-j)*i/31), 31);
			LIGHT_R[i][j] = val<<10;
			LIGHT_G[i][j] = val<<5;
			LIGHT_B[i][j] = val;

			// Darken Effect 테이블 계산 (i=source color, j=background color)
			val = max(j - j*(31-i)/31, 0);
			DARK_R[i][j] = val<<10;
			DARK_G[i][j] = val<<5;
			DARK_B[i][j] = val;
			
			// 리버스 알파 블렌딩 그레이드 테이블 계산(a=alpha grade, i=sorce color)
			for(int a=0; a<32; a++)
				RVS_ABGRADE[a][i] = i + (int)((31-i) * (31-a) / 31);	

			// 알파 그레이드 테이블 계산(i=target grade, j=source alpha)
			ABGRADE[i][j] = (int)(j * i / 31);
		}
	}
	else 
	for(i=0; i<64; i++) // Green의 범위가 0-63이기 때문
	{
		for(j=0; j<64; j++)
		{
			// 알파 블렌딩 테이블 계산 (i=source color, j=alpha depth)
			if(i<32 && j<32)
			{
				R[i][j] = (i * j / 32)<<11;
			}

			if(j<32)
			{
				G[i][j] = ((i * j)>>5)<<5;
			}

			if(i<32 && j<32)
			{
				B[i][j] = (i * j / 32);
			}

			// 광원 테이블 계산 (i=source color, j=background color)
			if(i<32 && j<32) 
			{
				val = min((j + (31-j)*i/32), 31);
				LIGHT_R[i][j] = val<<11;
				LIGHT_B[i][j] = val;
			}
			val = min((j + (63-j)*i/64), 63);
			LIGHT_G[i][j] = val<<5;

			// Darken Effect 테이블 계산 (i=source color, j=background color)
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
   그래픽 라이브러리 제거
---------------------------------------------------------------------------*/
void RemoveGrpLib()
{
    //CloseDirectDraw();
	//if(PagePtr!=NULL) free(PagePtr);
}

///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////




/*---------------------------------------------------------------------------
   수평선 그리기(클리핑)
---------------------------------------------------------------------------*/
void DrawHLine(int x, int y, int len, WORD *tar, WORD color )
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
   수직선 그리기(클리핑)
---------------------------------------------------------------------------*/
void DrawVLine(int x, int y, int len, WORD *tar, WORD color )
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
   XOR 수평선 그리기(클리핑)
---------------------------------------------------------------------------*/
void DrawHLineXOR(int x, int y, int len, WORD *tar)
{
	int i;
    if(y < _CY1 || y > _CY2 || x+len-1 < _CX1 || x > _CX2) return;

    if(x<_CX1) x = _CX1;
    if(x + len -1 > _CX2) len = _CX2 - x +1;

    tar += _LPITCH * y + x;

	for(i=1; i<len; i++) *(tar+i ) ^= 0xFFFF;
}

/*---------------------------------------------------------------------------
   XOR 수직선 그리기(클리핑)
---------------------------------------------------------------------------*/
void DrawVLineXOR(int x, int y, int len, WORD *tar)
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


inline BOOL ClipLine(int& x1, int& y1, int& x2, int& y2)
{
	if((x1 >= _CX1 && x1 <= _CX2) && (y1 >= _CY1 && y1 <= _CY2) && (x2 >= _CX1 && x2 <= _CX2) && (y2 >= _CY1 && y2 <= _CY2))
		return TRUE;

	if((x1 < _CX1 && x2 < _CX1) || (x1 > _CX2 && x2 > _CX2) || (y1 < _CY1 && y2 < _CY1) || (y1 > _CY2 && y2 > _CY2))
		return FALSE;

	// 넘어선 길이는 클리핑
	if((x2 - x1) != 0 && (y2 - y1) != 0) {
		if(x1 < _CX1) {
			float alpha = abs(_CX1 - x1) / (float)abs(x2 - x1);
			x1 = _CX1;
			y1 = (int)LERP(alpha, y1, y2);
		}
		if(x1 > _CX2) {
			float alpha = abs(x1 - _CX2) / (float)abs(x2 - x1);
			x1 = _CX2;
			y1 = (int)LERP(alpha, y1, y2);
		}

		if(x2 < _CX1) {
			float alpha = abs(_CX1 - x2) / (float)abs(x2 - x1);
			x2 = _CX1;
			y2 = (int)LERP(alpha, y2, y1);
		}
		if(x2 > _CX2) {
			float alpha = abs(x2 - _CX2) / (float)abs(x2 - x1);
			x2 = _CX2;
			y2 = (int)LERP(alpha, y2, y1);
		}

		if(y1 < _CY1) {
			float alpha = abs(_CY1 - y1) / (float)abs(y2 - y1);
			y1 = _CY1;
			x1 = (int)LERP(alpha, x1, x2);
		}
		if(y1 > _CY2) {
			float alpha = abs(y1 - _CY2) / (float)abs(y2 - y1);
			y1 = _CY2;
			x1 = (int)LERP(alpha, x1, x2);
		}

		if(y2 < _CY1) {
			float alpha = abs(_CY1 - y2) / (float)abs(y2 - y1);
			y2 = _CY1;
			x2 = (int)LERP(alpha, x2, x1);
		}
		if(y2 > _CY2) {
			float alpha = abs(y2 - _CY2) / (float)abs(y2 - y1);
			y2 = _CY2;
			x2 = (int)LERP(alpha, x2, x1);
		}

		if(x1 < _CX1 || x2 < _CX1 || y1 < _CY1 || y2 < _CY1 || x1 > _CX2 || x2 > _CX2 || y1 > _CY2 || y2 > _CY2)
			return FALSE;
	}
	else {
		if(x1 < _CX1) x1 = _CX1; 
		if(x1 > _CX2) x1 = _CX2;
		if(y1 < _CY1) y1 = _CY1; 
		if(y1 > _CY2) y1 = _CY2;

		if(x2 < _CX1) x2 = _CX1; 
		if(x2 > _CX2) x2 = _CX2;
		if(y2 < _CY1) y2 = _CY1; 
		if(y2 > _CY2) y2 = _CY2;
	}


	return TRUE;
}

/*---------------------------------------------------------------------------
   선 그리기(브레제남 알고리즘:Bresenham Algorithm)
---------------------------------------------------------------------------*/
void DrawBresLine(int xs, int ys, int xe, int ye, WORD *tar, WORD color )
{

	if(!ClipLine(xs, ys, xe, ye))
		return;

	//------------------------------------------------------------------------
	// INITIALIZE THE COMPONENTS OF THE ALGORITHM THAT ARE NOT AFFECTED BY THE
	// SLOPE OR DIRECTION OF THE LINE
	//------------------------------------------------------------------------
	int dX = abs(xe-xs);	// store the change in X and Y of the line endpoints
	int dY = abs(ye-ys);
	
	//------------------------------------------------------------------------
	// DETERMINE "DIRECTIONS" TO INCREMENT X AND Y (REGARDLESS OF DECISION)
	//------------------------------------------------------------------------
	int Xincr, Yincr;
	if(xs > xe) { Xincr=-1;			} else { Xincr=1; }				// which direction in X?
	if(ys > ye) { Yincr=-(int)_LPITCH;	} else { Yincr=(int)_LPITCH; }	// which direction in Y?

	// 프레임 버퍼의 시작 픽셀 주소를 계산
	tar += _LPITCH * ys + xs;
	
	//------------------------------------------------------------------------
	// DETERMINE INDEPENDENT VARIABLE (ONE THAT ALWAYS INCREMENTS BY 1 (OR -1) )
	// AND INITIATE APPROPRIATE LINE DRAWING ROUTINE (BASED ON FIRST OCTANT
	// ALWAYS). THE X AND Y'S MAY BE FLIPPED IF Y IS THE INDEPENDENT VARIABLE.
	//------------------------------------------------------------------------

	if(dX >= dY)	// if X is the independent variable
	{           
		int dPr 	= dY<<1;			// amount to increment decision if right is chosen (always)
		int dPru 	= dPr - (dX<<1);	// amount to increment decision if up is chosen
		int P 		= dPr - dX;			// decision variable start value

		for(; dX>=0; --dX)				// process each point in the line one at a time (just use dX)
		{
			*tar = color;				// plot the pixel

			if(P > 0)					// is the pixel going right AND up?
			{ 
				tar += Xincr;			// increment independent variable
				tar += Yincr;			// increment dependent variable
				P += dPru;				// increment decision (for up)		
			}
			else						// is the pixel just going right?
			{
				tar += Xincr;			// increment independent variable
				P += dPr;				// increment decision (for right)
			}
		}		
	}
	else			// if Y is the independent variable
	{
		int dPr 	= dX<<1;			// amount to increment decision if right is chosen (always)
		int dPru 	= dPr - (dY<<1);	// amount to increment decision if up is chosen
		int P 		= dPr - dY;			// decision variable start value

		for(; dY>=0; --dY)				// process each point in the line one at a time (just use dY)
		{
			*tar = color;				// plot the pixel

			if(P > 0)					// is the pixel going up AND right?
			{ 
				tar += Xincr;			// increment dependent variable
				tar += Yincr;			// increment independent variable
				P += dPru;				// increment decision (for up)
			}
			else						// is the pixel just going up?
			{
				tar += Yincr;			// increment independent variable
				P += dPr;				// increment decision (for right)
			}
		}		
	}
}

void DrawBresLineXOR(int xs, int ys, int xe, int ye, WORD *tar)
{
	if(!ClipLine(xs, ys, xe, ye))
		return;

	int dX = abs(xe-xs);	// store the change in X and Y of the line endpoints
	int dY = abs(ye-ys);
	
	int Xincr, Yincr;
	if(xs > xe) { Xincr=-1;			} else { Xincr=1; }				// which direction in X?
	if(ys > ye) { Yincr=-(int)_LPITCH;	} else { Yincr=(int)_LPITCH; }	// which direction in Y?

	// 프레임 버퍼의 시작 픽셀 주소를 계산
	tar += _LPITCH * ys + xs;
	
	if(dX >= dY)	// if X is the independent variable
	{           
		int dPr 	= dY<<1;			// amount to increment decision if right is chosen (always)
		int dPru 	= dPr - (dX<<1);	// amount to increment decision if up is chosen
		int P 		= dPr - dX;			// decision variable start value

		for(; dX>=0; --dX)				// process each point in the line one at a time (just use dX)
		{
			*tar ^= (WORD)0xFFFF;		// plot the pixel

			if(P > 0)					// is the pixel going right AND up?
			{ 
				tar += Xincr;			// increment independent variable
				tar += Yincr;			// increment dependent variable
				P += dPru;				// increment decision (for up)		
			}
			else						// is the pixel just going right?
			{
				tar += Xincr;			// increment independent variable
				P += dPr;				// increment decision (for right)
			}
		}		
	}
	else			// if Y is the independent variable
	{
		int dPr 	= dX<<1;			// amount to increment decision if right is chosen (always)
		int dPru 	= dPr - (dY<<1);	// amount to increment decision if up is chosen
		int P 		= dPr - dY;			// decision variable start value

		for(; dY>=0; --dY)				// process each point in the line one at a time (just use dY)
		{
			*tar ^= (WORD)0xFFFF;		// plot the pixel

			if(P > 0)					// is the pixel going up AND right?
			{ 
				tar += Xincr;			// increment dependent variable
				tar += Yincr;			// increment independent variable
				P += dPru;				// increment decision (for up)
			}
			else						// is the pixel just going up?
			{
				tar += Yincr;			// increment independent variable
				P += dPr;				// increment decision (for right)
			}
		}		
	}
}

void DrawBresLineGB(int xs, int ys, int xe, int ye, WORD *tar, WORD color, int alpha )
{
	if(!ClipLine(xs, ys, xe, ye))
		return;

	int dX = abs(xe-xs);	// store the change in X and Y of the line endpoints
	int dY = abs(ye-ys);
	
	int Xincr, Yincr;
	if(xs > xe) { Xincr=-1;			} else { Xincr=1; }				// which direction in X?
	if(ys > ye) { Yincr=-(int)_LPITCH;	} else { Yincr=(int)_LPITCH; }	// which direction in Y?

	int dest_alpha = 31 - alpha;

	// 프레임 버퍼의 시작 픽셀 주소를 계산
	tar += _LPITCH * ys + xs;
	
	if(dX >= dY)	// if X is the independent variable
	{           
		int dPr 	= dY<<1;			// amount to increment decision if right is chosen (always)
		int dPru 	= dPr - (dX<<1);	// amount to increment decision if up is chosen
		int P 		= dPr - dX;			// decision variable start value

		for(; dX>=0; --dX)				// process each point in the line one at a time (just use dX)
		{
			*tar = ALPHAMIXW_555(*tar, color, dest_alpha, alpha);

			if(P > 0)					// is the pixel going right AND up?
			{ 
				tar += Xincr;			// increment independent variable
				tar += Yincr;			// increment dependent variable
				P += dPru;				// increment decision (for up)		
			}
			else						// is the pixel just going right?
			{
				tar += Xincr;			// increment independent variable
				P += dPr;				// increment decision (for right)
			}
		}		
	}
	else			// if Y is the independent variable
	{
		int dPr 	= dX<<1;			// amount to increment decision if right is chosen (always)
		int dPru 	= dPr - (dY<<1);	// amount to increment decision if up is chosen
		int P 		= dPr - dY;			// decision variable start value

		for(; dY>=0; --dY)				// process each point in the line one at a time (just use dY)
		{
			*tar = ALPHAMIXW_555(*tar, color, dest_alpha, alpha);

			if(P > 0)					// is the pixel going up AND right?
			{ 
				tar += Xincr;			// increment dependent variable
				tar += Yincr;			// increment independent variable
				P += dPru;				// increment decision (for up)
			}
			else						// is the pixel just going up?
			{
				tar += Yincr;			// increment independent variable
				P += dPr;				// increment decision (for right)
			}
		}		
	}
}

/*---------------------------------------------------------------------------
   색을 채운 박스 그리기(클리핑)
---------------------------------------------------------------------------*/
void FillBox(int x1, int y1, int x2, int y2, WORD *tar, WORD color )
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

// 다단계 반투명으로 메모리를 세팅(주의: WORD단위 세팅)
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
	
	for(i=0; i<HalfLen; i++)
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

		second	=	*p4_spr & d4AM_RGB555_2;
		second	*=	alpha;
		second	>>=	5;
		second	&=	d4AM_RGB555_2;

		ic4		=	first | second;


		*p4_scr  =	sc4+ic4;
		
		
		p4_scr++; 
		//p4_spr++;
	}

	if(Len & 0x00000001)	// == Len%2
	{
		/*
		WORD* p2_scr = (WORD*)p4_scr;
		*p2_scr	= ( (WORD)((((*p4_scr & d4AM_RGB555_1) >> 5) * dest_alpha) & d4AM_RGB555_1) | \
					((((*p4_scr & d4AM_RGB555_2)*dest_alpha)>>5) & d4AM_RGB555_2) )\
					+ \
				  ( (WORD)((((*p4_spr & d4AM_RGB555_1)>>5)*alpha) & d4AM_RGB555_1) | \
					((((*p4_spr & d4AM_RGB555_2)*alpha)>>5) & d4AM_RGB555_2) );
		*/

		// 스크린과 스프라이트의 마지막 픽셀에 접근하는 경우 DWORD 접근에서 
		// Read 억세스 바이얼레이션이 일어나므로 미리 WORD로 캐스팅한 값을 사용
		DWORD vscr = (DWORD)(*((WORD*)p4_scr));
		DWORD vspr = (DWORD)(*((WORD*)p4_spr));
		*((WORD*)p4_scr) = ( (WORD)((((vscr & d4AM_RGB555_1) >> 5) * dest_alpha) & d4AM_RGB555_1) | \
					(WORD)((((vscr & d4AM_RGB555_2)*dest_alpha)>>5) & d4AM_RGB555_2) )\
					+ \
				  ( (WORD)((((vspr & d4AM_RGB555_1)>>5)*alpha) & d4AM_RGB555_1) | \
					(WORD)((((vspr & d4AM_RGB555_2)*alpha)>>5) & d4AM_RGB555_2) );

	}
}

/*---------------------------------------------------------------------------
   다단계 반투명 박스 그리기(클리핑)
---------------------------------------------------------------------------*/
void FillBoxGB(int x1, int y1, int x2, int y2, WORD *tar, WORD color, int grade)
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
   속이 빈 박스 그리기(클리핑)
---------------------------------------------------------------------------*/
void DrawBox(int x1, int y1, int x2, int y2, WORD *tar, WORD color )
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
   속이 빈 XOR 박스 그리기(클리핑)
---------------------------------------------------------------------------*/
void DrawBoxXOR(int x1, int y1, int x2, int y2, WORD *tar)
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
   선택박스 그리기(색상 선택)
---------------------------------------------------------------------------*/
void DrawSelectBox(int x1, int y1, int x2, int y2, WORD *tar, WORD color)
{
	int dx1, dy1, dx2, dy2;
	dx1 = x1-1; dy1= y1-1; dx2 = x1+1; dy2 = y1+1;
	DrawBox(dx1, dy1, dx2, dy2, tar, color);
	dx1 = x2-1; dy1= y1-1; dx2 = x2+1; dy2 = y1+1;
	DrawBox(dx1, dy1, dx2, dy2, tar, color);
	dx1 = x2-1; dy1= y2-1; dx2 = x2+1; dy2 = y2+1;
	DrawBox(dx1, dy1, dx2, dy2, tar, color);
	dx1 = x1-1; dy1= y2-1; dx2 = x1+1; dy2 = y2+1;
	DrawBox(dx1, dy1, dx2, dy2, tar, color);
}

/*---------------------------------------------------------------------------
   선택박스 그리기(XOR)
---------------------------------------------------------------------------*/
void DrawXORSelectBox(int x1, int y1, int x2, int y2, WORD *tar)
{
	int dx1, dy1, dx2, dy2;
	dx1 = x1-1; dy1= y1-1; dx2 = x1+1; dy2 = y1+1;
	DrawBoxXOR(dx1, dy1, dx2, dy2, tar);
	dx1 = x2-1; dy1= y1-1; dx2 = x2+1; dy2 = y1+1;
	DrawBoxXOR(dx1, dy1, dx2, dy2, tar);
	dx1 = x2-1; dy1= y2-1; dx2 = x2+1; dy2 = y2+1;
	DrawBoxXOR(dx1, dy1, dx2, dy2, tar);
	dx1 = x1-1; dy1= y2-1; dx2 = x1+1; dy2 = y2+1;
	DrawBoxXOR(dx1, dy1, dx2, dy2, tar);
}

/*---------------------------------------------------------------------------
   서피스를 지정한 색으로 체움
---------------------------------------------------------------------------*/
void FillPage(WORD *tar, WORD fillColor )
{
	int i,j;
    for(i=0; i<_MYL; i++)
	{
		for(j=0; j<_MXL; j++) *(tar+j) = fillColor;
        tar += _LPITCH;
	}
}

/*---------------------------------------------------------------------------
   서피스에 점 출력
---------------------------------------------------------------------------*/
void PutPixel(int x, int y, WORD *tar, WORD color )
{
    *(tar+_LPITCH * y + x)  = color;
}

/*---------------------------------------------------------------------------
   특정 페이지에서 이미지를 얻음
---------------------------------------------------------------------------*/
void GetImage(int x, int y, int xl, int yl, WORD *page, WORD *tar )
{
    int i;

    page += _LPITCH * y + x;

    for(i=0; i<yl; i++)
    {
        memcpy(tar , page, xl<<1);
        page += _LPITCH;
        tar += xl;
    }
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------------------
이미지 출력 함수
---------------------------------------------------------------------------*/
void _PutImage(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor )
{
    int x=0, y=0, h=0, t=0;  // 초과치
    register int i;

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                            // 클리핑 영역 밖으로 나가면 출력하지 않음

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp; 
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || y || h || t)   // 클리핑이 발생되면
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
    else                    // 클리핑이 발생되지 않았을 경우
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
 이미지 출력 함수(상하 반전)
--------------------------------------------------------------------------*/
void _PutImageUD(int xp, int yp, short xLen, short yLen, WORD *tar, WORD *sor )
{
    int x=0, y=0, h=0, t=0;  // 초과치
    int rx, ry;              // 실제 찍힐 좌표
    register int i;
    register int xLimit, yLimit;

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                            // 클리핑 영역 밖으로 나가면 출력하지 않음

    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;
    if( xp < _CX1 ) { x = _CX1 - xp; rx = _CX1; } else rx = xp;
    if( yp < _CY1 ) { y = _CY1 - yp; ry=_CY1; } else ry = yp;


    if( x || y || h || t)   // 클리핑이 발생되면
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
    else                      // 클리핑이 발생되지 않았을 경우
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

void _PutImage_GB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade)
{
	if(grade == 0)
		return;

    int x=0, y=0, h=0, t=0;  // 초과치
    register int i;

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                            // 클리핑 영역 밖으로 나가면 출력하지 않음

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp; 
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || y || h || t)   // 클리핑이 발생되면
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
    else                    // 클리핑이 발생되지 않았을 경우
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
   스프라이트를 출력하는 함수
---------------------------------------------------------------------------*/
void _PutSpr(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey)
{
    int x=0, y=0, h=0, t=0;  // 초과치
	int i,j;

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
    int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
    int skip = _LPITCH - xLimit;
	int skip2 = x + h;

    tar += (yp+y) * _LPITCH + (xp+x);  // 실제찍을 좌표를 초과치로 보정하여 주소계산
	sor += y * xLen + x;

	for(i=0; i<yLimit; ++i) {
		//memcpy( tar , sor, xLimit);
		for(j=0; j<xLimit; ++j) {
			if(*sor != colorkey) 
				*tar = *sor;
			++tar;
			++sor;
		}
        tar += skip;
        sor += skip2;
    }
}


/*---------------------------------------------------------------------------
   스프라이트를 출력하는 함수(상하 반전)
---------------------------------------------------------------------------*/
void _PutSprUD(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey)
{
    int x=0, y=0, h=0, t=0;  // 초과치
	int i,j;

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                            // 클리핑 영역 밖으로 나가면 출력하지 않음
	if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
	if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;
    
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

// 비압축 데이터를 컬러키를 적용하여 다단계 반투명으로 메모리를 복사(주의: WORD단위 복사)
void _fastcall ColorKeyMemCpyGB555(WORD *tar, WORD *sor, WORD colorkey, int Len, int alpha)
{
	int i;
	DWORD	first,second,sc2,ic2;
	int dest_alpha = 31-alpha;

	WORD *p2_scr = tar;
	WORD *p2_spr = sor;
	
	for(i=0; i<Len; ++i,++p2_scr,++p2_spr)
	{
		if(colorkey == *p2_spr) 
			continue;

		first	=	*p2_scr & d4AM_RGB555_1;
		first	>>=	5;
		first	*=	dest_alpha;
		first	&=	d4AM_RGB555_1;

		second	=	*p2_scr & d4AM_RGB555_2;
		second	*=	dest_alpha;
		second	>>=	5;
		second	&=	d4AM_RGB555_2;

		sc2		=	first | second;
		
		first	=	*p2_spr & d4AM_RGB555_1;
		first	>>=	5;
		first	*=	alpha;
		first	&=	d4AM_RGB555_1;

		second	=	*p2_spr &d4AM_RGB555_2;
		second	*=	alpha;
		second	>>=	5;
		second	&=	d4AM_RGB555_2;

		ic2		=	first | second;

		*p2_scr  =	(WORD)(sc2+ic2);
	}
}

/*---------------------------------------------------------------------------
   스프라이트를 다단계 반투명으로 출력하는 함수
---------------------------------------------------------------------------*/
void _PutSpr_GB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey, int grade)
{
	if(grade == 0)
		return;

    int x=0, y=0, h=0, t=0;  // 초과치
	int i;

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
    int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산

    tar += (yp+y) * _LPITCH + (xp+x);  // 실제찍을 좌표를 초과치로 보정하여 주소계산
	sor += y * xLen + x;

    for(i=0; i<yLimit; i++) {
		ColorKeyMemCpyGB(tar, sor, colorkey, xLimit, grade);
        tar += _LPITCH;
        sor += xLen;
    }
}

/*---------------------------------------------------------------------------
   스프라이트를 라이트닝 효과로 출력하는 함수
---------------------------------------------------------------------------*/
void _PutSpr_LE(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey, int grade)
{
    int x=0, y=0, h=0, t=0;  // 초과치
	int i, j;

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

	int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
    int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
    int skip = _LPITCH - xLimit;
	int skip2 = x + h;

    tar += (yp+y) * _LPITCH + (xp+x);  // 실제찍을 좌표를 초과치로 보정하여 주소계산
	sor += y * xLen + x;

    for(i=0; i<yLimit; ++i) {
		for(j=0; j<xLimit; ++j) {
			if(*sor != colorkey) {
				*tar = LEMIXW_555(*tar, *sor, grade);
			}
			++tar;
			++sor;
		}
        tar += skip;
        sor += skip2;
    }
}

/*---------------------------------------------------------------------------
   스프라이트를 다크닝닝 효과로 출력하는 함수
---------------------------------------------------------------------------*/
void _PutSpr_DE(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey, int grade)
{
    int x=0, y=0, h=0, t=0;  // 초과치
	int i, j;

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

	int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
    int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
    int skip = _LPITCH - xLimit;
	int skip2 = x + h;

    tar += (yp+y) * _LPITCH + (xp+x);  // 실제찍을 좌표를 초과치로 보정하여 주소계산
	sor += y * xLen + x;

    for(i=0; i<yLimit; ++i) {
		for(j=0; j<xLimit; ++j) {
			if(*sor != colorkey) {
				*tar = DEMIXW_555(*tar, *sor, grade);
			}
			++tar;
			++sor;
		}
        tar += skip;
        sor += skip2;
    }
}

/*---------------------------------------------------------------------------
   스프라이트를 알파 블렌딩 효과로 출력하는 함수
---------------------------------------------------------------------------*/
void _PutSpr_AB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey, BYTE* alpha_table, int grade)
{
    int x=0, y=0, h=0, t=0;  // 초과치
	int i, j;

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

	int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
    int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
    int skip = _LPITCH - xLimit;
	int skip2 = x + h;

    tar += (yp+y) * _LPITCH + (xp+x);  // 실제찍을 좌표를 초과치로 보정하여 주소계산
	sor += y * xLen + x;

	// 알파테이블 포인터를 찍기 시작할 라인의 X좌표로 이동
	BYTE* palpha = alpha_table + ((y * xLen) + x);

    for(i=0; i<yLimit; ++i) {
		for(j=0; j<xLimit; ++j) {
			if(*sor != colorkey) {
				int alpha = ABGRADE[grade][(*palpha)];
				int dest_alpha = 31-alpha;
				if(alpha > 0) {
					if(alpha == 31) *tar = *sor;
					else *tar = ALPHAMIXW_555(*tar, *sor, dest_alpha, alpha);
				}
			}
			++tar;
			++sor;
			++palpha;
		}
        tar += skip;
        sor += skip2;
		palpha += skip2;
    }
}

////////////////////////////////////////////////////////////////////////////////////
// Compress Sprite Drawing Function
////////////////////////////////////////////////////////////////////////////////////

// 4바이트 메모리 역순 복사(주의: WORD단위 복사, Len=WORD길이)
void _fastcall RvrsCpy(WORD *tar, WORD *sor, int Len)
{
	int i;
	tar += Len-1;
	for(i=0; i<Len; i++) *tar-- = *sor++;
}


/*---------------------------------------------------------------------------
   압축 스프라이트 출력
---------------------------------------------------------------------------*/
void _PutCsp(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor )
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // 클리핑이 발생되면
    {
		int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(y<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // 패턴수를 읽음
			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
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
    else  // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++; // 투명색 길이(점의 개수)
				pbyte = *sor++;  // 이미지 길이(점의 개수)
				
				memcpy(tar+sbyte, sor, pbyte<<1);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}


/*---------------------------------------------------------------------------
   압축 스프라이트를 출력하는 함수(좌우반전)
---------------------------------------------------------------------------*/
void _PutCspLR(int xp, int yp, int xLen, int yLen, WORD* tar, WORD* sor )
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // 좌우 클리핑이 발생되면
    {
        int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
	    int sbyte,pbyte;
		int temp;

        tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // 찍을 좌표 계산(초과치 보정) - 역순
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동 - 순차

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt=*sor++;  // 패턴수를 읽음

			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte =  *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						temp = xLimit-sbyte; // 찍을 도트수
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
    else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산
        
		tar += (yp+y) * _LPITCH + xp + (xLen-1);   // 찍을 좌표계산
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++; // 투명색 길이(점의 개수)
				pbyte = *sor++;  // 이미지 길이(점의 개수)
				
				RvrsCpy(tar-sbyte-pbyte+1, sor, pbyte); // 역순 복사

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}


/*---------------------------------------------------------------------------
   압축 스프라이트를 출력하는 함수(상하반전)
---------------------------------------------------------------------------*/
void _PutCspUD(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor)
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // 좌우 클리핑이 발생되면
    {
        int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y+yLimit-1) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(t<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar-=_LPITCH)
		{
			cnt= *sor++;  // 패턴수를 읽음

			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
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
    else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

		int i,cnt;
		int sbyte,pbyte;
        
		tar += (yp+y+yLimit-1) * _LPITCH + xp;   // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(t<<1)));  // 찍기 시작할 라인으로 이동

		for(i=0; i<yLimit; i++,tar-=_LPITCH)
		{
			cnt = *sor++;

			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
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

// 반투명으로 메모리를 복사한다(주의: 길이는 WORD단위)
void _fastcall MemCpyHB555(WORD *tar, WORD *sor, int Len)
{
	int i;
	DWORD *p4_scr=(DWORD *)tar;
	DWORD *p4_spr=(DWORD *)sor;
	int HalfLen	= Len/2;
	
	for(i=0; i<HalfLen; i++, p4_scr++, p4_spr++)
		*p4_scr = (((*p4_scr & d4HM_RGB555)>>1)+((*p4_spr & d4HM_RGB555)>>1));

	if(Len & 0x00000001)	// == Len%2
	{
		tar += Len-1;
		sor += Len-1;
		*tar = HBMIXW_555(*tar, *sor);
	}
}

void _fastcall MemCpyHB565(WORD *tar, WORD *sor, int Len)
{
	int i;
	DWORD *p4_scr=(DWORD *)tar;
	DWORD *p4_spr=(DWORD *)sor;
	int HalfLen	= Len/2;
	
	for(i=0; i<HalfLen; i++, p4_scr++, p4_spr++)
		*p4_scr = (((*p4_scr & d4HM_RGB565)>>1)+((*p4_spr & d4HM_RGB565)>>1));

	if(Len & 0x00000001)	// == Len%2
	{
		tar += Len-1;
		sor += Len-1;
		*tar = (((*tar & d2HM_RGB565)>>1)+((*sor & d2HM_RGB565)>>1));
	}
}

// 반투명으로 메모리를 역순 복사한다(주의: 길이는 WORD단위)
void _fastcall RvrsCpyHB555(WORD *tar, WORD *sor, int Len)
{
	int i;
	
	tar += (Len-1);
	int HalfLen	= Len/2;

	if(Len & 0x00000001)	// == Len%2
	{
		*tar = (((*tar & d2HM_RGB555)>>1)+((*sor & d2HM_RGB555)>>1));
		// 타겟 감소
		--tar;
		++sor;
	}

	DWORD *p4_scr=(DWORD*)(tar-1); // 더블워드이기 때문에 역순 복사시 핀트를 맞춘다
	DWORD *p4_spr=(DWORD*)sor;
		
	DWORD sval;
	for(i=0; i<HalfLen; ++i, p4_scr--, p4_spr++) {
		sval = ((*p4_spr<<16)|(*p4_spr>>16));	// 소스의 워드 뒤집기
		*p4_scr = (((*p4_scr & d4HM_RGB555)>>1)+((sval & d4HM_RGB555)>>1));
	}
}

void _fastcall RvrsCpyHB565(WORD *tar, WORD *sor, int Len)
{
	int i;
	
	tar += (Len-1);
	int HalfLen	= Len/2;

	if(Len & 0x00000001)	// == Len%2
	{
		*tar = (((*tar & d2HM_RGB565)>>1)+((*sor & d2HM_RGB565)>>1));
		// 타겟 감소
		--tar;
		++sor;
	}

	DWORD *p4_scr=(DWORD*)(tar-1); // 더블워드이기 때문에 역순 복사시 핀트를 맞춘다
	DWORD *p4_spr=(DWORD*)sor;
		
	DWORD sval;
	for(i=0; i<HalfLen; ++i, p4_scr--, p4_spr++) {
		sval = ((*p4_spr<<16)|(*p4_spr>>16));	// 소스의 워드 뒤집기
		*p4_scr = (((*p4_scr & d4HM_RGB565)>>1)+((sval & d4HM_RGB565)>>1));
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 반투명으로 출력
---------------------------------------------------------------------------*/
void _PutCsp_HB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor )
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // 클리핑이 발생되면
    {
		int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(y<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // 패턴수를 읽음
			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
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
    else  // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++; // 투명색 길이(점의 개수)
				pbyte = *sor++;  // 이미지 길이(점의 개수)
				
				MemCpyHB(tar+sbyte, sor, pbyte);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}


/*---------------------------------------------------------------------------
   압축 스프라이트를 반투명으로 출력 (좌우 반전)
---------------------------------------------------------------------------*/
void _PutCspLR_HB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor )
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // 좌우 클리핑이 발생되면
    {
        int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
	    int sbyte,pbyte;
		int temp;

        tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // 찍을 좌표 계산(초과치 보정) - 역순
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동 - 순차

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt=*sor++;  // 패턴수를 읽음

			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte =  *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						temp = xLimit-sbyte; // 찍을 도트수
						RvrsCpyHB(tar-sbyte-temp+1, sor, temp);
						doing = ENDPUTSPR;
					}
					else RvrsCpyHB(tar-sbyte-pbyte+1, sor, pbyte);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산
        
		tar += (yp+y) * _LPITCH + xp + (xLen-1);   // 찍을 좌표계산
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++; // 투명색 길이(점의 개수)
				pbyte = *sor++;  // 이미지 길이(점의 개수)
				
				RvrsCpyHB(tar-sbyte-pbyte+1, sor, pbyte); // 역순 복사

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 반투명으로 출력(상하반전)
---------------------------------------------------------------------------*/
void _PutCspUD_HB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor )
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // 좌우 클리핑이 발생되면
    {
        int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y+yLimit-1) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(t<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar-=_LPITCH)
		{
			cnt= *sor++;  // 패턴수를 읽음

			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
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
    else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

		int i,cnt;
		int sbyte,pbyte;
        
		tar += (yp+y+yLimit-1) * _LPITCH + xp;   // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(t<<1)));  // 찍기 시작할 라인으로 이동

		for(i=0; i<yLimit; i++,tar-=_LPITCH)
		{
			cnt = *sor++;

			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
				
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

// 안티 복사(주의: WORD단위 복사, Len=WORD길이)
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

// 안티 복사(주의: WORD단위 복사, Len=WORD길이)
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
   압축 스프라이트를 안티-알라이싱 해서 출력
---------------------------------------------------------------------------*/
void _PutCsp_Anti(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor )
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // 클리핑이 발생되면
    {
		int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(y<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // 패턴수를 읽음
			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
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
    else  // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++; // 투명색 길이(점의 개수)
				pbyte = *sor++;  // 이미지 길이(점의 개수)
				
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

// 글로우효과 복사(주의: WORD단위 복사, Len=WORD길이)
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
	
	for(i=0; i<HalfLen; i++)
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

	if(Len & 0x00000001)	// == Len%2
	{
		/*
		WORD* p2_scr = (WORD*)p4_scr;
		*p2_scr	= (WORD)(sc4)+
				  ((WORD)((((*p4_spr & d4AM_RGB555_1)>>5)*alpha) & d4AM_RGB555_1) | \
					((((*p4_spr & d4AM_RGB555_2)*alpha)>>5) & d4AM_RGB555_2) );
		*/

		// 스크린과 스프라이트의 마지막 픽셀에 접근하는 경우 DWORD 접근에서 
		// Read 억세스 바이얼레이션이 일어나므로 미리 WORD로 캐스팅한 값을 사용
		DWORD vspr = (DWORD)(*((WORD*)p4_spr));
		*((WORD*)p4_scr) = (WORD)(sc4) +
				  ((WORD)((((vspr & d4AM_RGB555_1)>>5)*alpha) & d4AM_RGB555_1) | \
					(WORD)((((vspr & d4AM_RGB555_2)*alpha)>>5) & d4AM_RGB555_2) );
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

	first	=	color4 & d4AM_RGB565_1;
	first	>>=	5;
	first	*=	dest_alpha;
	first	&=	d4AM_RGB565_1;

	second	=	color4 & d4AM_RGB565_2;
	second	*=	dest_alpha;
	second	>>=	5;
	second	&=	d4AM_RGB565_2;

	sc4		=	first | second;
	
	for(i=0; i<HalfLen; i++)
	{
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

	if(Len & 0x00000001)	// == Len%2
	{
		/*
		WORD* p2_scr = (WORD*)p4_scr;
		*p2_scr	= ( (WORD)((((color4 & d4AM_RGB565_1) >> 5) * dest_alpha) & d4AM_RGB565_1) | \
					((((color4 & d4AM_RGB565_2)*dest_alpha)>>5) & d4AM_RGB565_2) )\
					+ \
				  ( (WORD)((((*p4_spr & d4AM_RGB565_1)>>5)*alpha) & d4AM_RGB565_1) | \
					((((*p4_spr & d4AM_RGB565_2)*alpha)>>5) & d4AM_RGB565_2) );
		*/

		// 스크린과 스프라이트의 마지막 픽셀에 접근하는 경우 DWORD 접근에서 
		// Read 억세스 바이얼레이션이 일어나므로 미리 WORD로 캐스팅한 값을 사용
		DWORD vspr = (DWORD)(*((WORD*)p4_spr));
		*((WORD*)p4_scr) = ( (WORD)((((color4 & d4AM_RGB565_1) >> 5) * dest_alpha) & d4AM_RGB565_1) | \
					(WORD)((((color4 & d4AM_RGB565_2)*dest_alpha)>>5) & d4AM_RGB565_2) )\
					+ \
				  ( (WORD)((((vspr & d4AM_RGB565_1)>>5)*alpha) & d4AM_RGB565_1) | \
					(WORD)((((vspr & d4AM_RGB565_2)*alpha)>>5) & d4AM_RGB565_2) );
	}
}

// 글로우효과로 메모리를 역으로 복사(주의: WORD단위 복사)
void _fastcall RvrsCpyGlow555(WORD *tar, WORD *sor, int Len, WORD color, int alpha)
{
	int HalfLen = Len>>1;
	int i;

	UINT	first,second,sc4,ic4;
	int dest_alpha = 31-alpha;

	tar += Len-1;

	UINT* p4_scr=(UINT *)tar; // 타겟은 역순 증가
	UINT* p4_spr=(UINT *)sor; // 소스는 순차 증가
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

	if(Len & 0x00000001)	// == Len%2
	{
		// 스크린과 스프라이트의 마지막 픽셀에 접근하는 경우 DWORD 접근에서 
		// Read 억세스 바이얼레이션이 일어나므로 미리 WORD로 캐스팅한 값을 사용
		DWORD vspr = (DWORD)(*((WORD*)p4_spr));
		*((WORD*)p4_scr) = (WORD)(sc4) +
				  ((WORD)((((vspr & d4AM_RGB555_1)>>5)*alpha) & d4AM_RGB555_1) | \
					(WORD)((((vspr & d4AM_RGB555_2)*alpha)>>5) & d4AM_RGB555_2) );
		
		// 타겟 감소
		--tar;
		++sor;

		p4_spr=(UINT *)sor;

	}

	p4_scr=(UINT *)(tar-1); // 더블워드이기 때문에 역순 복사시 핀트를 맞춘다

	for(i=0; i<HalfLen; i++)
	{
		first	=	((*p4_spr<<16)|(*p4_spr>>16)) & d4AM_RGB555_1; // 소스 워드 뒤집기
		first	>>=	5;
		first	*=	alpha;
		first	&=	d4AM_RGB555_1;

		second	=	((*p4_spr<<16)|(*p4_spr>>16)) &d4AM_RGB555_2; // 소스 워드 뒤집기
		second	*=	alpha;
		second	>>=	5;
		second	&=	d4AM_RGB555_2;

		ic4		=	first | second;


		*p4_scr  =	sc4+ic4;
		
		
		p4_scr--; 
		p4_spr++;
	}
}

// 글로우효과로 메모리를 역으로 복사(주의: WORD단위 복사)
void _fastcall RvrsCpyGlow565(WORD *tar, WORD *sor, int Len, WORD color, int alpha)
{
	int HalfLen = Len>>1;
	int i;

	UINT	first,second,sc4,ic4;
	//WORD	sc2,ic2;
	int dest_alpha = 31-alpha;

	tar += Len-1;

	UINT *p4_scr=(UINT *)tar; // 타겟은 역순 증가
	UINT *p4_spr=(UINT *)sor; // 소스는 순차 증가
	UINT color4 = (UINT)(color<<16)|(UINT)color;

	first	=	color4 & d4AM_RGB565_1;
	first	>>=	5;
	first	*=	dest_alpha;
	first	&=	d4AM_RGB565_1;

	second	=	color4 & d4AM_RGB565_2;
	second	*=	dest_alpha;
	second	>>=	5;
	second	&=	d4AM_RGB565_2;

	sc4		=	first | second;

	if(Len & 0x00000001)	// == Len%2
	{
		// 스크린과 스프라이트의 마지막 픽셀에 접근하는 경우 DWORD 접근에서 
		// Read 억세스 바이얼레이션이 일어나므로 미리 WORD로 캐스팅한 값을 사용
		DWORD vspr = (DWORD)(*((WORD*)p4_spr));
		*((WORD*)p4_scr) = (WORD)(sc4) +
				  ((WORD)((((vspr & d4AM_RGB565_1)>>5)*alpha) & d4AM_RGB565_1) | \
					(WORD)((((vspr & d4AM_RGB565_2)*alpha)>>5) & d4AM_RGB565_2) );

		// 타겟 감소
		--tar;
		++sor;

		p4_spr=(UINT *)sor;
	}

	p4_scr=(UINT *)(tar-1); // 더블워드이기 때문에 역순 복사시 핀트를 맞춘다
	
	for(i=0; i<HalfLen; i++)
	{	
		first	=	((*p4_spr<<16)|(*p4_spr>>16)) & d4AM_RGB565_1; // 소스 워드 뒤집기
		first	>>=	5;
		first	*=	alpha;
		first	&=	d4AM_RGB565_1;

		second	=	((*p4_spr<<16)|(*p4_spr>>16)) &d4AM_RGB565_2; // 소스 워드 뒤집기
		second	*=	alpha;
		second	>>=	5;
		second	&=	d4AM_RGB565_2;

		ic4		=	first | second;


		*p4_scr  =	sc4+ic4;
		
		
		p4_scr--; 
		p4_spr++;
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 글로우 효과 출력
---------------------------------------------------------------------------*/
void _PutCsp_Glow(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color, int grade)
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // 클리핑이 발생되면
    {
		int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(y<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // 패턴수를 읽음
			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
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
    else  // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++; // 투명색 길이(점의 개수)
				pbyte = *sor++;  // 이미지 길이(점의 개수)
				
				MemCpyGlow(tar+sbyte, sor, pbyte, color, grade);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 글로우 효과 출력(좌우 반전)
---------------------------------------------------------------------------*/
void _PutCspLR_Glow(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color, int grade)
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // 좌우 클리핑이 발생되면
    {
        int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
	    int sbyte,pbyte;
		int temp;

        tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // 찍을 좌표 계산(초과치 보정) - 역순
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동 - 순차

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt=*sor++;  // 패턴수를 읽음

			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte =  *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						temp = xLimit-sbyte; // 찍을 도트수
						RvrsCpyGlow(tar-sbyte-temp+1, sor, temp, color, grade);
						doing = ENDPUTSPR;
					}
					else RvrsCpyGlow(tar-sbyte-pbyte+1, sor, pbyte, color, grade);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산
        
		tar += (yp+y) * _LPITCH + xp + (xLen-1);   // 찍을 좌표계산
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++; // 투명색 길이(점의 개수)
				pbyte = *sor++;  // 이미지 길이(점의 개수)
				
				RvrsCpyGlow(tar-sbyte-pbyte+1, sor, pbyte, color, grade); // 역순 복사

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 글로우 효과 출력(상하반전)
---------------------------------------------------------------------------*/
void _PutCspUD_Glow(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color, int grade)
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // 좌우 클리핑이 발생되면
    {
        int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y+yLimit-1) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(t<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar-=_LPITCH)
		{
			cnt= *sor++;  // 패턴수를 읽음

			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
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
    else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

		int i,cnt;
		int sbyte,pbyte;
        
		tar += (yp+y+yLimit-1) * _LPITCH + xp;   // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(t<<1)));  // 찍기 시작할 라인으로 이동

		for(i=0; i<yLimit; i++,tar-=_LPITCH)
		{
			cnt = *sor++;

			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
				
				MemCpyGlow(tar+sbyte, sor, pbyte, color, grade);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
// Grayscale Drawing Function
////////////////////////////////////////////////////////////////////////////////////

// 그레이 스케일 효과 복사(주의: WORD단위 복사, Len=WORD길이)
void _fastcall MemCpyGrayscl555(WORD *tar, WORD *sor, int Len)
{
	int i;
	WORD val;
	for(i=0; i<Len; ++i, ++tar, ++sor) {
		//val = (R555(*sor)>>1) + (G555(*sor)>>1) + (B555(*sor)>>1);	// RGB 각 원소별 평균값(약간 오차가 생기나 속도가 빠름)
		val = (R555(*sor) + G555(*sor) + B555(*sor)) / 3;			// RGB 각 원소별 평균값(정확한 산출식)
		*tar = (val<<10) | (val<<5) | (val);
	}
}

void _fastcall MemCpyGrayscl565(WORD *tar, WORD *sor, int Len)
{
	int i;
	WORD val;
	for(i=0; i<Len; ++i, ++tar, ++sor) {
		//val = (R565(*sor)>>1) + (G565(*sor)>>2) + (B565(*sor)>>1);	// RGB 각 원소별 평균값(약간 오차가 생기나 속도가 빠름)
		val = (R565(*sor) + (G565(*sor)>>1) + B565(*sor)) / 3;	// RGB 각 원소별 평균값(정확한 산출식)
		*tar = (val<<11) | (val<<6) | (val);
	}
}

// 그레이 스케일 효과로 메모리를 역으로 복사(주의: WORD단위 복사)
void _fastcall RvrsCpyGrayscl555(WORD *tar, WORD *sor, int Len)
{
	int i;
	WORD val;
	tar += Len-1;
	sor += Len-1;
	for(i=0; i<Len; ++i, --tar, --sor) {
		val = (R555(*sor) + G555(*sor) + B555(*sor)) / 3;			// RGB 각 원소별 평균값(정확한 산출식)
		*tar = (val<<10) | (val<<5) | (val);
	}
}

// 그레이 스케일 효과로 메모리를 역으로 복사(주의: WORD단위 복사)
void _fastcall RvrsCpyGrayscl565(WORD *tar, WORD *sor, int Len)
{
	int i;
	WORD val;
	tar += Len-1;
	sor += Len-1;
	for(i=0; i<Len; ++i, --tar, --sor) {
		val = (R565(*sor) + (G565(*sor)>>1) + B565(*sor)) / 3;	// RGB 각 원소별 평균값(정확한 산출식)
		*tar = (val<<11) | (val<<6) | (val);
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 그레이스케일 효과 출력
---------------------------------------------------------------------------*/
void _PutCsp_Grayscl(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor)
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // 클리핑이 발생되면
    {
		int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(y<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // 패턴수를 읽음
			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpyGrayscl(tar+sbyte, sor, (xLimit-sbyte));
						doing = ENDPUTSPR;
					}
					else MemCpyGrayscl(tar+sbyte, sor, pbyte);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else  // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++; // 투명색 길이(점의 개수)
				pbyte = *sor++;  // 이미지 길이(점의 개수)
				
				MemCpyGrayscl(tar+sbyte, sor, pbyte);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 그레이 스케일 효과 출력(좌우 반전)
---------------------------------------------------------------------------*/
void _PutCspLR_Grayscl(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor)
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // 좌우 클리핑이 발생되면
    {
        int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
	    int sbyte,pbyte;
		int temp;

        tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // 찍을 좌표 계산(초과치 보정) - 역순
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동 - 순차

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt=*sor++;  // 패턴수를 읽음

			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte =  *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						temp = xLimit-sbyte; // 찍을 도트수
						MemCpyGrayscl(tar-sbyte-temp+1, sor, temp);
						doing = ENDPUTSPR;
					}
					else MemCpyGrayscl(tar-sbyte-pbyte+1, sor, pbyte);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산
        
		tar += (yp+y) * _LPITCH + xp + (xLen-1);   // 찍을 좌표계산
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++; // 투명색 길이(점의 개수)
				pbyte = *sor++;  // 이미지 길이(점의 개수)
				
				MemCpyGrayscl(tar-sbyte-pbyte+1, sor, pbyte); // 역순 복사

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 그레이 스케일 효과 출력(상하반전)
---------------------------------------------------------------------------*/
void _PutCspUD_Grayscl(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor)
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // 좌우 클리핑이 발생되면
    {
        int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y+yLimit-1) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(t<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar-=_LPITCH)
		{
			cnt= *sor++;  // 패턴수를 읽음

			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpyGrayscl(tar+sbyte, sor, (xLimit-sbyte));
						doing = ENDPUTSPR;
					}
					else MemCpyGrayscl(tar+sbyte, sor, pbyte);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

		int i,cnt;
		int sbyte,pbyte;
        
		tar += (yp+y+yLimit-1) * _LPITCH + xp;   // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(t<<1)));  // 찍기 시작할 라인으로 이동

		for(i=0; i<yLimit; i++,tar-=_LPITCH)
		{
			cnt = *sor++;

			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
				
				MemCpyGrayscl(tar+sbyte, sor, pbyte);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
//  Mono Filtering Function
////////////////////////////////////////////////////////////////////////////////////

// 4바이트 메모리 세팅(주의: WORD단위 세팅, Len=WORD길이)
void _fastcall MemSet(WORD *tar, WORD color, int Len)
{
	int i;
	for(i=0; i<Len; i++) *tar++ = color;
}


// 4바이트 메모리 역순 세팅(주의: WORD단위 세팅, Len=WORD길이)
void _fastcall RvrsMemSet(WORD *tar, WORD color, int Len)
{
	int i;
	tar += Len-1;
	for(i=0; i<Len; i++) *tar-- = color;
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 모노 필터 효과 출력
---------------------------------------------------------------------------*/
void _PutCsp_Mono(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color)
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음

    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // 클리핑이 발생되면
    {
		int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(y<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // 패턴수를 읽음
			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemSet(tar+sbyte, color, (xLimit-sbyte));
						doing = ENDPUTSPR;
					}
					else MemSet(tar+sbyte, color, pbyte);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else  // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++; // 투명색 길이(점의 개수)
				pbyte = *sor++;  // 이미지 길이(점의 개수)
				
				MemSet(tar+sbyte, color, pbyte);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 모노 필터 효과 출력(좌우 반전)
---------------------------------------------------------------------------*/
void _PutCspLR_Mono(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color)
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // 좌우 클리핑이 발생되면
    {
        int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
	    int sbyte,pbyte;
		int temp;

        tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // 찍을 좌표 계산(초과치 보정) - 역순
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동 - 순차

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt=*sor++;  // 패턴수를 읽음

			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte =  *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						temp = xLimit-sbyte; // 찍을 도트수
						RvrsMemSet(tar-sbyte-temp+1, color, temp);
						doing = ENDPUTSPR;
					}
					else RvrsMemSet(tar-sbyte-pbyte+1, color, pbyte);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산
        
		tar += (yp+y) * _LPITCH + xp + (xLen-1);   // 찍을 좌표계산
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++; // 투명색 길이(점의 개수)
				pbyte = *sor++;  // 이미지 길이(점의 개수)
				
				RvrsMemSet(tar-sbyte-pbyte+1, color, pbyte); // 역순 복사

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 모노 필터 효과 출력(상하반전)
---------------------------------------------------------------------------*/
void _PutCspUD_Mono(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color)
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // 좌우 클리핑이 발생되면
    {
        int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y+yLimit-1) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(t<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar-=_LPITCH)
		{
			cnt= *sor++;  // 패턴수를 읽음

			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemSet(tar+sbyte, color, (xLimit-sbyte));
						doing = ENDPUTSPR;
					}
					else MemSet(tar+sbyte, color, pbyte);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

		int i,cnt;
		int sbyte,pbyte;
        
		tar += (yp+y+yLimit-1) * _LPITCH + xp;   // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(t<<1)));  // 찍기 시작할 라인으로 이동

		for(i=0; i<yLimit; i++,tar-=_LPITCH)
		{
			cnt = *sor++;

			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
				
				MemSet(tar+sbyte, color, pbyte);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////
//  Multi Grade Blandigs Function
////////////////////////////////////////////////////////////////////////////////////

// 다단계 반투명으로 메모리를 복사(주의: WORD단위 복사)
void _fastcall MemCpyGB555(WORD *tar, WORD *sor, int Len, int alpha)
{
	int HalfLen = Len>>1;
	int i;

	UINT	first,second,sc4,ic4;
	//WORD	sc2,ic2;
	int dest_alpha = 31-alpha;

	UINT *p4_scr=(UINT *)tar;
	UINT *p4_spr=(UINT *)sor;
	
	for(i=0; i<HalfLen; ++i)
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
		
		
		++p4_scr; 
		++p4_spr;
	}

	if(Len & 0x00000001)	// == Len%2
	{
		/*
		// 문제 있는 버전
		WORD* p2_scr = (WORD*)p4_scr;
		*p2_scr	= ( (WORD)((((*p4_scr & d4AM_RGB555_1) >> 5) * dest_alpha) & d4AM_RGB555_1) | \
					((((*p4_scr & d4AM_RGB555_2)*dest_alpha)>>5) & d4AM_RGB555_2) )\
					+ \
				  ( (WORD)((((*p4_spr & d4AM_RGB555_1)>>5)*alpha) & d4AM_RGB555_1) | \
					((((*p4_spr & d4AM_RGB555_2)*alpha)>>5) & d4AM_RGB555_2) );
		*/

		/*
		// 스크린과 스프라이트의 마지막 픽셀에 접근하는 경우 DWORD 접근에서 
		// Read 억세스 바이얼레이션이 일어나므로 미리 WORD로 캐스팅한 값을 사용
		DWORD vscr = (DWORD)(*((WORD*)p4_scr));
		DWORD vspr = (DWORD)(*((WORD*)p4_spr));
		*((WORD*)p4_scr) = ( (WORD)((((vscr & d4AM_RGB555_1) >> 5) * dest_alpha) & d4AM_RGB555_1) | \
					(WORD)((((vscr & d4AM_RGB555_2)*dest_alpha)>>5) & d4AM_RGB555_2) )\
					+ \
				  ( (WORD)((((vspr & d4AM_RGB555_1)>>5)*alpha) & d4AM_RGB555_1) | \
					(WORD)((((vspr & d4AM_RGB555_2)*alpha)>>5) & d4AM_RGB555_2) );
		*/

		// 스크린과 스프라이트의 마지막 픽셀에 접근하는 경우 DWORD 접근에서 
		// Read 억세스 바이얼레이션이 일어나므로 미리 WORD로 캐스팅한 값을 사용
		*((WORD*)p4_scr) = ALPHAMIXW_555(*((WORD*)p4_scr), *((WORD*)p4_spr), dest_alpha, alpha);
	}
}

// 반투명으로 메모리를 복사(주의: WORD단위 복사)
void _fastcall MemCpyGB565(WORD *tar, WORD *sor, int Len, int alpha)
{
	int HalfLen = Len>>1;
	int i;

	UINT	first,second,sc4,ic4;
	//WORD	sc2,ic2;
	int dest_alpha = 31-alpha;

	UINT *p4_scr=(UINT *)tar;
	UINT *p4_spr=(UINT *)sor;
	
	for(i=0; i<HalfLen; ++i) // 2도트씩 처리
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
		
		
		++p4_scr; 
		++p4_spr;
	}

	if(Len & 0x00000001)	// == Len%2
	{
		/*
		WORD* p2_scr = (WORD*)p4_scr;
		*p2_scr	= ( (WORD)((((*p4_scr & d4AM_RGB565_1) >> 5) * dest_alpha) & d4AM_RGB565_1) | \
					((((*p4_scr & d4AM_RGB565_2)*dest_alpha)>>5) & d4AM_RGB565_2) )\
					+ \
				  ( (WORD)((((*p4_spr & d4AM_RGB565_1)>>5)*alpha) & d4AM_RGB565_1) | \
					((((*p4_spr & d4AM_RGB565_2)*alpha)>>5) & d4AM_RGB565_2) );
		*/

		// 스크린과 스프라이트의 마지막 픽셀에 접근하는 경우 DWORD 접근에서 
		// Read 억세스 바이얼레이션이 일어나므로 미리 WORD로 캐스팅한 값을 사용
		DWORD vscr = (DWORD)(*((WORD*)p4_scr));
		DWORD vspr = (DWORD)(*((WORD*)p4_spr));
		*((WORD*)p4_scr) = ( (WORD)((((vscr & d4AM_RGB565_1) >> 5) * dest_alpha) & d4AM_RGB565_1) | \
					(WORD)((((vscr & d4AM_RGB565_2)*dest_alpha)>>5) & d4AM_RGB565_2) )\
					+ \
				  ( (WORD)((((vspr & d4AM_RGB565_1)>>5)*alpha) & d4AM_RGB565_1) | \
					(WORD)((((vspr & d4AM_RGB565_2)*alpha)>>5) & d4AM_RGB565_2) );
	}
}

// 반투명으로 메모리를 역으로 복사(주의: WORD단위 복사)
void _fastcall RvrsCpyGB555(WORD *tar, WORD *sor, int Len, int alpha)
{
	int HalfLen = Len>>1;
	int i;

	UINT	first,second,sc4,ic4;
	int dest_alpha = 31-alpha;

	tar += Len-1;

	UINT* p4_scr=(UINT *)tar; // 타겟은 역순 증가
	UINT* p4_spr=(UINT *)sor; // 소스는 순차 증가

	if(Len & 0x00000001)	// == Len%2
	{
		/*
		WORD* p2_scr = (WORD*)p4_scr;
		*p2_scr	= ( (WORD)((((*p4_scr & d4AM_RGB555_1) >> 5) * dest_alpha) & d4AM_RGB555_1) | \
					((((*p4_scr & d4AM_RGB555_2)*dest_alpha)>>5) & d4AM_RGB555_2) )\
					+ \
				  ( (WORD)((((*p4_spr & d4AM_RGB555_1)>>5)*alpha) & d4AM_RGB555_1) | \
					((((*p4_spr & d4AM_RGB555_2)*alpha)>>5) & d4AM_RGB555_2) );
		*/

		// 스크린과 스프라이트의 마지막 픽셀에 접근하는 경우 DWORD 접근에서 
		// Read 억세스 바이얼레이션이 일어나므로 미리 WORD로 캐스팅한 값을 사용
		DWORD vscr = (DWORD)(*((WORD*)p4_scr));
		DWORD vspr = (DWORD)(*((WORD*)p4_spr));
		*((WORD*)p4_scr) = ( (WORD)((((vscr & d4AM_RGB555_1) >> 5) * dest_alpha) & d4AM_RGB555_1) | \
					(WORD)((((vscr & d4AM_RGB555_2)*dest_alpha)>>5) & d4AM_RGB555_2) )\
					+ \
				  ( (WORD)((((vspr & d4AM_RGB555_1)>>5)*alpha) & d4AM_RGB555_1) | \
					(WORD)((((vspr & d4AM_RGB555_2)*alpha)>>5) & d4AM_RGB555_2) );
		
		// 타겟 감소
		--tar;
		++sor;

		p4_spr=(UINT *)sor;

	}

	p4_scr=(UINT *)(tar-1); // 더블워드이기 때문에 역순 복사시 핀트를 맞춘다
	
	for(i=0; i<HalfLen; i++)
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
		
		first	=	((*p4_spr<<16)|(*p4_spr>>16)) & d4AM_RGB555_1; // 소스 워드 뒤집기
		first	>>=	5;
		first	*=	alpha;
		first	&=	d4AM_RGB555_1;

		second	=	((*p4_spr<<16)|(*p4_spr>>16)) &d4AM_RGB555_2; // 소스 워드 뒤집기
		second	*=	alpha;
		second	>>=	5;
		second	&=	d4AM_RGB555_2;

		ic4		=	first | second;


		*p4_scr  =	sc4+ic4;
		
		
		--p4_scr; 
		++p4_spr;
	}
}

// 반투명으로 메모리를 역으로 복사(주의: WORD단위 복사)
void _fastcall RvrsCpyGB565(WORD *tar, WORD *sor, int Len, int alpha)
{
	int HalfLen = Len>>1;
	int i;

	UINT	first,second,sc4,ic4;
	//WORD	sc2,ic2;
	int dest_alpha = 31-alpha;

	UINT *p4_scr=(UINT *)(tar+Len); // 타겟은 역순 증가
	UINT *p4_spr=(UINT *)sor;       // 소스는 순차 증가

	if(Len & 0x00000001)	// == Len%2
	{
		/*
		WORD* p2_scr = (WORD*)p4_scr;
		*p2_scr	= ( (WORD)((((*p4_scr & d4AM_RGB565_1) >> 5) * dest_alpha) & d4AM_RGB565_1) | \
					((((*p4_scr & d4AM_RGB565_2)*dest_alpha)>>5) & d4AM_RGB565_2) )\
					+ \
				  ( (WORD)((((*p4_spr & d4AM_RGB565_1)>>5)*alpha) & d4AM_RGB565_1) | \
					((((*p4_spr & d4AM_RGB565_2)*alpha)>>5) & d4AM_RGB565_2) );
		*/

		// 스크린과 스프라이트의 마지막 픽셀에 접근하는 경우 DWORD 접근에서 
		// Read 억세스 바이얼레이션이 일어나므로 미리 WORD로 캐스팅한 값을 사용
		DWORD vscr = (DWORD)(*((WORD*)p4_scr));
		DWORD vspr = (DWORD)(*((WORD*)p4_spr));
		*((WORD*)p4_scr) = ( (WORD)((((vscr & d4AM_RGB565_1) >> 5) * dest_alpha) & d4AM_RGB565_1) | \
					(WORD)((((vscr & d4AM_RGB565_2)*dest_alpha)>>5) & d4AM_RGB565_2) )\
					+ \
				  ( (WORD)((((vspr & d4AM_RGB565_1)>>5)*alpha) & d4AM_RGB565_1) | \
					(WORD)((((vspr & d4AM_RGB565_2)*alpha)>>5) & d4AM_RGB565_2) );

		// 타겟 감소
		--tar;
		++sor;

		p4_spr=(UINT *)sor;

	}

	p4_scr=(UINT *)(tar-1); // 더블워드이기 때문에 역순 복사시 핀트를 맞춘다
	
	for(i=0; i<HalfLen; ++i)
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
		
		first	=	((*p4_spr<<16)|(*p4_spr>>16)) & d4AM_RGB565_1; // 소스 워드 뒤집기
		first	>>=	5;
		first	*=	alpha;
		first	&=	d4AM_RGB565_1;

		second	=	((*p4_spr<<16)|(*p4_spr>>16)) &d4AM_RGB565_2; // 소스 워드 뒤집기
		second	*=	alpha;
		second	>>=	5;
		second	&=	d4AM_RGB565_2;

		ic4		=	first | second;


		*p4_scr  =	sc4+ic4;
		
		
		--p4_scr; 
		++p4_spr;
	}
}


/*---------------------------------------------------------------------------
   압축 스프라이트를 멀티그레이드 반투명으로 출력하는 함수
---------------------------------------------------------------------------*/
void _PutCsp_GB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade)
{
	if(grade == 0)
		return;

	int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // 클리핑이 발생되면
    {
		int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(y<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // 패턴수를 읽음
			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
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
    else  // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		//sor+=yLen<<1;   // Skip Line-Distance-Table(yLen*2)
		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++; // 투명색 길이(점의 개수)
				pbyte = *sor++;  // 이미지 길이(점의 개수)
				
				MemCpyGB(tar+sbyte, sor, pbyte, grade);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 멀티그레이드 반투명으로 출력하는 함수(좌우반전)
---------------------------------------------------------------------------*/
void _PutCspLR_GB(int xp, int yp, int xLen, int yLen, WORD* tar, WORD* sor, int grade)
{
	if(grade == 0)
		return;

    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // 좌우 클리핑이 발생되면
    {
        int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
	    int sbyte,pbyte;
		int temp;

        tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // 찍을 좌표 계산(초과치 보정) - 역순
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동 - 순차

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt=*sor++;  // 패턴수를 읽음

			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte =  *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						temp = xLimit-sbyte; // 찍을 도트수
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
    else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산
        
		tar += (yp+y) * _LPITCH + xp + (xLen-1);   // 찍을 좌표계산
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++; // 투명색 길이(점의 개수)
				pbyte = *sor++;  // 이미지 길이(점의 개수)
				
				RvrsCpyGB(tar-sbyte-pbyte+1, sor, pbyte, grade); // 역순 복사

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 멀티그레이드 반투명으로 출력하는 함수(상하반전)
---------------------------------------------------------------------------*/
void _PutCspUD_GB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade)
{
	if(grade == 0)
		return;

    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // 좌우 클리핑이 발생되면
    {
        int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y+yLimit-1) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(t<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar-=_LPITCH)
		{
			cnt= *sor++;  // 패턴수를 읽음

			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpyGB(tar+sbyte, sor, (xLimit-sbyte), grade);
						doing = ENDPUTSPR;
					}
					else MemCpyGB(tar+sbyte, sor, pbyte, grade);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

		int i,cnt;
		int sbyte,pbyte;
        
		tar += (yp+y+yLimit-1) * _LPITCH + xp;   // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(t<<1)));  // 찍기 시작할 라인으로 이동

		for(i=0; i<yLimit; i++,tar-=_LPITCH)
		{
			cnt = *sor++;

			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
				
				MemCpyGB(tar+sbyte, sor, pbyte, grade);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
//  Alpha Blandigs Function
////////////////////////////////////////////////////////////////////////////////////


// 알파 블렌딩으로 메모리를 복사(주의: WORD단위 복사)
void _fastcall MemCpyAB555(WORD *tar, WORD *sor, int Len, BYTE* palpha, int grade)
{
	for(int i=0; i<Len; ++i)
	{
		int alpha = ABGRADE[grade][(*palpha)];
		int dest_alpha = 31-alpha;

		if(alpha > 0) {
			//*tar = (R[R555(*sor)][alpha] | G[G555(*sor)][alpha] | B[B555(*sor)][alpha]) +
			//	   (R[R555(*tar)][dest_alpha] | G[G555(*tar)][dest_alpha] | B[B555(*tar)][dest_alpha]);

			if(alpha == 31) *tar = *sor;
			else *tar = ALPHAMIXW_555(*tar, *sor, dest_alpha, alpha);
		}

		++sor;
		++tar;
		++palpha;
	}
}

// 알파 블렌딩으로 메모리를 복사(주의: WORD단위 복사)
void _fastcall MemCpyAB565(WORD *tar, WORD *sor, int Len, BYTE* palpha, int grade)
{
	for(int i=0; i<Len; ++i)
	{
		int alpha = ABGRADE[grade][(*palpha)];
		int dest_alpha = 31-alpha;

		if(alpha > 0) {
			if(alpha == 31) *tar = *sor;
			// 아직 미완성(사용 불가)
			else *tar = (R[R565(*sor)][alpha] | G[G565(*sor)][alpha] | B[B565(*sor)][alpha]) +
				   (R[R565(*tar)][dest_alpha] | G[G565(*tar)][dest_alpha] | B[B565(*tar)][dest_alpha]);
		}

		++sor;
		++tar;
		++palpha;
	}
}

// 알파 블렌딩으로 메모리를 역순 복사(주의: WORD단위 복사)
void _fastcall RvrsCpyAB555(WORD *tar, WORD *sor, int Len, BYTE* palpha, int grade)
{
	tar += (Len-1);
	for(int i=0; i<Len; ++i) 
	{
		int alpha = ABGRADE[grade][(*palpha)];
		int dest_alpha = 31-alpha;

		if(alpha > 0) {
			//*tar = (R[R555(*sor)][alpha] | G[G555(*sor)][alpha] | B[B555(*sor)][alpha]) +
			//	   (R[R555(*tar)][dest_alpha] | G[G555(*tar)][dest_alpha] | B[B555(*tar)][dest_alpha]);
			*tar = ALPHAMIXW_555(*tar, *sor, dest_alpha, alpha);
		}

		++sor;
		--tar;
		++palpha;
	}
}

// 알파 블렌딩으로 메모리를 역순 복사(주의: WORD단위 복사)
void _fastcall RvrsCpyAB565(WORD *tar, WORD *sor, int Len, BYTE* palpha, int grade)
{
	tar += (Len-1);
	for(int i=0; i<Len; ++i) 
	{
		int alpha = ABGRADE[grade][(*palpha)];
		int dest_alpha = 31-alpha;

		if(alpha > 0) {
			// 아직 미완성(사용 불가)
			*tar = (R[R565(*sor)][alpha] | G[G565(*sor)][alpha] | B[B565(*sor)][alpha]) +
				   (R[R565(*tar)][dest_alpha] | G[G565(*tar)][dest_alpha] | B[B565(*tar)][dest_alpha]);
		}

		++sor;
		--tar;
		++palpha;
	}
}


/*---------------------------------------------------------------------------
   압축 스프라이트를 알파 블렌딩으로 출력하는 함수
---------------------------------------------------------------------------*/
void _PutCsp_AB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, BYTE* alpha_table, int grade)
{
	if(!alpha_table)
		return;

	int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // 클리핑이 발생되면
    {
		int xLimit = xLen - x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen - y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i, cnt, doing;
		int sbyte, pbyte;

		// 알파테이블 포인터를 찍기 시작할 라인의 X좌표로 이동
		BYTE* palpha = alpha_table + (xLen * y) + x;

        tar += (yp+y) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(y<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0; i<yLimit; ++i,tar+=_LPITCH)
		{
			cnt = *sor++;  // 패턴수를 읽음
			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpyAB(tar+sbyte, sor, xLimit-sbyte, palpha+sbyte, grade);
						doing = ENDPUTSPR;
					}
					else MemCpyAB(tar+sbyte, sor, pbyte, palpha+sbyte, grade); 
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}

			palpha += xLen;
		}
    }
    else  // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen - y - t;           // Y축으로 찍을 소스의 도트수 계산
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i, cnt;
	    int sbyte, pbyte;

		// 알파테이블 포인터를 찍기 시작할 라인으로 이동
		BYTE* palpha = alpha_table + (xLen * y);

		for(i=0; i<yLimit; ++i, tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;  // 투명색 길이(점의 개수)
				pbyte = *sor++;   // 이미지 길이(점의 개수)
				
				MemCpyAB(tar+sbyte, sor, pbyte, palpha+sbyte, grade);

				sor+=pbyte;
				sbyte+=pbyte;
			}

			palpha += xLen;
		}
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 알파 블렌딩으로 출력하는 함수(좌우반전)
---------------------------------------------------------------------------*/
void _PutCspLR_AB(int xp, int yp, int xLen, int yLen, WORD* tar, WORD* sor, BYTE* alpha_table, int grade)
{
	if(!alpha_table)
		return;

    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // 좌우 클리핑이 발생되면
    {
        int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
	    int sbyte,pbyte;
		int temp;

		// 알파테이블 포인터를 찍기 시작할 라인의 X좌표로 이동
		BYTE* palpha = alpha_table + (xLen * y) + h;

        tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // 찍을 좌표 계산(초과치 보정) - 역순
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동 - 순차

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt=*sor++;  // 패턴수를 읽음

			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte =  *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
				{
					if(sbyte >= h) { sbyte -= h; doing=BEGINPUTSPR; }
					else if(sbyte+pbyte > h) 
					{ 
						sor += h-sbyte; 
						pbyte = (pbyte - (h-sbyte));
						sbyte = 0;
						doing = BEGINPUTSPR;
					}
				}

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						temp = xLimit-sbyte; // 찍을 도트수
						RvrsCpyAB(tar-sbyte-temp+1, sor, temp, palpha+sbyte, grade);
						doing = ENDPUTSPR;
					}
					else RvrsCpyAB(tar-sbyte-pbyte+1, sor, pbyte, palpha+sbyte, grade);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}

			palpha += xLen;
		}
    }
    else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산
        
		tar += (yp+y) * _LPITCH + xp + (xLen-1);   // 찍을 좌표계산
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		// 알파테이블 포인터를 찍기 시작할 라인으로 이동
		BYTE* palpha = alpha_table + (xLen * y);

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++; // 투명색 길이(점의 개수)
				pbyte = *sor++;  // 이미지 길이(점의 개수)
				
				RvrsCpyAB(tar-sbyte-pbyte+1, sor, pbyte, palpha+sbyte, grade); // 역순 복사

				sor+=pbyte;
				sbyte+=pbyte;
			}

			palpha += xLen;
		}
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 알파 블렌딩으로 출력하는 함수(상하반전)
---------------------------------------------------------------------------*/
void _PutCspUD_AB(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, BYTE* alpha_table, int grade)
{
	if(!alpha_table)
		return;

    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // 좌우 클리핑이 발생되면
    {
        int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

		// 알파테이블 포인터를 찍기 시작할 라인의 X좌표로 이동
		BYTE* palpha = alpha_table + (xLen * t) + x;

        tar += (yp+y+yLimit-1) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(t<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar-=_LPITCH)
		{
			cnt= *sor++;  // 패턴수를 읽음

			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpyAB(tar+sbyte, sor, xLimit-sbyte, palpha+sbyte, grade);
						doing = ENDPUTSPR;
					}
					else MemCpyAB(tar+sbyte, sor, pbyte, palpha+sbyte, grade);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}

			palpha += xLen;
		}
    }
    else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

		int i,cnt;
		int sbyte,pbyte;
        
		tar += (yp+y+yLimit-1) * _LPITCH + xp;   // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(t<<1)));  // 찍기 시작할 라인으로 이동

		// 알파테이블 포인터를 찍기 시작할 라인으로 이동
		BYTE* palpha = alpha_table + (xLen * t);

		for(i=0; i<yLimit; i++,tar-=_LPITCH)
		{
			cnt = *sor++;

			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
				
				MemCpyAB(tar+sbyte, sor, pbyte, palpha+sbyte, grade);

				sor+=pbyte;
				sbyte+=pbyte;
			}

			palpha += xLen;
		}
	}
}



////////////////////////////////////////////////////////////////////////////////////
//  Lightning Effect Function
////////////////////////////////////////////////////////////////////////////////////

// Lightning Effect로 메모리를 복사(주의: WORD단위 복사)
void _fastcall MemCpyLE555(WORD *tar, WORD *sor, int Len, int grade)
{
	if(grade == 31) {
		for(int i=0; i<Len; ++i)
		{
			// 알파값에 영향 받지 않는 빠른 방식(알파 단계를 설정할 수 없음)
			// *tar = (LIGHT_R[R555(*sor)][R555(*tar)] | LIGHT_G[G555(*sor)][G555(*tar)] | LIGHT_B[B555(*sor)][B555(*tar)]);
			*tar = LEMIXW_555_FAST(*tar, *sor);
			++sor;
			++tar;
		}
	}
	else {
		for(int i=0; i<Len; ++i)
		{
			// 속도는 느리지만 알파 단계를 조절 할 수 있는 방식
			//*tar = (LIGHT_R[ABGRADE[alpha][R555(*sor)]][R555(*tar)] | LIGHT_G[ABGRADE[alpha][G555(*sor)]][G555(*tar)] | LIGHT_B[ABGRADE[alpha][B555(*sor)]][B555(*tar)]);
			*tar = LEMIXW_555(*tar, *sor, grade);
			++sor;
			++tar;
		}
	}
}

void _fastcall MemCpyLE565(WORD *tar, WORD *sor, int Len, int grade)
{
	if(grade == 31) {
		for(int i=0; i<Len; ++i)
		{
			// 알파값에 영향 받지 않는 빠른 방식(알파 단계를 설정할 수 없음)
			*tar = LEMIXW_565_FAST(*tar, *sor);
			++sor;
			++tar;
		}
	}
	else {
		for(int i=0; i<Len; ++i)
		{
			// 속도는 느리지만 알파 단계를 조절 할 수 있는 방식
			//*tar = (LIGHT_R[R565(*sor)][R565(*tar)] | LIGHT_G[G565(*sor)][G565(*tar)] | LIGHT_B[B565(*sor)][B565(*tar)]);
			*tar = LEMIXW_565(*tar, *sor, grade);
			++sor;
			++tar;
		}
	}
}

// Lightning Effect로 메모리를 역순 복사(주의: WORD단위 복사)
void _fastcall RvrsCpyLE555(WORD *tar, WORD *sor, int Len, int grade)
{
	tar += (Len-1);

	if(grade == 31) {
		for(int i=0; i<Len; ++i)
		{
			*tar = LEMIXW_555_FAST(*tar, *sor);
			++sor;
			--tar;
		}
	}
	else {
		for(int i=0; i<Len; ++i)
		{
			*tar = LEMIXW_555(*tar, *sor, grade);
			++sor;
			--tar;
		}
	}
}

void _fastcall RvrsCpyLE565(WORD *tar, WORD *sor, int Len, int grade)
{
	tar += (Len-1);
	if(grade == 31) {
		for(int i=0; i<Len; ++i)
		{
			*tar = LEMIXW_565_FAST(*tar, *sor);
			++sor;
			--tar;
		}
	}
	else {
		for(int i=0; i<Len; ++i)
		{
			*tar = LEMIXW_565(*tar, *sor, grade);
			++sor;
			--tar;
		}
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 Lightning Effect로 출력하는 함수
---------------------------------------------------------------------------*/
void _PutCsp_LE(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade)
{
	int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // 클리핑이 발생되면
    {
		int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(y<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // 패턴수를 읽음
			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpyLE(tar+sbyte, sor, xLimit-sbyte, grade);
						doing = ENDPUTSPR;
					}
					else MemCpyLE(tar+sbyte, sor, pbyte, grade);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else  // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++, tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;  // 투명색 길이(점의 개수)
				pbyte = *sor++;   // 이미지 길이(점의 개수)
				
				MemCpyLE(tar+sbyte, sor, pbyte, grade);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 Lightning Effect로 출력하는 함수 (좌우 반전)
---------------------------------------------------------------------------*/
void _PutCspLR_LE(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade)
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // 좌우 클리핑이 발생되면
    {
        int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
	    int sbyte,pbyte;
		int temp;

        tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // 찍을 좌표 계산(초과치 보정) - 역순
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동 - 순차

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt=*sor++;  // 패턴수를 읽음

			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte =  *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						temp = xLimit-sbyte; // 찍을 도트수
						RvrsCpyLE(tar-sbyte-temp+1, sor, temp, grade);
						doing = ENDPUTSPR;
					}
					else RvrsCpyLE(tar-sbyte-pbyte+1, sor, pbyte, grade);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산
        
		tar += (yp+y) * _LPITCH + xp + (xLen-1);   // 찍을 좌표계산
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++; // 투명색 길이(점의 개수)
				pbyte = *sor++;  // 이미지 길이(점의 개수)
				
				RvrsCpyLE(tar-sbyte-pbyte+1, sor, pbyte, grade); // 역순 복사

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를  Lightning Effect로 출력하는 함수 (상하반전)
---------------------------------------------------------------------------*/
void _PutCspUD_LE(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade)
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // 좌우 클리핑이 발생되면
    {
        int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y+yLimit-1) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(t<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar-=_LPITCH)
		{
			cnt= *sor++;  // 패턴수를 읽음

			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpyLE(tar+sbyte, sor, (xLimit-sbyte), grade);
						doing = ENDPUTSPR;
					}
					else MemCpyLE(tar+sbyte, sor, pbyte, grade);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

		int i,cnt;
		int sbyte,pbyte;
        
		tar += (yp+y+yLimit-1) * _LPITCH + xp;   // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(t<<1)));  // 찍기 시작할 라인으로 이동

		for(i=0; i<yLimit; i++,tar-=_LPITCH)
		{
			cnt = *sor++;

			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
				
				MemCpyLE(tar+sbyte, sor, pbyte, grade);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
//  Darken Effect Function
////////////////////////////////////////////////////////////////////////////////////

// Darken Effect로 메모리를 복사(주의: WORD단위 복사)
void _fastcall MemCpyDE555(WORD *tar, WORD *sor, int Len, int grade)
{
	if(grade == 31) {
		for(int i=0; i<Len; ++i)
		{
			// 알파값에 영향 받지 않는 빠른 방식(알파 단계를 설정할 수 없음)
			*tar = DEMIXW_555_FAST(*tar, *sor);
			++sor;
			++tar;
		}
	}
	else {
		for(int i=0; i<Len; ++i)
		{
			// 속도는 느리지만 알파 단계를 조절 할 수 있는 방식
			*tar = DEMIXW_555(*tar, *sor, grade);
			++sor;
			++tar;
		}
	}
}

void _fastcall MemCpyDE565(WORD *tar, WORD *sor, int Len, int grade)
{
	if(grade == 31) {
		for(int i=0; i<Len; ++i)
		{
			// 알파값에 영향 받지 않는 빠른 방식(알파 단계를 설정할 수 없음)
			*tar = DEMIXW_565_FAST(*tar, *sor);
			++sor;
			++tar;
		}
	}
	else {
		for(int i=0; i<Len; ++i)
		{
			// 속도는 느리지만 알파 단계를 조절 할 수 있는 방식
			*tar = DEMIXW_565(*tar, *sor, grade);
			++sor;
			++tar;
		}
	}
}

// Darken Effect로 메모리를 역순 복사(주의: WORD단위 복사)
void _fastcall RvrsCpyDE555(WORD *tar, WORD *sor, int Len, int grade)
{
	tar += (Len-1);

	if(grade == 31) {
		for(int i=0; i<Len; ++i)
		{
			*tar = DEMIXW_555_FAST(*tar, *sor);
			++sor;
			--tar;
		}
	}
	else {
		for(int i=0; i<Len; ++i)
		{
			*tar = DEMIXW_555(*tar, *sor, grade);
			++sor;
			--tar;
		}
	}
}

void _fastcall RvrsCpyDE565(WORD *tar, WORD *sor, int Len, int grade)
{
	tar += (Len-1);

	if(grade == 31) {
		for(int i=0; i<Len; ++i)
		{
			*tar = DEMIXW_565_FAST(*tar, *sor);
			++sor;
			--tar;
		}
	}
	else {
		for(int i=0; i<Len; ++i)
		{
			*tar = DEMIXW_565(*tar, *sor, grade);
			++sor;
			--tar;
		}
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 Darken Effect로 출력하는 함수
---------------------------------------------------------------------------*/
void _PutCsp_DE(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade)
{
	int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // 클리핑이 발생되면
    {
		int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(y<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // 패턴수를 읽음
			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpyDE(tar+sbyte, sor, xLimit-sbyte, grade);
						doing = ENDPUTSPR;
					}
					else MemCpyDE(tar+sbyte, sor, pbyte, grade);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else  // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++, tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;  // 투명색 길이(점의 개수)
				pbyte = *sor++;   // 이미지 길이(점의 개수)
				
				MemCpyDE(tar+sbyte, sor, pbyte, grade);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를 Darken Effect로 출력하는 함수 (좌우 반전)
---------------------------------------------------------------------------*/
void _PutCspLR_DE(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade)
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // 좌우 클리핑이 발생되면
    {
        int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
	    int sbyte,pbyte;
		int temp;

        tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // 찍을 좌표 계산(초과치 보정) - 역순
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동 - 순차

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt=*sor++;  // 패턴수를 읽음

			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte =  *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						temp = xLimit-sbyte; // 찍을 도트수
						RvrsCpyDE(tar-sbyte-temp+1, sor, temp, grade);
						doing = ENDPUTSPR;
					}
					else RvrsCpyDE(tar-sbyte-pbyte+1, sor, pbyte, grade);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산
        
		tar += (yp+y) * _LPITCH + xp + (xLen-1);   // 찍을 좌표계산
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++,tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++; // 투명색 길이(점의 개수)
				pbyte = *sor++;  // 이미지 길이(점의 개수)
				
				RvrsCpyDE(tar-sbyte-pbyte+1, sor, pbyte, grade); // 역순 복사

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

/*---------------------------------------------------------------------------
   압축 스프라이트를  Darken Effect로 출력하는 함수 (상하반전)
---------------------------------------------------------------------------*/
void _PutCspUD_DE(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade)
{
    int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )         // 좌우 클리핑이 발생되면
    {
        int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y+yLimit-1) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(t<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar-=_LPITCH)
		{
			cnt= *sor++;  // 패턴수를 읽음

			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
				{
					if(sbyte>=xLimit) doing = ENDPUTSPR;
					else if(sbyte + pbyte > xLimit)
					{
						MemCpyDE(tar+sbyte, sor, (xLimit-sbyte), grade);
						doing = ENDPUTSPR;
					}
					else MemCpyDE(tar+sbyte, sor, pbyte, grade);
				}

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
    }
    else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH + xLen;
		int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

		int i,cnt;
		int sbyte,pbyte;
        
		tar += (yp+y+yLimit-1) * _LPITCH + xp;   // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(t<<1)));  // 찍기 시작할 라인으로 이동

		for(i=0; i<yLimit; i++,tar-=_LPITCH)
		{
			cnt = *sor++;

			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
				
				MemCpyDE(tar+sbyte, sor, pbyte, grade);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////
//  change pixel format Function
////////////////////////////////////////////////////////////////////////////////////

// RGB555포맷을 RGB565로 메모리를 복사(주의: WORD단위 복사)
void _fastcall MemCpy555TO565(WORD *tar, WORD *sor, int Len)
{
	for(register int i=0; i<Len; i++) {
		*tar = ((*sor)&(WORD)0x001F) | ((*sor)&(WORD)0xFFE0)<<1;
		sor++;
		tar++;
	}
}

/*---------------------------------------------------------------------------
   RGB555압축 스프라이트를 RGB565로 출력하는 함수
---------------------------------------------------------------------------*/
void _PutCsp_RGB555TORGB565(int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor)
{
	int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

    if( x || h )               // 클리핑이 발생되면
    {
		int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(y<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar+=_LPITCH)
		{
			cnt = *sor++;  // 패턴수를 읽음
			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
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
    else  // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
        tar += (yp+y) * _LPITCH + xp;
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++, tar+=_LPITCH)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;  // 투명색 길이(점의 개수)
				pbyte = *sor++;   // 이미지 길이(점의 개수)
				
				MemCpy555TO565(tar+sbyte, sor, pbyte);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

// RGB555포맷을 RGB24로 메모리를 복사(주의: WORD단위 복사)
void _fastcall MemCpy555TORGB24(BYTE *tar, WORD *sor, int Len)
{
	for(register int i=0; i<Len; i++) {
		// 24비트는 BGR순이다
		tar[2] = (BYTE)(((*sor)&(WORD)0x7C00) >> 7);	// r
		tar[1] = (BYTE)(((*sor)&(WORD)0x03E0) >> 2);	// g
		tar[0] = (BYTE)(((*sor)&(WORD)0x001F) << 3);	// b
		sor++;
		tar+=3;
	}
}

/*---------------------------------------------------------------------------
   RGB555압축 스프라이트를 RGB24로 출력하는 함수
---------------------------------------------------------------------------*/
void _PutCsp_RGB555TORGB24(int xp, int yp, int xLen, int yLen, BYTE *tar, int lpitch, WORD *sor)
{
	int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

	int bytepitch = lpitch;

    if( x || h )               // 클리핑이 발생되면
    {
		int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += ((yp+y) * bytepitch) + (xp+x)*3; // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(y<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0; i<yLimit; i++, tar+=bytepitch)
		{
			cnt = *sor++;  // 패턴수를 읽음
			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
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
    else  // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        //int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
        tar += (yp+y) * bytepitch + xp*3;
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++, tar+=bytepitch)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;  // 투명색 길이(점의 개수)
				pbyte = *sor++;   // 이미지 길이(점의 개수)
				
				MemCpy555TORGB24(tar+sbyte*3, sor, pbyte);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}

// RGB555포맷을 RGB32로 메모리를 복사(주의: WORD단위 복사)
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
   RGB555압축 스프라이트를 RGB32로 출력하는 함수
---------------------------------------------------------------------------*/
void _PutCsp_RGB555TORGB32(int xp, int yp, int xLen, int yLen, DWORD *tar, int lpitch, WORD *sor)
{
	int x=0, y=0, h=0, t=0;  // 초과치

    if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
                             // 클리핑 영역 밖으로 나가면 출력하지 않음
    if( xp < _CX1 )          x = _CX1 - xp;
    if( yp < _CY1 )          y = _CY1 - yp;
    if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
    if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

	int qurdpitch = lpitch/4;

    if( x || h )               // 클리핑이 발생되면
    {
		int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
        int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
			
		int i,cnt,doing;
		int sbyte,pbyte;

        tar += (yp+y) * qurdpitch + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
		sor += (*(long*)(sor+(y<<1)));    // 찍기 시작할 라인으로 이동

		for(i=0;i<yLimit;i++,tar+=qurdpitch)
		{
			cnt = *sor++;  // 패턴수를 읽음
			sbyte=0;
			doing=0;

			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;
				pbyte = *sor++;
								
				if(doing==0)  // 출력이 시작되는 곳을 계산
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

				if(doing==BEGINPUTSPR)   // 출력할 상황이면
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
    else  // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
    {
        //int skip = _LPITCH - xLen;
		int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
        tar += (yp+y) * qurdpitch + xp;
		sor += *((long*)(sor+(y<<1)));  // 찍기 시작할 라인으로 이동

		int i,cnt;
	    int sbyte,pbyte;

		for(i=0; i<yLimit; i++, tar+=qurdpitch)
		{
			cnt = *sor++;
			sbyte=0;
			while(cnt--)         // 패턴수 만큼 반복
			{
				sbyte += *sor++;  // 투명색 길이(점의 개수)
				pbyte = *sor++;   // 이미지 길이(점의 개수)
				
				MemCpy555TORGB32(tar+sbyte, sor, pbyte);

				sor+=pbyte;
				sbyte+=pbyte;
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////                             /////////////////////////////////
////////////////////////////      스프라이트 HitTest     /////////////////////////////////
////////////////////////////                             /////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------------------
   일반 스프라이트 포인트 충돌 검사
---------------------------------------------------------------------------*/
BOOL _PointHitTestSpr(int xLen, int yLen, WORD *sor, WORD colorkey, int txp, int typ)
{
    if(txp<0 || txp>=xLen || typ<0 || typ>=yLen) 
		return FALSE;

	return (sor[(typ * xLen) + txp] != colorkey);
}

/*---------------------------------------------------------------------------
   압축 스프라이트 포인트 충돌 검사
---------------------------------------------------------------------------*/
BOOL _PointHitTestCsp(int xLen, int yLen, WORD *sor, int txp, int typ)
{
	if(txp<0 || txp>=xLen || typ<0 || typ>=yLen) 
		return FALSE;

	WORD* src = GET_CSPLINEPTR(sor, typ); // 검사할 소스의 라인으로 이동

	int cnt = *src++;
	int sbyte=0, pbyte;
	while(cnt--)         // 패턴수 만큼 반복
	{
		sbyte += *src++; // 투명색 길이(점의 개수)
		pbyte = *src++;  // 이미지 길이(점의 개수)

		// 검사할 위치가 투명색임
		if(sbyte > txp) return FALSE;

		src+=pbyte;
		sbyte+=pbyte;

		// 검사할 위치가 투명색이 아님
		if(sbyte > txp) return TRUE;
	}

	return FALSE;
}
BOOL _PointHitTestCspAlpha(int xLen, int yLen, BYTE *sor, int txp, int typ)
{


	if(txp<0 || txp>=xLen || typ<0 || typ>=yLen) 
	return FALSE;

	BYTE* src = GET_CSPLINEPTR(sor, typ); // 검사할 소스의 라인으로 이동

	int cnt = *src++;
	int sbyte=0, pbyte;
	while(cnt--)         // 패턴수 만큼 반복
	{
		sbyte += *src++; // 투명색 길이(점의 개수)
		pbyte = *src++;  // 이미지 길이(점의 개수)

		// 검사할 위치가 투명색임
		if(sbyte > txp) return FALSE;

		src+=pbyte;
		sbyte+=pbyte;

		// 검사할 위치가 투명색이 아님
		if(sbyte > txp) return TRUE;
	}
	return FALSE;

}

BOOL _PointHitTestSprpAlpha(int xLen, int yLen, BYTE *sor, WORD colorkey, int txp, int typ)
{
	if(txp<0 || txp>=xLen || typ<0 || typ>=yLen) 
		return FALSE;

	return (sor[(typ * xLen) + txp] != 0);
}
