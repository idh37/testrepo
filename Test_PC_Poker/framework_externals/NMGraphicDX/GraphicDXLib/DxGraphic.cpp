// DxGraphic.cpp: implementation of the DxGraphic class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <math.h>
#include "DxGraphic.h"
#include "D3DXSprite.h"
#include "Jpegfile.h"
#include "HUtil.h"
#include "Global.h"
#include "PointRender.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define TO_RADIAN(x) ((x) * 0.0174773f)
#define FLOATROUNDINT(x) ( (int)((x)+0.5f) )
#define FLOATROUND2(x) ( floor(10.0*((x)+0.05))/10.0 )

#define _swap(a,b) {(a)^=(b); (b)^=(a); (a)^=(b);}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DWORD _WordtoDwordColor(WORD data)
{
	return 0xFF000000 | (((data>> 10) & 0x1f) <<19) | (((data>> 5)  & 0x1f) <<11) | (((data) & 0x1f) <<3); 
}


WORD _DWordtoWordColor(DWORD data)
{
	WORD r = (data >> 19) & 0x00ff ;
	WORD g = (data >> 11) & 0x00ff;
	WORD b = (data >> 3 ) & 0x00ff;

	return r << 10 | g << 5 | b ;
	//	return 0xFF000000 | (((data>> 10) & 0x1f) <<19) | (((data>> 5)  & 0x1f) <<11) | (((data) & 0x1f) <<3); 
}
/*
void CDxPage::SetTempClip(int x1, int y1, int cx, int cy)
{	
m_ViewPortQueue.push( pDirectx->GetViewPort() );
pDirectx->SetViewPort( x1 , y1 , x2 , y2 , 0.0f , 2.0f);
}

void CDxPage::ReleaseTempClip()
{
if( m_ViewPortQueue.empty() ) return;

D3DVIEWPORT9 vp = m_ViewPortQueue.front();
m_ViewPortQueue.pop();
}
// �ӽ� Ŭ���� ������ Ȱ��ȭ ���θ� ����
void CDxPage::EnableTempClip(BOOL bEnable)
{
//������� �ʴ´�.
//bEnableTempClip = bEnable;
}

// �̷���� ���� ��ǥ�� �����Ѵ�
void CDxPage::SetMirrorX(int x)
{
MirrorX = x;
}
*/
//
//                                   �׷��� ó�� ���
//
//#include "StdAfx.h"

//////////////////////////////////////////////////


CDxPage::CDxPage()
{
	lpBmpInfo = NULL;
	lpBit     = NULL;
	lPitch    = 0;
	lPitchHalf= 0;
	nBitNum   = 8;
	nBPD      = 1;
	nColorNum = 0;
	lpLogPal  = NULL;
	hPal      = NULL;
	hBmp      = NULL;

	nAllocSize = 0;

	////////////

	bEnableTempClip = FALSE;

	////////////

	TClipX1 = 0;
	TClipY1 = 0;
	TClipX2 = 0;
	TClipY2 = 0;

	MirrorX = 0;

	////////////

	bAttached = FALSE;

	////////////

	xpFlip = 0;
	ypFlip = 0;
	iZoomX = 1;

	////////////

	iDrawingTrackBegin = -1;
	iDrawingTrackEnd = -1;
}


CDxPage::~CDxPage()
{
	Destroy();
}

void CDxPage::Destroy()
{
	if(bAttached) Detach();

	nColorNum = 0;
	nAllocSize = 0;

	if(lpBmpInfo != NULL){ delete lpBmpInfo; lpBmpInfo=NULL;}
	//if(lpBit != NULL) { delete lpBit; lpBit=NULL; }
	if(lpLogPal) { free( lpLogPal );  lpLogPal=NULL; }
	if(hPal) { DeleteObject(hPal); hPal=NULL;}
	if(hBmp) { DeleteObject(hBmp); hBmp=NULL;}
}

// �ӽ�Ŭ���� ������ �����Ѵ�
void CDxPage::SetTempClip(int x1, int y1, int x2, int y2)
{
	if(x1 > x2) _swap(x1, x2);
	if(y1 > y2) _swap(y1, y2);

	x1 = max(x1, 0);
	x2 = max(x2, 0);
	y1 = max(y1, 0);
	y2 = max(y2, 0);

	TClipX1 = x1;
	TClipY1 = y1;
	TClipX2 = x2;
	TClipY2 = y2;
	//	pDirectx->SetViewPort( x1,y1,x2-x1,y2-y1 , 0,1);
}

// �ӽ� Ŭ���� ������ Ȱ��ȭ ���θ� ����
void CDxPage::EnableTempClip(BOOL bEnable)
{
	bEnableTempClip = bEnable;

}

// �̷���� ���� ��ǥ�� �����Ѵ�
void CDxPage::SetMirrorX(int x)
{
	MirrorX = x;
}

/*-----------------------------------------------------------------------------------------
�������� �ʱ�ȭ �Ѵ�
------------------------------------------------------------------------------------------*/
BOOL CDxPage::Init( CDirectX* pdirectx, LONG xl, LONG yl, INT bit, RGBQUAD *lpPal, LPCTSTR palfname)
{
	pDirectx = pdirectx;
	_LPITCH = lPitch;

	_MXL = xl;
	_MYL = yl;

	nBitNum = bit;
	width = xl;
	lPitch = xl*nBPD;
	height = yl;

	SetClipArea(0, 0, xl, yl);


	return TRUE;
}

BOOL CDxPage::Attach(LONG xl, LONG yl, void *lpbit, LONG lpitch)
{
	return TRUE;
}

BOOL CDxPage::Detach()
{

	return TRUE;
}

BOOL CDxPage::ReCreateDIBSection()
{
	return TRUE;
}

BOOL CDxPage::OpenPal(LPCTSTR filename)
{
	return TRUE;
}


/*-----------------------------------------------------------------------------------------
�������� ��Ʈ�� ���Ϸ� �����Ѵ�
------------------------------------------------------------------------------------------*/
BOOL CDxPage::SaveToBitmap(LPSTR fname)
{
	return TRUE;
}

/*-----------------------------------------------------------------------------------------
�������� JPG ���Ϸ� �����Ѵ�(quality = 0-100)
------------------------------------------------------------------------------------------*/
BOOL CDxPage::SaveToJpeg(LPSTR fname, int quality)
{
	return TRUE;
}

/*-----------------------------------------------------------------------------------------
�������� �ķ�Ʈ�� �����Ѵ�

- �μ� : RGBQUAD�� ������
------------------------------------------------------------------------------------------*/
BOOL CDxPage::SetPal(RGBQUAD *lpPal)
{
	return TRUE;
}

BOOL CDxPage::SetLogPal(LPLOGPALETTE lpPal)
{
	return TRUE;
}


BOOL CDxPage::SetPalHandle(HPALETTE h)
{
	return TRUE;
}

/*-----------------------------------------------------------------------------------------
GDI ǥ���� Ư�� Į��� ä���
------------------------------------------------------------------------------------------*/
BOOL CDxPage::FillGDI(HDC hTarDC, int nXStart, int nYStart, COLORREF crColor)
{
	return TRUE;
}

/*-----------------------------------------------------------------------------------------
�ٸ� �������� ������ ���� ������ ǥ�鿡 �Ѹ���
------------------------------------------------------------------------------------------*/
BOOL CDxPage::PutFromPage(LONG xp, LONG yp, CDxPage *pPage, int blendgrade)
{
	return TRUE;
}

/*-----------------------------------------------------------------------------------------
�ٸ� �������� ������ �÷�Ű�� �����Ͽ� ���� ������ ǥ�鿡 �Ѹ���
------------------------------------------------------------------------------------------*/
BOOL CDxPage::PutFromPageWithColorKey(LONG xp, LONG yp, CDxPage *pPage, WORD colorkey, int blendgrade)
{
	return TRUE;
}

/*-----------------------------------------------------------------------------------------
�������� �ѷ��� Ŭ���̾�Ʈ�� Ư�� ��ǥ�� �������� ��ǥ�� ��ȯ��Ų��
------------------------------------------------------------------------------------------*/
void CDxPage::ToPagePos(LONG xp, LONG yp, LONG* X, LONG* Y)
{
	*X = (xp-xpFlip)/(int)(iZoomX);
	*Y = (yp-ypFlip)/(int)(iZoomX);
}

/*-----------------------------------------------------------------------------------------
�������� �ø��� GDIǥ�� ��ġ(Ŭ���̾�Ʈ ����)�� �����Ѵ�
------------------------------------------------------------------------------------------*/
void CDxPage::SetFlipPos(LONG xp, LONG yp, int zoom)
{
	xpFlip = xp;
	ypFlip = yp;
	iZoomX = zoom;
}

/*-----------------------------------------------------------------------------------------
�������� ������ GDI ǥ�鿡 ������� �Ѹ���(SetPagePos()�� ��ġ�� �ٲܼ� �ִ�)
------------------------------------------------------------------------------------------*/
BOOL CDxPage::Flip(HWND hwnd)
{
	return TRUE;
}

BOOL CDxPage::Flip(HDC hdc)
{
	return TRUE;
}

