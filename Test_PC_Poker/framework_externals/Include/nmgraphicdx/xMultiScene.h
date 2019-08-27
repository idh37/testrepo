
#pragma once

#include "GraphicDXDefine.h"


#define MULTISCENE_VERSION	101


// 트랙 타입
enum ENUM_TRACKTYPE
{
	TTYPE_NONE = 0,
	TTYPE_SCENE = 1,
	TTYPE_SOUND = 2,
	TTYPE_CLIPBOX = 3,
	TTYPE_SCRIPT = 4,
};

//////////////// 트랙 정보 구조체

// 장면 트랙 정보 구조체
struct TRACK_SCENE
{
	int   SceneNo;			// 그릴 씬 번호
	POINT Pos;				// 씬 드로잉 위치
	int   RepeatCnt;		// 씬 플레이 반복회수
	int   CpiNum;			// 컨트롤 포인트 정보 개수
	char  cbClipping;		// 클리핑 박스 적용 여부
	char  Alpha;			// 투명도
	short Angle;			// 회전 각도
	short ZoomX;			// 가로 확대 비율
	short ZoomY;			// 세로 확대 비율
	DWORD LifeTime;			// 트랙의 수명(0일 경우 자동으로 RepeatCnt에 따름)

	short CenterX;			// 줌 및 회전의 중심 X축(Pos 기준 상대 좌표)
	short CenterY;			// 줌 및 회전의 중심 Y축(Pos 기준 상대 좌표)

	char  Reserved1[20];
};

// 사운드 트랙 정보 구조체
struct TRACK_SOUND
{
	char  SndFile[64];		// 사운드 파일 명
	int   Volume;			// 연주할 볼륨
	char  Reserved[32];
};

// 클립 박스 트랙 정보 구조체
struct TRACK_CLIPBOX
{
	DWORD LifeTime;			// 클립박스의 수명
	RECT  Rect;				// 클립 박스의 좌표
	int   CpiNum;			// 컨트롤 포인트 정보 개수
	char  Reserved[32];
};

// 스크립트 트랙 정보 구조체
struct TRACK_SCRIPT
{
	char  ScriptName[32];	// 스크립트 명
	int   ScriptBufLen;		// 스크립트 스트링 버퍼의 길이(NULL문자 포함)
	char  Reserved[32];
};

//////////////// 컨트롤 포인트 정보 구조체

// 장면 트랙 컨트롤 포인트 정보 구조체
struct CPI_SCENE
{
	DWORD DeltaTime;		// 트랙 시작 시간으로부터 떨어진 상대 위치
	POINT Pos;				// 움직일 좌표
	char  Bias;				// 바이어스
	char  Alpha;			// 투명도
	short Angle;			// 회전 각도
	short ZoomX;			// 가로 확대 비율
	short ZoomY;			// 세로 확대 비율
	char  Reserved1[8];
};
struct CPI_SCENE2
{
	DWORD DeltaTime;		// 트랙 시작 시간으로부터 떨어진 상대 위치
	FLOAT PosX;				// 움직일 좌표
	FLOAT PosY;				// 움직일 좌표
	char  Bias;				// 바이어스
	char  Alpha;			// 투명도
	short Angle;			// 회전 각도
	short ZoomX;			// 가로 확대 비율
	short ZoomY;			// 세로 확대 비율
	char  Reserved1[8];
};
// 클립박스 컨트롤 포인트 정보 구조체
struct CPI_CLIPBOX
{
	DWORD DeltaTime;		// 트랙 시작 시간으로부터 떨어진 상대 위치
	RECT  Rect;				// 박스의 움직일 좌표
	char  Bias;				// 바이어스
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
	////// 파일 저장용 데이터

	int  MSceneVer;			// 멀티씬 버전
	int  TotMSceneNum;		// 총 멀티씬 개수
	char Reserved[32];

	////// 클래스 내부 사용 데이터

	MSceneData** ppMScene;	// 멀티씬 배열용 포인터

	xMultiScene();
	~xMultiScene();
	void Destroy();

	// 에니메이션 데이터를 로드, 세이브
	BOOL Load(FILE* fp);
	BOOL Save(FILE* fp);

	// 장면 데이터의 변동이 있을때 사용하는 함수들
	void OnDeleteScene(int scn);
	void OnMoveScene(int snum, int tnum);

	// 멀티씬 추가, 삭제, 이동
	int AddMScene(char* name);
	void DelMScene(int num);
	void MovMScene(int num, int tnum);

	// 각 노드의 포인터를 구하는 함수
	MSceneData* GetMScenePt(char* name, int* pos=NULL);
	MSceneData* GetMScenePt(int num);
	TrackData* GetTrackPt(int MSceneNum, int TrackNum);
};


// MSceneData Class
class NMGRAPHICDX_CLASS MSceneData
{
public:
	////// 파일 저장용 데이터

	char MSceneName[32];	// 멀티씬 이름
	int  TotTrackNum;		// 총 트랙 개수
	char  Reserved[32];

	////// 클래스 내부 사용 데이터

	TrackData** ppTrack;	// 트랙 배열용 포인터
	DWORD TotPlayTime;		// 총 플레이 시간(1/1000초 단위)

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

	void UpdateAffectClipboxTrack();	// 클립박스 트랙에 영향 받는 트랙 정보를 업데이트
};


// TrackData Class
class NMGRAPHICDX_CLASS TrackData
{
public:
	////// 파일 저장용 데이터

	int   TrackType;			// 트랙 타입
	DWORD Delay;				// 트랙 딜레이
	int   DataSize;				// 데이터 사이즈
	char  Reserved[32];

	////// 클래스 내부 사용 데이터

	BOOL  bHideTrack;			// 트랙 감추기 여부
	void* pData;				// 데이터 포인터

	// 장면 트랙용
	int AffectClipboxTrack;		// 장면에 영향을 미치는 클립박스 트랙 번호
	CPI_SCENE* pCpiScene;		// 장면 컨트롤 포인트 정보(장면 트랙인 경우)

	// 클립박스 트랙용
	CPI_CLIPBOX* pCpiClipbox;	// 트랙 박스 컨트롤 포인트 정보(클립박스 트랙인 경우)

	// 스크립트 트랙용
	char *pstrScript;			// 스크립트 내용(스크립트 트랙인 경우)

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