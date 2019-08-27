// Texture.cpp: implementation of the CTexture class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Texture.h"
#include "Global.h"
//#include "..\\ClientDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DWORD A8R8G8B8TOA8R8G8B8(WORD data ,char alpha)
{
	return D3DCOLOR_ARGB(  (alpha == 0)? 0 : (alpha*8 + alpha/4)  ,  ((data >> 10) & 0x1f)<<3 , ((data>> 5) & 0x1f)<<3, (data & 0x1f)<<3);
}

CTexture::CTexture()
{
	m_Texture = NULL;
	m_BitCount = 0;
}

CTexture::~CTexture()
{
	if( m_Texture )
	{	
		int count = m_Texture->Release();	
		//		if( count != 0 ) OutputDebugString("0이 아닌데 지울라함\n");
		m_Texture = NULL;
	}
}

bool CTexture::CreateTexture( int width, int height , D3DFORMAT Pixelformat , D3DPOOL pool)
{
	m_Texture = g_clDirectX.CreateEmptyTexture( width , height, 1 , 0 , Pixelformat );

	if( m_Texture == NULL )
	{
		MessageBox( g_clDirectX.d3dpp.hDeviceWindow , "텍스쳐 생성이 실패 하였습니다\n비디오 장치가 3D 하드웨어 가속기능을 지원한다면\n해당 비디오 카드에 알맞은 최신 드라이버를 설치해보시기 바랍니다." ," 경고" , MB_OK );

		if( AfxMessageBox("최신 버전의 다이렉트X 밑 드라이버를 설치 할수 있는 홈페이지로 이동 하시겠습니까?",MB_YESNO) == IDOK  )
		{
			// 종환 임시 주석
			//g_pMainDlg->BrowserMove("http://game3.netmarble.net/CP_site/common/driver/index.asp");			
		}		


	}
	D3DSURFACE_DESC temp; 
	m_Texture->GetLevelDesc( 0 , &temp );
	m_Width = temp.Width;
	m_Height = temp.Height;
	m_BitCount = 	PixelFormatToBitCount( Pixelformat );
	if( m_Texture )	return true;

	MessageBox( g_clDirectX.d3dpp.hDeviceWindow , "텍스쳐가 생성실패." ," 경고" , MB_OK );
	return false;
}


bool CTexture::SetTextureFromData_A1R5G5B5( int width, int height , D3DFORMAT Pixelformat, unsigned char* pdataBuf,WORD colorkey,bool reverse ,int surfacewidth )
{
	int bitcount = PixelFormatToBitCount( Pixelformat );
	if( bitcount == 0 ) return false;
	if( m_Texture == NULL )
	{
		//생성되지 않은 텍스쳐
		MessageBox( g_clDirectX.d3dpp.hDeviceWindow , "텍스쳐가 생성되지 않았습니다." ," 경고" , MB_OK );
		return false;
	}

	D3DLOCKED_RECT rerect;
	if( D3DERR_INVALIDCALL == m_Texture->LockRect( 0 , &rerect , NULL , D3DLOCK_NOSYSLOCK ) )	return false;

	int swidth = rerect.Pitch / bitcount ;
	int sheight = height;
	WORD* Lockdata = (WORD*) rerect.pBits;
	WORD* pBuf = (WORD*) pdataBuf;
	if( surfacewidth == 0 )
	{
		surfacewidth = width;
	}
	DWORD count = 0;
	int lineno = 0;
	for(int nY = 0 ; nY < sheight ; nY++ )
	{
		if( !reverse ) lineno = sheight - ( nY + 1 );
		else lineno = nY;

		for(int  nX = 0 ; nX < swidth ; nX++ )
		{
			if( width > nX)
			{
				Lockdata[swidth * lineno + nX] = pBuf[count];
				if( pBuf[count] == colorkey)	Lockdata[swidth * lineno + nX] = 0x0000;				
				else							Lockdata[swidth * lineno + nX] |= 0x8000;

				count++;
			}
			else
			{
				Lockdata[swidth * lineno + nX] = 0x0000;
			}
		}
		count = surfacewidth * ( nY + 1 );
	}

	m_Texture->UnlockRect(0);
	return true;
}


bool  CTexture::SetTextureFromData_A8R5G5B5toA8R8G8B8(int width, int height , D3DFORMAT Pixelformat, unsigned char* pdataBuf,unsigned char *pAlpha , WORD colorkey,bool reverse ,int surfacewidth )
{
	if( surfacewidth == 0 )
	{
		surfacewidth = width;
	}
	int bitcount = PixelFormatToBitCount( Pixelformat );
	if( bitcount == 0 ) return false;
	if( m_Texture == NULL )
	{
		//생성되지 않은 텍스쳐
		MessageBox( g_clDirectX.d3dpp.hDeviceWindow , "텍스쳐가 생성되지 않았습니다." ," 경고" , MB_OK );
		return false;
	}


	D3DLOCKED_RECT rerect;
	if( D3DERR_INVALIDCALL == m_Texture->LockRect( 0 , &rerect , NULL , D3DLOCK_NOSYSLOCK ) )	return false;

	int swidth = rerect.Pitch / bitcount ;
	int sheight = height;
	DWORD* Lockdata = (DWORD*) rerect.pBits;
	WORD* pBuf = (WORD*) pdataBuf;

	DWORD count = 0;
	int lineno = 0;
	for(int nY = 0 ; nY < sheight ; nY++ )
	{
		if( !reverse ) lineno = sheight - ( nY + 1 );
		else lineno = nY;

		for(int  nX = 0 ; nX < swidth ; nX++ )
		{
			if( width > nX)
			{
				Lockdata[swidth * lineno + nX] = A8R8G8B8TOA8R8G8B8( pBuf[count] , pAlpha[count]);	
				count++;
			}
			else
			{
				Lockdata[swidth * lineno + nX] = 0x0000;
			}
		}
		count = surfacewidth * ( nY + 1 );
	}

	m_Texture->UnlockRect(0);
	return true;
}

D3DFORMAT CTexture::BitCountToPixelFormat(WORD bitCount)
{
	switch( bitCount )
	{
	case 32:  return D3DFMT_A8R8G8B8;
	case 24:  return D3DFMT_R8G8B8;
	case 16:  return D3DFMT_A1R5G5B5;
	default: return D3DFMT_UNKNOWN;
	}
}
WORD CTexture::PixelFormatToBitCount( D3DFORMAT PixelFormat )
{
	switch( PixelFormat )
	{
	case D3DFMT_A8R8G8B8:	return 4;
	case D3DFMT_R8G8B8:		return 3;
	case D3DFMT_A1R5G5B5:	return 2;
	default: return 0;
	}
}