/*-----------------------------------------------------------------------------------------
�������� ������ GDI ǥ�鿡 ������ Ȯ��� �Ѹ���(SetFlipPos()�� �̸�����)
------------------------------------------------------------------------------------------*/
BOOL CDxPage::FlipToGDI( HDC hdc, LONG xp, LONG yp, int zoom)
{
	SetFlipPos(xp, yp, zoom);
	return TRUE;
}

BOOL CDxPage::FlipToGDI( HWND hwnd, LONG xp, LONG yp, int zoom)
{
	SetFlipPos(xp, yp, zoom);
	return TRUE;
}

/*-----------------------------------------------------------------------------------------
�������� ������ GDI ǥ�鿡 Stretch�� �Ѹ���

- �ҽ��� Ÿ���� RECT�� NULL�̸� ��ü �������� �����ȴ�
------------------------------------------------------------------------------------------*/
BOOL CDxPage::StretchFlip(HWND hwnd, RECT* rcDest, RECT* rcSrc )
{
	return TRUE;
}


/////////////////////
//  ����� ����
/////////////////////

/*-----------------------------------------------------------------------------------------
�������� ������ �׸���(flag�� TRUE�̸� XOR�� �׸���)
------------------------------------------------------------------------------------------*/
void CDxPage::HLine(int x, int y, int len, WORD color, BOOL flag)
{
	DWORD color2 = _WordtoDwordColor(color) | 0xff000000;
	pDirectx->HLine( x , y , len , color2,flag);
}

/*-----------------------------------------------------------------------------------------
�������� �������� �׸���(flag�� TRUE�̸� XOR�� �׸���)
------------------------------------------------------------------------------------------*/
void CDxPage::VLine(int x, int y, int len, WORD color, BOOL flag)
{
	DWORD color2 = 	_WordtoDwordColor(color) | 0xff000000;
	pDirectx->VLine( x , y , len , color2,flag);
}

/*-----------------------------------------------------------------------------------------
�������� ���� �׸���(�극���� �˰���)
------------------------------------------------------------------------------------------*/
void CDxPage::DrawLine(int x1, int y1, int x2, int y2, WORD color)
{
	DWORD color2 = _WordtoDwordColor(color);
	pDirectx->DrawLine( x1 , y1 , x2 , y2 ,color2);
}

void CDxPage::DrawLineXOR(int x1, int y1, int x2, int y2)
{
	pDirectx->DrawLine( x1 , y1 , x2 , y2 , 0xffffffff);
}

void CDxPage::DrawLineGB(int x1, int y1, int x2, int y2, WORD color, int grade)
{
	if(grade !=248 ) pDirectx->SetAlphaBlend(5);
	DWORD color2 = 	_WordtoDwordColor(color);// & (((grade+1)*8-1) << 24 | 0x00ffffff);
	pDirectx->DrawLine( x1 , y1 , x2 , y2 , color2 );
	pDirectx->SetAlphaBlend(0);
}

/*-----------------------------------------------------------------------------------------
�������� ���� �� �ڽ��� �׸���(flag�� TRUE�̸� XOR�� �׸���)
------------------------------------------------------------------------------------------*/
void CDxPage::DrawEmptyBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag)
{
	DWORD color2 = _WordtoDwordColor(color) ;
	pDirectx->DrawEmptyBox( x1 , y1 , x2 , y2 ,  color2 ,flag);
}

/*-----------------------------------------------------------------------------------------
�������� ���� �� �ڽ��� �׸���
------------------------------------------------------------------------------------------*/
void CDxPage::DrawFillBox(int x1, int y1, int x2, int y2, WORD color , int opt)
{
	DWORD color2 = _WordtoDwordColor(color) ;
	pDirectx->DrawFillBox(x1,y1,x2,y2, color2,opt);
}
void CDxPage::DrawFillBox(int x1, int y1, int x2, int y2, DWORD color , int opt)
{
	pDirectx->DrawFillBox(x1,y1,x2,y2, color , opt );
}

/*-----------------------------------------------------------------------------------------
�������� ���� �� ������ �ڽ��� �׸���
------------------------------------------------------------------------------------------*/
void CDxPage::DrawFillBoxGB(int x1, int y1, int x2, int y2, WORD color, int grade)
{
	if(grade !=248 )pDirectx->SetAlphaBlend(5);
	DWORD color2 = _WordtoDwordColor(color);// &  (((grade+1)*8-1) << 24 | 0x00ffffff);
	pDirectx->DrawFillBox(x1,y1,x2,y2, color2,0);
	pDirectx->SetAlphaBlend(0);

}

/*-----------------------------------------------------------------------------------------
���� �ڽ��� �׸���
------------------------------------------------------------------------------------------*/
void CDxPage::DrawSelectBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag)
{
	DWORD color2 = _WordtoDwordColor(color) ;
	pDirectx->DrawSelectBox(x1,y1,x2,y2, color2);
}

/*--------------------------------------------------------------------------------------
ȸ�� X-Box ��� �Լ�
----------------------------------------------------------------------------------------*/
void CDxPage::DrawRotateXBox(int x1, int y1, int x2, int y2, float angle,  WORD color , int opt,int centerx , int centery )
{

	DWORD color2 = _WordtoDwordColor(color) ;
	pDirectx->DrawRotateXYZBox(x1,y1,x2,y2 ,angle, color2 , opt , centerx ,  centery);
}

void CDxPage::DrawRotateXBox(int x1, int y1, int x2, int y2, float angle,  DWORD color, int opt,int centerx , int centery )
{
	pDirectx->DrawRotateXYZBox(x1,y1,x2,y2,angle , color , opt, centerx , centery );
}

///////////////////////////
// 8��Ʈ �̹��� ��� �Լ� 
//////////////////////////


/*--------------------------------------------------------------------------------------
8��Ʈ �ڵ� ��������Ʈ ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
----------------------------------------------------------------------------------------*/
BOOL CDxPage::PutSprAuto(int x, int y, CSprite8 *pSpr8, int sprnum, int opt, int grade, BYTE color)
{
	if(pSpr8==NULL) return FALSE;
	if(pSpr8->spr==NULL) return FALSE;
	if(sprnum<0 || sprnum >= pSpr8->sfhead.TotalSprNum) return FALSE;
	DWORD color2 = _WordtoDwordColor(color) ;
	if(grade !=248 )pDirectx->SetAlphaBlend(5);
	DrawFillBox( x , y ,pSpr8->spr[sprnum].xl ,  pSpr8->spr[sprnum].yl, color2 , opt);
	pDirectx->SetAlphaBlend(0);
	return TRUE;
}

/*--------------------------------------------------------------------------------------
8��Ʈ �ڵ� ��������Ʈ ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
----------------------------------------------------------------------------------------*/
BOOL CDxPage::PutSprAuto(int x, int y, SPRITE8 *pSpr, int opt, int grade, BYTE color)
{
	if(pSpr==NULL) return FALSE;

	if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
		InitGrpLib(width, height, lPitchHalf, 555);

	if(opt & TEMPCLIP) {
		opt = (~(opt&TEMPCLIP))&opt;
		SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
	}
	else {
		SetClipArea(0, 0, width, height);
	}

	// ������ ����̸�(�����ǥ ����)
	if(opt & MIRROR)
	{
		opt = (~(opt&MIRROR))&opt;
		opt |= LR;
		if(opt & RXY)
		{
			x = MirrorX + (MirrorX - (x + pSpr->rx + pSpr->xl));
			opt = (~(opt&RXY))&opt;
		}
		else x = MirrorX + (MirrorX - (x + pSpr->xl));
		y+=pSpr->ry;
	}
	else if(opt & RXY) { x+=pSpr->rx; y+=pSpr->ry; opt = (~(opt&RXY))&opt; }

	DWORD color2 = _WordtoDwordColor(color);// &  (((grade+1)*8-1) << 24 | 0x00ffffff);
	DrawFillBox( x , y ,pSpr->xl , pSpr->yl, color2 , opt);
	return TRUE;
}

/*--------------------------------------------------------------------------------------
8��Ʈ Zoom ��������Ʈ ��� �Լ�(����/����� ��� ��밡��, �¿�/�����ø� ����)
----------------------------------------------------------------------------------------*/
BOOL CDxPage::ZoomSprAuto(int x, int y, CSprite8 *pSpr8, int sprnum, int tx, int ty, int opt, int grade, BYTE color)
{
	if(pSpr8==NULL) return FALSE;
	if(pSpr8->spr==NULL) return FALSE;
	if(sprnum<0 || sprnum >= pSpr8->sfhead.TotalSprNum) return FALSE;
	if(grade !=248 )	pDirectx->SetAlphaBlend(5);
	ZoomSprAuto(x, y, &pSpr8->spr[sprnum], tx, ty, opt, grade, color);
	pDirectx->SetAlphaBlend(0);
	return TRUE;
}

