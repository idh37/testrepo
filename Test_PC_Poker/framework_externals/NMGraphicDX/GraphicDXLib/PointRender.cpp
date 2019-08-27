#include "stdafx.h"
#include <assert.h>
#include "PointRender.h"
#include "Global.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }


CPointRender::CPointRender()
{

	ZeroMemory(m_stPointRenderData, sizeof(stPOINT_RENDER_DATA) * MAX_POINT_RENDER_DATA);
	m_iPointRenderDataCount = 0;



	m_pPointVB = NULL;

	m_bPointRenderFlag = false;


}

CPointRender::~CPointRender()
{
	if( m_pPointVB != NULL )
	{
		m_pPointVB->Release();
		m_pPointVB = NULL;
	}

}

//-----------------------------------------------------------------------------
// Name: CreatePointRender()
// Desc: 
//-----------------------------------------------------------------------------
void CPointRender::CreatePointRender()
{
	if( m_pPointVB != NULL )
	{
		m_pPointVB->Release();
		m_pPointVB = NULL;
	}


	if( FAILED( g_clDirectX.GetDevice()->CreateVertexBuffer( 
		MAX_POINT_UNIT * sizeof(stPOINT_VECTOR), 
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS, 
		stPOINT_VECTOR::FVF_Flags, // Our custom FVF
		D3DPOOL_SYSTEMMEM, 
		&m_pPointVB, NULL )))
	{
		return ;
	}

}

