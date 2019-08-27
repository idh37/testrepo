// DirectX.cpp: implementation of the CDirectX class.
//
//////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
//#include <afxwin.h>

#include "stdafx.h"
#include "DirectX.h"
#include "dxgraphic.h"



//#include "GameDefine.h"
//#include "..\\ClientDlg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_DIFFUSE;
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDirectX* pThisClass;

CDirectX::CDirectX()
{

	pThisClass = this;

	_INIT(pParent);
	LastError = 0;

	_INIT(p3D9);
	_INIT(p3D9Device);

	pEffect = NULL;
	pDecl = NULL;
	ClearColor = 0x00000000;

	bUseZ = true;
	bUseStencil = false;
	bPreLight = false;

	bFogOn = false;

	bSpecOn = false;

	LastBlend = -1;

	GierDegree = 0.0f;

	::CoInitialize(NULL);
	//	Teapot = NULL;

	m_iMAX_TEXTURE_SIZE_X		= 1024;
	m_iMAX_TEXTURE_SIZE_Y		= 1024;

	m_iMAX_TEXTURE_SIZE			= 1024;

	m_iDrawVertexCount = 0;

	m_pTestVertexBuffer = NULL;



}

CDirectX::~CDirectX()
{

	if( m_pTestVertexBuffer != NULL )
	{
		m_pTestVertexBuffer->Release();
		m_pTestVertexBuffer = NULL;
	}

	Close();

	::CoUninitialize();

}
void CDirectX::MoveCameraEyepoint(float m, bool bHeight)
{
	if(m == 0.0f)
		return;

	TriB = Cam.Targetpoint.x - Cam.Eyepoint.x;
	TriA = Cam.Eyepoint.z - Cam.Targetpoint.z;
	TriC = sqrt(TriB * TriB + TriA * TriA);
	Alphasin = TriA / TriC;
	Alphacos = TriB / TriC;

	if(bHeight)
	{
		TriB = TriC;
		TriA = Cam.Eyepoint.y - Cam.Targetpoint.y;
		TriC = sqrt(TriA * TriA + TriB * TriB);
		Betasin = TriA / TriC;
		Betacos = TriB / TriC;

		nx = ax = m;
		ny = ay = 0.0f;

		nx = ax * Betasin - ay * Betacos;
		ny = ax * Betacos + ay * Betasin;

		Cam.Eyepoint.y -= nx;
	}

	nx = ax = 0.0f;
	nz = az = m;

	nx = ax * Alphasin - az * Alphacos;
	nz = ax * Alphacos + az * Alphasin;

	Cam.Eyepoint.x -= nx;
	Cam.Eyepoint.z -= nz;
}

void CDirectX::CloseDirect3D()
{
	ReleaseEffect();
	if( p3D9 )
	{	
		int count = 0;
		_RELEASE(p3D9Device);
		_RELEASE(p3D9);
	}					
}
void    CDirectX::ReleaseEffect()
{
	if(pEffect)
		_RELEASE(pEffect);
	if(pDecl)
		_RELEASE(pDecl);
}
void CDirectX::ResetState()
{
	ZeroMemory( m_bActivationFlagsForRenderStates , sizeof(m_bActivationFlagsForRenderStates) );
	ZeroMemory( m_nRenderStates , sizeof(m_nRenderStates) );
	ZeroMemory( m_bActivationFlagsForTextures , sizeof(m_bActivationFlagsForTextures) );
	ZeroMemory( m_bActivationFlagsForTextureStageStates , sizeof(m_bActivationFlagsForTextureStageStates) );
	ZeroMemory( m_nTextureStageStates , sizeof(m_nTextureStageStates) );
	ZeroMemory( m_bActivationFlagsForSamplerStates , sizeof(m_bActivationFlagsForSamplerStates) );
	ZeroMemory( m_nSamplerStates , sizeof(m_nSamplerStates) );
	ZeroMemory( m_pTextures , sizeof(m_pTextures) );
}