/*--------------------------------------------------------------------------------------
8��Ʈ Zoom ��������Ʈ ��� �Լ�(����/����� ��� ��밡��, �¿�/�����ø� ����)
----------------------------------------------------------------------------------------*/
BOOL CDxPage::ZoomSprAuto(int x, int y, SPRITE8 *pSpr, int tx, int ty, int opt, int grade, BYTE color)
{
	if(pSpr==NULL) return FALSE;

	if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
		InitGrpLib(width, height, lPitchHalf, 555);

	if(opt & TEMPCLIP) {
		opt = (~(opt&TEMPCLIP))&opt;
		SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
	}
	else {
		SetClipArea(0, 0, width, height);
	}

	if(opt & PERCENT) { tx = pSpr->xl*tx/100; ty = pSpr->yl*ty/100; }	// 100% �������� Ȯ��,���
	if(opt & RXY) { x+=pSpr->rx*tx/pSpr->xl; y+=pSpr->ry*ty/pSpr->yl; }	// �����ǥ�� ����
	if(opt & CENTER) { x -= (tx>>1); y -= (ty>>1); }					// ĳ���͸� ��ǥ�� �߽ɿ� ��ġ��Ų��

	DWORD color2 = _WordtoDwordColor(color);//  &  (((grade+1)*8-1) << 24 | 0x00ffffff);
	DrawFillBox( x , y ,pSpr->xl , pSpr->yl, color2 , opt);

	return TRUE;
}


/////////////////////////////
//  16��Ʈ ��������Ʈ ����
/////////////////////////////
void CDxPage::SetSprBlend( int PixelFormat , int opt , int grade )
{
	//	pDirectx->pEffect->SetTechnique("NORMAL");
	//	return;

	if(pDirectx->GetShaderEnable())
	{
		if( PixelFormat == 8888 || (opt & AB)  )
		{
			//pDirectx->SetAlphaBlend(6);
			pDirectx->pEffect->SetTechnique("AB");
			pDirectx->pEffect->SetFloat("Grade", (float) grade );
			return;
		}


		//else if(opt&LE)
		if(opt&LE)
		{
			//pDirectx->SetAlphaBlend(7);
			pDirectx->pEffect->SetTechnique("LE");
			pDirectx->pEffect->SetFloat("Grade", (float) grade );

		}
		else
		{
			if( grade != 248 )
			{
				pDirectx->SetAlphaBlend(5);
				pDirectx->pEffect->SetTechnique("GB");
				pDirectx->pEffect->SetFloat("Grade", (float) grade );
			}
			else
			{
				if(opt&AB)
				{
					pDirectx->pEffect->SetTechnique("AB");
					pDirectx->pEffect->SetFloat("Grade", (float) grade );
				}
				else
				{
					pDirectx->pEffect->SetTechnique("NORMAL");
				}
			}
		}

		if( opt & DE )
		{
			pDirectx->pEffect->SetTechnique("DE");
			pDirectx->pEffect->SetFloat("Grade", (float) grade );
		}
	}
	else
	{
		if( PixelFormat == 8888 || (opt & AB)  )
		{
			pDirectx->SetAlphaBlend(6);
			return;
		}
		if(opt&LE)
		{
			pDirectx->SetAlphaBlend(7);
		}
		else
		{
			if( opt & DE )
			{
				pDirectx->SetAlphaBlend(12);
			}
			else if( grade != 248 )
			{
				pDirectx->SetAlphaBlend(5);
			}
		}
	}


}
/*--------------------------------------------------------------------------------------
�ڵ� ��������Ʈ ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
----------------------------------------------------------------------------------------*/
BOOL CDxPage::PutSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int opt, int grade, WORD color)
{
	return PutSprAuto((float) x, (float) y, pXSpr, sprnum, opt, grade, color);
}

BOOL CDxPage::PutSprAuto(float x, float y, xSprite *pXSpr, int sprnum, int opt, int grade, WORD color)
{
	if(pXSpr==NULL) return FALSE;
	if(pXSpr->spr==NULL) return FALSE;
	if(sprnum<0 || sprnum >= pXSpr->sfhead.TotalSprNum) return FALSE;

	CD3DXSprite* temp = (CD3DXSprite*)pXSpr;
	SetSprBlend( pXSpr->spr[sprnum].PixelFormat , opt , grade );

	CPlane * pPlane = temp->GetPlane( sprnum );
	if(pPlane == NULL  )
	{
		int a =10;
	}
	// 	if(bEnableTempClip)
	// 		opt |= TEMPCLIP;
	// 	
	// 	if(opt & TEMPCLIP) 
	// 	{
	// 			opt = (~(opt&TEMPCLIP))&opt;
	// 			SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
	// 	}
	// 	else {
	// 		SetClipArea(0, 0, width, height);
	// 	}

	// If we run this we can first do 1 single pass for YUV2RGB conversion


	// The 2nd param, 0 specifies that ID3DXEffect::Begin and ID3DXEffect::End will save and restore all state modified by the effect.

	// Set the state for a particular pass in a technique.
	if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
		InitGrpLib(width, height, lPitchHalf, 555);

	if(opt & TEMPCLIP) {
		opt = (~(opt&TEMPCLIP))&opt;
		SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
	}
	else {
		SetClipArea(0, 0, width, height);
	}
	if( pPlane == NULL ) return TRUE;

	for( int i = 0 ; i < pPlane->m_pTextureNum ; i++ )
	{


		SPRITE30 *pSpr = &pXSpr->spr[sprnum];

		if( (pSpr->PixelFormat != 5558 && pSpr->PixelFormat != 8888) || pSpr->pAlpha==NULL) {
			// ���� ���̺��� ���� �����̶�� ȿ�� �÷��� ����
			opt = (~(opt&AB))&opt;
		}

		// ������ ����̸�(�����ǥ ����)
		if(opt & MIRROR)
		{
			opt = (~(opt&MIRROR))&opt;
			//opt &= ~MIRROR;
			opt |= LR;
			if(opt & RXY)
			{
				/*
				x = x - (x+ pSpr->xl) - (x + pSpr->rx);


				//x = x + ((x + pSpr->rx + pSpr->xl) - x);
				//x = x + ( pSpr->xl - pSpr->rx) ;
				opt = (~(opt&RXY))&opt;
				*/
				x = MirrorX + (MirrorX - (x + pSpr->rx + pSpr->xl));
				opt = (~(opt&RXY))&opt;

			}
			else
			{
				x = x + (x - (x + pSpr->xl));
			}
			y+=pSpr->ry;
		}
		else if(opt & RXY) { x+=pSpr->rx; y+=pSpr->ry; opt = (~(opt&RXY))&opt; }

		if(grade < 0) grade = 0;
		if(grade > 248) grade = 248;

		DWORD color2 = _WordtoDwordColor(color) &  ((grade + (grade-1)/31) << 24 | 0x00ffffff);
		if( color == 0)color2 |=0x00ffffff;


		if( g_clPointRender.m_bPointRenderFlag == true )
		{
			color2 = GetCustomEffectColor(color2);

			g_clPointRender.AddPointRender(
				(float )x,
				(float )y,
				1.0f,
				color2,
				pPlane->m_pTexture[i].m_Texture,
				pPlane->m_pRect[i].right - pPlane->m_pRect[i].left);

			continue ;
		}


		pDirectx->SetTexture( 0 ,  pPlane->m_pTexture[i].m_Texture );


		//color2 = 0x00ffffff;

		color2 = GetCustomEffectColor(color2);

		if( opt == LE ||
			opt == DE )
		{

			int iR = 0 ;
			int iG = 0 ;
			int iB = 0 ;
			int aa;

			GetColorbyARGB(color2, &aa, &iR, &iG, &iB);

			float ftemp = (float )grade / (float )248;

			iR = (float )iR * ftemp ;
			iG = (float )iG * ftemp ;
			iB = (float )iB * ftemp ;

			// 				BYTE byR = (((color2>> 10) & 0x1f) <<19) * ftemp;
			// 				BYTE byB = (((color2>> 5)  & 0x1f) <<11) * ftemp;
			// 				BYTE byG = (((color2) & 0x1f) <<3) * ftemp;

			color2 = D3DCOLOR_ARGB(
				255, 
				iR,
				iG,
				iB);
		}



		static D3DTLVERTEX g_vertices[4];

		if( bEnableTempClip == TRUE )
		{//TClipX1, TClipY1, TClipX2, TClipY2
			if( TClipX1 > pPlane->m_pRect[i].right+x)
				continue ;	 			
			int iTempLeft		= max(pPlane->m_pRect[i].left+x, TClipX1 );
			int iTempTop		= max(pPlane->m_pRect[i].top+y,  TClipY1 );
			int iTempRight		= min(pPlane->m_pRect[i].right+x,  TClipX2 );
			int iTempBottom		= min(pPlane->m_pRect[i].bottom+y, TClipY2 );

			if( iTempLeft >= pPlane->m_pRect[i].right +x ||
				iTempTop >= pPlane->m_pRect[i].bottom +y|| 
				iTempRight <= pPlane->m_pRect[i].left+x ||
				iTempBottom <= pPlane->m_pRect[i].top+y ) continue ;



			pDirectx->SetVertex( g_vertices,
				iTempLeft,
				iTempTop,
				iTempRight,
				iTempBottom
				);

			float fTempUV_X1 = (float )((int )(iTempLeft-x) %((int )pPlane->GetWidth(i))  ) / pPlane->GetWidth(i);
			float fTempUV_Y1 = (float )((int )(iTempTop-y) % ((int )pPlane->GetHeight(i))   ) / pPlane->GetHeight(i);
			float fTempUV_X2 = (float )((iTempRight - iTempLeft) ) / pPlane->GetWidth(i) +fTempUV_X1;
			float fTempUV_Y2 = (float )((iTempBottom -iTempTop) )  / pPlane->GetHeight(i) +fTempUV_Y1;

			pDirectx->SetUV( 
				g_vertices, 
				fTempUV_X1,
				fTempUV_Y1,
				fTempUV_X2,//pPlane->GetTextureU(i),
				fTempUV_Y2,//pPlane->GetTextureV(i),
				opt);					
		}
		else
		{
			pDirectx->SetVertex( g_vertices,
				x + pPlane->m_pRect[i].left,
				y + pPlane->m_pRect[i].top,
				x + pPlane->m_pRect[i].right,
				y + pPlane->m_pRect[i].bottom 
				);
			pDirectx->SetUV( g_vertices, 0,0,pPlane->GetTextureU(i) , pPlane->GetTextureV(i) , opt);	

		}


		pDirectx->SetColor(g_vertices, color2 );
		pDirectx->DrawVertex( g_vertices );

	}

	//pDirectx->SetAlphaBlend(0);
	return TRUE;
}


