// DirectX.h: interface for the CDirectX class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_DIRECTX_H__BFFFA192_7959_4771_8FDB_9CB61385A3FD__INCLUDED_)
//#define AFX_DIRECTX_H__BFFFA192_7959_4771_8FDB_9CB61385A3FD__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000


#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <math.h>
#include <mmsystem.h>

#include <d3d9.h>
#include <d3dx9tex.h>
#include <ddraw.h>
#include <dinput.h>

#include <string>

#include "GraphicDXDefine.h"

using namespace std;


#define _INIT(x)		x = NULL
#define _RELEASE(x)		if(x) {x->Release(); x = NULL;}
#define _DELETE(x)		if(x) {delete x; x = NULL;}
#define _DELARRAY(x)	if(x) {delete [] x; x = NULL;}

#define D3DM_PURE_HARDWARE		0
#define D3DM_MMX_3DNOW			1
#define D3DM_SOFTWARE			2
#define D3DM_NONE				3

#define SCULL_NONE				1
#define SCULL_CLOCKWISE			2
#define SCULL_COUNTERCLOCKWISE	3

#define DM_POINT				1
#define DM_WIREFRAME			2
#define DM_SOLID				3

#define SM_FLAT					1
#define SM_GOURAUD				2
#define SM_PHONG				3

#define		OFF			0
#define		HALF		1
#define		DARK		2
#define		BRIGHT		3

#define LT_SPOT			2
#define LT_POINT		1
#define LT_DIRECTIONAL	3

#define ESM_NUM_TEXTURES 8
#define ESM_NUM_SAMPLER_STATES 13
#define ESM_NUM_RENDER_STATES 255
#define ESM_NUM_TEXTURE_STAGE_STATES 8
#define __PI__ 3.1415926535897932384626433832795
#define __GRAD__ (__PI__ / 180.0)

class ClassRect
{
public:
	ClassRect()
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	virtual ~ClassRect(){};
	void SetRect( long l , long t ,long r , long b )
	{
		left = l ;
		top = t;
		right = r;
		bottom = b;
	}

	long	left;
	long	top;
	long	right;
	long	bottom;
	long Width(){ return right - left;}
	long Height(){ return bottom - top;}

};


struct PCSVERTEX
{
	float x,y,z;		// Vertexposition
	float nx,ny,nz;		// Vertexnormal (Only for use when not prelit

	COLORREF Color;		// Prelightcolor of the vertex
	float       tu1, tv1;	// Texturecoordinates (0.0f - 1.0f  tu1 = x tv1 = y)


};

struct D3DTLVERTEX{

	D3DXVECTOR3	pos;
	//float		rhw;
	//D3DXVECTOR3 normal;   /// 정점의 노멀 벡터
	D3DCOLOR    color;
	D3DXVECTOR2	tex;
};

#define D3DFVF_TLVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
//#define D3DFVF_TLVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
struct PCS_CAMVECTOR
{
	void SetVector(float fx, float fy, float fz) {x = fx; y = fy; z = fz;};
	double x,y,z;
};

struct KAMERA9
{
	PCS_CAMVECTOR Eyepoint;			// Represents the Virtual camerapoint in space
	PCS_CAMVECTOR Targetpoint;		// Represents the Vertex the camera is focus?g
	PCS_CAMVECTOR Upvector;			// Defines the 'top' direction of the world. It's like a Stick on a plank. the stick always points in 'topdirection' from the plank. This vector is used to simulate Z-rotation of the camera
};

typedef LPDIRECT3DTEXTURE9 TEXTURE_;
//extern void _3DRotatePoint(D3DXVECTOR3* pPoint, float anglex , float angley ,float anglez );
//extern void _3DAxisRotatePoint(D3DXVECTOR3* pPoint, float anglex , float angley ,float anglez , D3DXVECTOR3 axis);
//extern void _3DAxisReRotatePoint(D3DXVECTOR3* pPoint, float anglex , float angley ,float anglez , D3DXVECTOR3 axis);

extern void _3DRotatePoint(D3DXVECTOR3* pPoint, float angle );
extern void _3DAxisRotatePoint(D3DXVECTOR3* pPoint, float angle  , D3DXVECTOR3 axis);
extern void _3DAxisReRotatePoint(D3DXVECTOR3* pPoint, float angle  , D3DXVECTOR3 axis);