HRESULT CDirectX::CreateEffect(LPCSTR filename)
{
	LPD3DXBUFFER  pErrors;
	HRESULT hr;

	if(FAILED(hr = D3DXCreateEffectFromResource(        
		p3D9Device,
		NULL,
		filename,
		NULL,
		NULL,
		NULL,
		NULL,
		&pEffect,
		&pErrors)))
	{
		return E_FAIL;
	}
	D3DVERTEXELEMENT9 decl[5]=
	{
		{ 0,  0,     D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12,     D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,   D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24,   D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,    D3DDECLUSAGE_COLOR, 0 },
		{ 0, 28,     D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};


	GetDevice()->CreateVertexDeclaration(decl, &pDecl); 
	GetDevice()->SetVertexDeclaration(pDecl); 

	return S_OK;
}
bool CDirectX::SnapshotBackbuffer(string File)
{
	if(!p3D9Device)
		return false;

	LPDIRECT3DSURFACE9 BB = GetBackBuffer();

	if(!BB)
		return false;

	Result = D3DXSaveSurfaceToFile(File.c_str(), D3DXIFF_BMP, BB, NULL, NULL);

	if(Result != D3D_OK)
		return false;

	return true;
}
bool CDirectX::InitBumpMap(TEXTURE_ Surface, TEXTURE_* BumpSurface)
{
	if(!p3D9Device)
		return false;

	TEXTURE_ m_psBumpMap = *BumpSurface;
	TEXTURE_ psBumpSrc = Surface;
	D3DSURFACE_DESC    d3dsd;
	D3DLOCKED_RECT     d3dlr;

	psBumpSrc->GetLevelDesc( 0, &d3dsd );
	if( FAILED( p3D9Device->CreateTexture( d3dsd.Width, d3dsd.Height, 1, 0,
		Bumpformat, D3DPOOL_MANAGED, &m_psBumpMap, NULL ) ) )
	{
		return false;
	}

	psBumpSrc->LockRect( 0, &d3dlr, 0, 0 );
	DWORD dwSrcPitch = (DWORD)d3dlr.Pitch;
	BYTE* pSrcTopRow = (BYTE*)d3dlr.pBits;
	BYTE* pSrcCurRow = pSrcTopRow;
	BYTE* pSrcBotRow = pSrcTopRow + (dwSrcPitch * (d3dsd.Height - 1) );

	m_psBumpMap->LockRect( 0, &d3dlr, 0, 0 );
	DWORD dwDstPitch = (DWORD)d3dlr.Pitch;
	BYTE* pDstTopRow = (BYTE*)d3dlr.pBits;
	BYTE* pDstCurRow = pDstTopRow;

	for( DWORD y=0; y<d3dsd.Height; y++ )
	{
		BYTE* pSrcB0;
		BYTE* pSrcB1;
		BYTE* pSrcB2;
		BYTE* pDstT;

		pSrcB0 = pSrcCurRow;

		if( y == d3dsd.Height - 1)
			pSrcB1 = pSrcTopRow;
		else
			pSrcB1 = pSrcCurRow + dwSrcPitch;

		if( y == 0 )
			pSrcB2 = pSrcBotRow;
		else
			pSrcB2 = pSrcCurRow - dwSrcPitch;

		pDstT = pDstCurRow;

		for( DWORD x=0; x<d3dsd.Width; x++ )
		{
			LONG v00;
			LONG v01;
			LONG vM1;
			LONG v10;
			LONG v1M;

			v00 = *(pSrcB0+0);

			if( x == d3dsd.Width - 1 )
				v01 = *(pSrcCurRow);
			else
				v01 = *(pSrcB0+4);

			if( x == 0 )
				vM1 = *(pSrcCurRow + (4 * (d3dsd.Width - 1)));
			else
				vM1 = *(pSrcB0-4);
			v10 = *(pSrcB1+0);
			v1M = *(pSrcB2+0);

			LONG iDu = (vM1-v01);
			LONG iDv = (v1M-v10);

			WORD uL = ( v00>1 ) ? 63 : 127;

			switch( Bumpformat )
			{
			case D3DFMT_V8U8:
				*pDstT++ = (BYTE)iDu;
				*pDstT++ = (BYTE)iDv;
				break;

			case D3DFMT_L6V5U5:
				*(WORD*)pDstT  = (WORD)( ( (iDu>>3) & 0x1f ) <<  0 );
				*(WORD*)pDstT |= (WORD)( ( (iDv>>3) & 0x1f ) <<  5 );
				*(WORD*)pDstT |= (WORD)( ( ( uL>>2) & 0x3f ) << 10 );
				pDstT += 2;
				break;

			case D3DFMT_X8L8V8U8:
				*pDstT++ = (BYTE)iDu;
				*pDstT++ = (BYTE)iDv;
				*pDstT++ = (BYTE)uL;
				*pDstT++ = (BYTE)0L;
				break;
			}

			pSrcB0+=4;
			pSrcB1+=4;
			pSrcB2+=4;
		}

		pSrcCurRow += dwSrcPitch;
		pDstCurRow += dwDstPitch;
	}

	m_psBumpMap->UnlockRect(0);
	psBumpSrc->UnlockRect(0);

	*BumpSurface = m_psBumpMap;

	return true;
}
void CDirectX::MoveCamera(float m, bool bHeight )
{
	if(m == 0.0f)
		return;

	TriB = Cam.Targetpoint.x - Cam.Eyepoint.x;
	TriA = Cam.Eyepoint.z - Cam.Targetpoint.z;
	TriC = sqrt(TriB * TriB + TriA * TriA);
	Alphasin = TriA / TriC;
	Alphacos = TriB / TriC;

	if(bHeight)
	{
		TriB = TriC;
		TriA = Cam.Eyepoint.y - Cam.Targetpoint.y;
		TriC = sqrt(TriA * TriA + TriB * TriB);
		Betasin = TriA / TriC;
		Betacos = TriB / TriC;

		nx = ax = m;
		ny = ay = 0.0f;

		nx = ax * Betasin - ay * Betacos;
		ny = ax * Betacos + ay * Betasin;

		Cam.Eyepoint.y -= nx;
		Cam.Targetpoint.y -= nx;
	}

	nx = ax = 0.0f;
	nz = az = m;

	nx = ax * Alphasin - az * Alphacos;
	nz = ax * Alphacos + az * Alphasin;

	Cam.Eyepoint.x -= nx;
	Cam.Eyepoint.z -= nz;
	Cam.Targetpoint.x -= nx;
	Cam.Targetpoint.z -= nz;
}
void CDirectX::GierCamera(double Degree)
{
	if(Degree == 0.0f && GierDegree == 0.0f)
		return;

	GierDegree = Degree;

	TriB = Cam.Targetpoint.x - Cam.Eyepoint.x;
	TriA = Cam.Eyepoint.z - Cam.Targetpoint.z;
	TriC = sqrt(TriB * TriB + TriA * TriA);

	Alphasin = TriA / TriC;
	Alphacos = TriB / TriC;

	nx = ax = 0.0f;
	ny = ay = 1.0f;
	nz = az = 0.0f;

	Angle = ((GierDegree+90)	* __GRAD__);
	Gammasin = sin(Angle);
	Gammacos = cos(Angle);

	nx = ax * Gammasin - ay * Gammacos;
	ny = ax * Gammacos + ay * Gammasin;

	ax = nx;
	ay = ny;

	nx = ax * Alphasin - az * Alphacos;
	nz = ax * Alphacos + az * Alphasin;

	ax = nx;
	az = nz;

	Cam.Upvector.SetVector((float)nx,(float)ny,(float)nz);
}
void CDirectX::InclineCameraTarget(double Degree)
{
	ax = Cam.Targetpoint.y - Cam.Eyepoint.y;
	ay = Cam.Targetpoint.z - Cam.Eyepoint.z;

	Angle = ((Degree+90)	* __GRAD__);

	Asin = sin(Angle);
	Acos = cos(Angle);

	nx = ax * Asin - ay * Acos;
	ny = ax * Acos + ay * Asin;

	ax -= nx;
	ay -= ny;

	Cam.Targetpoint.y -= ax;
	Cam.Targetpoint.z -= ay;

	GierCamera(GierDegree);
}
void CDirectX::InclineCamera(double Degree)
{
	ax = Cam.Eyepoint.y - Cam.Targetpoint.y;
	ay = Cam.Eyepoint.z - Cam.Targetpoint.z;

	Angle = ((Degree+90.0f)	* __GRAD__);

	Asin = sin(Angle);
	Acos = cos(Angle);

	nx = ax * Asin - ay * Acos;
	ny = ax * Acos + ay * Asin;

	ax -= nx;
	ay -= ny;

	Cam.Eyepoint.y += ax;
	Cam.Eyepoint.z += ay;

	GierCamera(GierDegree);
}
void CDirectX::RotateCameraTarget(double Degree)
{
	ax = Cam.Targetpoint.x - Cam.Eyepoint.x;
	ay = Cam.Targetpoint.z - Cam.Eyepoint.z;

	Angle = ((Degree+90)	* __GRAD__);

	Asin = sin(Angle);
	Acos = cos(Angle);

	nx = ax * Asin - ay * Acos;
	ny = ax * Acos + ay * Asin;

	nx -= ax;
	ny -= ay;

	Cam.Targetpoint.x += nx;
	Cam.Targetpoint.z += ny;

	GierCamera(GierDegree);
}
void CDirectX::RotateCamera(double Degree)
{
	ax = Cam.Eyepoint.x - Cam.Targetpoint.x;
	ay = Cam.Eyepoint.z - Cam.Targetpoint.z;

	Angle = ((Degree+90.0f)	* __GRAD__);

	Asin = sin(Angle);
	Acos = cos(Angle);

	nx = ax * Asin - ay * Acos;
	ny = ax * Acos + ay * Asin;

	nx -= ax;
	ny -= ay;

	Cam.Eyepoint.x += nx;
	Cam.Eyepoint.z += ny;

	GierCamera(GierDegree);
}
void CDirectX::ValidateCamera()
{
	if(p3D9Device)
	{
		D3DXVECTOR3 vEyePt( (float)Cam.Eyepoint.x, (float)Cam.Eyepoint.y,(float)Cam.Eyepoint.z );
		D3DXVECTOR3 vLookatPt( (float)Cam.Targetpoint.x, (float)Cam.Targetpoint.y, (float)Cam.Targetpoint.z );
		D3DXVECTOR3 vUpVec( (float)Cam.Upvector.x, (float)Cam.Upvector.y, (float)Cam.Upvector.z );
		D3DXMATRIXA16 matView;
		D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
		p3D9Device->SetTransform( D3DTS_VIEW, &matView );
	}
}
void CDirectX::MoveCameraStatic(float x, float y, float z)
{
	MoveCamera(x,y,z);
	MoveCameraTarget(x,y,z);
}
void CDirectX::MoveCameraTarget(float x, float y, float z)
{
	Cam.Targetpoint.x += x;
	Cam.Targetpoint.y += y;
	Cam.Targetpoint.z += z;
}
void CDirectX::MoveCamera(float x, float y, float z)
{
	Cam.Eyepoint.x += x;
	Cam.Eyepoint.y += y;
	Cam.Eyepoint.z += z;
}
void CDirectX::SetCamera(KAMERA9 Cam, bool bUpdate )
{
	this->Cam = Cam;

	if(bUpdate)
	{
		ValidateCamera();
	}
}
KAMERA9 CDirectX::GetCamera()
{
	return Cam;
}
void CDirectX::SetUseSpecularLights(bool bOn)
{
	SetMaterialSpec(true);
	EnableSpeclight(bOn);
	EnableNormalisation(bOn);
}
void CDirectX::SetBlendMode(bool bOn)
{
	if(!p3D9Device)
		return;

	if(bOn)
	{
		Result = SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
		Result = SetRenderState( D3DRS_ALPHAREF,        0x00 );
		Result = SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_NOTEQUAL );
	}
	else
	{
		SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	}
}
bool CDirectX::Set3DViewCaps(int Angle, float Aspect, float Close, float Far)
{
	D3DMATRIX proj_m;
	float dAngle = ((float)__PI__/180.0f) * (float)Angle;
	D3DUtil_SetProjectionMatrix(proj_m,dAngle,Aspect,Close,Far);
	p3D9Device->SetTransform(  D3DTS_PROJECTION,&proj_m);
	return true;
}
HRESULT CDirectX::D3DUtil_SetProjectionMatrix( D3DMATRIX& mat, FLOAT fFOV, FLOAT fAspect,FLOAT fNearPlane, FLOAT fFarPlane )
{
	if( fabs(fFarPlane-fNearPlane) < 0.01f )
		return E_INVALIDARG;
	if( fabs(sin(fFOV/2)) < 0.01f )
		return E_INVALIDARG;

	FLOAT w = fAspect * ( cosf(fFOV/2)/sinf(fFOV/2) );
	FLOAT h =   1.0f  * ( cosf(fFOV/2)/sinf(fFOV/2) );
	FLOAT Q = fFarPlane / ( fFarPlane - fNearPlane );

	ZeroMemory( &mat, sizeof(D3DMATRIX) );
	mat._11 = w;
	mat._22 = h;
	mat._33 = Q;
	mat._34 = 1.0f;
	mat._43 = -Q*fNearPlane;

	return S_OK;
}
D3DLIGHT9 CDirectX::GetStandardLight(int Type)
{
	D3DCOLORVALUE Max,Min;
	Max.r = 1.0f;
	Max.g = 1.0f;
	Max.b = 1.0f;
	Max.a = 1.0f;
	Min.r = 0.0f;
	Min.g = 0.0f;
	Min.b = 0.0f;
	Min.a = 0.0f;

	D3DVECTOR Center,To;

	Center.x = 0.0f;
	Center.y = 0.0f;
	Center.z = 0.0f;

	To.x = 0.0f;
	To.y = -1.0f;
	To.z = 0.0f;

	D3DLIGHT9 l;
	ZeroMemory( &l, sizeof(D3DLIGHT9) );

	l.Type = (D3DLIGHTTYPE)Type;
	l.Diffuse = Max;
	l.Specular = Max;
	l.Ambient = Max;
	l.Position = Center;
	l.Direction = To;
	l.Range = 2000.0f;
	l.Falloff = 1.0f;
	l.Attenuation0 = 0.0f;
	l.Attenuation1 = 1.0f;
	l.Attenuation2 = 0.0f;
	l.Theta = (float)(30.0 * __GRAD__);
	l.Phi = (float)(50.0*__GRAD__);

	return l;
}
D3DMATERIAL9 CDirectX::GetStandardMaterial()
{
	D3DMATERIAL9 Mat;
	ZeroMemory( &Mat, sizeof(D3DMATERIAL9) );

	D3DCOLORVALUE Max,Min;
	Max.r = 1.0f;
	Max.g = 1.0f;
	Max.b = 1.0f;
	Max.a = 1.0f;
	Min.r = 0.0f;
	Min.g = 0.0f;
	Min.b = 0.0f;
	Min.a = 0.0f;

	Mat.Diffuse = Max;
	Mat.Ambient = Max;
	Mat.Specular = Max;
	Mat.Emissive = Min;
	Mat.Power = 20.0f;

	return Mat;
}
void CDirectX::SetViewPort(DWORD x,DWORD y,DWORD cx,DWORD cy,float MinZ,float MaxZ)
{
	//	if(p3D9Device)
	//		return;

	ViewPort.X = x;
	ViewPort.Y = y;
	ViewPort.Width = cx;
	ViewPort.Height = cy;
	ViewPort.MinZ = MinZ;
	ViewPort.MaxZ = MaxZ;

	//	p3D9Device->SetViewport(&ViewPort);
}
D3DVIEWPORT9 CDirectX::GetViewPort()
{
	if(p3D9Device)
		return ViewPort;

	p3D9Device->GetViewport(&ViewPort);

	return ViewPort;
}
bool CDirectX::GetFrontbufferCopy(LPDIRECT3DSURFACE9 Surface)
{
	if(!p3D9Device)
	{
		LastError = 5;
		return false;
	}

	Result = p3D9Device->GetFrontBufferData(0,Surface);

	if(Result != D3D_OK)
	{
		LastError = 8;
		return false;
	}

	return true;
}
void CDirectX::FillSurface(string Filename, LPDIRECT3DSURFACE9 Surface)
{
	HBITMAP hbm = (HBITMAP)LoadImage( GetModuleHandle(NULL), Filename.c_str(), 
		IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION );
	if( !hbm )
	{
		hbm = (HBITMAP)LoadImage( NULL, Filename.c_str(), IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE|LR_CREATEDIBSECTION );
		if( !hbm )
			return;
	}

	//	BITMAP  BitMap;
	//	BitMap.
	//	CBitmap Bitmap;
	//	Bitmap.Attach(hbm);


	FillSurface(hbm,Surface);

	//	Bitmap.DeleteObject();
}
void CDirectX::FillSurface(HBITMAP bmp, LPDIRECT3DSURFACE9 Surface)
{
	if(!Surface)
		return;

	BITMAP bm;
	GetObject( bmp , sizeof( BITMAP ) , &bm );


	D3DSURFACE_DESC desc;

	Surface->GetDesc(&desc);

	if(Surface)
	{
		HDC hdc;
		Surface->GetDC(&hdc);

		/*		HDC temphdc = CreateCompatibleDC( hdc );

		HBITMAP* oldbitmap = (HBITMAP*)SelectObject( temphdc , bmp);
		SetStretchBltMode( temphdc , HALFTONE );
		SetStretchBltMode( hdc , HALFTONE );
		StretchBlt( hdc ,0,0,desc.Width,desc.Height,temphdc,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
		*/
		RECT rc;
		if( hdc != NULL )
		{
			SetRect( &rc , 10,110,200,220 );
			SetTextColor( hdc , RGB(0,128,128 ));
			DrawText(hdc , "sadawdrqqwegfqg" ,15 , &rc , DT_CENTER );

			Surface->ReleaseDC(hdc);
			Surface->Release();
		}

		//		SelectObject( temphdc , oldbitmap );
		//		ReleaseDC(NULL ,  temphdc );

	}
	/*
	CDC cdc;
	cdc.Attach(hdc);

	CDC dc;
	dc.CreateCompatibleDC(&cdc);
	CBitmap* pbmp = dc.SelectObject(bmp);

	dc.SetStretchBltMode(HALFTONE);
	cdc.SetStretchBltMode(HALFTONE);

	cdc.StretchBlt(0,0,desc.Width,desc.Height,&dc,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);

	dc.SelectObject(pbmp);
	cdc.Detach();
	Surface->ReleaseDC(hdc);
	}
	*/
}
LPDIRECT3DSURFACE9 CDirectX::CreateSurface(int Width, int Height)
{
	if(!p3D9Device)
	{
		LastError = 5;
		return NULL;
	}

	LPDIRECT3DSURFACE9 Surface = NULL;

	if( TextureFormat == D3DFMT_UNKNOWN )
		TextureFormat = D3DFMT_X1R5G5B5;
	p3D9Device->CreateOffscreenPlainSurface(Width,Height,TextureFormat,D3DPOOL_DEFAULT,&Surface,NULL);

	return Surface;
}
void CDirectX::FillTexture(TEXTURE_ Texture, HBITMAP pBitmap, COLORREF BlendColor )
{
	if(!Texture)
		return;
	/*
	BITMAP bm;
	pBitmap->GetBitmap(&bm);

	D3DSURFACE_DESC desc;

	LPDIRECT3DSURFACE9 Tex = GetTextureSurface(0,Texture);

	Tex->GetDesc(&desc);

	if(Tex)
	{
	HDC hdc;
	Tex->GetDC(&hdc);

	CDC cdc;
	cdc.Attach(hdc);

	CDC dc;
	dc.CreateCompatibleDC(&cdc);
	CBitmap* pbmp = dc.SelectObject(pBitmap);
	dc.SetStretchBltMode(HALFTONE);
	cdc.SetStretchBltMode(HALFTONE);

	cdc.StretchBlt(0,0,desc.Width,desc.Height,&dc,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);

	dc.SelectObject(pbmp);
	cdc.Detach();
	Tex->ReleaseDC(hdc);
	}

	if(BlendColor & 0xff000000 != 0xff000000)
	{
	//MakeAlphaTexture(Texture,BlendColor);
	}
	*/
}
void CDirectX::FillTexture(TEXTURE_ Texture, string Filename, COLORREF BlendColor )
{
	/*
	HBITMAP hbm = (HBITMAP)LoadImage( GetModuleHandle(NULL), Filename, 
	IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION );
	if( !hbm )
	{
	hbm = (HBITMAP)LoadImage( NULL, Filename, IMAGE_BITMAP, 0, 0, 
	LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	if( !hbm )
	return;
	}

	CBitmap Bitmap;
	Bitmap.Attach(hbm);

	FillTexture(Texture, &Bitmap);

	Bitmap.DeleteObject();
	*/
}
TEXTURE_ CDirectX::CreateEmptyTexture(int Width, int Height)
{
	if(!p3D9Device)
	{
		LastError = 5;
		return NULL;
	}

	TEXTURE_ Surface = NULL;
	Result = p3D9Device->CreateTexture(Width,Height,0,0,TextureFormat,D3DPOOL_MANAGED,&Surface,NULL);
	if(Result != D3D_OK)
		return NULL;

	return Surface;
}
TEXTURE_ CDirectX::CreateEmptyTexture(int Width, int Height , UINT level , DWORD usage ,D3DFORMAT format )
{
	if(!p3D9Device)
	{
		LastError = 5;
		return NULL;
	}

	TEXTURE_ Surface = NULL;
	Result = p3D9Device->CreateTexture(Width,Height,level,usage,format,D3DPOOL_MANAGED,&Surface,NULL);
	//if( Result != D3D_OK)
	if( Surface == NULL )
	{
		Result = D3DXCreateTexture(p3D9Device, Width,Height,level,usage,format,D3DPOOL_MANAGED,&Surface);

		int i = 0 ;
		//while( Result == E_OUTOFMEMORY )
		while( Surface == NULL )
		{
			Sleep(1);
			Result = D3DXCreateTexture(p3D9Device, Width,Height,level,usage,format,D3DPOOL_MANAGED,&Surface);
			i++;
			if( i >=  3) break ;


		}
		if( Surface == NULL )
		{
			return NULL;
		}

	}

	return Surface;
}
LPDIRECT3DSURFACE9 CDirectX::GetTextureSurface(int Level, TEXTURE_ Text)
{
	if(!Text)
		return NULL;

	LPDIRECT3DSURFACE9 Surface;

	Result = Text->GetSurfaceLevel(Level,&Surface);

	if(Result != D3D_OK)
		return NULL;

	return Surface;
}
TEXTURE_ CDirectX::CreateTexture(HBITMAP pBitmap, COLORREF BlendColor )
{
	if(!p3D9Device)
	{
		LastError = 5;
		return NULL;
	}
	TEXTURE_ Surface = NULL;

	BITMAP bm;
	//	pBitmap->GetBitmap(&bm);

	GetObject( pBitmap , sizeof( BITMAP ) , &bm );
	Result = D3DXCreateTexture(p3D9Device,bm.bmWidth,bm.bmHeight,1,0,Bits16 ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8,D3DPOOL_MANAGED,&Surface);

	LPDIRECT3DSURFACE9 Tex = GetTextureSurface(0,Surface);
	/*
	if(Tex)
	{
	HDC hdc = NULL;
	Result = Tex->GetDC(&hdc);


	HDC htempdc = CreateCompatibleDC( hdc );
	SelectObject( htempdc ,  )

	CDC cdc;
	cdc.Attach(hdc);

	CDC dc;
	dc.CreateCompatibleDC(&cdc);
	CBitmap* pbmp = dc.SelectObject(pBitmap);

	cdc.BitBlt(0,0,bm.bmWidth,bm.bmHeight,&dc,0,0,SRCCOPY);

	dc.SelectObject(pbmp);
	cdc.Detach();
	Tex->ReleaseDC(hdc);
	}
	*/
	if(!(BlendColor & 0xff000000))
	{
		//MakeAlphaTexture(Surface,BlendColor);
	}

	return Surface;
}
TEXTURE_ CDirectX::CreateTexture(string Filename,  COLORREF BlendColor )
{
	TEXTURE_ Surface = NULL;

	BlendColor = D3DCOLOR_ARGB((BlendColor & 0xff000000) >> 24,BlendColor & 0x000000ff,(BlendColor & 0x0000ff00) >> 8, (BlendColor & 0x00ff0000)  >> 16);

	Result = D3DXCreateTextureFromFileEx(p3D9Device,Filename.c_str(),0,0,1,0,/*D3DFMT_A8R8G8B8*/TextureFormat,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,BlendColor,NULL,NULL,&Surface);

	return Surface;
}
void CDirectX::EnableSmoothFilter(bool bOn)
{
	if(!p3D9Device)
	{
		LastError = 5;
		return;
	}

	if(bOn)
	{
		SetSamplerState( 0, D3DSAMP_MAGFILTER,     D3DTEXF_ANISOTROPIC );
		SetSamplerState( 0, D3DSAMP_MINFILTER,     D3DTEXF_ANISOTROPIC );
		SetSamplerState( 0, D3DSAMP_MAXANISOTROPY, 8 );
	}
	else
	{
		SetSamplerState( 0, D3DSAMP_MAGFILTER,     D3DTEXF_POINT );
		SetSamplerState( 0, D3DSAMP_MINFILTER,     D3DTEXF_POINT );
		SetSamplerState( 0, D3DSAMP_MAXANISOTROPY, 1 );
	}

}
void CDirectX::EnableLineAntiAliasing(bool bOn)
{
	if(!p3D9Device)
	{
		LastError = 5;
		return;
	}

	SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, bOn ? TRUE : FALSE);
}
void CDirectX::SetMaterialSpec(bool bOn)
{
	if(!p3D9Device)
	{
		LastError = 5;
		return;
	}

	SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, bOn ? D3DMCS_MATERIAL : D3DMCS_COLOR2);
}
void CDirectX::EnableNormalisation(bool bOn)
{
	if(!p3D9Device)
	{
		LastError = 5;
		return;
	}

	SetRenderState( D3DRS_NORMALIZENORMALS, bOn ? TRUE : FALSE);
}
void CDirectX::SetAmbientLight(COLORREF Col)
{
	if(!p3D9Device)
	{
		LastError = 5;
		return;
	}

	SetRenderState( D3DRS_AMBIENT, ConvertColor(Col));
}
void CDirectX::SetAlphaBlend(int Mode)
{
	if(!p3D9Device)
	{
		LastError = 5;
		return;
	}

	if(LastBlend == Mode)
		return;

	LastBlend = Mode;

	// 알파 블렌딩 상태를 읽어옴

	if(!Mode) 
	{
		//	if( dwAlphaState != FALSE )           
		SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
		//	if( dwSrcBlend != D3DBLEND_SRCCOLOR)  
		SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCCOLOR);
		//	if( dwDestBlend != D3DBLEND_DESTCOLOR)
		SetRenderState(D3DRS_DESTBLEND,D3DBLEND_DESTCOLOR);

		//	if( LastBlend ==5 || LastBlend ==6)
		{
			SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
			SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		}
	}
	else
	{
		SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);

		if(Mode < 4)  
			SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCCOLOR);
		if(Mode == 1)
			SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCCOLOR);
		else if(Mode == 2) 
			SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
		else if(Mode == 3)
		{
			SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
			SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
		}
		else if(Mode == 4)
		{
			SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO);
			SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

			SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO);
			SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

		}
		else if( Mode == 5)
		{
			SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

			SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
			SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

			SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
			SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );	
		}
		else if( Mode == 6 )
		{
			SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
			SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

			SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
			SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );	

			SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
		}
		else if( Mode == 7 ) //소스컬러와 
		{
			SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCCOLOR);
		}
		else if( Mode == 8 )
		{
			SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
		}
		else if( Mode == 9 )
		{
			SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVDESTCOLOR);
			SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCCOLOR);
		}
		else if( Mode == 10 ) //소스컬러와 
		{
			SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHASAT);
			SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCCOLOR);
		}
		else if( Mode == 12 ) // De
		{
			SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR); //D3DBLEND_SRCCOLOR ); //D3DBLEND_INVDESTCOLOR);//D3DBLEND_DESTCOLOR);
			SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO ) ;// D3DBLEND_SRCCOLOR ); //D3DBLEND_ZERO); 
		}
		else if( Mode == 13 ) // LE
		{
			SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			// 알파비트 없이 라이팅 구현시
			SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
			SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

	}
}
void CDirectX::EnableDithering(bool bOn)
{
	if(!p3D9Device)
	{
		LastError = 5;
		return;
	}

	SetRenderState( D3DRS_DITHERENABLE, bOn ? TRUE : FALSE);
}
void CDirectX::SetShadeMode(UINT Mode)
{
	if(!p3D9Device)
	{
		LastError = 5;
		return;
	}

	SetRenderState( D3DRS_SHADEMODE, Mode);
}
void CDirectX::SetRenderMode(UINT Mode)
{
	if(!p3D9Device)
	{
		LastError = 5;
		return;
	}

	SetRenderState( D3DRS_FILLMODE, Mode);
}
bool CDirectX::EnableSpeclight(bool bOn)
{
	if(!p3D9Device)
	{
		LastError = 5;
		return false;
	}

	bSpecOn = bOn;

	SetRenderState( D3DRS_SPECULARENABLE, bOn ? TRUE : FALSE);

	return true;
}
bool CDirectX::GetSpecEnabled()
{
	return bSpecOn;
}
bool CDirectX::GetFogEnabled()
{
	return bFogOn;
}
bool CDirectX::SetFog(bool bOn, float FogBegin, float FogEnd , COLORREF FogColor)
{
	if(!p3D9Device)
	{
		LastError = 5;
		return false;
	}

	bFogOn = bOn;

	FogColor = ConvertColor(FogColor);

	SetRenderState( D3DRS_FOGENABLE, bOn ? TRUE : FALSE);
	SetRenderState( D3DRS_FOGCOLOR, (DWORD)FogColor);
	SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_LINEAR );
	SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_LINEAR );
	SetRenderState( D3DRS_FOGSTART, *((DWORD *)(&FogBegin)) );
	SetRenderState( D3DRS_FOGEND,   *((DWORD *)(&FogEnd)) );

	return true;
}
bool CDirectX::SetScullMode(int CullMode)
{
	if(!p3D9Device)
	{
		LastError = 5;
		return false;
	}

	SetRenderState(D3DRS_CULLMODE,(DWORD)CullMode);	// vorbeleuchtung aktivieren

	return true;
}
bool CDirectX::GetPrelight()
{
	return bPreLight;
}
bool CDirectX::PreLight3D(bool bPrelight)
{
	if(!p3D9Device)
	{
		LastError = 5;
		return false;
	}

	bPreLight = bPrelight;

	SetRenderState(D3DRS_LIGHTING,bPrelight );	// vorbeleuchtung aktivieren

	return true;
}
/*
bool CDirectX::Set3DViewCaps(int _Angle , float _Close , float _Far , float _Aspect)
{
if(!p3D9Device)
{
LastError = 5;
return false;
}

float dAngle = ((float)__PI__/180.0f) * (float)_Angle;

return true;
}*/
bool CDirectX::ShowCursor(bool bShow)
{
	if(!p3D9Device)
	{
		LastError = 5;
		return true;
	}

	BOOL Result = p3D9Device->ShowCursor(bShow ? TRUE : FALSE);

	return (Result == TRUE);
}
bool CDirectX::SetLight(int Index, D3DLIGHT9 Light)
{
	this->Light = Light;

	if(!p3D9Device)
		return false;

	Result = p3D9Device->SetLight(Index,&this->Light);

	if(Result != D3D_OK)
		return false;

	return true;
}
void CDirectX::EnableDialogBoxes(bool bOn)
{
	if(!p3D9Device)
		return;

	Result = p3D9Device->SetDialogBoxMode(bOn ? TRUE : FALSE);
}
void CDirectX::SetCustomCursor(int SpotX,int SpotY, LPDIRECT3DSURFACE9 Cursor)
{
	if(!p3D9Device)
		return;

	Result = p3D9Device->SetCursorProperties(SpotX,SpotY,Cursor);
}
void CDirectX::SetCursorPos(int x, int y)
{
	if(!p3D9Device)
		return;

	p3D9Device->SetCursorPosition(x,y, D3DCURSOR_IMMEDIATE_UPDATE);
}
void CDirectX::EnableLight(int Index, bool bOn)
{
	if(!p3D9Device)
		return;

	Result = p3D9Device->LightEnable(Index, bOn ? TRUE : FALSE);
}
D3DMATERIAL9 CDirectX::GetMaterial()
{
	return GetStandardMaterial();
}
bool CDirectX::IsLightEnabled(int Index)
{
	if(!p3D9Device)
		return false;

	BOOL ON;

	Result = p3D9Device->GetLightEnable(Index,&ON);

	if(Result != D3D_OK)
		return false;

	return (ON != 0);
}
D3DLIGHT9* CDirectX::GetLight(int Index)
{
	if(!p3D9Device)
		return NULL;

	Result = p3D9Device->GetLight(Index, & Light);

	if(Result != D3D_OK)
		return NULL;

	return & Light;
}
LPDIRECT3DSURFACE9 CDirectX::GetZBuffer()
{
	if(!p3D9Device)
		return NULL;

	LPDIRECT3DSURFACE9 Surface = NULL;

	Result = p3D9Device->GetDepthStencilSurface(&Surface);

	if(Result != D3D_OK)
		return NULL;

	return Surface;
}
int CDirectX::GetTextureMemoryMB()
{
	if(!p3D9Device)
		return -1;

	return (int)p3D9Device->GetAvailableTextureMem();
}
LPDIRECT3DSURFACE9 CDirectX::GetBackBuffer()
{
	if(!p3D9Device)
		return NULL;

	LPDIRECT3DSURFACE9 Surface = NULL;

	Result = p3D9Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&Surface);

	if(Result != D3D_OK)
		return NULL;

	return Surface;
}
void CDirectX::EnableZBuffer(bool bEnable)
{
	if(!p3D9Device) return;

	bUseZ = bEnable;

	if(bEnable) 
	{
		SetRenderState( D3DRS_ZENABLE, TRUE );
	}
	else 
	{
		SetRenderState( D3DRS_ZENABLE, FALSE );
	}
}
bool CDirectX::Flip()
{
	if(!p3D9Device)
	{
		LastError = 5;
		return false;
	}

	Result = p3D9Device->Present(NULL,NULL,NULL,NULL);
	if(Result != D3D_OK)
	{
		LastError = 7;
		return false;
	}

	return true;
}
D3DCOLOR CDirectX::ConvertColor(COLORREF Col)
{
	D3DCOLOR dcol;
	dcol = D3DCOLOR_ARGB(0,Col & 0x000000ff,(Col & 0x0000ff00) >> 8, (Col & 0x00ff0000)  >> 16);
	return dcol;

}
LPDIRECT3DDEVICE9 CDirectX::GetDevice()
{
	return p3D9Device;
}
void CDirectX::SetClearColor(COLORREF Color)
{
	ClearColor = Color;
	_3DClearColor = ConvertColor(ClearColor) ;
}
COLORREF CDirectX::GetClearColor()
{
	return ClearColor;
}
bool CDirectX::Render(bool bStartScene)
{
	if(!p3D9Device)
	{
		LastError = 5;
		return false;
	}

	if(bStartScene)
	{
		UINT Flags = D3DCLEAR_TARGET;
		if(bUseZ)
			Flags |= D3DCLEAR_ZBUFFER;
		if(bUseStencil)
			Flags |= D3DCLEAR_STENCIL;
		if( GetShaderEnable() )			pEffect->SetMatrix("matWorldViewProj",&m_matWorldViewProj);
		Result = p3D9Device->BeginScene();
	}
	else
	{
		Result = p3D9Device->EndScene();
	}

	if(Result != D3D_OK)
	{
		//		OutputDebugString("Render Error\n ");
		LastError = 6;
		return false;
	}

	return true;
}
int CDirectX::Get3DMode()
{
	return Mode;
}
bool CDirectX::InitDirect3D(int Width, int Height, int BPP, int BufferCount, HWND TargetWindow, UINT Refreshrate)
{
	//	if(p3D9)
	//		return false;

	Bits16 = (BPP == 16);

	Bumpformat = Bits16 ? D3DFMT_L6V5U5 : D3DFMT_X8L8V8U8;

	if(!pParent)
	{
		LastError = 1;
		return false;
	}
	if(!p3D9)
	{
		p3D9 = Direct3DCreate9(D3D_SDK_VERSION);
		if(!p3D9)
		{
			LastError = 2;
			AfxMessageBox("다이렉트 3D 초기화에 실패하였습니다. 최신 버전의 다이렉트X를 설치하시기 바랍니다.");

#ifdef _REPORT 	
			AfxMessageBox("Direct3DCreate9(D3D_SDK_VERSION)");
#endif
			return false;
		}

	}

	if(BPP != 16 && BPP != 32)
	{
		LastError = 3;
#ifdef _REPORT 	
		AfxMessageBox("(BPP != 16 && BPP != 32)");
#endif
		return false;
	}

	ZeroMemory(&d3dpp,sizeof(d3dpp));
	d3dpp.Windowed = (TargetWindow != NULL) ? TRUE : FALSE;
	if(d3dpp.Windowed)
		d3dpp.FullScreen_RefreshRateInHz = 0;
	else
		d3dpp.FullScreen_RefreshRateInHz = Refreshrate;

	d3dpp.BackBufferWidth = Width;
	d3dpp.BackBufferHeight = Height;
	d3dpp.BackBufferFormat = (BPP == 16) ? D3DFMT_R5G6B5 : D3DFMT_A8R8G8B8;
	if(d3dpp.Windowed)
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	TextureFormat = d3dpp.BackBufferFormat;
	d3dpp.BackBufferCount = BufferCount;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.EnableAutoDepthStencil = FALSE;
	//d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// 종환 임시주석
	/*if( g_pMainDlg->CFG.iGraphicOptionKind == GRAPHIC_OPTION_LOW)
	{
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}*/




	Mode = D3DM_PURE_HARDWARE;
	int count = 0;



	if(FAILED(p3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, pParent, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &p3D9Device)))
	{
		//Mode = D3DM_MMX_3DNOW;
		if(FAILED(p3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, pParent, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &p3D9Device)))
		{

			return false ;

			Mode = D3DM_NONE;
			if(FAILED(p3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_SW, pParent, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &p3D9Device)))
			{

				//다른기반으로 생성
				ZeroMemory( &d3dpp, sizeof(d3dpp) );
				d3dpp.Windowed = TRUE;
				d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
				d3dpp.BackBufferFormat = (BPP == 16) ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8;
				if(d3dpp.Windowed)
					d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
				d3dpp.BackBufferCount = 1;
				d3dpp.BackBufferWidth = Width;
				d3dpp.BackBufferHeight = Height;
				d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
				d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
				d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

				HRESULT HR =  p3D9->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,  pParent,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &p3D9Device );
				if( FAILED( HR ) )
				{
#ifdef _REPORT 	
					if( HR ==	D3DERR_INVALIDCALL) AfxMessageBox("메서드의 호출이 무효이다. 예를 들어, 메서드의 파라미터에 무효인 값이 설정되어 있는 경우 등이다.  ");
					if( HR ==D3DERR_NOTAVAILABLE )AfxMessageBox("이 장치는, 문의한 테크닉을 지원 하고 있지 않다.  ");
					if( HR ==D3DERR_OUTOFVIDEOMEMORY) AfxMessageBox("Direct3D 가 처리를 실시하는데 충분한 디스플레이 메모리가 없다. "); 
#endif
					Mode = D3DM_NONE;
					LastError = 4;
					return false;
				}
			}
		}
	}


	if( p3D9Device == NULL ) return false;


	ViewPort.X = 0;
	ViewPort.Y = 0;
	ViewPort.Width = Width;
	ViewPort.Height = Height;
	ViewPort.MinZ = 0.0f;
	ViewPort.MaxZ = 1.0f;

	//this is supposed to be the camera
	/*    D3DXVECTOR3 vEyePt( 0.0f, 0.0f,5.0f );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	*/
	D3DXVECTOR3 vEyePt( 0.0f, 0.0f,100.0f );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, -1.0f, 0.0f );

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	p3D9Device->SetTransform( D3DTS_VIEW, &matView );

	D3DXMATRIXA16 matProj;

	//D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, -100.0f, 100.0f );

	D3DXMatrixOrthoOffCenterLH(&matProj , -Width/2.0f,Width/2.0f ,-Height/2.0f,Height/2.0f , -100, 100 );

	p3D9Device->SetTransform( D3DTS_PROJECTION, &matProj );
	D3DXMATRIXA16 matWorld;
	D3DXMatrixTranslation( &matWorld, -Width/2.0f , -Height/2.0f , 0 );
	p3D9Device->SetTransform( D3DTS_WORLD, &matWorld);
	//Result = p3D9Device->SetViewport(&ViewPort);
	//PreLight3D(false);

	Result = p3D9Device->SetFVF( D3DFVF_TLVERTEX );
	EnableZBuffer(TRUE);
	//	EnableZBuffer(FALSE);
	D3DCAPS9 caps;
	p3D9Device->GetDeviceCaps(&caps);    // initialize m_pd3dDevice before using

	bVertexShader = TRUE;
	if( caps.VertexShaderVersion < D3DVS_VERSION(2,0) )
		bVertexShader = FALSE;

	bPixelShader = TRUE;
	if( caps.PixelShaderVersion < D3DVS_VERSION(2,0) )														//EnableVertexShader
		bPixelShader = FALSE;															//EnablePixelShader
	ResetState();

	// 텍스쳐 사이즈를 정하장.

	if( caps.MaxTextureWidth < m_iMAX_TEXTURE_SIZE_X )
	{
		//m_iMAX_TEXTURE_SIZE_X = caps.MaxTextureWidth;
		m_iMAX_TEXTURE_SIZE_X = 256;
	}

	if( caps.MaxTextureHeight < m_iMAX_TEXTURE_SIZE_Y )
	{
		//m_iMAX_TEXTURE_SIZE_Y = caps.MaxTextureHeight;
		m_iMAX_TEXTURE_SIZE_Y = 256;
	}

	if( m_iMAX_TEXTURE_SIZE_X >= m_iMAX_TEXTURE_SIZE_Y )
	{
		m_iMAX_TEXTURE_SIZE = m_iMAX_TEXTURE_SIZE_Y;
	}
	else
	{
		m_iMAX_TEXTURE_SIZE = m_iMAX_TEXTURE_SIZE_X;
	}

	HRESULT hr;
	if( FAILED( hr = GetDevice()->CreateVertexBuffer( 
		4 * sizeof(D3DTLVERTEX), 
		D3DUSAGE_DYNAMIC , 
		D3DFVF_TLVERTEX, // Our custom FVF
		D3DPOOL_SYSTEMMEM, 
		&m_pTestVertexBuffer, NULL )))
	{
		return false;
	}







	return true;
}
bool CDirectX::Close()
{
	CloseDirect3D();
	return true;
}
int CDirectX::GetLastError(string* pErrorText)
{

	if(pErrorText)
	{
		string Text;
		switch(LastError)
		{
		case -1:
			Text = "No real Error";
			break;
		case 1:
			Text = "No parentwindow specified. Please call 'SetParent' first";
			break;
		case 2:
			Text = "Error creating a baseplatform of D3D9";
			break;
		case 3:
			Text = "Only 16 Bit or 32 Bit supported";
			break;
		case 4:
			Text = "Error connecting Direct3D compatible device";
			break;
		case 5:
			Text = "No De3vice initialized. Please call 'InitDirect3D' first";
			break;
		case 6:
			Text = "Error while controlling the rendering pipeline";
			break;
		case 7:
			Text = "Error while presenting the viewport";
			break;
		case 8:
			Text = "Error acquiring data from the frontbuffer calling 'GetFrontBufferCopy'.\n\nPlease check your parameters and your surface passed to the function";
			break;
		default:
			Text = "Any nonspecified Error occured";
			break;
		}
		char temp[256];
		string Out;
		sprintf( temp , "CDirectX API Error N?:  %d\n\n%s",LastError,Text.c_str());
		Out = temp;
		*pErrorText = Out;
	}

	int RetVal = LastError;
	LastError = 0;

	return RetVal;
}
void CDirectX::SetParent(HWND pWnd)
{
	pParent = pWnd;
}