/*--------------------------------------------------------------------------------------
�ڵ� ��������Ʈ ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
----------------------------------------------------------------------------------------*/

BOOL CDxPage::PutSprAuto(int x, int y, SPRITE30 *pSpr, int opt, int grade, WORD color)
{
	return TRUE;
}

/*--------------------------------------------------------------------------------------
Zoom ��������Ʈ ��� �Լ�(����/����� ��� ��밡��, �¿�/�����ø� ����)
----------------------------------------------------------------------------------------*/
BOOL CDxPage::ZoomSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int tx, int ty, int opt, int grade, WORD color)
{
	return ZoomSprAuto((float)x, (float)y, pXSpr, sprnum, tx, ty, opt, grade, color);
}
BOOL CDxPage::ZoomSprAuto(float x, float y, xSprite *pXSpr, int sprnum, int tx, int ty, int opt, int grade, WORD color)
{
	if(pXSpr==NULL) return FALSE;
	if(pXSpr->spr==NULL) return FALSE;
	if(sprnum<0 || sprnum >= pXSpr->sfhead.TotalSprNum) return FALSE;

	CD3DXSprite* temp = (CD3DXSprite*)pXSpr;
	SetSprBlend( pXSpr->spr[sprnum].PixelFormat , opt , grade );

	CPlane * pPlane = temp->GetPlane( sprnum );
	if(bEnableTempClip)
		opt |= TEMPCLIP;

	if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
		InitGrpLib(width, height, lPitchHalf, 555);

	if(opt & TEMPCLIP) {
		opt = (~(opt&TEMPCLIP))&opt;
		SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
	}
	else {
		SetClipArea(0, 0, width, height);
	}
	if( pPlane == NULL ) return TRUE;
	for( int i = 0 ; i < pPlane->m_pTextureNum ; i++ )
	{
		if( temp )
		{	
			if(grade < 0) grade = 0;
			if(grade > 248) grade = 248;


			DWORD color2 = _WordtoDwordColor(color)  &  ((grade + (grade-1)/31) << 24 | 0x00ffffff);
			if( color == 0 )color2 |=0x00ffffff;

			color2 = GetCustomEffectColor(color2);



			if( opt == LE ||
				opt == DE )
			{

				int iR = 0 ;
				int iG = 0 ;
				int iB = 0 ;
				int aa;

				GetColorbyARGB(color2, &aa, &iR, &iG, &iB);

				float ftemp = (float )grade / (float )248;

				iR = (float )iR * ftemp ;
				iG = (float )iG * ftemp ;
				iB = (float )iB * ftemp ;

				// 				BYTE byR = (((color2>> 10) & 0x1f) <<19) * ftemp;
				// 				BYTE byB = (((color2>> 5)  & 0x1f) <<11) * ftemp;
				// 				BYTE byG = (((color2) & 0x1f) <<3) * ftemp;

				color2 = D3DCOLOR_ARGB(
					255, 
					iR,
					iG,
					iB);
			}


			SPRITE30 *pSpr = &pXSpr->spr[sprnum];

			if( (pSpr->PixelFormat != 5558 && pSpr->PixelFormat != 8888)  || pSpr->pAlpha==NULL) {
				// ���� ���̺��� ���� �����̶�� ȿ�� �÷��� ����
				opt = (~(opt&AB))&opt;
			}

			if(opt & PERCENT) { tx = pSpr->xl*tx/100; ty = pSpr->yl*ty/100; }	// 100% �������� Ȯ��,���
			if(opt & RXY) { x+=pSpr->rx*tx/pSpr->xl; y+=pSpr->ry*ty/pSpr->yl; }	// �����ǥ�� ����
			if(opt & CENTER) { x -= (tx>>1); y -= (ty>>1); }					// ĳ���͸� ��ǥ�� �߽ɿ� ��ġ��Ų��


			// 1x Zoom
			if(tx == pSpr->xl && ty == pSpr->yl) { 
				PutSprAuto(x, y, pXSpr , sprnum , opt, grade, color); 
				return TRUE; 
			}

			if( g_clPointRender.m_bPointRenderFlag == true )
			{
				color2 = GetCustomEffectColor(color2);

				g_clPointRender.AddPointRender(
					(float )x,
					(float )y,
					1.0f,
					color2,
					pPlane->m_pTexture[i].m_Texture,
					(float )tx);

				continue ;
			}



			if( pPlane->m_pTexture[i].m_Texture )
			{
				pDirectx->SetTexture( 0 ,  pPlane->m_pTexture[i].m_Texture );
			}

			D3DTLVERTEX g_vertices[4];
			D3DXVECTOR3 Center;
			Center.x = x + pPlane->m_pRect[0].left * tx/ pSpr->xl;
			Center.y = y + pPlane->m_pRect[0].top  * ty/ pSpr->yl;
			Center.z = 0;

			pDirectx->SetRotationVertex(
				g_vertices ,
				x + pPlane->m_pRect[i].left * tx/pSpr->xl ,
				y + pPlane->m_pRect[i].top  * ty/pSpr->yl ,
				x + pPlane->m_pRect[i].right* tx/pSpr->xl,
				y + pPlane->m_pRect[i].bottom* ty/pSpr->yl,
				0,Center );
			pDirectx->SetColor( g_vertices , color2  );
			pDirectx->SetUV( g_vertices ,0.0f , 0.0f , pPlane->GetTextureU(i) , pPlane->GetTextureV(i) , opt);
			pDirectx->DrawVertex( g_vertices );

		}	
	}

	return TRUE;
}

/*--------------------------------------------------------------------------------------
Zoom ��������Ʈ ��� �Լ�(����/����� ��� ��밡��, �¿�/�����ø� ����)
----------------------------------------------------------------------------------------*/

BOOL CDxPage::ZoomSprAuto(int x, int y, SPRITE30 *pSpr, int tx, int ty, int opt, int grade, WORD color)
{
	return TRUE;
}

