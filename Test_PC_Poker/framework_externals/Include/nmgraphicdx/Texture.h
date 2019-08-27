//// Texture.h: interface for the CTexture class.
////
////////////////////////////////////////////////////////////////////////
//
//#if !defined(AFX_TEXTURE_H__3FD90722_4C93_4625_A7D0_F3979D9F2FB0__INCLUDED_)
//#define AFX_TEXTURE_H__3FD90722_4C93_4625_A7D0_F3979D9F2FB0__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

#pragma once

#include "d3dx9tex.h"

class CTexture  
{
public:
	CTexture();
	virtual ~CTexture();

	LPDIRECT3DTEXTURE9 m_Texture;

	int			m_Width;
	int			m_Height;
	int			m_BitCount;

	int GetImageWidth()				{ return m_Width;					}
	int GetImageHeight()			{ return m_Height;					}
	int	GetBitCount()				{ return m_BitCount;				}

	bool CreateTexture( int width, int height , D3DFORMAT Pixelformat , D3DPOOL pool = D3DPOOL_SYSTEMMEM);
	bool SetTextureFromData_A1R5G5B5( int width, int height , D3DFORMAT Pixelformat, unsigned char* pdta ,WORD colorkey = 0x7c1f , bool reverse = false ,int Surfacewidth = 0 );
	bool SetTextureFromData_A8R5G5B5toA8R8G8B8(int width, int height , D3DFORMAT Pixelformat, unsigned char* pdta ,unsigned char *pAlpha,WORD colorkey = 0x7c1f , bool reverse = false ,int Surfacewidth = 0 );
	D3DFORMAT BitCountToPixelFormat( WORD bitCount );
	WORD	  PixelFormatToBitCount( D3DFORMAT PixelFormat );
};

extern DWORD A8R8G8B8TOA8R8G8B8(WORD data ,char alpha);