DWORD CDirectX::F2DW(float f)
{
	return *((DWORD*)&f);
}
void CDirectX::MakeAlphaTexture(LPDIRECT3DTEXTURE9 Texture, COLORREF Alpha)
{
	/*	if(!Texture)
	return;

	LPDIRECT3DSURFACE9 Tex = GetTextureSurface(0,Texture);

	D3DSURFACE_DESC desc;
	Tex->GetDesc(&desc);

	HDC hdc;
	Tex->GetDC(&hdc);

	CDC dc;
	dc.Attach(hdc);

	UINT Bytes = desc.Width * desc.Height * 4;
	BYTE* Buffer = new BYTE[Bytes];
	dc.GetCurrentBitmap()->GetBitmapBits(Bytes, Buffer);
	DWORD Pixel = (DWORD)Buffer,
	CompCol = Alpha;

	DWORD dwwidth = desc.Width;
	DWORD dwheight = desc.Height;

	// This asembler routine applys a transparent alpha value to each pixel in the texture that matches the desired blending color

	//의사코드 
	// 
	// 	DWORD m_Pixel, m_Color , m_Width ,m_Height;
	// 
	// 	m_Pixel = Pixel 
	// 	m_Color = CompCol
	// 	m_Width = dwwidth;
	// 	m_Height = dwheight;
	// 
	// 	for( int i = m_Width-1 ; i >=0 ;)
	// 	{
	// 		for( int j = m_Height-1 ; j >=0 ;)
	// 		{
	// 			if( ( *Pixel & 0x00ffffff ) ==  CompCol )
	// 			{
	// 				*Pixel |=0x01ffffff; // 알파 세팅
	// 			}
	// 		
	// 			Pixel+=4;
	// 			j--;
	// 		}
	// 		i--;
	// 	}

	__asm
	{

	//사용할 변수 스택에 저장
	push eax
	push ebx
	push ecx
	push edx
	mov  edx,Pixel    
	mov  eax,CompCol  
	mov  ecx,dwwidth  
	push ecx
	_STARTROW_:

	mov  ecx,dwheight

	_TEST_:

	mov  ebx,edx
	and  ebx,0x00ffffff
	cmp  ebx,eax
	jne	 _NOMATCH_

	or   ebx,0x01ffffff
	mov  edx,ebx

	_NOMATCH_:

	add  edx,4
	dec  ecx
	jnz  _TEST_

	pop  ecx
	dec  ecx
	jz   _LEAVE_
	push ecx
	jmp  _STARTROW_

	_LEAVE_:

	pop  edx
	pop  ecx
	pop  ebx
	pop  eax
	}

	dc.GetCurrentBitmap()->SetBitmapBits(Bytes,Buffer);

	delete [] Buffer;

	dc.Detach();
	Tex->ReleaseDC(hdc);
	*/
}
float CDirectX::Byte2Float(BYTE byte)
{
	return (float)((float)byte / 255.0f);
}
BOOL CDirectX::CheckShaderVersion( )
{
	return FALSE;
	return bVertexShader & bPixelShader;
}

