
#pragma once

#include "GraphicDXDefine.h"


#define MULTISCENE_VERSION	101


// Ʈ�� Ÿ��
enum ENUM_TRACKTYPE
{
	TTYPE_NONE = 0,
	TTYPE_SCENE = 1,
	TTYPE_SOUND = 2,
	TTYPE_CLIPBOX = 3,
	TTYPE_SCRIPT = 4,
};

//////////////// Ʈ�� ���� ����ü

// ��� Ʈ�� ���� ����ü
struct TRACK_SCENE
{
	int   SceneNo;			// �׸� �� ��ȣ
	POINT Pos;				// �� ����� ��ġ
	int   RepeatCnt;		// �� �÷��� �ݺ�ȸ��
	int   CpiNum;			// ��Ʈ�� ����Ʈ ���� ����
	char  cbClipping;		// Ŭ���� �ڽ� ���� ����
	char  Alpha;			// ����
	short Angle;			// ȸ�� ����
	short ZoomX;			// ���� Ȯ�� ����
	short ZoomY;			// ���� Ȯ�� ����
	DWORD LifeTime;			// Ʈ���� ����(0�� ��� �ڵ����� RepeatCnt�� ����)

	short CenterX;			// �� �� ȸ���� �߽� X��(Pos ���� ��� ��ǥ)
	short CenterY;			// �� �� ȸ���� �߽� Y��(Pos ���� ��� ��ǥ)

	char  Reserved1[20];
};

// ���� Ʈ�� ���� ����ü
struct TRACK_SOUND
{
	char  SndFile[64];		// ���� ���� ��
	int   Volume;			// ������ ����
	char  Reserved[32];
};

// Ŭ�� �ڽ� Ʈ�� ���� ����ü
struct TRACK_CLIPBOX
{
	DWORD LifeTime;			// Ŭ���ڽ��� ����
	RECT  Rect;				// Ŭ�� �ڽ��� ��ǥ
	int   CpiNum;			// ��Ʈ�� ����Ʈ ���� ����
	char  Reserved[32];
};

// ��ũ��Ʈ Ʈ�� ���� ����ü
struct TRACK_SCRIPT
{
	char  ScriptName[32];	// ��ũ��Ʈ ��
	int   ScriptBufLen;		// ��ũ��Ʈ ��Ʈ�� ������ ����(NULL���� ����)
	char  Reserved[32];
};

//////////////// ��Ʈ�� ����Ʈ ���� ����ü

// ��� Ʈ�� ��Ʈ�� ����Ʈ ���� ����ü
struct CPI_SCENE
{
	DWORD DeltaTime;		// Ʈ�� ���� �ð����κ��� ������ ��� ��ġ
	POINT Pos;				// ������ ��ǥ
	char  Bias;				// ���̾
	char  Alpha;			// ����
	short Angle;			// ȸ�� ����
	short ZoomX;			// ���� Ȯ�� ����
	short ZoomY;			// ���� Ȯ�� ����
	char  Reserved1[8];
};
struct CPI_SCENE2
{
	DWORD DeltaTime;		// Ʈ�� ���� �ð����κ��� ������ ��� ��ġ
	FLOAT PosX;				// ������ ��ǥ
	FLOAT PosY;				// ������ ��ǥ
	char  Bias;				// ���̾
	char  Alpha;			// ����
	short Angle;			// ȸ�� ����
	short ZoomX;			// ���� Ȯ�� ����
	short ZoomY;			// ���� Ȯ�� ����
	char  Reserved1[8];
};
// Ŭ���ڽ� ��Ʈ�� ����Ʈ ���� ����ü
struct CPI_CLIPBOX
{
	DWORD DeltaTime;		// Ʈ�� ���� �ð����κ��� ������ ��� ��ġ
	RECT  Rect;				// �ڽ��� ������ ��ǥ
	char  Bias;				// ���̾
	char  Reserved1[3];
	char  Reserved2[12];
};


///////////////////////////////////////////////////////////////////////////////////////


class MSceneData;
class TrackData;


// xMultiScene Class
class NMGRAPHICDX_CLASS xMultiScene
{
public:
	////// ���� ����� ������

	int  MSceneVer;			// ��Ƽ�� ����
	int  TotMSceneNum;		// �� ��Ƽ�� ����
	char Reserved[32];

	////// Ŭ���� ���� ��� ������

