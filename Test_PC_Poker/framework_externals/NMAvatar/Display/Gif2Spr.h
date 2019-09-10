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

// GIF �̹����� �޸𸮻��� ��������Ʈ�� ��ȯ��Ű�� �Լ�
BOOL ConvertGif2Spr(char* gif_name, xSprite *pSprite, BOOL bOptimizeSize=TRUE);

// �޸𸮻��� GIF �̹����� �޸𸮻��� ��������Ʈ�� ��ȯ��Ű�� �Լ�
BOOL ConvertGif2SprFromMemory(void* pGifImage, DWORD dwSize, xSprite *pSprite, BOOL bOptimizeSize=TRUE);

// GIF �̹����� ��������Ʈ�� ��ȯ���� ���Ϸ� �����ϴ� �Լ�
BOOL ConvertGif2SprFile(char* gif_name, char* spr_name, BOOL bOptimizeSize=TRUE);


//#endif // !defined(AFX_GIF2SPR_H__04BCCB65_0A2C_4601_BEA8_C6962722AD0E__INCLUDED_)