//helper 함수

void CDirectX::HLine(int x, int y, int len, DWORD color, BOOL flag)
{
	D3DTLVERTEX pvertices[2];
	float rhw = 0.5f;

	float depth = 0.0f;
	if( flag == TRUE )
	{
		color ^= 0x00ffffff;
		SetAlphaBlend(9);
	}
	x+=1;
	y+=1;
	pvertices[0].pos.x = float(x)-0.5f; pvertices[0].pos.y = float(y) - 0.5f; pvertices[0].pos.z = depth;
	pvertices[0].color = color ;
	//	pvertices[0].rhw = 1.0f; 
	pvertices[0].tex.x = 0; pvertices[0].tex.y = 0;


	pvertices[1].pos.x = float(x) + len -0.5f; pvertices[1].pos.y = float(y) - 0.5f; pvertices[1].pos.z = depth;
	pvertices[1].color = color ; 
	//	pvertices[1].rhw = 1.0f; 
	pvertices[1].tex.x = 0; pvertices[1].tex.y = 0;

	p3D9Device->SetTexture(0,0);
	p3D9Device->SetFVF(D3DFVF_TLVERTEX);
	p3D9Device->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, pvertices, sizeof(D3DTLVERTEX));
	if( flag == TRUE )
	{
		SetAlphaBlend(0);
	}
}// 수평선을 그림(빠름)
void CDirectX::VLine(int x, int y, int len, DWORD color, BOOL flag)
{

	D3DTLVERTEX pvertices[2];
	float rhw = 0.5f;
	x+=1;
	y+=1;
	float depth = 0.0f;
	if( flag == TRUE )
	{
		color ^= 0x00ffffff;
		SetAlphaBlend(9);
	}
	pvertices[0].pos.x =  float(x)-0.5f; pvertices[0].pos.y = float(y) - 0.5f; pvertices[0].pos.z = depth;
	pvertices[0].color = color ; 
	//	pvertices[0].rhw = 1.0f; 
	pvertices[0].tex.x = 0; pvertices[0].tex.y = 0;


	pvertices[1].pos.x =  float(x)-0.5f; pvertices[1].pos.y = float(y) + len -0.5f; pvertices[1].pos.z = depth;
	pvertices[1].color = color ; 
	//	pvertices[1].rhw = 1.0f; 
	pvertices[1].tex.x = 0; pvertices[1].tex.y = 0;

	p3D9Device->SetTexture(0,0);
	p3D9Device->SetFVF(D3DFVF_TLVERTEX);
	p3D9Device->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, pvertices, sizeof(D3DTLVERTEX));
	if( flag == TRUE )
	{
		SetAlphaBlend(0);
	}

}					// 수직선을 그림(빠름)
void CDirectX::DrawLine(int x1, int y1, int x2, int y2, DWORD color)
{

	D3DTLVERTEX pvertices[2];
	float rhw = 0.5f;

	float depth = 0.0f;
	pvertices[0].pos.x = float(x1)-0.5f; pvertices[0].pos.y = float(y1) - 0.5f; pvertices[0].pos.z = depth;
	pvertices[0].color = color ;
	//	pvertices[0].rhw = 1.0f; 
	pvertices[0].tex.x = 0; pvertices[0].tex.y = 0;


	pvertices[1].pos.x = float(x2)-0.5f; pvertices[1].pos.y = float(y2) -0.5f; pvertices[1].pos.z = depth;
	pvertices[1].color = color ;
	//	pvertices[1].rhw = 1.0f; 
	pvertices[1].tex.x = 0; pvertices[1].tex.y = 0;

	p3D9Device->SetTexture(0,0);
	p3D9Device->SetFVF(D3DFVF_TLVERTEX);
	p3D9Device->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, pvertices, sizeof(D3DTLVERTEX));

}						// 선을 그림(브레제남 알고리즘)
void CDirectX::DrawEmptyBox(int x1, int y1, int x2, int y2, DWORD color, BOOL flag)
{
	D3DTLVERTEX pvertices[5];
	float rhw = 0.5f;

	float depth = 0.0f;
	pvertices[0].pos.x = float(x1)-0.5f; pvertices[0].pos.y = float(y1) - 0.5f; pvertices[0].pos.z = depth;
	pvertices[0].color = color ;
	//	pvertices[0].rhw = 1.0f; 
	pvertices[0].tex.x = 0; pvertices[0].tex.y = 0;


	pvertices[1].pos.x = float(x2)-0.5f; pvertices[1].pos.y = float(y1) -0.5f; pvertices[1].pos.z = depth;
	pvertices[1].color = color ; 
	//	pvertices[1].rhw = 1.0f; 
	pvertices[1].tex.x = 0; pvertices[1].tex.y = 0;


	pvertices[2].pos.x = float(x2)-0.5f; pvertices[2].pos.y = float(y2) - 0.5f; pvertices[2].pos.z = depth;
	pvertices[2].color = color ; 
	//	pvertices[2].rhw = 1.0f; 
	pvertices[2].tex.x = 0; pvertices[2].tex.y = 0;


	pvertices[3].pos.x = float(x1)-0.5f; pvertices[3].pos.y = float(y2) -0.5f; pvertices[3].pos.z = depth;
	pvertices[3].color = color ; 
	//	pvertices[3].rhw = 1.0f; 
	pvertices[3].tex.x = 0; pvertices[3].tex.y = 0;

	pvertices[4].pos.x = float(x1)-0.5f; pvertices[4].pos.y = float(y1) -1.5f; pvertices[4].pos.z = depth;
	pvertices[4].color = color;
	//	pvertices[4].rhw = 1.0f; 
	pvertices[4].tex.x = 0; pvertices[4].tex.y = 0;


	p3D9Device->SetTexture(0,0);
	p3D9Device->SetFVF(D3DFVF_TLVERTEX);
	p3D9Device->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, pvertices, sizeof(D3DTLVERTEX));

}		
// 속이 빈 박스를 그림


