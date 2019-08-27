

#pragma once

#include "GraphicDXDefine.h"
#include "D3DXSprite.h"


#define MAX_POINT_RENDER_DATA	30
#define MAX_POINT_UNIT			1000


////////////////////////////////////////////////////////////////////////////////////////
// 
typedef struct
{
	float		fRenderPosX;
	float		fRenderPosY;
	float		fRenderPosZ;

	float		fSize;

	DWORD		dwColor;	
} stPOINT_RENDER_UNIT;


typedef struct
{
	stPOINT_RENDER_UNIT		stPointRenderUnit[MAX_POINT_UNIT];
	int						iPointRenderUnitCount;

	LPDIRECT3DTEXTURE9		cllpPointTexture;

} stPOINT_RENDER_DATA;




class NMGRAPHICDX_CLASS CPointRender
{
public :	
	void							CreatePointRender();
	void							RenderPointRender();
	void							AddPointRender(float fRenderPosX, float fRenderPosY, float fRenderPosZ, DWORD dwColor, LPDIRECT3DTEXTURE9	cllpRenderTexture, float fSize);

	bool							m_bPointRenderFlag;
public :
protected :
protected :
	stPOINT_RENDER_DATA				m_stPointRenderData[MAX_POINT_RENDER_DATA];
	int								m_iPointRenderDataCount;

	LPDIRECT3DVERTEXBUFFER9			m_pPointVB;

	struct stPOINT_VECTOR
	{
		D3DXVECTOR3 posit;
		D3DCOLOR    color;

		enum FVF
		{
			FVF_Flags = D3DFVF_XYZ | D3DFVF_DIFFUSE
		};
	};

public :
	CPointRender();
	~CPointRender();
};
