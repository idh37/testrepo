#include "stdafx.h"
#include <math.h>
#include <assert.h>
#include "HUtil.h"
#include <sys/stat.h> // _stati64()

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



float cos_look[360] = {0.0f};
float sin_look[360] = {0.0f};
//const double PI2 = 3.1415926535;

void Build_Sin_Cos_Tables(void)
{
	// create sin/cos lookup table
	for( int ang = 0; ang < 360; ang++)
    {
		float theta = (float )((double )ang * 3.1415926535 / (double )180);
		
		cos_look[ang] = (float )cos(theta);
		sin_look[ang] = (float )sin(theta);		
    }
}

//-----------------------------------------------------------------------------
// Name: GetRandomValue()
// Desc: 난수값 얻어 오기
//-----------------------------------------------------------------------------
int GetRandomValue(int startVal, int endVal)
{
	if( startVal == endVal ) return startVal;

	if(startVal > endVal) 
	{
		int temp = startVal;
		startVal = endVal;
		endVal = temp;
	}

	int seed = endVal - startVal + 1;

	if( (seed + startVal) == 0 ) return 0;
	
	return rand() % seed + startVal;
}
//-----------------------------------------------------------------------------
// Name: Linear()
// Desc: 
//-----------------------------------------------------------------------------
float Linear(float v0, float v1, float t)
{
	return v0 + t * (v1 - v0);
}

//-----------------------------------------------------------------------------
// Name: GetBigNumDivide()
// Desc: 큰숫자를 한자리수 숫자로 잘게 나눈다.
//-----------------------------------------------------------------------------
int GetBigNumDivide(int iBigNum, int *ilp1Num, int *ilp10Num, int *ilp100Num, int *ilp1000Num)
{
	char cTemp[10];
	ZeroMemory(cTemp, 10);
	sprintf(cTemp, "%d", iBigNum);

	int iLen = strlen( cTemp);
	if( iLen >= 5 ) AfxMessageBox("if( iLen >= 10000 ) Error");

	char cTemp2[3];

	for(int i = iLen - 1; i >= 0 ; i-- )
	{
		ZeroMemory(cTemp2, 3);

		if( ilp1Num != NULL && i == iLen - 1)
		{
			cTemp2[0] = cTemp[i];
			(*ilp1Num) = atoi(cTemp2);
		}
		else
		if( ilp10Num != NULL && i == iLen - 2)
		{
			cTemp2[0] = cTemp[i];
			(*ilp10Num) = atoi(cTemp2);
		}
		else
		if( ilp100Num != NULL && i == iLen - 3)
		{
			cTemp2[0] = cTemp[i];
			(*ilp100Num) = atoi(cTemp2);
		}
		else
		if( ilp1000Num != NULL && i == iLen - 4)
		{
			cTemp2[0] = cTemp[i];
			(*ilp1000Num) = atoi(cTemp2);
		}
	}	

	return iLen;
}
//-----------------------------------------------------------------------------
// Name: FilePrintf()
// Desc: 
//-----------------------------------------------------------------------------
void FilePrintf(char *clpFileName, char *clpStr)
{
	
	FILE* Save;
	Save = fopen("Output.txt", "a+t");
		
	fwrite(clpStr, strlen(clpStr), 1, Save);
	
	fclose(Save);
	
}


void XChange( int *a, int *b )
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

//D3DCOLOR_ARGB 반대되는 함수.
void GetColorbyARGB(DWORD dwARGB, int *ilpA, int *ilpR, int *ilpG, int *ilpB)
{
	(*ilpA) = (dwARGB)       >> 24;	
	(*ilpR) = (dwARGB <<  8) >> 24;
	(*ilpG) = (dwARGB << 16) >> 24;
	(*ilpB) = (dwARGB << 24) >> 24;
}
//-----------------------------------------------------------------------------
// Name: FilePrintf()
// Desc: 
//-----------------------------------------------------------------------------
int GetAngle(int iStandardPixelPosX, int iStandardPixelPosY, int iCheckPixelPosX, int iCheckPixelPosY)
{

	int iAngle = 0;
	
	int iTempX = iCheckPixelPosX - iStandardPixelPosX;
	int iTempY = iStandardPixelPosY - iCheckPixelPosY;
	
	if( iTempX >= 0 &&
		iTempY >= 0 )
	{
		if( iTempY == 0 ) return 0;
		if( iTempX == 0 ) return 90;
		iAngle = (int )((atan((double)iTempY / (double)iTempX))*180/3.1415926535);
	}
	else
	if( iTempX <= 0 &&
		iTempY >= 0 )
	{		
		if( iTempY == 0 ) return 180;
		if( iTempX == 0 ) return 180;
		
		iAngle = (int )((atan((double)iTempY / (double)iTempX))*180/3.1415926535);
		iAngle = iAngle + 180;
	}
	else
	if( iTempX < 0 &&
		iTempY <= 0 )
	{
		if( iTempY == 0 ) return 180;
		if( iTempX == 0 ) return 180;

		iAngle = (int )((atan((double)iTempY / (double)iTempX))*180/3.1415926535);
		iAngle = iAngle + 180;
	}
	else
	if( iTempX >= 0 &&
		iTempY <= 0 )
	{
		if( iTempY == 0 ) return 0;
		if( iTempX == 0 ) return 270;
		iAngle = (int )((atan((double)iTempY / (double)iTempX))*180/3.1415926535);
		iAngle = iAngle + 360;		
	}
		
	if( iAngle < 0 || iAngle >= 360 )
	{
		iAngle = 0;
	}

	return iAngle;
		
}
//-----------------------------------------------------------------------------
// Name: ValidWindowPixelFormat()
// Desc: 
//-----------------------------------------------------------------------------
bool ValidWindowPixelFormat()
{
	CRect rect;
	int ColorDepth;

	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rect , 0);
	HDC d = ::GetDC(NULL);
	ColorDepth = GetDeviceCaps(d,BITSPIXEL);
	::ReleaseDC(NULL,d);

	if(rect.Width() <= 800 || rect.Height() <= 600 || (ColorDepth ==24) || ColorDepth <16)
	{
		return false;
	}
	return true;
} 