void CDirectX::SetVertex( D3DTLVERTEX *pvertices, int x1,int y1,int _x2,int _y2 )
{
	SetVertex(pvertices,(float)x1,(float)y1,(float)_x2,(float)_y2);
}
void CDirectX::SetVertex( D3DTLVERTEX *pvertices, float x1,float y1,float _x2,float _y2 )
{
	float rhw = 0.5f;
	float depth = 0.0f;
	float x2 = _x2;
	float y2 = _y2; 
	if( x1 == x2 ) x2+=1;
	if( y1 == y2 ) y2+=1;
	int i = 0;

	//	pvertices[i].rhw = 1.0f; 
	pvertices[i].pos.x = float(x1)-0.5f; pvertices[i].pos.y = float(y2) -0.5f; pvertices[i].pos.z = depth;

	//pvertices[i].normal.x = 0;
	//pvertices[i].normal.y = 0;
	//pvertices[i].normal.z = 1;

	//D3DXVec3Normalize(&pvertices[i].normal,&pvertices[i].pos);
	//pvertices[i].normal.z *= -1;
	//	pvertices[i].normal.x = 0;
	//	pvertices[i].normal.y = 0;
	i++;
	//	pvertices[i].rhw = 1.0f; 	
	pvertices[i].pos.x = float(x1)-0.5f; pvertices[i].pos.y = float(y1) -0.5f; pvertices[i].pos.z = depth;
	//D3DXVec3Normalize(&pvertices[i].normal,&pvertices[i].pos);
	//pvertices[i].normal.z *= -1;
	//pvertices[i].normal.x = 0;
	//pvertices[i].normal.y = 0;
	//pvertices[i].normal.z = 1;
	i++;
	//	pvertices[i].rhw = 1.0f; 
	pvertices[i].pos.x = float(x2)-0.5f; pvertices[i].pos.y = float(y2) - 0.5f; pvertices[i].pos.z = depth;
	//D3DXVec3Normalize(&pvertices[i].normal,&pvertices[i].pos);
	//pvertices[i].normal.z *= -1;
	//pvertices[i].normal.x = 0;
	//pvertices[i].normal.y = 0;
	//pvertices[i].normal.z = 1;
	i++;
	//	pvertices[i].rhw = 1.0f; 
	pvertices[i].pos.x = float(x2)-0.5f; pvertices[i].pos.y = float(y1) - 0.5f; pvertices[i].pos.z = depth;
	//D3DXVec3Normalize(&pvertices[i].normal,&pvertices[i].pos);
	//pvertices[i].normal.z *= -1;
	//pvertices[i].normal.x = 0;
	//pvertices[i].normal.y = 0;
	//pvertices[i].normal.z = 1;
}	
void CDirectX::SetColor( D3DTLVERTEX *vert , DWORD color)
{
	int i = 0;
	vert[i].color = color ;
	i++;
	vert[i].color = color ;
	i++;
	vert[i].color = color ;
	i++;
	vert[i].color = color ;
}
void CDirectX::SetUV( D3DTLVERTEX *vert ,float u1, float v1 , float u2, float v2 ,int opt)
{
	int i = 0;
	vert[i].tex.x = u1; vert[i].tex.y = v2;
	i++;
	vert[i].tex.x = u1; vert[i].tex.y = v1;
	i++;
	vert[i].tex.x = u2; vert[i].tex.y = v2;
	i++;
	vert[i].tex.x = u2; vert[i].tex.y = v1;
	if( opt & MIRROR || opt & LR )
	{
		swap( vert[0].tex ,  vert[2].tex);
		swap( vert[1].tex ,  vert[3].tex);
	}
	if( opt & UD )
	{
		swap( vert[1].tex ,  vert[0].tex);
		swap( vert[3].tex ,  vert[2].tex);
	}
}
void CDirectX::DrawFillBox(int x1, int y1, int _x2, int _y2, DWORD color , int opt)
{
	D3DTLVERTEX pvertices[4];
	SetVertex( pvertices, x1,y1,_x2,_y2 );
	SetColor(pvertices, color );
	SetUV( pvertices, 0.0f , 0.0f , 1.0f , 1.0f , opt);	
	DrawVertex( pvertices );

}				// 속을 채운 박스를 그림
void CDirectX::DrawSelectBox(int x1, int y1, int x2, int y2, DWORD color, BOOL flag)
{
	DrawFillBox(x1,y1,x2,y2,color,0);

	color ^=0xffffffff;
	DrawEmptyBox(x1,y1,x2,y2,color);

	color = 0xffffffff;
	DrawEmptyBox(x1-2				,y1-2,x1-2+4,y1-2+ 4,color);
	DrawEmptyBox(x2-2				,y1-2,x2-2+4,y1-2+4,color);
	DrawEmptyBox((x2 + x1)/2-2		,y1-2,(x2 + x1)/2-2+4,y1-2+4,color);

	DrawEmptyBox(x1-2				,(y1+y2)/2-2,x1-2+4,(y1+y2)/2-2+4,color);
	DrawEmptyBox(x2-2				,(y1+y2)/2-2,x2-2+4,(y1+y2)/2-2+4,color);

	DrawEmptyBox(x1-2				,y2-2,x1-2+4,y2-2+4,color);
	DrawEmptyBox(x2-2				,y2-2,x2-2+4,y2-2+4,color);
	DrawEmptyBox((x2 + x1)/2-2		,y2-2,(x2 + x1)/2+4,y2-2+4,color);

}	// 선택 상자를 그림
D3DXVECTOR3 Cross(  D3DXVECTOR3& vector_,const D3DXVECTOR3& V_ )
{
	return D3DXVECTOR3( ( vector_.y*V_.z - vector_.z*V_.y ), ( vector_.z*V_.x - vector_.x*V_.z ), ( vector_.x*V_.y - vector_.y*V_.x )  );
}
D3DXVECTOR3 Multiply(D3DXVECTOR3& vector_, const D3DXQUATERNION& Q_ )
{
	D3DXVECTOR3 uv, uuv;
	D3DXVECTOR3 qvec( Q_.x, Q_.y, Q_.z );
	uv = Cross( vector_ ,  qvec );
	uuv = Cross( uv , qvec );
	uv *= ( 2.0f * Q_.w );
	uuv *= 2.0f;

	return (vector_ + uv + uuv);
}