//-----------------------------------------------------------------------------
// Name: RenderPointRender()
// Desc: 
//-----------------------------------------------------------------------------
void CPointRender::RenderPointRender()
{
	if( g_clDirectX.GetDevice() == NULL ) return; 
	if( m_iPointRenderDataCount <= 0 ) return ;

	g_clDirectX.SetAlphaBlend(6);



	g_clDirectX.SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

	g_clDirectX.SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );













	g_clDirectX.SetRenderState( D3DRS_POINTSPRITEENABLE, TRUE );       // Turn on point sprites	

	//g_clDirectX.SetRenderState( D3DRS_POINTSCALEENABLE,  TRUE );       // Allow sprites to be scaled with distance
	//g_clDirectX.SetRenderState( D3DRS_POINTSIZE,     FtoDW(fTempSize) ); // Float value that specifies the size to use for point size computation in cases where point size is not specified for each vertex.
	//g_clDirectX.SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW(1.0f) );    // Float value that specifies the minimum size of point primitives. Point primitives are clamped to this size during rendering. 
	//g_clDirectX.SetRenderState( D3DRS_POINTSCALE_A,  FtoDW(1.0f) );    // Default 1.0
	//g_clDirectX.SetRenderState( D3DRS_POINTSCALE_B,  FtoDW(1.0f) );    // Default 0.0
	//g_clDirectX.SetRenderState( D3DRS_POINTSCALE_C,  FtoDW(1.0f) );    // Default 0.0


	stPOINT_VECTOR *pVertices;


	for(int i = 0 ; i < m_iPointRenderDataCount ; i++ )
	{
		if( m_stPointRenderData[i].cllpPointTexture == NULL ) continue ;


		int iPointNum = m_stPointRenderData[i].iPointRenderUnitCount;


		g_clDirectX.SetTexture( 0, m_stPointRenderData[i].cllpPointTexture );

		if( FAILED( m_pPointVB->Lock( 0,
			iPointNum * sizeof(stPOINT_VECTOR),   
			(void**) &pVertices, 
			0)))
		{
			return ;
		}





		float fTempSize = m_stPointRenderData[i].stPointRenderUnit[0].fSize + 0.7f;


		g_clDirectX.SetRenderState( D3DRS_POINTSIZE,		FtoDW(fTempSize) );
		g_clDirectX.SetRenderState( D3DRS_POINTSIZE_MIN,     FtoDW(fTempSize) );
		g_clDirectX.SetRenderState( D3DRS_POINTSIZE_MAX,     FtoDW(fTempSize) );


		for( int j = 0 ; j < iPointNum ; j++ )
		{
			pVertices->color   = m_stPointRenderData[i].stPointRenderUnit[j].dwColor;
			pVertices->posit.x = m_stPointRenderData[i].stPointRenderUnit[j].fRenderPosX + (fTempSize / 2.0f) - 1;
			pVertices->posit.y = m_stPointRenderData[i].stPointRenderUnit[j].fRenderPosY + (fTempSize / 2.0f) - 1;
			pVertices->posit.z = m_stPointRenderData[i].stPointRenderUnit[j].fRenderPosZ;

			pVertices++;

		}



		m_pPointVB->Unlock();

		g_clDirectX.GetDevice()->SetStreamSource( 0, m_pPointVB, 0, sizeof(stPOINT_VECTOR) );
		g_clDirectX.GetDevice()->SetFVF( stPOINT_VECTOR::FVF_Flags );

		if(FAILED(g_clDirectX.GetDevice()->DrawPrimitive( D3DPT_POINTLIST, 0, 
			iPointNum )))
		{

		}
	}

	g_clDirectX.m_iDrawVertexCount++;


	g_clDirectX.SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
	g_clDirectX.SetRenderState( D3DRS_POINTSCALEENABLE,  FALSE );

	g_clDirectX.SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	g_clDirectX.SetAlphaBlend(0);

	ZeroMemory(m_stPointRenderData, sizeof(stPOINT_RENDER_DATA) * MAX_POINT_RENDER_DATA);
	m_iPointRenderDataCount = 0;
}
//-----------------------------------------------------------------------------
// Name: AddPointRender()
// Desc: 
//-----------------------------------------------------------------------------
void CPointRender::AddPointRender(float fRenderPosX, float fRenderPosY, float fRenderPosZ, DWORD dwColor, LPDIRECT3DTEXTURE9	cllpRenderTexture, float fSize)
{


	for(int i = 0 ; i < MAX_POINT_RENDER_DATA ; i++ )
	{
		if( m_stPointRenderData[i].cllpPointTexture != NULL )
		{
			if( m_stPointRenderData[i].cllpPointTexture == cllpRenderTexture )
			{
				int iTemp = m_stPointRenderData[i].iPointRenderUnitCount;

				m_stPointRenderData[i].stPointRenderUnit[iTemp].fRenderPosX = fRenderPosX;
				m_stPointRenderData[i].stPointRenderUnit[iTemp].fRenderPosY = fRenderPosY;
				m_stPointRenderData[i].stPointRenderUnit[iTemp].fRenderPosZ = fRenderPosZ;
				m_stPointRenderData[i].stPointRenderUnit[iTemp].fSize		= fSize;
				m_stPointRenderData[i].stPointRenderUnit[iTemp].dwColor		= dwColor;


				m_stPointRenderData[i].iPointRenderUnitCount++;
				assert( m_stPointRenderData[i].iPointRenderUnitCount < MAX_POINT_UNIT );

				break ;
			}
			else
			{
				continue ;
			}
		}
		else
		{
			m_stPointRenderData[i].cllpPointTexture = cllpRenderTexture;
			m_stPointRenderData[i].stPointRenderUnit[0].fRenderPosX = fRenderPosX;
			m_stPointRenderData[i].stPointRenderUnit[0].fRenderPosY = fRenderPosY;
			m_stPointRenderData[i].stPointRenderUnit[0].fRenderPosZ = fRenderPosZ;
			m_stPointRenderData[i].stPointRenderUnit[0].fSize		= fSize;
			m_stPointRenderData[i].stPointRenderUnit[0].dwColor		= dwColor;

			m_stPointRenderData[i].iPointRenderUnitCount++;

			m_iPointRenderDataCount++;
			assert( m_iPointRenderDataCount < MAX_POINT_RENDER_DATA );
			break ;
		}
	}
}
