// D3DXSprite.cpp: implementation of the CD3DXSprite class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <assert.h>
#include "D3DXSprite.h"
#include "global.h"

//#include "..\\ClientDlg.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD3DXSprite::CD3DXSprite()
{
	//	m_pTexture = NULL;
}

CD3DXSprite::~CD3DXSprite()
{
	Remove();
	DeleteDeviceObject();
}
BOOL CD3DXSprite::ConvertTo3DSprFile(char *name)
{
	if( xSprite::Load( name ) )
	{
		return TRUE;
		//변경
	}

	return FALSE;
}
BOOL CD3DXSprite::Load3DSprFile(char *name)
{
	DeleteDeviceObject();
	return  xSprite::Load( name );
}
BOOL CD3DXSprite::Load(char *name)
{
	DeleteDeviceObject();
	if(!xSprite::Load( name ) ) 
		return FALSE;
	RestoreDeviceObject();

	for( int iMScene = 0 ; iMScene < multiscene.TotMSceneNum ; iMScene++ ) 
	{
		for( int iTrack = 0 ; iTrack < multiscene.ppMScene[iMScene]->TotTrackNum; iTrack++ )	
		{
			TrackData* pTrack = multiscene.ppMScene[iMScene]->ppTrack[iTrack];
			if(pTrack->TrackType == TTYPE_SCENE)
			{
				TRACK_SCENE* pSceneData = (TRACK_SCENE*)pTrack->pData;
				pSceneData->Alpha *= 8;
				for(int i=0; i<pSceneData->CpiNum; i++) {
					pTrack->pCpiScene[i].Alpha *= 8;
				}

			}
		}
	}

	for( int iScene = 0 ; iScene < scene.TotSceneNum ; iScene++ ) 
	{
		for( int iFrame = 0 ; iFrame < scene.ppScene[iScene]->TotFrameNum; iFrame++ )	
		{
			FrameData* pFrame = scene.ppScene[iScene]->ppFrame[iFrame];

			for( int icell = 0 ; icell < pFrame->TotCellNum; icell++)
			{
				pFrame->ppCell[icell]->Grade *= 8;
			}
		}
	}



	return TRUE;
}
D3DFORMAT CD3DXSprite::GetPixelFormat( int pixelformat )	
{		
	if( pixelformat == 5558 ) 		return D3DFMT_A8R8G8B8;	
	if( pixelformat == 8888 ) 		return D3DFMT_A8R8G8B8;	
	else if( pixelformat == 555 )	return D3DFMT_A1R5G5B5;
	return D3DFMT_A1R5G5B5;	
}
std::map< int, CPlane * >::iterator CD3DXSprite::GetPlaneIterator( int sprno )
{


	std::map< int, CPlane * >::iterator iter = m_ListPlane.find(sprno);

	if( iter != m_ListPlane.end() )
	{
		return iter;
	}

	return NULL;


	// 	if( sprno > m_ListPlane.size() ) return NULL;
	// 	int count = 0;
	// 	for( std::list< CPlane * >::iterator i = m_ListPlane.begin() ; i!= m_ListPlane.end() ; i++ )
	// 	{
	// 		CPlane * pPlane = (*i);
	// 		if( sprno == count )
	// 		{
	// 			return  i;
	// 		}
	// 		count++;
	// 	}
	// 	return NULL;
}
BOOL CD3DXSprite::MovePlaneTexture( int source , int target )
{
	// 수정 해야됨.
	assert(0);
	// 	CPlane * pPlaneSource = NULL;
	// 
	// 	if( source > m_ListPlane.size() ) return FALSE;
	// 	if( target > m_ListPlane.size() ) return FALSE;
	// 
	// 	std::list< CPlane * >::iterator targetIter = GetPlaneIterator(target);
	// 	int count = 0;
	// 	for( std::list< CPlane * >::iterator i = m_ListPlane.begin() ; i!= m_ListPlane.end() ; i++ )
	// 	{
	// 		CPlane * pPlane = (*i);
	// 		if( source == count )
	// 		{
	// 			pPlaneSource = pPlane;
	// 			m_ListPlane.erase( i );
	// 			break;
	// 		}	
	// 		count++;
	// 	}
	// 	if( pPlaneSource != NULL)
	// 		m_ListPlane.insert(targetIter , pPlaneSource) ;

	return TRUE;
}