//void _3DRotatePoint(D3DXVECTOR3* pPoint, float anglex , float angley ,float anglez )
void _3DRotatePoint(D3DXVECTOR3* pPoint, float angle )
{
	D3DXMATRIX matrotxyz;
	//	D3DXMatrixRotationYawPitchRoll(&matrotxyz , angley/180.0f * __PI__ , anglex/180.0f * __PI__ ,  anglez/180.0f * __PI__ );
	D3DXMatrixRotationYawPitchRoll(&matrotxyz , 0 , 0 , angle/180.0f * __PI__  );

	D3DXMATRIX Trans;
	D3DXMatrixTranslation( &Trans , pPoint->x , pPoint->y , pPoint->z );
	Trans = Trans * matrotxyz;
	pPoint->x = Trans._41;
	pPoint->y = Trans._42;
	pPoint->z = Trans._43;

}

//void _3DAxisRotatePoint(D3DXVECTOR3* pPoint, float anglex , float angley ,float anglez , D3DXVECTOR3 axis)
void _3DAxisRotatePoint(D3DXVECTOR3* pPoint, float angle, D3DXVECTOR3 axis)
{
	D3DXVECTOR3 pt;
	pt.x = pPoint->x - axis.x;
	pt.y = pPoint->y - axis.y;
	pt.z = pPoint->z - axis.z;
	//	_3DRotatePoint(&pt, -anglex , -angley , -anglez);
	_3DRotatePoint(&pt, -angle );

	pPoint->x = pt.x + axis.x;
	pPoint->y = pt.y + axis.y;
	pPoint->z = pt.z + axis.z;

}