/*--------------------------------------------------------------------------------------
Rotate & Zoom ��������Ʈ ��� �Լ�(����/����� ��� ��밡��, �¿�/�����ø� ����)
----------------------------------------------------------------------------------------*/
//OL CDxPage::RotateZoomSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int tx, int ty, float anglex, float angley, float anglez, int opt, int grade, WORD color)
BOOL CDxPage::RotateZoomSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int tx, int ty, float angle, int opt, int grade, WORD color)
{
	return RotateZoomSprAuto((float) x, (float) y, pXSpr, sprnum, tx, ty, angle, opt, grade, color);
}
BOOL CDxPage::RotateZoomSprAuto(float x, float y, xSprite *pXSpr, int sprnum, int tx, int ty, float angle, int opt, int grade, WORD color)
{
	if(pXSpr==NULL) return FALSE;
	if(pXSpr->spr==NULL) return FALSE;
	if(sprnum<0 || sprnum >= pXSpr->sfhead.TotalSprNum) return FALSE;

	CD3DXSprite* temp = (CD3DXSprite*)pXSpr;
	SetSprBlend( pXSpr->spr[sprnum].PixelFormat , opt , grade );
	CPlane * pPlane = temp->GetPlane( sprnum );


	if(bEnableTempClip)
		opt |= TEMPCLIP;

	if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
		InitGrpLib(width, height, lPitchHalf, 555);

	if(opt & TEMPCLIP) {
		opt = (~(opt&TEMPCLIP))&opt;
		SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
	}
	else {
		SetClipArea(0, 0, width, height);
	}

	for( int i = 0 ; i < pPlane->m_pTextureNum ; i++ )
	{
		if( temp )
		{
			pDirectx->SetTexture( 0 ,  pPlane->m_pTexture[i].m_Texture );	
			SPRITE30 *pSpr = &pXSpr->spr[sprnum];


			if( (pSpr->PixelFormat != 5558 && pSpr->PixelFormat != 8888)  || pSpr->pAlpha==NULL) {
				// ���� ���̺��� ���� �����̶�� ȿ�� �÷��� ����
				opt = (~(opt&AB))&opt;
			}



			if(opt & RXY) 
			{ x += pSpr->rx*tx/pSpr->xl; y += pSpr->ry*ty/pSpr->yl; }	// �����ǥ�� ����
			if(opt & CENTER) { 
				// ĳ���͸� ��ǥ�� �߽ɿ� ��ġ��Ų��
				x -= (tx>>1); 
				y -= (ty>>1);

				// ȸ���� ��ŭ ��ǥ�� �����Ѵ�
				//	if(anglez != 0 || anglex != 0 || angley != 0) {
				if(angle != 0) {
					D3DXVECTOR3 dt, odt;
					dt.x = -(tx>>1);
					dt.y = -(ty>>1);
					dt.z = 0;
					odt = dt;

					//_3DRotatePoint(&dt , anglex,angley, anglez);
					_3DRotatePoint(&dt , angle);
					x += dt.x - odt.x;
					y += dt.y - odt.y;
				}
			}

			if(grade < 0) grade = 0;
			if(grade > 248) grade = 248;

			DWORD color2 = _WordtoDwordColor(color) &  ((grade + (grade-1)/31) << 24 | 0x00ffffff);
			if( color == 0 )color2 |= 0x00ffffff;

			color2 = GetCustomEffectColor(color2);

			D3DTLVERTEX g_vertices[4];
			D3DXVECTOR3 Center;
			Center.x = x +  pPlane->m_pRect[0].left * tx/ pSpr->xl;
			Center.y = y +  pPlane->m_pRect[0].top  * ty/ pSpr->yl;
			Center.z = 0;
			pDirectx->SetRotationVertex(
				g_vertices , 
				x +  pPlane->m_pRect[i].left * tx/ pSpr->xl,
				y +  pPlane->m_pRect[i].top  * ty/ pSpr->yl,
				x +  pPlane->m_pRect[i].right* tx/ pSpr->xl,
				y +  pPlane->m_pRect[i].bottom* ty/ pSpr->yl,
				//	anglex,angley,anglez , Center );
				angle , Center );
			pDirectx->SetColor( g_vertices , color2  );
			pDirectx->SetUV( g_vertices ,0.0f , 0.0f ,  pPlane->GetTextureU(i) ,  pPlane->GetTextureV(i) , opt);
			pDirectx->DrawVertex( g_vertices );

		}	

	}
	return TRUE;
}

/*--------------------------------------------------------------------------------------
�ڵ� �̹��� ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
----------------------------------------------------------------------------------------*/
BOOL CDxPage::PutImageAuto(int x, int y, xSprite *pXSpr, int sprnum, int opt, int grade, WORD color)
{
	// 	if(pXSpr==NULL) return FALSE;
	// 	if(pXSpr->spr==NULL) return FALSE;
	// 	if(sprnum<0 || sprnum >= pXSpr->sfhead.TotalSprNum) return FALSE;
	// 	CD3DXSprite* temp = (CD3DXSprite*)pXSpr;
	// 	SetSprBlend( pXSpr->spr[sprnum].PixelFormat , opt , grade );
	// 	CPlane*  pPlane = temp->GetPlane( sprnum );
	// 	for( int i = 0 ; i <  pPlane->m_pTextureNum ; i++ )
	// 	{
	// 		if( temp )
	// 		{
	// 			g_clDirectX.SetTexture( 0 ,  pPlane->m_pTexture[i].m_Texture );
	// 			DWORD color2 = _WordtoDwordColor(color) &  ((grade*8 + grade/4) << 24 | 0x00ffffff);
	// 			if( color == 0)color2 |=0x00ffffff;
	// 			D3DTLVERTEX g_vertices[4];
	// 			pDirectx->SetVertex( g_vertices,
	// 				x +  pPlane->m_pRect[i].left,
	// 				y +  pPlane->m_pRect[i].top,
	// 				x +  pPlane->m_pRect[i].right,
	// 				y +  pPlane->m_pRect[i].bottom 
	// 				);
	// 
	// 			color2 = GetCustomEffectColor(color2);
	// 			pDirectx->SetColor(g_vertices, color2 );
	// 			pDirectx->SetUV( g_vertices, 0,0, pPlane->GetTextureU(i) ,  pPlane->GetTextureV(i) , opt);	
	// 
	// 			pDirectx->DrawVertex( g_vertices );
	// 		
	// 		}
	// 	}

	return TRUE;
}

/*--------------------------------------------------------------------------------------
�ڵ� �̹��� ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
----------------------------------------------------------------------------------------*/
BOOL CDxPage::PutImageAuto(int x, int y, SPRITE30 *pSpr, int opt, int grade, WORD color)
{
	if(pSpr==NULL) return FALSE;

	// 	if(bEnableTempClip)
	// 		opt |= TEMPCLIP;
	// 
	// 	if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
	// 		InitGrpLib(width, height, lPitchHalf, 555);
	// 
	// 	if(opt & TEMPCLIP) {
	// 		opt = (~(opt&TEMPCLIP))&opt;
	// 		SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
	// 	}
	// 	else {
	// 		SetClipArea(0, 0, width, height);
	// 	}
	// 
	// 	if(opt & RXY) { x+=pSpr->rx; y+=pSpr->ry; opt = (~(opt&RXY))&opt; }
	// 
	// 	if(grade < 0) grade = 0;
	// 	if(grade > 248) grade = 248;
	// 
	// 
	// 	DWORD color2 = _WordtoDwordColor(color);//  &  (((grade+1)*8) << 24 | 0x00ffffff);
	// 	DrawFillBox( x , y ,pSpr->xl , pSpr->yl, color2, opt);

	return TRUE;
}


///////// �ִϸ��̼� ����


//--------------------------------------------------------------------------------------
//      ��� �ִϸ��̼� ��� �Լ�
//--------------------------------------------------------------------------------------
BOOL CDxPage::DrawScene(int x, int y, xSprite *pXSpr, int scn, DWORD time, BOOL loop, int n_opt, int n_grade, WORD n_color)
{
	if(!pXSpr) 
		return FALSE;

	SceneData* pScene = pXSpr->scene.GetScenePt(scn);

	if(!pScene)
		return FALSE;

	// �־��� �ð��� �÷��� �Ǿ�� �� �������� ã�´�
	int frame = pScene->SearchFrame(time, loop);

	if(frame < 0)
		return FALSE;

	// ������ ������ ��Ÿ���� ����
	float delta = pScene->GetFrameDelta(frame, time, loop);

	return DrawFrameOfScene(x, y, pXSpr, scn, frame, delta, n_opt, n_grade, n_color);
}

//--------------------------------------------------------------------------------------
//      ��� �ִϸ��̼��� Ư�� ������ ��� �Լ�
//--------------------------------------------------------------------------------------
BOOL CDxPage::DrawFrameOfScene(int x, int y, xSprite *pXSpr, int scn, int frame, float fdelta, int n_opt, int n_grade, WORD n_color)
{
	SceneData* pScene = pXSpr->scene.GetScenePt(scn);

	if(!pScene) 
		return FALSE;

	if(pScene->TotFrameNum == 0)
		return FALSE;

	FrameData* pFrame = pScene->GetPtrFrame(frame);

	if(!pFrame)
		return FALSE;

	if(pFrame->TotCellNum == 0)
		return FALSE;

	for(int i=0; i<pFrame->TotCellNum; i++)
	{
		DRAWINGCELLDESC desc = {0,};

		// �� ��� ������ ����
		if(!GetDrawingCellInfo(x, y, pXSpr, scn, frame, i, fdelta, &desc))
			continue;

		desc.opt |= n_opt;

		// �������̵� �ɼ��� �����Ǿ� �ִٸ� �� ������ ��ü
		if(n_opt & GB) desc.grade = n_grade;
		if(n_opt & GLOW) { desc.grade = n_grade; desc.color = n_color; }
		if(n_opt & MONO) { desc.color = n_color; }

		if(pXSpr->spr[desc.sprno].xl == desc.width && pXSpr->spr[desc.sprno].yl == desc.height && desc.angle == 0) {
			PutSprAuto(desc.xp, desc.yp, pXSpr, desc.sprno, desc.opt, desc.grade, desc.color);
		}
		else if(desc.angle == 0) { //anglez == 0 &&desc.anglex == 0 &&desc.angley == 0 ){
			if(desc.width != 0 && desc.height != 0)
				ZoomSprAuto(desc.xp, desc.yp, pXSpr, desc.sprno, desc.width, desc.height, desc.opt, desc.grade, desc.color);
		} 
		else {
			if(desc.width != 0 && desc.height != 0)
			{
				//RotateZoomSprAuto(desc.xp, desc.yp, pXSpr, desc.sprno, desc.width, desc.height, (float)desc.anglex, (float)desc.angley, (float)desc.anglez, desc.opt, desc.grade, desc.color);
				RotateZoomSprAuto(desc.xp, desc.yp, pXSpr, desc.sprno, desc.width, desc.height, (float)desc.angle, desc.opt, desc.grade, desc.color);
			}
		}
	}

	return TRUE;
}