BOOL CD3DXSprite::RemovePlaneTexture( int sprno )
{
	// 수정 해야됨.
	assert(0);

	std::map< int, CPlane * >::iterator iter = m_ListPlane.find(sprno);

	if( iter != m_ListPlane.end() )
	{
		delete (*iter).second;
		(*iter).second = NULL;

		m_ListPlane.erase(iter);


		return TRUE;
	}




	// 	if( sprno > m_ListPlane.size() ) return FALSE;
	// 	int count = 0;
	// 
	// 	for( std::list< CPlane * >::iterator i = m_ListPlane.begin() ; i!= m_ListPlane.end() ; i++ )
	// 	{
	// 		CPlane * pPlane = (*i);
	// 		if( sprno == count )
	// 		{
	// 			delete pPlane;
	// 			pPlane = NULL;
	// 			m_ListPlane.erase( i );
	// 			return TRUE;
	// 		}	
	// 		count++;
	// 	}
	return FALSE;
}
void CD3DXSprite::CreatePlaneTexture( int sprno )
{
	//	RemovePlaneTexture( sprno );
	CPlane *pPlane = CreatePlane( sprno );	



	std::map< int, CPlane * >::iterator iter = m_ListPlane.find(sprno);
	if( iter == m_ListPlane.end() )
	{
		m_ListPlane.insert( pair<int , CPlane * >(sprno, pPlane ) ) ;
	}
	else
	{
		AfxMessageBox("Erro if( iter == m_ListPlane.end(sprno) )");
	}

	//

}
CPlane * CD3DXSprite::CreatePlane( int sprno )
{
	CPlane *pPlane = new CPlane;

	if( !spr[sprno].data ) 
	{
		//		OutputDebugString("spr[sprno].data 값이없음\n");
		return FALSE;
	}

	D3DFORMAT format = D3DFMT_UNKNOWN;
	if( spr[sprno].pAlpha != NULL &&  spr[sprno].PixelFormat != 8888 ) spr[sprno].PixelFormat = 5558;
	format = GetPixelFormat( spr[sprno].PixelFormat );

	if( spr[sprno].compress )
	{
		DeCompress( sprno );
	}	
	if( pPlane->CreatePlane(spr[sprno].xl,spr[sprno].yl , format , g_clDirectX.m_iMAX_TEXTURE_SIZE) )
	{
		//(Copydata)

		if( D3DFMT_A8R8G8B8 == format )
		{
			if( spr[sprno].PixelFormat == 8888 )
			{
				pPlane->SetPlaneData_A8R8G8B8toA8R8G8B8(	spr[sprno].xl,
					spr[sprno].yl, 
					format,
					(unsigned char *)spr[sprno].data, 
					(unsigned char *)spr[sprno].pAlpha,
					spr[sprno].colorkey,
					true ,spr[sprno].xl );
			}
			else
			{	
				pPlane->SetPlaneData_A8R5G5B5toA8R8G8B8(	spr[sprno].xl,
					spr[sprno].yl, 
					format,
					(unsigned char *)spr[sprno].data, 
					(unsigned char *)spr[sprno].pAlpha,
					spr[sprno].colorkey,
					true ,spr[sprno].xl );
			}

		}
		else// if( D3DFMT_A1R5G5B5 == format )
		{
			pPlane->SetPlaneData_A1R5G5B5(spr[sprno].xl,
				spr[sprno].yl,
				format,
				(unsigned char *)spr[sprno].data,
				spr[sprno].colorkey,
				true,spr[sprno].xl );	
		}
	}
	//	pPlane->m_Sprno = sprno;
	return pPlane;
}
CPlane* CD3DXSprite::GetPlane( int sprno )
{


	std::map< int, CPlane * >::iterator iter = m_ListPlane.find(sprno);

	if( iter != m_ListPlane.end() )
	{
		return (*iter).second;

	}

	//if( sprno > m_ListPlane.size() ) return NULL;
	// 	int count = 0;
	// 	for( std::list< CPlane * >::iterator i = m_ListPlane.begin() ; i!= m_ListPlane.end() ; i++ )
	// 	{
	// 		CPlane * pPlane = (*i);
	// 		if( sprno == count )
	// 		{
	// 			return  pPlane;
	// 		}
	// 		count++;
	// 	}
	return NULL;
}
BOOL CD3DXSprite::RestoreDeviceObject()
{
	for( int i = 0 ; i < sfhead.TotalSprNum ; i++ )
	{
		//m_ListPlane.push_back( CreatePlane(i) );

		m_ListPlane.insert(pair< int, CPlane * >( i, CreatePlane(i) ));

	} 


	if(spr != NULL)  
	{
		for(int i=0; i < sfhead.TotalSprNum; i++) 
		{			
			if( spr[i].data != NULL ) { free(spr[i].data); spr[i].data = NULL; }
			if( spr[i].pJpgData != NULL) { free(spr[i].pJpgData); spr[i].pJpgData = NULL; }
		}		

	}

	return TRUE;
}
void CD3DXSprite::DeleteDeviceObject()
{
	//	if( m_pTexture ) delete[] m_pTexture;
	//	m_Piece.Clear();

	if( m_ListPlane.empty() == true ) return ;


	for( std::map< int, CPlane * >::iterator i = m_ListPlane.begin() ; i != m_ListPlane.end() ; i++ )
	{
		CPlane * pPlane = (*i).second;
		delete pPlane;
		pPlane = NULL;
	}
	m_ListPlane.clear();



	// 	for( std::list< CPlane * >::iterator i = m_ListPlane.begin() ; i!= m_ListPlane.end() ; i++ )
	// 	{
	// 		CPlane * pPlane = (*i);
	// 		delete pPlane;
	// 		pPlane = NULL;
	// 	}
	// 	m_ListPlane.clear();

}