class CDirectX  
{
public:
	CDirectX();
	virtual ~CDirectX();
	void MoveCameraEyepoint(float m, bool bHeight);
	void CloseDirect3D();
	bool SnapshotBackbuffer(string File);
	bool InitBumpMap(TEXTURE_ Surface, TEXTURE_* BumpSurface);
	void MoveCamera(float m, bool bHeight = false);
	void GierCamera(double Degree);
	void InclineCameraTarget(double Degree);
	void InclineCamera(double Degree);
	void RotateCameraTarget(double Degree);
	void RotateCamera(double Degree);
	void ValidateCamera();
	void MoveCameraStatic(float x, float y, float z);
	void MoveCameraTarget(float x, float y, float z);
	void MoveCamera(float x, float y, float z);
	void SetCamera(KAMERA9 Cam, bool bUpdate = true);
	KAMERA9 GetCamera();
	void SetUseSpecularLights(bool bOn);
	void SetBlendMode(bool bOn);
	//	void Set3DViewCaps(double Angle, float Aspect, float Close, float Far);
	HRESULT D3DUtil_SetProjectionMatrix( D3DMATRIX& mat, FLOAT fFOV, FLOAT fAspect,FLOAT fNearPlane, FLOAT fFarPlane );
	D3DLIGHT9 GetStandardLight(int Type);
	D3DMATERIAL9 GetStandardMaterial();
	void SetViewPort(DWORD x,DWORD y,DWORD cx,DWORD cy,float MinZ,float MaxZ);
	D3DVIEWPORT9 GetViewPort();
	bool GetFrontbufferCopy(LPDIRECT3DSURFACE9 Surface);
	void FillSurface(string Filename, LPDIRECT3DSURFACE9 Surface);
	void FillSurface(HBITMAP hbmp, LPDIRECT3DSURFACE9 Surface);
	LPDIRECT3DSURFACE9 CreateSurface(int Width, int Height);
	void FillTexture(TEXTURE_ Texture, HBITMAP hbmp, COLORREF BlendColor = 0xff000000);
	void FillTexture(TEXTURE_ Texture, string Filename, COLORREF BlendColor = 0xff000000);
	TEXTURE_ CreateEmptyTexture(int Width, int Height);
	TEXTURE_ CreateEmptyTexture(int Width, int Height , UINT level , DWORD usage ,D3DFORMAT format );
	LPDIRECT3DSURFACE9 GetTextureSurface(int Level, TEXTURE_ Text);
	TEXTURE_ CreateTexture(HBITMAP hbmp, COLORREF BlendColor = 0xff000000);
	TEXTURE_ CreateTexture(string Filename,  COLORREF BlendColor = 0xff000000);
	void EnableSmoothFilter(bool bOn);
	void EnableLineAntiAliasing(bool bOn);
	void SetMaterialSpec(bool bOn);
	void EnableNormalisation(bool bOn);
	void SetAmbientLight(COLORREF Col);
	void SetAlphaBlend(int Mode);
	void EnableDithering(bool bOn);
	void SetShadeMode(UINT Mode);
	void SetRenderMode(UINT Mode);
	bool EnableSpeclight(bool bOn);
	bool GetSpecEnabled();
	bool GetFogEnabled();
	bool SetFog(bool bOn, float FogBegin = 0.0f, float FogEnd = 1000.0f, COLORREF FogColor = 0x00000000);
	bool SetScullMode(int CullMode);
	bool GetPrelight();
	bool PreLight3D(bool bPrelight);
	bool Set3DViewCaps(int _Angle = 45, float _Close = -100.0f, float _Far = 100.0f, float _Aspect = 0.8f);
	bool ShowCursor(bool bShow);
	bool SetLight(int Index, D3DLIGHT9 Light);
	void EnableDialogBoxes(bool bOn);
	void SetCustomCursor(int SpotX,int SpotY, LPDIRECT3DSURFACE9 Cursor);
	void SetCursorPos(int x, int y);
	void EnableLight(int Index, bool bOn);
	D3DMATERIAL9 GetMaterial();