//--------------------------------------------------------------------------------------
//      ����� Ư�� ������ ���� �� ��� ������ ����
//--------------------------------------------------------------------------------------
BOOL CDxPage::GetDrawingCellInfo(float x, float y, xSprite *pXSpr, int scn, int frame, int cell, float fdelta, DRAWINGCELLDESC* pDesc)
{
	if(!pXSpr) return FALSE;

	SceneData* pScene = pXSpr->scene.GetScenePt(scn);
	if(!pScene) return FALSE;

	FrameData* pFrame = pScene->GetPtrFrame(frame);
	if(!pFrame) return FALSE;

	CellData* pCell = pFrame->GetPtrCell(cell);
	if(!pCell) return FALSE;

	if(pCell->SprNo < 0 || pCell->SprNo >= pXSpr->sfhead.TotalSprNum)
		return FALSE;

	float xp = (float)(x + pCell->Rx);
	float yp = (float)(y + pCell->Ry);
	int sprno = pCell->SprNo;
	float w = (float)pXSpr->spr[sprno].xl;
	float h = (float)pXSpr->spr[sprno].yl;
	int opt = 0;
	float grade = (float)pCell->Grade;
	WORD color = pCell->Color;

	switch(pCell->Effect) 
	{
	case 1 : opt |= HB; break;
	case 2 : opt |= GB; break;
	case 3 : opt |= LE; break;
	case 4 : opt |= DE; break;
	case 5 : opt |= GLOW; break;
	case 6 : opt |= MONO; break;
	case 7 : opt |= GRAYSCL; break;
	case 8 : opt |= AB; break;
	}

	if(pCell->Mirror == 1) {
		// �¿� ������
		if(opt & LR) opt &= ~LR;
		else opt |= LR;
	}
	if(pCell->Mirror == 2) {
		// ���� ������
		if(opt & UD) opt &= ~UD;
		else opt |= UD;
	}

	float zoomx = (float)pCell->ZoomX;
	float zoomy = (float)pCell->ZoomY;
	//	float anglez = (float)pCell->RotAnglez;
	//	float anglex = (float)pCell->RotAnglex;
	//	float angley = (float)pCell->RotAngley;
	float angle = (float)pCell->RotAngle;

	if(pCell->Interpol == 1) {
		// ���������̼�
		int nf = frame + 1;
		if(nf >= pScene->TotFrameNum) nf = 0;

		// ���� �������� ������ �����Ϳ� �� �����͸� ����
		FrameData* pFrame2 = pScene->GetPtrFrame(nf);
		CellData* pCell2 = pFrame2->GetPtrCell(cell);

		if(pCell2) {

			float delta = 0;
			float bias = (float)(pCell->Bias / 100.0);

			if(bias < 0.0001f && bias > -0.0001f) bias = 0;
			if(bias > 1.0f) bias = 1.0f;
			if(bias < -1.0f) bias = -1.0f;

			if(bias == 0) {
				delta = fdelta;
			}
			else if(bias < 0) {
				float ang = fdelta * TO_RADIAN(90);
				float m = (float)(1.0 - cos(ang));
				delta = fdelta + ((fdelta - m) * bias);
			}
			else {
				float ang = fdelta * TO_RADIAN(90);
				float m = (float)sin(ang);
				delta = fdelta + ((m - fdelta) * bias);
			}

			// ��ǥ ����
			xp += ((pCell2->Rx - pCell->Rx) * delta);
			yp += ((pCell2->Ry - pCell->Ry) * delta);

			// ���İ� ����
			grade += FLOATROUNDINT((pCell2->Grade - pCell->Grade) * delta);

			// �÷� ����
			if(pCell2->Effect == 5 || pCell2->Effect == 6) {
				int r1 = (int)R555(pCell->Color);
				int g1 = (int)G555(pCell->Color);
				int b1 = (int)B555(pCell->Color);
				int r2 = (int)R555(pCell2->Color);
				int g2 = (int)G555(pCell2->Color);
				int b2 = (int)B555(pCell2->Color);

				int r3 = r1 + FLOATROUNDINT((r2 - r1) * delta);
				int g3 = g1 + FLOATROUNDINT((g2 - g1) * delta);
				int b3 = b1 + FLOATROUNDINT((b2 - b1) * delta);

				color = RGBmix(r3, g3, b3);
			}

			// �� ���� ����
			zoomx += ((pCell2->ZoomX - pCell->ZoomX) * delta);
			zoomy += ((pCell2->ZoomY - pCell->ZoomY) * delta);

			// ȸ�� ���� ����
			//anglez += ((pCell2->RotAnglez - pCell->RotAnglez) * delta);
			//anglex += ((pCell2->RotAnglex - pCell->RotAnglex) * delta);
			//angley += ((pCell2->RotAngley - pCell->RotAngley) * delta);
			angle += ((pCell2->RotAngle - pCell->RotAngle) * delta);


		}
	}

	// Ȯ�� ��� ���� ���� ��� ��ǥ
	float oxp = xp;
	float oyp = yp;

	if(zoomx != 100 || zoomy != 100) {	
		if(zoomx > 0) w = (pXSpr->spr[sprno].xl * zoomx/100.0f);
		if(zoomy > 0) h = (pXSpr->spr[sprno].yl * zoomy/100.0f);

		// �߽���ǥ�� �������� Ȯ��/��ҵǵ��� ����
		float mx = (float)pXSpr->spr[sprno].mx;
		float my = (float)pXSpr->spr[sprno].my;
		xp += mx - (mx * zoomx/100.0f);
		yp += my - (my * zoomy/100.0f);
	}

	//if( anglez != 0 || anglex != 0 || angley !=0 ) {
	if( angle != 0 ) {
		// �߽� ��ǥ�� �������� ȸ���ǵ��� ����
		int mx = pXSpr->spr[sprno].mx;
		int my = pXSpr->spr[sprno].my;

		D3DXVECTOR3 rp, axis;
		rp.x = xp;
		rp.y = yp;
		rp.z = 0;

		axis.x = (oxp + mx);
		axis.y = (oyp + my);
		axis.z = 0;
		//_3DAxisRotatePoint(&rp, anglex,angley,anglez, axis);
		_3DAxisRotatePoint(&rp, angle, axis);
		xp = rp.x;
		yp = rp.y;
	}
	// �� �׸��� ����Ʈ�� ����Ǿ� �ִٸ�
	if(bScnEffect) {
		if((BYTE)ScnEffect.alpha >= 0 && (BYTE)ScnEffect.alpha < 248) {
			if(!(opt & GB || opt & LE || opt & DE || opt & AB)) {
				// ������ ������� �ʴ� ������ ��� �ٴܰ� �������� ���� ����
				grade = 248;
				opt |= GB;
			}

			grade = (grade * (ScnEffect.alpha / 248.0f));
		}

		if(ScnEffect.zoomx >= 0 && ScnEffect.zoomx != 100) {
			w = (w * (ScnEffect.zoomx / 100.0f));
			// �� ��� ��ǥ�� �������� Ȯ��/��ҵǵ��� ����
			float mx = (float)(ScnEffect.mx - xp);
			xp += mx - (mx * ScnEffect.zoomx/100.0f);
		}

		if(ScnEffect.zoomy >= 0 && ScnEffect.zoomy != 100) {
			h = (h * (ScnEffect.zoomy / 100.0f));
			// �� ��� ��ǥ�� �������� Ȯ��/��ҵǵ��� ����
			float my = (float)(ScnEffect.my - yp);
			yp += my - (my * ScnEffect.zoomy/100.0f);
		}

		if(ScnEffect.angle != 0) {
			// �� ��� ��ǥ�� �������� ȸ���ǵ��� ����

			int mx = ScnEffect.mx;
			int my = ScnEffect.my;

			D3DXVECTOR3 rp, axis;
			rp.x = xp;
			rp.y = yp;
			rp.z = 0;

			axis.x = (oxp + mx);
			axis.y = (oyp + my);
			axis.z = 0;
			//_3DAxisRotatePoint(&rp, anglex,angley,anglez, axis);
			_3DAxisRotatePoint(&rp, angle, axis);
			xp = rp.x;
			yp = rp.y;


			//	_3DAxisRotatePoint(&xp, &yp, (float)ScnEffect.angle, (float)ScnEffect.mx, (float)ScnEffect.my);
			angle += (float)ScnEffect.angle;
		}
	}

	pDesc->xp = xp;
	pDesc->yp = yp;
	pDesc->sprno = sprno;
	pDesc->width = w;
	pDesc->height = h;
	//	pDesc->anglez = anglez;
	//	pDesc->anglex = anglex;
	//	pDesc->angley = angley;
	pDesc->angle = angle;
	pDesc->opt = opt;
	pDesc->grade = grade;
	pDesc->color = color;
	return TRUE;
}