void CD3DXSprite::Remove()
{
	xSprite::Remove();
	DeleteDeviceObject();
}

BOOL CD3DXSprite::Insert(LONG snum, int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress,WORD *pAlpha)
{
	assert(0);

	if( FALSE == xSprite::Insert(snum, xl,  yl, pImage,  wColorKey, bCompress ) ) 
		return FALSE;

	spr[snum].PixelFormat = 8888;		// freetype font 때문에(태윤대리님 한테 문의)

	//덮어 쓰는경우

	std::map< int, CPlane * >::iterator iter = m_ListPlane.find(snum);
	if( iter != m_ListPlane.end() )
	{
		CPlane * pplane = (*iter).second;
		if( pplane ) 
		{
			delete pplane;
			pplane = NULL;
		}
		m_ListPlane.erase(iter);
	}


	// 	int count = 0;
	// 	for( std::list< CPlane * >::iterator i = m_ListPlane.begin() ; i!= m_ListPlane.end() ; i++ )
	// 	{
	// 		if( snum == count )
	// 		{
	// 			CPlane * pplane = (*i);
	// 			if( pplane ) 
	// 			{
	// 				delete pplane;
	// 				pplane = NULL;
	// 			}
	// 			m_ListPlane.remove(*i);
	// 			break;
	// 		}
	// 		count++;
	// 	}


	if( pAlpha == NULL )
	{
		CPlane *pPlane = CreatePlane(snum);
		m_ListPlane.insert(pair< int, CPlane * >( snum, pPlane));

		// 		if( snum >= m_ListPlane.size() )
		// 		{
		// 			m_ListPlane.push_back(pPlane);
		// 		}
		// 		else
		// 		{
		// 			int count = 0;
		// 			for( std::list< CPlane * >::iterator i = m_ListPlane.begin() ; i!= m_ListPlane.end() ; i++ )
		// 			{
		// 				if( snum == count )
		// 				{
		// 					m_ListPlane.insert( i , pPlane );
		// 					break;
		// 				}
		// 				count++;
		// 			}
		// 		}
	}
	else 
	{	// freetype font 때문에(태윤대리님 한테 문의)
		spr[snum].pAlpha = (BYTE*)pAlpha;
	}


	return TRUE;
}
void  CD3DXSprite::OptimizeSize(int snum, BOOL fixPos)
{
	xSprite::OptimizeSize( snum , fixPos );
	CPlane * tempplane = CreatePlane( snum );
	std::map< int, CPlane * >::iterator planeiter = GetPlaneIterator(snum);

	CPlane * temp = (CPlane*)(*planeiter).second;
	(CPlane*)(*planeiter).second = tempplane;
	delete temp;
}
BOOL CD3DXSprite::TransToSpr(int xl, int yl, WORD *pImage, WORD wColorKey)
{
	Remove();
	BOOL result = xSprite::TransToSpr(xl,yl,pImage,wColorKey);
	RestoreDeviceObject();
	return result;
}
int FloatRoundUpInt( float input )
{
	if( input == (int)input ) return (int)input;
	return int(input + 1);
}

