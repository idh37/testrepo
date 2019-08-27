// Global....


#include "StdAfx.h"
#include "Global.h"
#include "HUtil.h"
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDirectX			g_clDirectX;
CDxPage				g_clDxGamePage;
CPointRender		g_clPointRender;
/////////////////////////////////////////////////////////////////


//bool					g_bCustomColorEffectFlag = false; // 0 : 없음. : 1 Color = g_dwCustomColor설정.
int						g_iCustomColorEffectCount = 0;

float					g_fCustomColorA = 1;
float					g_fCustomColorR = 1;
float					g_fCustomColorG = 1;
float					g_fCustomColorB = 1;


float					g_fTempCustomColorA = 0;
float					g_fTempCustomColorR = 0;
float					g_fTempCustomColorG = 0;
float					g_fTempCustomColorB = 0;

//-----------------------------------------------------------------------------
// Name: GetCustomEffectColor()
// Desc: 
//-----------------------------------------------------------------------------
void ResetCustomEffectColor()
{
	assert( g_iCustomColorEffectCount == 0 );
	
	g_iCustomColorEffectCount = 0;

	g_fCustomColorA = 1;
	g_fCustomColorR = 1;
	g_fCustomColorG = 1;
	g_fCustomColorB = 1;
	
}



//-----------------------------------------------------------------------------
// Name: GetCustomEffectColor()
// Desc: 
//-----------------------------------------------------------------------------
void AddCustomEffectColor(DWORD dwColor)
{
	int iR1 = 0 ;
 	int iG1 = 0 ;
 	int iB1 = 0 ;
 	int iA1 = 0 ;
 
 	GetColorbyARGB(dwColor, &iA1, &iR1, &iG1, &iB1);

	float fR1 = (float )iR1 / 255.0f;
 	float fG1 = (float )iG1 / 255.0f;
 	float fB1 = (float )iB1 / 255.0f;
 	float fA1 = (float )iA1 / 255.0f;
	 

	if( fR1 == 0 )
	{
		g_fTempCustomColorR = g_fCustomColorR;
	}
	if( fG1 == 0 )
	{
		g_fTempCustomColorG = g_fCustomColorG;
	}
	if( fB1 == 0 )
	{
		g_fTempCustomColorB = g_fCustomColorB;
	}
	if( fA1 == 0 )
	{
		g_fTempCustomColorA = g_fCustomColorA;
	}
	
	g_fCustomColorR *= fR1;
	g_fCustomColorG *= fG1;
	g_fCustomColorB *= fB1;
	g_fCustomColorA *= fA1;
	 
 	g_iCustomColorEffectCount++;


	 

// 	int iR1 = 0 ;
// 	int iG1 = 0 ;
// 	int iB1 = 0 ;
// 	int iA1 = 0 ;
// 
// 	GetColorbyARGB(dwColor, &iA1, &iR1, &iG1, &iB1);
// 
// 	g_iCustomColorA += iA1;
// 	g_iCustomColorR += iR1;
// 	g_iCustomColorG += iG1;
// 	g_iCustomColorB += iB1;
// 
// 	g_iCustomColorEffectCount++;

}
//-----------------------------------------------------------------------------
// Name: GetCustomEffectColor()
// Desc: 
//-----------------------------------------------------------------------------
void DelCustomEffectColor(DWORD dwColor)
{
	int iR1 = 0 ;
 	int iG1 = 0 ;
 	int iB1 = 0 ;
 	int iA1 = 0 ;
 
 	GetColorbyARGB(dwColor, &iA1, &iR1, &iG1, &iB1);

	float fR1 = (float )iR1 / 255.0f;
 	float fG1 = (float )iG1 / 255.0f;
 	float fB1 = (float )iB1 / 255.0f;
 	float fA1 = (float )iA1 / 255.0f;
	 

	if( fR1 == 0 )
	{
		g_fCustomColorR = g_fTempCustomColorR;
		g_fTempCustomColorR = 0;

	}
	else
	{
		g_fCustomColorR /= fR1;

	}
	if( fG1 == 0 )
	{
		g_fCustomColorG = g_fTempCustomColorG;
		g_fTempCustomColorG = 0;
	}
	else
	{
		g_fCustomColorG /= fG1;

	}
	if( fB1 == 0 )
	{
		g_fCustomColorB = g_fTempCustomColorB;
		g_fTempCustomColorB = 0;
	}
	else
	{
		g_fCustomColorB /= fB1;

	}
	if( fA1 == 0 )
	{
		g_fCustomColorA = g_fTempCustomColorA;
		g_fTempCustomColorA = 0;
	}
	else
	{
		g_fCustomColorA /= fA1;
	}
		
	 
 	g_iCustomColorEffectCount--;
		

// 	int iR1 = 0 ;
// 	int iG1 = 0 ;
// 	int iB1 = 0 ;
// 	int iA1 = 0 ;
// 
// 	GetColorbyARGB(dwColor, &iA1, &iR1, &iG1, &iB1);
// 
// 	g_iCustomColorA -= iA1;
// 	g_iCustomColorR -= iR1;
// 	g_iCustomColorG -= iG1;
// 	g_iCustomColorB -= iB1;
// 
// 	g_iCustomColorEffectCount--;
}

