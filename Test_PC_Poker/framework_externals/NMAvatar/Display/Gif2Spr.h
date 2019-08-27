// Gif2Spr.h: interface for the CGif2Spr class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_GIF2SPR_H__04BCCB65_0A2C_4601_BEA8_C6962722AD0E__INCLUDED_)
//#define AFX_GIF2SPR_H__04BCCB65_0A2C_4601_BEA8_C6962722AD0E__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
#pragma once

class xSprite;

// GIF 이미지를 메모리상의 스프라이트로 변환시키는 함수
BOOL ConvertGif2Spr(char* gif_name, xSprite *pSprite, BOOL bOptimizeSize=TRUE);

// 메모리상의 GIF 이미지를 메모리상의 스프라이트로 변환시키는 함수
BOOL ConvertGif2SprFromMemory(void* pGifImage, DWORD dwSize, xSprite *pSprite, BOOL bOptimizeSize=TRUE);

// GIF 이미지를 스프라이트로 변환시켜 파일로 저장하는 함수
BOOL ConvertGif2SprFile(char* gif_name, char* spr_name, BOOL bOptimizeSize=TRUE);


//#endif // !defined(AFX_GIF2SPR_H__04BCCB65_0A2C_4601_BEA8_C6962722AD0E__INCLUDED_)
