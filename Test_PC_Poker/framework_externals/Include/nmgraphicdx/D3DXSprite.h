//// D3DXSprite.h: interface for the CD3DXSprite class.
////
////////////////////////////////////////////////////////////////////////
//
//#if !defined(AFX_D3DXSPRITE_H__CB0C5378_8271_4A64_95B9_E480B0492E6E__INCLUDED_)
//#define AFX_D3DXSPRITE_H__CB0C5378_8271_4A64_95B9_E480B0492E6E__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

#pragma once

#include  <list>
#include  <map>

#include "DirectX.h"
#include "Texture.h"
#include "xSprite.h"
#include "DataTaskManager.h"

class CPlane
{
public:
	CPlane( CPlane& plane)
	{
		m_pRect			= 	plane.m_pRect;
		m_pTexture		= 	plane.m_pTexture;
		m_pTextureNum	= 	plane.m_pTextureNum;
		m_Size			= 	plane.m_Size;
		m_Sprno			=	plane.m_Sprno;

	}
	CPlane(){ m_pTexture = NULL; m_pRect = NULL;m_pTextureNum = 0;m_Size = 0;m_Sprno = 0;}
	virtual ~CPlane()
	{
		if( m_pTexture ) { delete[] m_pTexture;	m_pTexture	= NULL;	} 
		if( m_pRect )	 { delete[] m_pRect;		m_pRect		= NULL;	} 
		m_pTextureNum = 0;
		m_Size = 0;
		m_Sprno = 0;
	}
	int m_Sprno;
	ClassRect *m_pRect;
	CTexture* m_pTexture;
	int  m_pTextureNum;
	int  m_Size;
	BOOL CreatePlane( int xl , int yl , D3DFORMAT format , int size);
	void SetPlaneData(int width, int height , D3DFORMAT Pixelformat, unsigned char* pdataBuf,WORD colorkey,bool reverse ,int surfacewidth = 0 );

	void SetPlaneData_A8R5G5B5toA8R8G8B8(int width, int height , D3DFORMAT Pixelformat, unsigned char* pdta ,unsigned char *pAlpha,WORD colorkey = 0x7c1f , bool reverse = false ,int Surfacewidth = 0 );
	void SetPlaneData_A8R8G8B8toA8R8G8B8(int width, int height , D3DFORMAT Pixelformat, unsigned char* pdta ,unsigned char *pAlpha,WORD colorkey = 0x7c1f , bool reverse = false ,int Surfacewidth = 0 );
	void SetPlaneData_A1R5G5B5(int width, int height , D3DFORMAT Pixelformat, unsigned char* pdataBuf,WORD colorkey,bool reverse ,int surfacewidth );

	int GetWidth(int index){ return  m_pRect[index].Width();}
	int GetHeight(int index){ return m_pRect[index].Height();}

	//float GetTextureU(int index){ return ( m_pRect[index].Width() ) /(float)m_Size;}
	//float GetTextureV(int index){ return ( m_pRect[index].Height() ) /(float)m_Size;}
	float GetTextureU(int index){ return ( m_pRect[index].Width() ) /(float)m_pTexture[index].m_Width;}
	float GetTextureV(int index){ return ( m_pRect[index].Height() ) /(float)m_pTexture[index].m_Height;}


};

class CD3DXSprite : public xSprite , public CDataTask
	//class CD3DXSprite : public xSprite
{
public:

	std::map< int, CPlane * > m_ListPlane;
	std::map< int, CPlane * >::iterator GetPlaneIterator( int sprno );


	CPlane* GetPlane( int sprno );
	BOOL MovePlaneTexture( int source , int target );
	BOOL RemovePlaneTexture( int sprno );
	void CreatePlaneTexture( int sprno );
	CPlane * CreatePlane( int sprno );
	CD3DXSprite();
	virtual ~CD3DXSprite();
	BOOL Load(char *name);
	void Remove();

	BOOL RestoreDeviceObject();
	D3DFORMAT GetPixelFormat( int pixelformat );	
	void OptimizeSize(int snum, BOOL fixPos);
	void DeleteDeviceObject();
	BOOL TransToSpr(int xl, int yl, WORD *pImage, WORD wColorKey);

	BOOL ConvertTo3DSprFile(char *name);
	BOOL Load3DSprFile(char *name);

	BOOL Insert(LONG snum, int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress, WORD *pAlpha = NULL);

	virtual void				Load();
	virtual void				Delete();
};