char	g_cMsgData[512] = {0, };
int		g_iMsgSizeCount = 0;
//-----------------------------------------------------------------------------
// Name: SetMsgData()
// Desc: 
//-----------------------------------------------------------------------------
void SetMsgData(char *clpMsgData, int iMsgSize)
{
	assert( g_iMsgSizeCount + iMsgSize < 512 );

	memcpy( &(g_cMsgData[g_iMsgSizeCount]), clpMsgData, iMsgSize);
	g_iMsgSizeCount = g_iMsgSizeCount + iMsgSize;
} 
//-----------------------------------------------------------------------------
// Name: GetMsgData()
// Desc: 
//-----------------------------------------------------------------------------
void GetMsgData(char *clpMsgData, char *clpData, int iMsgSize)
{
	assert( g_iMsgSizeCount + iMsgSize < 512 );

	memcpy( clpData, &(clpMsgData[g_iMsgSizeCount]), iMsgSize);
	g_iMsgSizeCount = g_iMsgSizeCount + iMsgSize;
}
//-----------------------------------------------------------------------------
// Name: GetMsgData()
// Desc: 
//-----------------------------------------------------------------------------
char* GetAllMsgData()
{
	return g_cMsgData;
}
//-----------------------------------------------------------------------------
// Name: GetAllMsgDataSize()
// Desc: 
//-----------------------------------------------------------------------------
int GetAllMsgDataSize()
{
	return g_iMsgSizeCount;
}
//-----------------------------------------------------------------------------
// Name: GetAllMsgDataSize()
// Desc: 
//-----------------------------------------------------------------------------
void MsgDataCleanup()
{
	ZeroMemory(g_cMsgData, 512);
	g_iMsgSizeCount = 0;	
}
//-----------------------------------------------------------------------------
// Name: ImpactCehck()
// Desc: 
//-----------------------------------------------------------------------------
bool ImpactCehck(int iLeft1, int iTop1, int iRight1, int iBottom1, int iLeft2, int iTop2, int iRight2, int iBottom2 )
{

	if( iLeft1   < iLeft2 &&
		iRight1  > iLeft2 &&
		iTop1    < iTop2 &&		
		iBottom1 > iTop2 )
	{
		return true;
	}

	if( iLeft1   < iLeft2 &&
		iRight1  > iLeft2 &&
		iTop1    < iBottom2 &&		
		iBottom1 > iBottom2 )
	{
		return true;
	}

	
	if( iLeft1   < iRight2 &&
		iRight1  > iRight2 &&
		iTop1    < iBottom2 &&		
		iBottom1 > iBottom2 )
	{
		return true;
	}

	if( iLeft1   < iRight2 &&
		iRight1  > iRight2 &&
		iTop1    < iTop2 &&		
		iBottom1 > iTop2 )
	{
		return true;
	}



	
	return false;
}

int GetLengthOfTwoPoint(int iPos_1X, int iPos_1Y, int iPos_2X, int iPos_2Y)
{
	int iTempX = abs(iPos_1X - iPos_2X);
	int iTempY = abs(iPos_1Y - iPos_2Y);


	return iTempX + iTempY;
}

bool CheckPosInRect(int iCheckPosX, int iCheckPosY, int iRectLeft,int iRectTop,  int iRectRight, int iRectBottom)
{

	if( iCheckPosX >= iRectLeft &&
		iCheckPosX <= iRectRight &&
		iCheckPosY >= iRectTop &&
		iCheckPosY <= iRectBottom )
	{
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Name: GetNowIntDate()
// Desc: 
//-----------------------------------------------------------------------------
int GetNowIntDate()
{
	SYSTEMTIME	stNowTime;
	ZeroMemory( &stNowTime, sizeof(SYSTEMTIME));
	GetLocalTime(&stNowTime);
	int iNowDay		= (stNowTime.wYear				* 10000) + (stNowTime.wMonth				* 100) + stNowTime.wDay;

	return iNowDay;
	
}

//-----------------------------------------------------------------------------
// Name: getFileSize)
// Desc: 파일 싸이즈를 알아 온다.
//-----------------------------------------------------------------------------
__int64 getFileSize(char *filename)
{
  struct _stati64 statbuf;

  if( _stati64(filename, &statbuf) ) return -1; // 파일 정보 얻기: 에러 있으면 -1 을 반환

  return statbuf.st_size;                        // 파일 크기 반환
}