//-----------------------------------------------------------------------------
// Name: GetCustomEffectColor()
// Desc: 
//-----------------------------------------------------------------------------
DWORD GetCustomEffectColor(DWORD dwColor)
{
	if( g_iCustomColorEffectCount <= 0 ) 
		return dwColor;

 	int iR1 = 0 ;
 	int iG1 = 0 ;
 	int iB1 = 0 ;
 	int iA1 = 0 ;
 
 	GetColorbyARGB(dwColor, &iA1, &iR1, &iG1, &iB1);

	float fR1 = (float )iR1 / 255.0f;
 	float fG1 = (float )iG1 / 255.0f;
 	float fB1 = (float )iB1 / 255.0f;
 	float fA1 = (float )iA1 / 255.0f;

	fR1 *= g_fCustomColorR;
	fG1 *= g_fCustomColorG;
	fB1 *= g_fCustomColorB;
	fA1 *= g_fCustomColorA;
	

 	return D3DXCOLOR( fR1, fG1, fB1, fA1 );
	


// 	if( g_iCustomColorEffectCount <= 0 ) return dwColor;
// 
//  	int iR1 = 0 ;
//  	int iG1 = 0 ;
//  	int iB1 = 0 ;
//  	int iA1 = 0 ;
//  
//  	GetColorbyARGB(dwColor, &iA1, &iR1, &iG1, &iB1);
//  
//  	int iR2 = g_iCustomColorR / g_iCustomColorEffectCount;
//  	int iG2 = g_iCustomColorG / g_iCustomColorEffectCount;
//  	int iB2 = g_iCustomColorB / g_iCustomColorEffectCount;
//  	int iA2 = g_iCustomColorA / g_iCustomColorEffectCount;
// 		
//  
//  	float fTempA2 = (float )iA2 / 255.0f;
//  	float fTempR2 = (float )iR2 / 255.0f;
//  	float fTempG2 = (float )iG2 / 255.0f;
//  	float fTempB2 = (float )iB2 / 255.0f;
//  
//  	fTempA2 = ((float )iA1 * fTempA2) / 255.0f;
//  	fTempR2 = ((float )iR1 * fTempR2) / 255.0f;
//  	fTempG2 = ((float )iG1 * fTempG2) / 255.0f;
//  	fTempB2 = ((float )iB1 * fTempB2) / 255.0f;
//  
//  	return D3DXCOLOR( fTempR2, fTempG2, fTempB2, fTempA2 );



// 	if( g_bCustomColorEffectFlag == false ) return dwColor;
// 
// 	int iR1 = 0 ;
// 	int iG1 = 0 ;
// 	int iB1 = 0 ;
// 	int iA1 = 0 ;
// 
// 	GetColorbyARGB(dwColor, &iA1, &iR1, &iG1, &iB1);
// 
// 	int iR2 = 0 ;
// 	int iG2 = 0 ;
// 	int iB2 = 0 ;
// 	int iA2 = 0;
// 
// 	GetColorbyARGB(g_dwCustomColor, &iA2, &iR2, &iG2, &iB2);
// 
// 	float fTempA2 = (float )iA2 / 255.0f;
// 	float fTempR2 = (float )iR2 / 255.0f;
// 	float fTempG2 = (float )iG2 / 255.0f;
// 	float fTempB2 = (float )iB2 / 255.0f;
// 
// 	fTempA2 = ((float )iA1 * fTempA2) / 255.0f;
// 	fTempR2 = ((float )iR1 * fTempR2) / 255.0f;
// 	fTempG2 = ((float )iG1 * fTempG2) / 255.0f;
// 	fTempB2 = ((float )iB1 * fTempB2) / 255.0f;
// 
// 	return D3DXCOLOR( fTempR2, fTempG2, fTempB2, fTempA2 );
}



//LPD3DXSPRITE				g_cllpTestSprite = NULL;