//--------------------------------------------------------------------------------------
//      ��Ƽ������ �׷��� Ʈ���� ���� ����(��� ��Ƽ���� ���� ����)
//--------------------------------------------------------------------------------------
void CDxPage::SetDrawingTrackScope(int begin/*=-1*/, int end/*=-1*/)
{
	iDrawingTrackBegin = begin;
	iDrawingTrackEnd = end;
}

//--------------------------------------------------------------------------------------
//      ��Ƽ�� �ִϸ��̼� ��� �Լ�
//--------------------------------------------------------------------------------------
BOOL CDxPage::DrawMultiScene(int x, int y, xSprite *pXSpr, int multiscn, DWORD time, BOOL loop, int n_opt, int n_grade, WORD n_color)
{
	if(!pXSpr) 
		return FALSE;

	MSceneData* pMScene = pXSpr->multiscene.GetMScenePt(multiscn);

	if(!pMScene)
		return FALSE;

	if(loop && pMScene->TotPlayTime > 0) {
		if(time > 0 ) {
			time = time % pMScene->TotPlayTime;
			if(time == 0)
				time = pMScene->TotPlayTime;
		}
		else {
			time = time % pMScene->TotPlayTime;
		}
	}

	pMScene->UpdateAffectClipboxTrack();

	for(int track=0; track<pMScene->TotTrackNum; track++) 
	{
		// ���Ƿ� ������ Ʈ�� ��ȿ ������ �ʰ��ϴ��� �˻�
		if(iDrawingTrackBegin >= 0 && track < iDrawingTrackBegin)
			continue;
		if(iDrawingTrackEnd >= 0 && track > iDrawingTrackEnd)
			continue;

		TrackData* pTrack = pMScene->GetPtrTrack(track);
		if(pTrack) {
			// ���� Ʈ���̸� ��ŵ
			if(pTrack->bHideTrack)
				continue;

			switch(pTrack->TrackType)
			{
			case TTYPE_SCENE:
				{
					DWORD stime, etime;
					if(pXSpr->CalcTrackPlayTime(multiscn, track, &stime, &etime)) {
						if(time < stime || time > etime) 
							continue;

						BOOL bSetTempClip = FALSE;
						// �ӽ� Ŭ���� ���� ���� ���� ����
						RECT orgtmpclip;
						BOOL orgbclip;

						if(pTrack->AffectClipboxTrack >= 0) {
							TrackData* pClipTrack = pMScene->GetPtrTrack(pTrack->AffectClipboxTrack);
							if(pClipTrack && pClipTrack->TrackType == TTYPE_CLIPBOX) {
								TRACK_CLIPBOX* pCbxData = (TRACK_CLIPBOX*)pClipTrack->pData;
								DWORD t1 = pClipTrack->Delay;
								DWORD t2 = pClipTrack->Delay + pCbxData->LifeTime;
								if(time >= t1 && time <= t2) {
									CPI_CLIPBOX cpi;
									if(pClipTrack->GetInterpol_Clipbox(time, &cpi)) {
										bSetTempClip = TRUE;
										// �ӽ� Ŭ���� ���� ���� ���� ����
										orgtmpclip.left = TClipX1;
										orgtmpclip.top = TClipY1;
										orgtmpclip.right = TClipX2;
										orgtmpclip.bottom = TClipY2;
										orgbclip = bEnableTempClip;
										// �ӽ� Ŭ���� ������ Ŭ���ڽ��� ���߾� ����
										OffsetRect(&cpi.Rect, x, y);
										if(bEnableTempClip) {
											// �̹� Ŭ������ �����Ǿ� �ִٸ� Ŭ���ڽ� ������ ���� �޴´�
											SetTempClip(max(cpi.Rect.left, TClipX1), 
												max(cpi.Rect.top, TClipY1), 
												min(cpi.Rect.right, TClipX2), 
												min(cpi.Rect.bottom, TClipY2));
										}
										else {
											SetTempClip(cpi.Rect.left, cpi.Rect.top, cpi.Rect.right, cpi.Rect.bottom);
										}

										EnableTempClip(TRUE);
										n_opt|=TEMPCLIP;
									}
								}
							}
						}
						CPI_SCENE2 cpiScene;
						if(pTrack->GetInterpol_Scene(time, &cpiScene)) {	
							TRACK_SCENE* pScnData = (TRACK_SCENE*)pTrack->pData;
							BOOL bscnloop = TRUE;
							if(pScnData->LifeTime != 0) {
								// ������Ÿ���� 0�� �ƴ� ���� �ݺ� ȸ����ŭ �÷����� ���Ŀ� ������ �����Ӹ� ǥ�õǵ��� �Ѵ�
								SceneData* pScene = pXSpr->GetScenePt(pScnData->SceneNo);
								if(pScene) 
									if(cpiScene.DeltaTime >  pScene->TotPlayTime * pScnData->RepeatCnt)
										bscnloop = FALSE;
							}

							if(cpiScene.Alpha != 248 || cpiScene.ZoomX != 100 || cpiScene.ZoomY != 100 || cpiScene.Angle != 0) {
								SCNEFFECTDESC effect;
								effect.mx = x + cpiScene.PosX + pScnData->CenterX;
								effect.my = y + cpiScene.PosY + pScnData->CenterY;
								effect.alpha = cpiScene.Alpha;
								effect.zoomx = cpiScene.ZoomX;
								effect.zoomy = cpiScene.ZoomY;
								effect.angle = cpiScene.Angle;

								SetSceneEffect(&effect);
							}
							else {
								SetSceneEffect(NULL);
							}

							DrawScene(x + cpiScene.PosX, y + cpiScene.PosY, pXSpr, pScnData->SceneNo, cpiScene.DeltaTime, bscnloop, n_opt, n_grade, n_color);

							if(bScnEffect)
								SetSceneEffect(NULL);
						}


						if(bSetTempClip) {
							SetTempClip(orgtmpclip.left, orgtmpclip.top, orgtmpclip.right, orgtmpclip.bottom);
							EnableTempClip(orgbclip);
						}
					}
				} break;

			case TTYPE_SOUND:
				{
					//TRACK_SOUND* pSndData = (TRACK_SOUND*)pTrack->pData;
				} break;

			case TTYPE_CLIPBOX:
				{
					//TRACK_CLIPBOX* pCbxData = (TRACK_CLIPBOX*)pTrack->pData;
				} break;

			case TTYPE_SCRIPT:
				{
					//TRACK_SCRIPT* pScrData = (TRACK_SCRIPT*)pTrack->pData;
				} break;

			default:
				break;
			}
		}
	}

	return TRUE;
}

//--------------------------------------------------------------------------------------
//      ��Ƽ���� Ư�� �� Ʈ���� �� ��� ������ ��� �Լ�
//--------------------------------------------------------------------------------------
BOOL CDxPage::GetMSceneDrawingCellInfo(int x, int y, xSprite *pXSpr, int multiscn, int track, int cell, DWORD time, BOOL loop, DRAWINGCELLDESC* pDesc)
{
	if(!pXSpr) 
		return FALSE;

	MSceneData* pMScene = pXSpr->multiscene.GetMScenePt(multiscn);

	if(!pMScene)
		return FALSE;

	if(loop && pMScene->TotPlayTime > 0) {
		if(time > 0 ) {
			time = time % pMScene->TotPlayTime;
			if(time == 0)
				time = pMScene->TotPlayTime;
		}
		else {
			time = time % pMScene->TotPlayTime;
		}
	}


	TrackData* pTrack = pMScene->GetPtrTrack(track);
	if(!pTrack || !pTrack->pData) 
		return FALSE;

	if(pTrack->TrackType != TTYPE_SCENE)
		return FALSE;

	CPI_SCENE2 cpi;
	if(!pTrack->GetInterpol_Scene(time, &cpi)) 
		return FALSE;

	TRACK_SCENE* pScnData = (TRACK_SCENE*)pTrack->pData;
	int scn = pScnData->SceneNo;

	SceneData* pScene = pXSpr->scene.GetScenePt(scn);

	if(!pScene)
		return FALSE;

	if(time < pTrack->Delay)
		return FALSE;

	if(pScnData->LifeTime == 0) {
		if(time > pTrack->Delay + (pScene->TotPlayTime * pScnData->RepeatCnt))
			return FALSE;
	}
	else {
		if(time > pTrack->Delay + pScnData->LifeTime)
			return FALSE;
	}

	BOOL bscnloop = TRUE;
	if(pScnData->LifeTime != 0) 
	{
		// ������Ÿ���� 0�� �ƴ� ���� �ݺ� ȸ����ŭ �÷����� ���Ŀ� ������ �����Ӹ� ǥ�õǵ��� �Ѵ�
		SceneData* pScene = pXSpr->GetScenePt(pScnData->SceneNo);
		if(pScene) 
			if(cpi.DeltaTime >  pScene->TotPlayTime * pScnData->RepeatCnt)
				bscnloop = FALSE;
	}


	// �ð��� �� �������� ��ȯ
	time -= pTrack->Delay;

	// �־��� �ð��� �÷��� �Ǿ�� �� �������� ã�´�
	int frame = pScene->SearchFrame(time, bscnloop);

	if(frame < 0)
		return FALSE;

	// ������ ������ ��Ÿ���� ����
	float delta = pScene->GetFrameDelta(frame, time, bscnloop);

	if(cpi.Alpha != 248 || cpi.ZoomX != 100 || cpi.ZoomY != 100 || cpi.Angle != 0) {
		SCNEFFECTDESC effect;
		effect.mx = x + cpi.PosX + pScnData->CenterX;
		effect.my = y + cpi.PosY + pScnData->CenterY;
		effect.alpha = cpi.Alpha;
		effect.zoomx = cpi.ZoomX;
		effect.zoomy = cpi.ZoomY;
		effect.angle = cpi.Angle;

		SetSceneEffect(&effect);
	}
	else {
		SetSceneEffect(NULL);
	}

	// �� ��� ������ ����
	if(!GetDrawingCellInfo(x + cpi.PosX, y + cpi.PosY, pXSpr, scn, frame, cell, delta, pDesc))
		return FALSE;

	if(bScnEffect)
		SetSceneEffect(NULL);

	return TRUE;
}