	bool IsLightEnabled(int Index);
	D3DLIGHT9* GetLight(int Index);

	LPDIRECT3DSURFACE9 GetZBuffer();
	int GetTextureMemoryMB();
	LPDIRECT3DSURFACE9 GetBackBuffer();
	void EnableZBuffer(bool bEnable);
	bool Flip();
	D3DCOLOR ConvertColor(COLORREF Col);
	LPDIRECT3DDEVICE9 GetDevice();
	LPDIRECT3D9		  GetDirect3D9();
	void SetClearColor(COLORREF Color);
	COLORREF GetClearColor();
	bool Render(bool bStartScene);
	int Get3DMode();
	bool InitDirect3D(int Width, int Height, int BPP, int BufferCount = 1, HWND TargetWindow = NULL, UINT Refreshrate = D3DPRESENT_RATE_DEFAULT);
	bool Close();
	int GetLastError(string* pErrorText = NULL);
	void SetParent(HWND pWnd);
	HWND GetParent() {return pParent;}
	BOOL CheckShaderVersion( );

private:

	DWORD F2DW(float f);
	void MakeAlphaTexture(TEXTURE_ Texture, COLORREF Alpha);
	float Byte2Float(BYTE byte);
	HWND										pParent;					// Globales Fensterhandle f? DX

	int
		LastError,					// Beinhaltet den Code des letzten Fehlers.
		Mode,						// Beinhaltet den 3D Mode
		LastBlend;					// Letzte Alphablendschablone

	LPDIRECT3D9									p3D9;						// 3D-Schnittstelle
public:
	D3DPRESENT_PARAMETERS						d3dpp;						// Legt fest, wie die Ausgabe von Direct3D aussehen soll.
	LPD3DXEFFECT								pEffect;
	LPDIRECT3DVERTEXDECLARATION9				pDecl;
	D3DXMATRIX m_matWorldViewProj;


	int											m_iMAX_TEXTURE_SIZE_X;
	int											m_iMAX_TEXTURE_SIZE_Y;
	int											m_iMAX_TEXTURE_SIZE;


private:
	LPDIRECT3DDEVICE9							p3D9Device;					// Device f? Direct3D9
	D3DCOLOR									_3DClearColor;				// L?chfarbe

	HRESULT										Result;						// Globales Ereignisregister;

	COLORREF									ClearColor;					// Color which with the Viewport is to be cleared;

	DDPIXELFORMAT								m_ddpfZPuffer;				// Struktur zur Erstellung und Verwaltung von zPuffern

	bool
		bUseZ,						// Indiziert das Benutzen eines ZPuffers:
		bUseStencil,				// indiziert das Benutzen eines Stencilbuffers
		bPreLight,					// Indiziert, ob die Szene vorbeleuchtet ist.
		bFogOn,						// Indiziert, ob Nebel aktiviert ist
		bSpecOn;					// Indiziert, ob SpecularLights aktiviert sind

	D3DLIGHT9									Light;						// Benutzt, um Lichtoperationen durchzuschleifen.

	D3DVIEWPORT9								ViewPort;					// Speichert Informationen ?er die Sichtmetrik

	D3DFORMAT									TextureFormat;				// Format f? zu erstellende Texturen

	bool										Bits16;						// Definiert 16 Bit Modus

	KAMERA9										Cam;						// Die Kamera im Raum;

	double ax,ay,az,nx,ny,nz,Asin,Acos,Angle;								// Verschiedene Berechnungsvarable

	double GierDegree;														// Beinhaltet den Giergrad der Kamera

	double Alphasin,Alphacos,Betasin,Betacos,Gammasin,Gammacos;				// Winkelvariablen f? die Gierungsberechnung der Kamera
	double TriA,TriB,TriC;													// Seitenl?genvariablen zur Pythagoreischen Berechnung von Winkeln

	D3DFORMAT Bumpformat;													// Format, mit dem Bumpmaps erstellt werden

	BOOL bVertexShader;															//EnableVertexShader
	BOOL bPixelShader;															//EnablePixelShader