BOOL CPlane::CreatePlane( int xl , int yl , D3DFORMAT format , int size )
{
	int Mulx = FloatRoundUpInt( xl / (float)size ); 
	int Muly = FloatRoundUpInt( yl / (float)size );  
	m_pTextureNum = Mulx * Muly;
	m_pTexture	= new CTexture[ m_pTextureNum ];
	m_pRect		= new ClassRect[ m_pTextureNum ];
	m_Size		= size;
	for( int i = 0 ; i< m_pTextureNum ; i++ )
	{
		int x = (i % Mulx) * size;
		int y = (i / Mulx) * size;
		m_pRect[i].SetRect( x , y, min(x + size , xl) ,min( y + size , yl ) );
		if( !m_pTexture[i].CreateTexture( m_pRect[i].Width() , m_pRect[i].Height(), format ) )
		{
			//실패
		}
	}
	return TRUE;
}
void CPlane::SetPlaneData(int width, int height , D3DFORMAT Pixelformat, unsigned char* pdataBuf,WORD colorkey,bool reverse ,int surfacewidth )
{

	int Mulx = FloatRoundUpInt( width  / (float)m_Size ); 
	int Muly = FloatRoundUpInt( height / (float)m_Size );  

	WORD* pBuf = (WORD*) pdataBuf;
	DWORD  count = 0;

	for( int i = 0 ; i< m_pTextureNum ; i++ )
	{
		count = (( (i / Mulx) * surfacewidth * m_Size) + (( i % Mulx )* m_Size));// * m_pTexture[i].PixelFormatToBitCount( Pixelformat );

		D3DLOCKED_RECT rerect;
		if( D3DERR_INVALIDCALL == m_pTexture[i].m_Texture->LockRect( 0 , &rerect , NULL , D3DLOCK_NOSYSLOCK ) )	return;

		int swidth = rerect.Pitch / m_pTexture[i].PixelFormatToBitCount( Pixelformat ) ;
		int sheight = min( m_Size , height - ( (i / Mulx) * m_Size) );
		WORD* Lockdata = (WORD*) rerect.pBits;
		WORD* pBuf = (WORD*) pdataBuf;

		int lineno = 0;
		for(int nY = 0 ; nY < sheight ; nY++ )
		{
			if( !reverse ) lineno = sheight - ( nY + 1 );
			else lineno = nY;

			for(int  nX = 0 ; nX < swidth ; nX++ )
			{
				if( width > ( i % Mulx )* m_Size + nX )
				{
					Lockdata[swidth * lineno + nX] = pBuf[count];
					if( pBuf[count] == colorkey)	Lockdata[swidth * lineno + nX] = 0;				
					else							Lockdata[swidth * lineno + nX] |= 0x8000;

					count++;
				}
				else
				{
					Lockdata[swidth * lineno + nX] = 0;
				}
			}
			count = (( (i / Mulx) * surfacewidth * m_Size) + (( i % Mulx )* m_Size)) + surfacewidth * ( nY + 1 );
		}

		m_pTexture[i].m_Texture->UnlockRect(0);
	}	

}
void CPlane::SetPlaneData_A8R8G8B8toA8R8G8B8(int width, int height , D3DFORMAT Pixelformat, unsigned char* pdta ,unsigned char *pAlpha,WORD colorkey , bool reverse ,int Surfacewidth )
{
	int Mulx = FloatRoundUpInt( width  / (float)m_Size ); 
	int Muly = FloatRoundUpInt( height / (float)m_Size );  

	DWORD  count = 0;
	for( int i = 0 ; i< m_pTextureNum ; i++ )
	{
		count = (( (i / Mulx) * Surfacewidth * m_Size) + (( i % Mulx )* m_Size));

		D3DLOCKED_RECT rerect;
		if( D3DERR_INVALIDCALL == m_pTexture[i].m_Texture->LockRect( 0 , &rerect , NULL , D3DLOCK_NOSYSLOCK ) )	return;

		int swidth = rerect.Pitch / m_pTexture[i].PixelFormatToBitCount( Pixelformat ) ;
		int sheight = min( m_Size , height - ( (i / Mulx) * m_Size) );
		DWORD* Lockdata = (DWORD*) rerect.pBits;
		DWORD* pBuf = (DWORD*) pdta;
		DWORD* alpha = (DWORD*) pAlpha;
		int lineno = 0;
		for(int nY = 0 ; nY < sheight ; nY++ )
		{
			if( !reverse ) lineno = sheight - ( nY + 1 );
			else lineno = nY;

			for(int  nX = 0 ; nX < swidth ; nX++ )
			{
				if( width > ( i % Mulx )* m_Size + nX)
				{

					DWORD B = (alpha[count]>>24)	& 0x00ff;
					DWORD G = (alpha[count]>>16)	& 0x00ff;
					DWORD R = (alpha[count]>>8)		& 0x00ff;
					DWORD a = (alpha[count])		& 0x00ff;

					Lockdata[swidth * lineno + nX] = alpha[count];	
					count++;
				}
				else
				{
					//SetAlpha
					Lockdata[swidth * lineno + nX] = 0;
				}
			}
			count = (( (i / Mulx) * Surfacewidth * m_Size) + (( i % Mulx )* m_Size)) + Surfacewidth * ( nY + 1 );
		}

		m_pTexture[i].m_Texture->UnlockRect(0);
	}	
}
void CPlane::SetPlaneData_A8R5G5B5toA8R8G8B8(int width, int height , D3DFORMAT Pixelformat, unsigned char* pdta ,unsigned char *pAlpha,WORD colorkey , bool reverse ,int Surfacewidth )
{
	int Mulx = FloatRoundUpInt( width  / (float)m_Size ); 
	int Muly = FloatRoundUpInt( height / (float)m_Size );  

	DWORD  count = 0;
	for( int i = 0 ; i< m_pTextureNum ; i++ )
	{
		count = (( (i / Mulx) * Surfacewidth * m_Size) + (( i % Mulx )* m_Size));

		D3DLOCKED_RECT rerect;
		if( D3DERR_INVALIDCALL == m_pTexture[i].m_Texture->LockRect( 0 , &rerect , NULL , D3DLOCK_NOSYSLOCK ) )	return;

		int swidth = rerect.Pitch / m_pTexture[i].PixelFormatToBitCount( Pixelformat ) ;
		int sheight = min( m_Size , height - ( (i / Mulx) * m_Size) );
		DWORD* Lockdata = (DWORD*) rerect.pBits;
		WORD* pBuf = (WORD*) pdta;

		int lineno = 0;
		for(int nY = 0 ; nY < sheight ; nY++ )
		{
			if( !reverse ) lineno = sheight - ( nY + 1 );
			else lineno = nY;

			for(int  nX = 0 ; nX < swidth ; nX++ )
			{
				if( width > ( i % Mulx )* m_Size + nX)
				{
					Lockdata[swidth * lineno + nX] = A8R8G8B8TOA8R8G8B8( pBuf[count] , pAlpha[count]);	
					count++;
				}
				else
				{
					//SetAlpha
					Lockdata[swidth * lineno + nX] = 0;
				}
			}
			count = (( (i / Mulx) * Surfacewidth * m_Size) + (( i % Mulx )* m_Size)) + Surfacewidth * ( nY + 1 );
		}

		m_pTexture[i].m_Texture->UnlockRect(0);
	}	

}
void CPlane::SetPlaneData_A1R5G5B5(int width, int height , D3DFORMAT Pixelformat, unsigned char* pdataBuf,WORD colorkey,bool reverse ,int surfacewidth )
{
	int Mulx = FloatRoundUpInt( width  / (float)m_Size ); 
	int Muly = FloatRoundUpInt( height / (float)m_Size );  

	WORD* pBuf = (WORD*) pdataBuf;
	DWORD  count = 0;

	for( int i = 0 ; i< m_pTextureNum ; i++ )
	{
		count = (( (i / Mulx) * surfacewidth * m_Size) + (( i % Mulx )* m_Size));// * m_pTexture[i].PixelFormatToBitCount( Pixelformat );

		D3DLOCKED_RECT rerect;
		if( D3DERR_INVALIDCALL == m_pTexture[i].m_Texture->LockRect( 0 , &rerect , NULL , D3DLOCK_NOSYSLOCK ) )	return;

		int swidth = rerect.Pitch / m_pTexture[i].PixelFormatToBitCount( Pixelformat ) ;

		int sheight =  min( m_Size , height - ( (i / Mulx) * m_Size) );

		WORD* Lockdata = (WORD*) rerect.pBits;
		WORD* pBuf = (WORD*) pdataBuf;

		int lineno = 0;
		for(int nY = 0 ; nY < sheight ; nY++ )
		{
			if( !reverse ) lineno = m_Size - ( nY + 1 );
			else lineno = nY;

			for(int  nX = 0 ; nX < swidth ; nX++ )
			{
				if( width  > ( i % Mulx )* m_Size + nX)
				{
					Lockdata[swidth * lineno + nX] = pBuf[count];
					if( pBuf[count] == colorkey)
						Lockdata[swidth * lineno + nX] = 0x0000;				
					else						
						Lockdata[swidth * lineno + nX] |= 0x8000;

					count++;
				}
				else
				{
					Lockdata[swidth * lineno + nX] = 0x0000;
				}
			}
			count = (( (i / Mulx) * surfacewidth * m_Size) + (( i % Mulx )* m_Size)) + surfacewidth * ( nY + 1 );
		}

		m_pTexture[i].m_Texture->UnlockRect(0);
	}	
}


void CD3DXSprite::Load()
{
	Load((char *)m_strDataTaskFileName.c_str());
}
void CD3DXSprite::Delete()
{
	Remove();
}