	MSceneData** ppMScene;	// ��Ƽ�� �迭�� ������

	xMultiScene();
	~xMultiScene();
	void Destroy();

	// ���ϸ��̼� �����͸� �ε�, ���̺�
	BOOL Load(FILE* fp);
	BOOL Save(FILE* fp);

	// ��� �������� ������ ������ ����ϴ� �Լ���
	void OnDeleteScene(int scn);
	void OnMoveScene(int snum, int tnum);

	// ��Ƽ�� �߰�, ����, �̵�
	int AddMScene(char* name);
	void DelMScene(int num);
	void MovMScene(int num, int tnum);

	// �� ����� �����͸� ���ϴ� �Լ�
	MSceneData* GetMScenePt(char* name, int* pos=NULL);
	MSceneData* GetMScenePt(int num);
	TrackData* GetTrackPt(int MSceneNum, int TrackNum);
};


// MSceneData Class
class NMGRAPHICDX_CLASS MSceneData
{
public:
	////// ���� ����� ������

	char MSceneName[32];	// ��Ƽ�� �̸�
	int  TotTrackNum;		// �� Ʈ�� ����
	char  Reserved[32];

	////// Ŭ���� ���� ��� ������

	TrackData** ppTrack;	// Ʈ�� �迭�� ������
	DWORD TotPlayTime;		// �� �÷��� �ð�(1/1000�� ����)

	MSceneData();
	~MSceneData();

	void Destroy();

	void Set(char* name);
	int  AddEmptyTrack();
	int  AddTrack(TrackData* pTrack);
	void DelTrack(int num);
	void MovTrack(int num, int tnum);
	int  MovTracks(int totnum, int* plist, int tnum);
	int  CopyTracks(int totnum, int* plist, int tnum);
	TrackData* GetPtrTrack(int num);

	void UpdateAffectClipboxTrack();	// Ŭ���ڽ� Ʈ���� ���� �޴� Ʈ�� ������ ������Ʈ
};


// TrackData Class
class NMGRAPHICDX_CLASS TrackData
{
public:
	////// ���� ����� ������

	int   TrackType;			// Ʈ�� Ÿ��
	DWORD Delay;				// Ʈ�� ������
	int   DataSize;				// ������ ������
	char  Reserved[32];

	////// Ŭ���� ���� ��� ������

	BOOL  bHideTrack;			// Ʈ�� ���߱� ����
	void* pData;				// ������ ������

	// ��� Ʈ����
	int AffectClipboxTrack;		// ��鿡 ������ ��ġ�� Ŭ���ڽ� Ʈ�� ��ȣ
	CPI_SCENE* pCpiScene;		// ��� ��Ʈ�� ����Ʈ ����(��� Ʈ���� ���)

	// Ŭ���ڽ� Ʈ����
	CPI_CLIPBOX* pCpiClipbox;	// Ʈ�� �ڽ� ��Ʈ�� ����Ʈ ����(Ŭ���ڽ� Ʈ���� ���)

	// ��ũ��Ʈ Ʈ����
	char *pstrScript;			// ��ũ��Ʈ ����(��ũ��Ʈ Ʈ���� ���)

	TrackData();
	~TrackData();
	void ClearMember();
	void Destroy();

	void SetBase(ENUM_TRACKTYPE type);
	BOOL RebuildInfo();
	void CopyFrom(TrackData* pSrc);

	BOOL GetInterpol_Scene(DWORD playtime, CPI_SCENE* pCpi);
	BOOL GetInterpol_Scene(DWORD playtime, CPI_SCENE2* pCpi2);
	BOOL GetInterpol_Clipbox(DWORD playtime, CPI_CLIPBOX* pCpi);

	int  AddCpi_Scene(CPI_SCENE* pcpi);
	int  AddCpi_Clipbox(CPI_CLIPBOX* pcpi);
	BOOL SetCpi_Script(char* str);

	int  DelCpi_Scene(DWORD stime, DWORD etime);
	int  DelCpi_Clipbox(DWORD stime, DWORD etime);

	BOOL InsertDelay(DWORD tartime, DWORD delay);
	BOOL DeleteDelay(DWORD tartime, DWORD delay);

	TRACK_SCENE* GetDataScene();
	TRACK_SOUND* GetDataSound();

	BOOL Read(FILE* fp);
	BOOL Write(FILE* fp);
};