//void _3DAxisReRotatePoint(D3DXVECTOR3* pPoint, float anglex , float angley ,float anglez , D3DXVECTOR3 axis)
void _3DAxisReRotatePoint(D3DXVECTOR3* pPoint, float angle , D3DXVECTOR3 axis)
{
	D3DXVECTOR3 pt;
	pt.x = pPoint->x - axis.x;
	pt.y = pPoint->y - axis.y;
	pt.z = pPoint->z - axis.z;

	D3DXMATRIX matrotxyz;
	//	D3DXMatrixRotationYawPitchRoll(&matrotxyz , angley/180.0f * __PI__ , anglex/180.0f * __PI__ ,  -anglez/180.0f * __PI__ );
	D3DXMatrixRotationYawPitchRoll(&matrotxyz , 0 ,  0, angle/180.0f * __PI__  );

	D3DXMATRIX invert;
	D3DXMatrixInverse( &invert , NULL , &matrotxyz );
	D3DXMATRIX Trans;
	D3DXMatrixTranslation( &Trans , pt.x , pt.y , pt.z );

	float z = (invert._13 * pt.x + invert._23 * pt.y) / (-invert._33);
	float x =  invert._11 * pt.x + invert._21 * pt.y + invert._31 * z;
	float y =  invert._12 * pt.x + invert._22 * pt.y + invert._32 * z;


	pPoint->x  =   x  + axis.x;
	pPoint->y  =   y  + axis.y;
	pPoint->z  =   z  + axis.z;
	/*
	char data[256];
	sprintf( data , "x =%f y = %f x'=%f y'=%f z'=%f  \n", pPoint->x,pPoint->y, x ,y,z );
	//	OutputDebugString( data );
	*/
}
//void CDirectX::SetRotationVertex( D3DTLVERTEX *vert, int x1,int y1,int x2,int y2 ,float rotx,float roty,float rotz ,D3DXVECTOR3 Center )
void CDirectX::SetRotationVertex( D3DTLVERTEX *vert, int x1,int y1,int x2,int y2 ,float rot,D3DXVECTOR3 Center )
{
	D3DXVECTOR3 pt[4];
	pt[0].x = x1;	pt[0].y = y2;pt[0].z = 0.0f;
	pt[1].x = x1;	pt[1].y = y1;pt[1].z = 0.0f;
	pt[2].x = x2;	pt[2].y = y2;pt[2].z = 0.0f;
	pt[3].x = x2;	pt[3].y = y1;pt[3].z = 0.0f;

	for(int ii=0; ii<4; ii++) 
	{
		//_3DAxisRotatePoint(&pt[ii], rotx,roty ,rotz, Center);
		_3DAxisRotatePoint(&pt[ii], rot, Center);
	}
	D3DTLVERTEX g_vertices[4];
	float rhw = 1.0f; ;

	float depth = 0.0f;

	D3DXVECTOR3 pos1(pt[0].x,pt[0].y,depth);
	D3DXVECTOR3 pos2(pt[1].x,pt[1].y,depth);
	D3DXVECTOR3 pos3(pt[2].x,pt[2].y,depth);
	D3DXVECTOR3 pos4(pt[3].x,pt[3].y,depth);
	int i = 0;
	vert[i].pos = pos1;
	//	vert[i].rhw = depth;
	i++;
	vert[i].pos = pos2;
	//	vert[i].rhw = depth;
	i++;
	vert[i].pos = pos3;
	//	vert[i].rhw = depth;
	i++;
	vert[i].pos = pos4;
	//	vert[i].rhw = depth;
}