void CDxPage::SetClipArea(int x1, int y1, int x2, int y2)
{
	SetTempClip( x1 , y1 ,x2 ,y2 );
}


BOOL CDxPage::PutSprAuto(int x, int y,xSprite *pXSpr, int sprnum, int opt, DWORD dwColor )
{
	if(pXSpr==NULL) return FALSE;
	if(pXSpr->spr==NULL) return FALSE;
	if(sprnum<0 || sprnum >= pXSpr->sfhead.TotalSprNum) return FALSE;

	CD3DXSprite* temp = (CD3DXSprite*)pXSpr;
	SetSprBlend( pXSpr->spr[sprnum].PixelFormat , opt , (dwColor>>24) / 8);

	CPlane * pPlane = temp->GetPlane( sprnum );

	if(bEnableTempClip)
		opt |= TEMPCLIP;

	if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
		InitGrpLib(width, height, lPitchHalf, 555);

	if(opt & TEMPCLIP) {
		opt = (~(opt&TEMPCLIP))&opt;
		SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
	}
	else {
		SetClipArea(0, 0, width, height);
	}

	for( int i = 0 ; i < pPlane->m_pTextureNum ; i++ )
	{
		if(  temp )
		{

			if( g_clPointRender.m_bPointRenderFlag == true )
			{
				dwColor = GetCustomEffectColor(dwColor);

				g_clPointRender.AddPointRender(
					(float )x,
					(float )y,
					1.0f,
					dwColor,
					pPlane->m_pTexture[i].m_Texture,
					pPlane->m_pRect[i].right - pPlane->m_pRect[i].left);

				continue ;
			}
			pDirectx->SetTexture( 0 ,  pPlane->m_pTexture[i].m_Texture );

			SPRITE30 *pSpr = &pXSpr->spr[sprnum];

			if(pSpr->PixelFormat != 5558 || pSpr->pAlpha==NULL) {
				// ���� ���̺��� ���� �����̶�� ȿ�� �÷��� ����
				opt = (~(opt&AB))&opt;
			}

			// ������ ����̸�(�����ǥ ����)
			if(opt & MIRROR)
			{
				opt = (~(opt&MIRROR))&opt;
				//opt &= ~MIRROR;
				opt |= LR;
				if(opt & RXY)
				{
					x = x - (x+ pSpr->xl) - (x + pSpr->rx);


					//x = x + ((x + pSpr->rx + pSpr->xl) - x);
					//x = x + ( pSpr->xl - pSpr->rx) ;
					opt = (~(opt&RXY))&opt;
				}
				else
				{
					x = x + (x - (x + pSpr->xl));
				}
				y+=pSpr->ry;
			}
			else if(opt & RXY) { x+=pSpr->rx; y+=pSpr->ry; opt = (~(opt&RXY))&opt; }

			dwColor = GetCustomEffectColor(dwColor);


			if( opt == LE ||
				opt == DE )
			{

				int iR = 0 ;
				int iG = 0 ;
				int iB = 0 ;
				int aa;

				GetColorbyARGB(dwColor, &aa, &iR, &iG, &iB);

				float ftemp = (float )aa / (float )255;

				iR = (float )iR * ftemp ;
				iG = (float )iG * ftemp ;
				iB = (float )iB * ftemp ;

				// 				BYTE byR = (((color2>> 10) & 0x1f) <<19) * ftemp;
				// 				BYTE byB = (((color2>> 5)  & 0x1f) <<11) * ftemp;
				// 				BYTE byG = (((color2) & 0x1f) <<3) * ftemp;

				dwColor = D3DCOLOR_ARGB(
					255, 
					iR,
					iG,
					iB);
			}

			//			if(grade < 0) grade = 0;
			//			if(grade > 248) grade = 248;

			//			DWORD color2 = _WordtoDwordColor(color) &  ((grade*8 + grade/4) << 24 | 0x00ffffff);
			//			if( color == 0)color2 |=0x00ffffff;
			static D3DTLVERTEX g_vertices[4];

			if( bEnableTempClip == TRUE )
			{//TClipX1, TClipY1, TClipX2, TClipY2

				if( TClipX1 > pPlane->m_pRect[i].right+x)
					continue ;	 			
				int iTempLeft		= max(pPlane->m_pRect[i].left+x, TClipX1 );
				int iTempTop		= max(pPlane->m_pRect[i].top+y,  TClipY1 );
				int iTempRight		= min(pPlane->m_pRect[i].right+x,  TClipX2 );
				int iTempBottom		= min(pPlane->m_pRect[i].bottom+y, TClipY2 );

				if( iTempLeft >= pPlane->m_pRect[i].right +x ||
					iTempTop >= pPlane->m_pRect[i].bottom +y|| 
					iTempRight <= pPlane->m_pRect[i].left+x ||
					iTempBottom <= pPlane->m_pRect[i].top+y ) continue ;



				pDirectx->SetVertex( g_vertices,
					iTempLeft,
					iTempTop,
					iTempRight,
					iTempBottom
					);

				float fTempUV_X1 = (float )((iTempLeft-x) %pPlane->GetWidth(i)  ) / pPlane->GetWidth(i);
				float fTempUV_Y1 = (float )((iTempTop-y) %pPlane->GetHeight(i)   ) / pPlane->GetHeight(i);
				float fTempUV_X2 = (float )((iTempRight - iTempLeft) ) / pPlane->GetWidth(i) +fTempUV_X1;
				float fTempUV_Y2 = (float )((iTempBottom -iTempTop) )  / pPlane->GetHeight(i) +fTempUV_Y1;

				pDirectx->SetUV( 
					g_vertices, 
					fTempUV_X1,
					fTempUV_Y1,
					fTempUV_X2,//pPlane->GetTextureU(i),
					fTempUV_Y2,//pPlane->GetTextureV(i),
					opt);					
			}
			else
			{
				pDirectx->SetVertex( g_vertices,
					x + pPlane->m_pRect[i].left,
					y + pPlane->m_pRect[i].top,
					x + pPlane->m_pRect[i].right,
					y + pPlane->m_pRect[i].bottom 
					);
				pDirectx->SetUV( g_vertices, 0,0,pPlane->GetTextureU(i) , pPlane->GetTextureV(i) , opt);	

			}

			pDirectx->SetColor(g_vertices, dwColor );

			pDirectx->DrawVertex( g_vertices );

		}	

	}

	//pDirectx->SetAlphaBlend(0);

	return TRUE;
}




BOOL CDxPage::RotateZoomSprAuto2(int x, int y, xSprite *pXSpr, int sprnum, int tx, int ty, float angle, int opt, int *ilpCenterPosX, int *ilpCenterPosY)
{
	if(pXSpr==NULL) return FALSE;
	if(pXSpr->spr==NULL) return FALSE;
	if(sprnum<0 || sprnum >= pXSpr->sfhead.TotalSprNum) return FALSE;

	int xp = x - (tx / 2);
	int yp = y - (ty / 2);

	int oxp = xp;
	int oyp = yp;

	D3DXVECTOR3 rp, axis;
	rp.x = xp;
	rp.y = yp;
	rp.z = 0;

	axis.x = (oxp + (tx / 2)) + (*ilpCenterPosX);
	axis.y = (oyp + (ty / 2)) + (*ilpCenterPosY);
	axis.z = 0;

	_3DAxisRotatePoint(&rp, angle, axis);

	xp = rp.x;
	yp = rp.y;

	RotateZoomSprAuto(
		xp,
		yp,
		pXSpr,
		sprnum,
		tx,
		ty,
		angle,
		opt);

	D3DXVECTOR3 clCenTempVec;

	clCenTempVec.x = oxp + tx / 2;
	clCenTempVec.y = oyp + ty / 2;
	clCenTempVec.z = 0;

	_3DAxisRotatePoint(&clCenTempVec, angle, axis);



	(*ilpCenterPosX) = clCenTempVec.x;
	(*ilpCenterPosY) = clCenTempVec.y;

	return TRUE;
}

void CDxPage::SetSceneEffect(SCNEFFECTDESC* pEffect)
{
	if(!pEffect) 
	{
		bScnEffect = FALSE;
		return;
	}

	bScnEffect = TRUE;
	ScnEffect = *pEffect;
}