	bool m_bActivationFlagsForRenderStates[ ESM_NUM_RENDER_STATES ];
	DWORD m_nRenderStates[ ESM_NUM_RENDER_STATES ];
	bool m_bActivationFlagsForTextures[ ESM_NUM_TEXTURES ];
	bool m_bActivationFlagsForTextureStageStates[ ESM_NUM_TEXTURES ][ ESM_NUM_TEXTURE_STAGE_STATES ];
	DWORD m_nTextureStageStates[ ESM_NUM_TEXTURES ][ ESM_NUM_TEXTURE_STAGE_STATES ];
	bool m_bActivationFlagsForSamplerStates[ ESM_NUM_TEXTURES ][ ESM_NUM_SAMPLER_STATES ];
	DWORD m_nSamplerStates[ ESM_NUM_TEXTURES ][ ESM_NUM_SAMPLER_STATES ];
	IDirect3DBaseTexture9* m_pTextures[ ESM_NUM_TEXTURES ];


	//helper 함수
public:
	void HLine(int x, int y, int len, DWORD color, BOOL flag);						// 수평선을 그림(빠름)
	void VLine(int x, int y, int len, DWORD color, BOOL flag);						// 수직선을 그림(빠름)
	void DrawLine(int x1, int y1, int x2, int y2, DWORD color);						// 선을 그림(브레제남 알고리즘)
	void DrawEmptyBox(int x1, int y1, int x2, int y2, DWORD color, BOOL flag=0);	// 속이 빈 박스를 그림
	void DrawFillBox(int x1, int y1, int x2, int y2, DWORD color ,int opt);			// 속을 채운 박스를 그림
	void DrawFillBoxGB(int x1, int y1, int x2, int y2, DWORD color, int grade);		// 속을 채운 박스를 그림(다단계 반투명)
	void DrawSelectBox(int x1, int y1, int x2, int y2, DWORD color, BOOL flag=0);	// 선택 상자를 그림
	void DrawRotateXBox(int x1, int y1, int x2, int y2, float angle, DWORD color , int opt, int centerx ,int centery);	// 회전 X-상자를 그림
	//	void DrawRotateXYZBox(int x1, int y1, int x2, int y2, float anglex, float angley, float anglez, DWORD color , int opt, int centerx ,int centery);
	void DrawRotateXYZBox(int x1, int y1, int x2, int y2, float angle, DWORD color, int opt ,int centerx , int centery );

	void SetVertex( D3DTLVERTEX *g_vertices, D3DXVECTOR3 p1,D3DXVECTOR3 p2,D3DXVECTOR3 p3,D3DXVECTOR3 p4 );
	void SetVertex( D3DTLVERTEX *g_vertices, int x1,int y1,int x2,int y2 );
	void SetVertex( D3DTLVERTEX *g_vertices, float x1,float y1,float x2,float y2 );

	//	void SetRotationVertex( D3DTLVERTEX *g_vertices, int x1,int y1,int x2,int y2 ,float rotx,float roty,float rotz  ,D3DXVECTOR3 Center );
	void SetRotationVertex( D3DTLVERTEX *vert, int x1,int y1,int x2,int y2 ,float rot,D3DXVECTOR3 Center );

	void SetColor( D3DTLVERTEX *g_vertices , DWORD color);
	void SetUV( D3DTLVERTEX *g_vertices ,float u1, float v1 , float u2, float v2 ,int opt );
	void DrawVertex( D3DTLVERTEX *g_vertices );

	void Draw3DLine(float fPosX1, float fPosY1, float fPosZ1, float fPosX2, float fPosY2, float fPosZ2, DWORD dwColor);
	HRESULT CreateEffect(LPCSTR filename);
	void    ReleaseEffect();
	void ResetState();
	BOOL GetShaderEnable() { return FALSE; };
	HRESULT SetTexture( int stage , TEXTURE_ texture );
	HRESULT SetSamplerState( int Stage , D3DSAMPLERSTATETYPE State, DWORD Value );
	HRESULT SetRenderState( D3DRENDERSTATETYPE State, DWORD Value);
	HRESULT SetTextureStageState( int Stage , D3DTEXTURESTAGESTATETYPE State, DWORD Value );

	// Test code
	int						m_iDrawVertexCount;
	LPDIRECT3DVERTEXBUFFER9 m_pTestVertexBuffer;
};