//void CDirectX::DrawRotateXYZBox(int x1, int y1, int x2, int y2, float anglex, float angley, float anglez, DWORD color, int opt ,int centerx , int centery )
void CDirectX::DrawRotateXYZBox(int x1, int y1, int x2, int y2, float angle, DWORD color, int opt ,int centerx , int centery )
{
	D3DTLVERTEX g_vertices[4];

	D3DXVECTOR3 Center;
	Center.x = x1;
	Center.y = y1;
	Center.z = 0;
	//SetRotationVertex( g_vertices , x1, y1, x2, y2, anglex, angley, anglez,Center);
	SetRotationVertex( g_vertices , x1, y1, x2, y2, angle,Center);
	SetColor( g_vertices , color );
	SetUV( g_vertices ,0.0f , 0.0f , 1.0f , 1.0f,opt);
	DrawVertex(g_vertices);

}
void CDirectX::DrawVertex( D3DTLVERTEX *vert )
{
	if( !p3D9Device )return;



	if( GetShaderEnable() )
	{
		UINT uPasses;
		if(D3D_OK == pEffect->Begin(&uPasses, 0)) 
		{
			for(UINT uPass = 0; uPass < uPasses; uPass++)
			{
				pEffect->BeginPass(uPass); 
				p3D9Device->SetFVF(D3DFVF_TLVERTEX);
				p3D9Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vert, sizeof(D3DTLVERTEX));
				pEffect->EndPass();
			}

			pEffect->End();
		}	 
	}
	else
	{
		p3D9Device->SetFVF(D3DFVF_TLVERTEX);
		p3D9Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vert, sizeof(D3DTLVERTEX));





		// 		VOID* pVertices;
		// 		if( FAILED( m_pTestVertexBuffer->Lock( 0, sizeof(D3DTLVERTEX) * 4, (void**)&pVertices, 0 ) ) )
		// 			return ;
		// 		memcpy( pVertices, vert,  sizeof(D3DTLVERTEX) * 4 );
		// 		m_pTestVertexBuffer->Unlock();
		// 
		// 		
		// 		p3D9Device->SetStreamSource( 0, m_pTestVertexBuffer, 0, sizeof(D3DTLVERTEX) );
		// 	
		// 		p3D9Device->SetFVF( D3DFVF_TLVERTEX );	
		// 	
		// 		p3D9Device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
		m_iDrawVertexCount++;

	}
}



void CDirectX::Draw3DLine(float fPosX1, float fPosY1, float fPosZ1, float fPosX2, float fPosY2, float fPosZ2, DWORD dwColor)
{

	D3DTLVERTEX pvertices[2];



	pvertices[0].pos.x = fPosX1; 
	pvertices[0].pos.y = fPosY1; 
	pvertices[0].pos.z = fPosZ1;
	pvertices[0].color = dwColor;
	pvertices[0].tex.x = 0; 
	pvertices[0].tex.y = 0;

	pvertices[1].pos.x = fPosX2; 
	pvertices[1].pos.y = fPosY2; 
	pvertices[1].pos.z = fPosZ2;
	pvertices[1].color = dwColor;
	pvertices[1].tex.x = 0; 
	pvertices[1].tex.y = 0;

	p3D9Device->SetTexture(0,0);
	p3D9Device->SetFVF(D3DFVF_TLVERTEX);
	p3D9Device->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, pvertices, sizeof(D3DTLVERTEX));

}

LPDIRECT3D9 CDirectX::GetDirect3D9()
{
	return p3D9;
}

HRESULT CDirectX::SetTexture(int stage, TEXTURE_ texture )
{

	if( m_bActivationFlagsForTextures[stage] && m_pTextures[stage] == texture  ) 
	{
		return S_OK;
	}


	m_bActivationFlagsForTextures[stage] = TRUE;
	m_pTextures[stage] = texture;	

	if( GetShaderEnable() )
	{
		pEffect->SetTexture("BlurTex", texture );
		return S_OK;
	}


	return p3D9Device->SetTexture(stage,texture);


}

HRESULT CDirectX::SetRenderState( D3DRENDERSTATETYPE State, DWORD Value)
{
	if( m_bActivationFlagsForRenderStates[State] && m_nRenderStates[State] == Value )
	{
		return S_OK;
	}

	m_bActivationFlagsForRenderStates[State] = true;
	m_nRenderStates[State] = Value;

	return p3D9Device->SetRenderState( State, Value );
}

HRESULT CDirectX::SetSamplerState( int Stage , D3DSAMPLERSTATETYPE State, DWORD Value )
{
	if( m_bActivationFlagsForSamplerStates[Stage][State] && m_nSamplerStates[Stage][State] == Value )
	{
		return S_OK;
	}

	m_bActivationFlagsForSamplerStates[Stage][State] = true;
	m_nSamplerStates[Stage][State] = Value;

	return p3D9Device->SetSamplerState( Stage , State, Value );
}
HRESULT CDirectX::SetTextureStageState( int Stage , D3DTEXTURESTAGESTATETYPE State, DWORD Value )
{
	if( m_bActivationFlagsForSamplerStates[Stage][State] && m_nTextureStageStates[Stage][State] == Value )
	{
		return S_OK;
	}
	m_bActivationFlagsForSamplerStates[Stage][State] = true;
	m_nTextureStageStates[Stage][State] = Value;
	return p3D9Device->SetTextureStageState( Stage , State, Value );
}
