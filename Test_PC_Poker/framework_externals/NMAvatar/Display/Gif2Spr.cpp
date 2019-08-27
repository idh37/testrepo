// Gif2Spr.cpp: implementation of the CGif2Spr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gif2Spr.h"

#include <Afxmt.h>

#include "PictureEx.h"
#include "Graphic.h"
#include "xSprite.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////

BOOL _Gif2Spr(CPictureEx& pic, xSprite *pSprite, BOOL bOptimizeSize) 
{
	if(!pSprite) 
		return FALSE;

	if(!pic.IsGIF()) 
		return FALSE;

	pSprite->Remove();

	CPage page, prepage;
	page.Init(pic.m_PictureSize.cx, pic.m_PictureSize.cy, 16);

	// 이미지의 배경을 지움
	page.DrawFillBox(0, 0, page.width, page.height, RGBmix(31,0,31));

	CDC memdc;
	memdc.CreateCompatibleDC(NULL);
	memdc.SelectObject(page.hBmp);

	if(pic.IsAnimatedGIF())
	{
		for(int i=0; i<pic.GetFrameCount(); i++)
		{

			///////////////////////////////////////////////////////
			// Before rendering a frame we should take care of what's 
			// behind that frame. TFrame::m_nDisposal will be our guide:
			//   0 - no disposal specified (do nothing)
			//   1 - do not dispose (again, do nothing)
			//   2 - restore to background color (m_clrBackground)
			//   3 - restore to previous

			if(pic.m_arrFrames[i].m_nDisposal == 3) 
			{
				// 현재 프레임을 백업해둠
				if(!prepage.lpBit)
				{
					prepage.Init(pic.m_PictureSize.cx, pic.m_PictureSize.cy, 16);
				}

				int x = pic.m_arrFrames[i].m_frameOffset.cx;
				int y = pic.m_arrFrames[i].m_frameOffset.cy;
				int w = pic.m_arrFrames[i].m_frameSize.cx;
				int h = pic.m_arrFrames[i].m_frameSize.cy;

				prepage.PutFromPage(x, y, w, h, &page);
			}

			// GIF 프레임을 페이지에 그림
			pic.DrawSingleFrameToDC(memdc.GetSafeHdc(), 0, 0, i);

			/////////////// 스프라이트 생성 ////////////////

			int xl = page.width;
			int yl = page.height;

			// 할당한 메모리는 해제하지 않음
			WORD* pImage = new WORD[xl*yl];
			if(!pImage)
				return FALSE;

			WORD* pTar = pImage;
			WORD* pSrc = page.lpBit;
			for(int y=0; y<yl; ++y) 
			{
				memcpy(pTar, pSrc, xl*2);
				pTar += xl;
				pSrc += page.lPitchHalf;
			}

			int nowspn = pSprite->sfhead.TotalSprNum;

			// 일단 압축되지 않은 상태로 스프라이트를 삽입
			if(!pSprite->Insert(nowspn, xl, yl, pImage, RGBmix(31,0,31), FALSE)) 
			{
				// 스프라이트 삽입에 실패한 경우만 메모리를 해제한다
				delete [] pImage;
				return FALSE;
			}

			if(bOptimizeSize) 
			{
				// 사이즈 최적화
				pSprite->OptimizeSize(nowspn);
			}

			// 스프라이트 압축
			pSprite->Compress(nowspn);

			pSprite->spr[nowspn].byUserVal = max(1, (BYTE)(pic.m_arrFrames[i].m_nDelay / 10));

			////////////////////////////////////////////////

			if(pic.m_arrFrames[i].m_nDisposal == 2)
			{
				// 이미지의 배경을 지움(이외의 것들은 이전 프레임의 이미지를 누적시킨다)
				RECT rect =
				{
					pic.m_arrFrames[i].m_frameOffset.cx,
					pic.m_arrFrames[i].m_frameOffset.cy,
					pic.m_arrFrames[i].m_frameOffset.cx + pic.m_arrFrames[i].m_frameSize.cx,
					pic.m_arrFrames[i].m_frameOffset.cy + pic.m_arrFrames[i].m_frameSize.cy 
				};

				page.DrawFillBox(rect.left, rect.top, rect.right-1, rect.bottom-1, RGBmix(31,0,31));
			}
			else if(pic.m_arrFrames[i].m_nDisposal == 3) 
			{
				// 백업해둔 이전 프레임으로 복구
				int x = pic.m_arrFrames[i].m_frameOffset.cx;
				int y = pic.m_arrFrames[i].m_frameOffset.cy;
				int w = pic.m_arrFrames[i].m_frameSize.cx;
				int h = pic.m_arrFrames[i].m_frameSize.cy;

				page.PutFromPage(x, y, w, h, &prepage);
			}
		}
	}
	else 
	{
		page.DrawFillBox(0, 0, page.width, page.height, RGBmix(31,0,31));
		pic.DrawSingleFrameToDC(memdc.GetSafeHdc(), 0, 0, 0);

		int xl = page.width;
		int yl = page.height;

		// 할당한 메모리는 해제하지 않음
		WORD* pImage = new WORD[xl*yl];
		if(!pImage)
			return FALSE;

		WORD* pTar = pImage;
		WORD* pSrc = page.lpBit;
		for(int y=0; y<yl; ++y) 
		{
			memcpy(pTar, pSrc, xl*2);
			pTar += xl;
			pSrc += page.lPitchHalf;
		}

		int nowspn = pSprite->sfhead.TotalSprNum;
		// 일단 압축되지 않은 상태로 스프라이트를 삽입
		if(!pSprite->Insert(nowspn, xl, yl, pImage, RGBmix(31,0,31), FALSE))
		{
			// 스프라이트 삽입에 실패한 경우만 메모리를 해제한다
			delete [] pImage;
			return FALSE;
		}

		if(bOptimizeSize) 
		{
			// 사이즈 최적화
			pSprite->OptimizeSize(nowspn);
		}

		// 스프라이트 압축
		pSprite->Compress(nowspn);

		pSprite->spr[nowspn].byUserVal = 1;
	}

	if(pSprite->sfhead.TotalSprNum > 0)
	{
		return TRUE;
	}

	return FALSE;
}


BOOL ConvertGif2Spr(char* gif_name, xSprite *pSprite, BOOL bOptimizeSize) 
{
	if(!pSprite) return FALSE;

	pSprite->Remove();

	CPictureEx pic;
	if(pic.Load(gif_name))
	{
		return _Gif2Spr(pic, pSprite, bOptimizeSize);
	}

	return FALSE;
}

BOOL ConvertGif2SprFromMemory(void* pGifImage, DWORD dwSize, xSprite *pSprite, BOOL bOptimizeSize) 
{
	if(!pGifImage)
		return FALSE;

	if(dwSize == 0)
		return FALSE;

	if(!pSprite)
		return FALSE;

	pSprite->Remove();

	CPictureEx pic;
	if(pic.LoadFromMemory(pGifImage, dwSize)) 
	{
		return _Gif2Spr(pic, pSprite, bOptimizeSize);
	}

	return FALSE;
}

BOOL ConvertGif2SprFile(char* gif_name, char* spr_name, BOOL bOptimizeSize) 
{
	xSprite spr;

	if(ConvertGif2Spr(gif_name, &spr, bOptimizeSize)) 
	{
		if(spr.sfhead.TotalSprNum > 0) 
		{
			if(spr.Save(spr_name)) 
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}
