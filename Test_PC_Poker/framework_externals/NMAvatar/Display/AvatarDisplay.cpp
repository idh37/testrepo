// AvatarDisplay.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "../resource.h"

#include "AvatarDisplay.h"
#include "Graphic.h"
#include "mmsystem.h"

#include "ResMan.h"
#include "PictureEx.h"
#include "Gif2Spr.h"
#include "MyMisc.h"

#define MAX_AVATARLAYER		26
#define MAX_AVATARRESOURCE	512

#define DEFAVATAR_WIDTH		75
#define DEFAVATAR_HEIGHT	125
#define SMALLAVATAR_WIDTH	25
#define SMALLAVATAR_HEIGHT	25

//#define FIRSTVISIBLE_LAYER	1

//#pragma comment(lib, "winmm.lib")

// 레이어 Define

#define LN_LAYER0		0		// 레이어0 - 게임에서는 보이지 않는 배경(웹이나 메신저에서만 보임)
#define LN_BACKGROUND1	1		// 짤리는 배경
#define LN_BACKGROUND2	2		// 안짤리는 배경
#define LN_BACKCOAT		3		// 코트(속)
#define LN_HEAD			4		// 머리
#define LN_BODY			5		// 몸통 -  (홀수:남자몸통  짝수:여자몸통)
#define LN_FACE			6		// 표정(얼굴) - (기본1,기쁜2,슬픈3,화난4,황당5)
#define LN_BACKHAIR		7		// 헤어(뒷머리)
#define LN_SHOES1		8		// 신발1
#define LN_SLACKS		9		// 바지, 치마
#define LN_SUPERTHEME	10		// 얼굴없는 테마(테마레이어와 같으나 얼굴을 그리지 않음)
#define LN_SKIRT		11		// 치마	- 현재는 볼터칭 아이템등으로 사용 -> 지금은 부츠 아이템 레이어로 사용 중
#define LN_JACKET		12		// 상의
#define LN_SETCLOTHES	13		// 세트복
#define LN_THEME		14		// 테마복
#define LN_FRONTCOAT	15		// 코트(겉)
#define LN_FRONTHAIR	16		// 헤어(앞머리)
#define LN_ACCESSORY1 	17		// 악세사리1(귀금속류 - 귀걸이, 목걸이등 장신구)
#define LN_ACCESSORY2 	18		// 악세사리2(손 아이템1)
#define LN_ACCESSORY3 	19		// 악세사리3(손 아이템2)
#define LN_ACCESSORY4 	20		// 악세사리4(안경 혹은 마스크, 키스마크 등등)
#define LN_ACCESSORY5	21		// 악세사리5(짤리는 악세사리 아이템)
#define LN_ACCESSORY6	22		// 악세사리6(애완동물겸 두손용 아이템)
#define LN_LAYER23		23		// 레이어23
#define LN_LAYER24		24		// 레이어24
#define LN_LAYER25		25		// 레이어25 (리더스 아이콘용 - 뒤집지 않고 항상 똑바로 출력)

// 스프라이트 리소스 메니저
static CResMan ResMan;

// 레이어 정보 구조체
struct LAYERINFO
{
	CSprFile*	pSprFile;
	int			LayerItem;
	WORD		NowFrameTick;
};


// 아바타 리소스 클래스
class CAvatarResource
{
public:

	LAYERINFO	LInfo[MAX_AVATARLAYER];
	BOOL		bSmall;
	BOOL		bBackgroundVisible;
	HBITMAP		hBackImage;
	int			BackImageSrcX;
	int			BackImageSrcY;

	DWORD		LastDrawTick;
	
	int			Width, Height;
	int			OffXp, OffYp;

	CAvatarResource() 
	{
		ClearMember();
	}
	
	~CAvatarResource()
	{
		Destroy();
	}

	void Destroy() 
	{
		for(int i=0; i<MAX_AVATARLAYER; i++) 
		{
			if(LInfo[i].pSprFile) 
			{
				ResMan.CloseSpr(LInfo[i].pSprFile);
				LInfo[i].pSprFile = NULL;
			}
		}
		ClearMember();
	}

	void ClearMember()
	{
		bSmall = FALSE;
		bBackgroundVisible = FALSE;
		hBackImage = NULL;
		BackImageSrcX = 0;
		BackImageSrcY = 0;
		Width = DEFAVATAR_WIDTH;
		Height = DEFAVATAR_HEIGHT;
		OffXp = OffYp = 0;
		memset(LInfo, 0, sizeof(LAYERINFO) * MAX_AVATARLAYER);
		LastDrawTick = 0;
	}
};

static CAvatarResource* AvatarRes[MAX_AVATARRESOURCE] = {NULL,};
static char BaseAvatarDir[512]={0,};
static CPage LocalPage;
static CPage AttachPage;
static xSprite LoadingSpr;

static BOOL g_bAvatarInited = FALSE;


// DLL 자동 파괴 클래스
class CAutoDestroy
{
public:
	BOOL m_bInited;

	CAutoDestroy() { 
		m_bInited = FALSE; 
	}

	~CAutoDestroy() { 
		if(!m_bInited) return;
		DestroyAvatarModule();
	}
	
	void Init() { 
		m_bInited = TRUE; 
	}

	void DestroyAvatarModule() {
		DestroyAvatarDisplayDLL();
	}
};

static CAutoDestroy g_AutoDestroy;

////////////////////////////////////////////////////////////////////////////////
// 내부 사용 함수들

BOOL OpenLoadingSprite(xSprite *pSprite)
{
	LPCTSTR szResourceName = MAKEINTRESOURCE(IDR_GIF_LOADING);
	LPCTSTR szResourceType = _T("GIF");

	HINSTANCE h = AfxGetResourceHandle();
	HRSRC hPicture = FindResource(AfxGetResourceHandle(),szResourceName,szResourceType);
	HGLOBAL hResData;
	if (!hPicture || !(hResData = LoadResource(AfxGetResourceHandle(),hPicture)))
	{
		TRACE(_T("OpenLoadingSprite(): Error loading resource\n"));
		return FALSE;
	};
	DWORD dwSize = SizeofResource(AfxGetResourceHandle(),hPicture);

	// hResData is not the real HGLOBAL (we can't lock it)
	// let's make it real

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD,dwSize);
	if (!hGlobal)
	{
		TRACE(_T("OpenLoadingSprite(): Error allocating memory\n"));
		FreeResource(hResData);
		return FALSE;
	};
	
	char *pDest = reinterpret_cast<char *> (GlobalLock(hGlobal));
	char *pSrc = reinterpret_cast<char *> (LockResource(hResData));
	if (!pSrc || !pDest)
	{
		TRACE(_T("OpenLoadingSprite(): Error locking memory\n"));
		GlobalFree(hGlobal);
		FreeResource(hResData);
		return FALSE;
	};
	CopyMemory(pDest,pSrc,dwSize);
	FreeResource(hResData);
	GlobalUnlock(hGlobal);

	// GIF를 스프라이트로 변환
	BOOL bRetValue = ConvertGif2SprFromMemory(pDest, dwSize, pSprite);

	GlobalFree(hGlobal);
	return bRetValue;
}


BOOL OpenSprToLayer(LAYERINFO* pLInfo, int layer, int itemno, BOOL bSmall)
{
	if(!pLInfo) return FALSE;

	// 이전에 있던 스프라이트 클래스 포인터를 저장(지금은 제거하지 않는다)
	CSprFile* pOldSprFile = pLInfo->pSprFile;

	pLInfo->pSprFile = NULL;
	pLInfo->NowFrameTick = 0;
	pLInfo->LayerItem = 0;

	if(itemno > 0) {
		// 아이템 번호를 저장
		pLInfo->LayerItem = itemno;
		// 스프라이트를 오픈한다
		pLInfo->pSprFile = ResMan.OpenSpr(layer, itemno, bSmall);
	}

	if(pOldSprFile) {
		// 이미 다른 스프라이트가 오픈되어 있었다면 제거한다
		// 나중에 제거하는 이유는 새로 오픈되는 파일이 지금 제거되는 파일과 같은 경우 
		// 참조 카운터만 증가하여 빠르게 오픈되도록 하기 위함
		ResMan.CloseSpr(pOldSprFile);
	}

	if(!pLInfo->pSprFile) return FALSE;
	else return TRUE;
}

BOOL PrepareResource(CAvatarResource* pRes, int *item, BOOL bSmall)
{
	if(!pRes || !item) return FALSE;

	// 배경 레이어인 0번 레이어도 일단 오픈함(존재하지 않으면 다운로드)
	for(int i=0; i<MAX_AVATARLAYER; i++) {
		// item번호가 0번이어도 호출(기존 리소스를 해제하기 위함)
		OpenSprToLayer(&pRes->LInfo[i], i, item[i], bSmall);
	}

	if(bSmall) {
		pRes->bSmall = TRUE;
		pRes->Width = SMALLAVATAR_WIDTH;
		pRes->Height = SMALLAVATAR_HEIGHT;
	}
	else {
		pRes->bSmall = FALSE;
		pRes->Width = DEFAVATAR_WIDTH;
		pRes->Height = DEFAVATAR_HEIGHT;
	}

	return TRUE;
}

BOOL PrepareResource(HAVATAR h, int *item, BOOL bSmall)
{
	if(h<1 || h>=MAX_AVATARRESOURCE)
		return FALSE;

	PrepareResource(AvatarRes[h], item, bSmall);

	return TRUE;
}

BOOL DrawAvatarToPage(CPage *pPage, int xp, int yp, CAvatarResource *pRes, BOOL bMirror, BOOL bTempClip, int TargetPixelFormat)
{
	if(!pPage || !pRes) return FALSE;

	int opt = RXY;
	
	// 임시 클리핑이 설정되어 있다면
	if(bTempClip) {
		opt |= TEMPCLIP;
	}

	// 뒤집어 찍기 모드이면
	if(bMirror) {
		// 2009.02.27 미러 모드 오류 수정 By 허재민
		pPage->SetMirrorXEx( xp, (pRes->bSmall ? SMALLAVATAR_WIDTH : DEFAVATAR_WIDTH));

		//pPage->SetMirrorX( xp + ((pRes->bSmall)?(SMALLAVATAR_WIDTH/2):(DEFAVATAR_WIDTH/2)) );
		opt |= MIRROR;
	}

	DWORD nowtick = timeGetTime();
	DWORD elapsetick = nowtick - pRes->LastDrawTick;

	BOOL bDrawLoading = FALSE;

	int firstlayer = 1;
	if(pRes->bBackgroundVisible) firstlayer = 0;

	for(int i=firstlayer; i<MAX_AVATARLAYER; i++) {

		if(!pRes->LInfo[i].pSprFile) continue;

		if(pRes->LInfo[i].LayerItem > 0 && pRes->LInfo[i].pSprFile->m_bReloadSpr) {
			// 스프라이트를 리로드 해야할 조건이라면 다시 로드(플래그는 초기화)
			pRes->LInfo[i].pSprFile->m_bReloadSpr = FALSE;
			// 스프라이트 리로드
			int layer = pRes->LInfo[i].pSprFile->m_Layer;
			int itemno = pRes->LInfo[i].pSprFile->m_ItemNo;
			BOOL bSmall = pRes->LInfo[i].pSprFile->m_bSmall;
			CString sprfname = GetFullpathOfSprFile(BaseAvatarDir, layer, itemno, bSmall);
			pRes->LInfo[i].pSprFile->LoadSprite(sprfname);
		}

		if(pRes->LInfo[i].pSprFile->m_bNowDownLoading)
			bDrawLoading = TRUE;

		int totspn = pRes->LInfo[i].pSprFile->m_Spr.sfhead.TotalSprNum;

		if(pRes->LInfo[i].LayerItem > 0 && totspn > 0) {
			int spn = 0;
			if(pRes->LastDrawTick > 0 && totspn > 1) {

				WORD TotFrameTick = (WORD)pRes->LInfo[i].pSprFile->m_TotFrameTick;

				pRes->LInfo[i].NowFrameTick += (WORD)elapsetick;
				if(TotFrameTick > 0) {
					pRes->LInfo[i].NowFrameTick %= TotFrameTick;
				}
				else {
					// 총 프레임 틱이 0인 경우는 적어도 1msec당 1프레임은 그리도록 한다
					pRes->LInfo[i].NowFrameTick %= totspn;
				}

				// 시간에 따른 현재 프레임번호를 계산
				DWORD tick = 0;
				for(int s = 0; s < totspn; s++) {
					tick += max(10, pRes->LInfo[i].pSprFile->m_Spr.spr[s].byUserVal * 100);
					if(tick >= pRes->LInfo[i].NowFrameTick) {
						spn = s;
						break;
					}
				}
			}

			int orgopt = opt;

			// 25번 레이어는 뒤집지 않고 무조건 정상으로 그림(리더스 아이콘용 레이어)
			if(i == 25) {
				opt &= (~(MIRROR));
			}

			pPage->PutSprAuto(xp, yp, &pRes->LInfo[i].pSprFile->m_Spr, spn, opt, 0, 0, TargetPixelFormat);

			opt = orgopt;
		}
	}

	pRes->LastDrawTick = nowtick;

	/////////////////////

	if(!pRes->bSmall && bDrawLoading) {

		// 스프라이트가 로딩중이라면 표시

		opt &= ~MIRROR; // 미러 옵션을 뺀다

		static UINT tottick = 0;
		if(tottick == 0) {
			for(int i=0; i<LoadingSpr.sfhead.TotalSprNum; i++) 
				tottick += LoadingSpr.spr[i].byUserVal * 100;
		}

		int totspn = LoadingSpr.sfhead.TotalSprNum;
		int spn = 0;

		if(totspn > 1 && tottick > 0) {
			// 시간에 따른 현재 프레임번호를 계산
			DWORD nowtick = timeGetTime() % tottick;
			DWORD tick = 0;
			for(int i = 0; i < totspn; i++) {
				tick += max(10, LoadingSpr.spr[i].byUserVal * 100);
				if(tick >= nowtick) {
					spn = i;
					break;
				}
			}
		}

		pPage->PutSprAuto(xp, yp, &LoadingSpr, spn, opt, 0, 0, TargetPixelFormat);
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////


BOOL InitAvatarDisplayDLL(const char *pstrBaseDir)
{
	if(!pstrBaseDir) return FALSE;

	// 그래픽 라이브러리 초기화
	InitGrpLib(555);
	InitAlphaTable(555);

	// 기본 디렉토리 설정
	strcpy(BaseAvatarDir, pstrBaseDir);

	// 페이지 초기화
	LocalPage.Init(DEFAVATAR_WIDTH, DEFAVATAR_HEIGHT, 16);

	// 리소스 관리자 초기화
	ResMan.Init(BaseAvatarDir);

	// 로딩 스프라이트 읽기
	OpenLoadingSprite(&LoadingSpr);

	// 자동 파괴자 초기화
	g_AutoDestroy.Init();

	g_bAvatarInited = TRUE;

	return TRUE;
}

 BOOL DestroyAvatarDisplayDLL()
{
	if(!g_bAvatarInited) return FALSE;

	LocalPage.Destroy();

	for(UINT i=1; i<MAX_AVATARRESOURCE; i++) {
		if(AvatarRes[i]) {
			delete AvatarRes[i];
			AvatarRes[i] = NULL;
		}
	}
		
	ResMan.Destroy();

	LoadingSpr.Remove();

	g_bAvatarInited = FALSE;

	return TRUE;
}

 HAVATAR CreateAvatarResource()
{
	HAVATAR h = 0;
	for(UINT i=1; i<MAX_AVATARRESOURCE; i++) {
		if(AvatarRes[i] == NULL) {
			AvatarRes[i] = new CAvatarResource;
			if(AvatarRes[i] == NULL) break;
			h = i;
			break;
		}
	}
	return h;
}

 BOOL DestroyAvatarResource(HAVATAR h)
{
	if(h<1 || h>=MAX_AVATARRESOURCE) return FALSE;
	if(!AvatarRes[h]) return FALSE;
	delete AvatarRes[h];
	AvatarRes[h] = NULL;
	return TRUE;
}

BOOL raw_SetAvatar(HAVATAR h, int *pLayerItem)
{
	if(h<1 || h>=MAX_AVATARRESOURCE) return FALSE;
	if(!AvatarRes[h]) return FALSE;
	if(!pLayerItem) return FALSE;
	if(strlen(BaseAvatarDir) == 0) return FALSE;
	
	int item[MAX_AVATARLAYER];
	memcpy(item, pLayerItem, sizeof(item));
	short defitem = ((item[LN_BODY]%2==1)?(1):(2));
	
	// 음수값이 있다면 0으로 처리한다
	for(int i=0; i<MAX_AVATARLAYER; i++) 
		if(item[i] < 0) item[i] = 0;
		
		// 기본적인 요소가 정의되지 않았다면 몸통번호를 기준으로 기본 설정함
		if(item[LN_HEAD] <= 0) item[LN_HEAD] = defitem;
		if(item[LN_FACE] <= 0) item[LN_FACE] = ((defitem==1)?(1):(6));	// 표정은 5개씩 그룹임
		if(item[LN_FRONTHAIR] <= 0) item[LN_FRONTHAIR] = defitem;
		
		// 슈퍼 테마복을 입었다면 특정 레이어외에는 그리지 않는다(일반 테마복에 우선함)
		if(item[LN_SUPERTHEME]) {
			for(int i=LN_BACKCOAT; i<=LN_ACCESSORY4; i++) {
				if(i != LN_SUPERTHEME) item[i]=0;
			}
		}
		
		// 테마복을 입었다면 특정 레이어외에는 그리지 않는다
		if(item[LN_THEME] > 0) {
			for(int i=LN_BACKCOAT; i<=LN_ACCESSORY4; i++) {
				if(!(i==LN_HEAD || i==LN_FACE || i==LN_THEME)) item[i]=0;
			}
		}
		// 세트복이 있으면 상하의를 그리지 않는다
		if(item[LN_SETCLOTHES] > 0) {
			item[LN_JACKET] = 0;
			item[LN_SLACKS] = 0;
			//item[LN_SKIRT] = 0; // 부츠 레이어로 사용하는 것으로 변경
		}
		// 상의, 세트복, 테마, 슈퍼테마복이 없으면 기본 상의를 입힌다.
		if(item[LN_JACKET]==0 && item[LN_SETCLOTHES]==0 && item[LN_THEME]==0 && item[LN_SUPERTHEME]==0) {
			item[LN_JACKET] = defitem;
		}
		// 하의, 치마, 세트복, 테마, 슈퍼테마복이 없으면 기본 하의를 입힌다.
		//if(item[LN_SLACKS]==0 && item[LN_SKIRT]==0 && item[LN_SETCLOTHES]==0 && item[LN_THEME]==0 && item[LN_SUPERTHEME]==0) {
		// 하의, 세트복, 테마, 슈퍼테마복이 없으면 기본 하의를 입힌다.
		if(item[LN_SLACKS]==0 && item[LN_SETCLOTHES]==0 && item[LN_THEME]==0 && item[LN_SUPERTHEME]==0) {
			item[LN_SLACKS] = defitem;
		}
		
		// 쌍으로 그려지는 아이템들(나중 레이어에 찍히는 아이템 번호를 기준한다)
		item[LN_BACKHAIR] = item[LN_FRONTHAIR];
		item[LN_BACKCOAT] = item[LN_FRONTCOAT];
		
		// 큰 아바타 이미지를 준비
		return PrepareResource(h, item, FALSE);
}

 BOOL SetAvatarUnsignedShort(HAVATAR h, unsigned short *pLayerItem)
{
	if(!pLayerItem) return FALSE;

	int item[MAX_AVATARLAYER];

	// 음수값이 있다면 0으로 처리한다
	for(int i=0; i<MAX_AVATARLAYER; i++) 
		item[i] = pLayerItem[i];

	return raw_SetAvatar(h, item);
}

 BOOL SetAvatar(HAVATAR h, short *pLayerItem)
{
	return SetAvatarUnsignedShort(h, (unsigned short*)pLayerItem);
}

 BOOL SetForceAvatar(HAVATAR h, short *pLayerItem, BOOL bAutoPair)
{
	if(h<1 || h>=MAX_AVATARRESOURCE) return FALSE;
	if(!AvatarRes[h]) return FALSE;
	if(!pLayerItem) return FALSE;
	if(strlen(BaseAvatarDir) == 0) return FALSE;

	int item[MAX_AVATARLAYER];

	for(int i=0; i<MAX_AVATARLAYER; i++) 
		item[i] = (unsigned short)pLayerItem[i];

// 	// 음수값이 있다면 0으로 처리한다
// 	for(int i=0; i<MAX_AVATARLAYER; i++) 
// 		if(item[i] < 0) item[i] = 0;

	// 쌍으로 그려지는 아이템들(나중 레이어에 찍히는 아이템 번호를 기준한다)
	if(bAutoPair) {
		item[LN_BACKHAIR] = item[LN_FRONTHAIR];
		item[LN_BACKCOAT] = item[LN_FRONTCOAT];
	}

	// 큰 아바타 이미지를 준비
	return PrepareResource(h, item, FALSE);
}

 BOOL SetSmallAvatarEx(HAVATAR h, unsigned short *pLayerItem)
{
	if(h<1 || h>=MAX_AVATARRESOURCE) return FALSE;
	if(!AvatarRes[h]) return FALSE;
	if(!pLayerItem) return FALSE;
	if(strlen(BaseAvatarDir) == 0) return FALSE;

	int item[MAX_AVATARLAYER] = {0,};

	short defitem = ((pLayerItem[LN_BODY]%2==1)?(1):(2));

	if(pLayerItem[LN_SUPERTHEME] > 0) {
		// 슈퍼 테마복을 입었다면 다른 레이어는 모두 그리지 않음
		item[LN_SUPERTHEME] = pLayerItem[LN_SUPERTHEME];
	}
	else {
		item[LN_HEAD]      = max(0, pLayerItem[LN_HEAD]);
		item[LN_FACE]      = max(0, pLayerItem[LN_FACE]);
		item[LN_FRONTHAIR] = max(0, pLayerItem[LN_FRONTHAIR]);
		item[LN_THEME]     = max(0, pLayerItem[LN_THEME]);

		// 기본적인 요소가 정의되지 않았다면 몸통번호를 기준으로 기본 설정함
		if(item[LN_HEAD] <= 0) item[LN_HEAD] = defitem;
		if(item[LN_FACE] <= 0) item[LN_FACE] = ((defitem==1)?(1):(6));	// 표정은 5개씩 그룹임
		if(item[LN_FRONTHAIR] <= 0) item[LN_FRONTHAIR] = defitem;

		if(item[LN_THEME] > 0) {
			// 일반 테마복을 입었다면 앞머리 레이어는 그리지 않음(테마 레이어만 그리게됨)
			item[LN_FRONTHAIR] = 0;
		}
	}

	// 작은 아바타 이미지를 준비
	return PrepareResource(h, item, TRUE);
}

 BOOL SetSmallAvatar(HAVATAR h, short *pLayerItem)
{
	return SetSmallAvatarEx(h, (unsigned short *)pLayerItem);
}

 BOOL SetBackgroundVisible(HAVATAR h, BOOL bShow)
{
	if(h<1 || h>=MAX_AVATARRESOURCE) return FALSE;
	if(!AvatarRes[h]) return FALSE;
	AvatarRes[h]->bBackgroundVisible = bShow;
	return TRUE;
}

 BOOL SetAvatarSize(HAVATAR h, int width, int height)
{
	if(h<1 || h>=MAX_AVATARRESOURCE) return FALSE;
	if(!AvatarRes[h]) return FALSE;
	AvatarRes[h]->Width = width;
	AvatarRes[h]->Height = height;
	return TRUE;
}

 BOOL SetAvatarOffset(HAVATAR h, int ox, int oy)
{
	if(h<1 || h>=MAX_AVATARRESOURCE) return FALSE;
	if(!AvatarRes[h]) return FALSE;
	AvatarRes[h]->OffXp = ox;
	AvatarRes[h]->OffYp = oy;
	return TRUE;
}

 BOOL ResetAvatarAnimation(HAVATAR h)
{
	if(h<1 || h>=MAX_AVATARRESOURCE) return FALSE;
	if(!AvatarRes[h]) return FALSE;
	for(int i=0; i<MAX_AVATARLAYER; i++) AvatarRes[h]->LInfo[i].NowFrameTick = 0;
	AvatarRes[h]->LastDrawTick = 0;
	return TRUE;
}

 BOOL SetAvatarAnimationTime(HAVATAR h, WORD tick)
{
	if(h<1 || h>=MAX_AVATARRESOURCE) return FALSE;
	if(!AvatarRes[h]) return FALSE;
	for(int i=0; i<MAX_AVATARLAYER; i++) AvatarRes[h]->LInfo[i].NowFrameTick = tick;
	return TRUE;
}

 BOOL SetAvatarBackBitmap(HAVATAR h, HBITMAP hBmp, int srcx, int srcy)
{
	if(h<1 || h>=MAX_AVATARRESOURCE) return FALSE;
	if(!AvatarRes[h]) return FALSE;
	AvatarRes[h]->hBackImage = hBmp;
	AvatarRes[h]->BackImageSrcX = srcx;
	AvatarRes[h]->BackImageSrcY = srcy;
	return TRUE;
}

 BOOL IsAvatarDownloading(HAVATAR h)
{
	if(h<1 || h>=MAX_AVATARRESOURCE) return FALSE;
	if(!AvatarRes[h]) return FALSE;

	int firstlayer = 1;
	if(AvatarRes[h]->bBackgroundVisible) firstlayer = 0;

	for(int i=firstlayer; i<MAX_AVATARLAYER; i++) {
		if(!AvatarRes[h]->LInfo[i].pSprFile) continue;
		if(AvatarRes[h]->LInfo[i].pSprFile->m_bNowDownLoading)
			return TRUE;
	}

	return FALSE;
}

 BOOL DrawAvatarToMemory(int xp, int yp, HAVATAR h, LONG txl, LONG tyl, void *lpbit, LONG lpitch, BOOL bMirror, int TargetPixelFormat)
{
	if(h<1 || h>=MAX_AVATARRESOURCE) return FALSE;
	if(!AvatarRes[h]) return FALSE;
	if(AttachPage.Attach(txl, tyl, (WORD*)lpbit, lpitch)==FALSE) return FALSE;
	int ox = AvatarRes[h]->OffXp;
	int oy = AvatarRes[h]->OffYp;
	BOOL bTempClip = FALSE;
	if(AvatarRes[h]->Width != DEFAVATAR_WIDTH || AvatarRes[h]->Height != DEFAVATAR_HEIGHT) {
		bTempClip = TRUE;
		AttachPage.SetTempClip(xp, yp, xp+AvatarRes[h]->Width-1, yp+AvatarRes[h]->Height-1);
	}
	BOOL rtn = DrawAvatarToPage(&AttachPage, xp+ox, yp+oy, AvatarRes[h], bMirror, bTempClip, TargetPixelFormat);
	AttachPage.Detach();
	return rtn;
}

/*
 BOOL DrawAvatarToDC(int xp, int yp, HAVATAR h, HDC hdcDest, BOOL bMirror)
{
	if(!hdcDest) return FALSE;
	if(h<1 || h>=MAX_AVATARRESOURCE) return FALSE;
	if(!AvatarRes[h]) return FALSE;

	int ox = AvatarRes[h]->OffXp;
	int oy = AvatarRes[h]->OffYp;

	int nWidth = AvatarRes[h]->Width;
	int nHeight = AvatarRes[h]->Height;

	if(nWidth > LocalPage.width) nWidth = LocalPage.width;
	if(nHeight > LocalPage.height) nHeight = LocalPage.height;

	LocalPage.DrawFillBox(0, 0, LocalPage.width, LocalPage.height, RGBmix(31,0,31));
	DrawAvatarToPage(&LocalPage, ox, oy, AvatarRes[h], bMirror, FALSE);
	
	COLORREF colorTransparent = RGB(255,0,255);
	
	// 이전 오브젝트 저장용
	HBITMAP hOldMaskBmp = NULL;
	HBITMAP hOldMemBmp = NULL;
	HBITMAP hOldTempBmp = NULL;

	HDC hdcMask = ::CreateCompatibleDC(hdcDest);
	if(!hdcMask) return FALSE;
	HDC hdcMem = ::CreateCompatibleDC(hdcDest);
	if(!hdcMem) return FALSE;
	HDC hdcTemp = ::CreateCompatibleDC(hdcDest);
	if(!hdcTemp) return FALSE;

	HBITMAP hbmpImage = ::CreateCompatibleBitmap(hdcDest, nWidth, nHeight);
	if(!hbmpImage) return FALSE;
	hOldMemBmp = (HBITMAP)::SelectObject(hdcMem, hbmpImage);
	hOldTempBmp = (HBITMAP)::SelectObject(hdcTemp, LocalPage.hBmp);
	::BitBlt(hdcMem, 0, 0, nWidth, nHeight, hdcTemp, 0, 0, SRCCOPY);
	
	// 마스크를 위한 흑백 비트맵 생성
	HBITMAP hbmpMask = ::CreateBitmap(nWidth, nHeight, 1, 1, NULL);
	if(!hbmpMask) return FALSE;
	hOldMaskBmp = (HBITMAP)::SelectObject(hdcMask, hbmpMask);
	::SetBkColor(hdcMem, colorTransparent);
	
	// 메모리 DC로부터 마스크 생성
	::BitBlt(hdcMask, 0, 0, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY );
	
	// memDC의 백그라운드를 검은색으로 세팅한다.
	// 검은색으로 SRCPAINT를 사용하여 다른색은 나타나도록 만들어 검은색이 투명색이된다.
	::SetBkColor(hdcMem, RGB(0,0,0));
	::SetTextColor(hdcMem, RGB(255,255,255));
	::BitBlt(hdcMem, 0, 0, nWidth, nHeight, hdcMask, 0, 0, SRCAND);
	
	// 포그라운드를 검은색으로 세팅한다
	::SetBkColor(hdcDest, RGB(255,255,255));
	::SetTextColor(hdcDest, RGB(0,0,0));
	::BitBlt(hdcDest, xp, yp, nWidth, nHeight, hdcMask, 0, 0, SRCAND);
	
	// 포그라운드로 백그라운드를 융합
	::BitBlt(hdcDest, xp, yp, nWidth, nHeight, hdcMem, 0, 0, SRCPAINT);

	if(hOldMaskBmp) ::SelectObject(hdcMask, hOldMaskBmp );
	if(hOldMemBmp) ::SelectObject(hdcMem, hOldMemBmp );
	if(hOldTempBmp) ::SelectObject(hdcTemp, hOldTempBmp);

	// 사용이 끝난 DC 및 비트맵 해제
	::DeleteDC(hdcMask);
	::DeleteDC(hdcMem);
	::DeleteDC(hdcTemp);
	::DeleteObject(hbmpImage);
	::DeleteObject(hbmpMask);

	return TRUE;
}
*/

// 약간 더 빠른 버전
 BOOL DrawAvatarToDC(int xp, int yp, HAVATAR h, HDC hdcDest, BOOL bMirror)
{
	if(!hdcDest) return FALSE;
	if(h<1 || h>=MAX_AVATARRESOURCE) return FALSE;
	if(!AvatarRes[h]) return FALSE;

	int ox = AvatarRes[h]->OffXp;
	int oy = AvatarRes[h]->OffYp;

	int nWidth = AvatarRes[h]->Width;
	int nHeight = AvatarRes[h]->Height;

	if(AvatarRes[h]->hBackImage == NULL) {
		//
		// 백그라운드 이미지용 비트맵이 설정되지 않은 경우는 컬러키를 적용하여 아바타 출력(속도가 느려짐)
		//

		if(nWidth > LocalPage.width) nWidth = LocalPage.width;
		if(nHeight > LocalPage.height) nHeight = LocalPage.height;

		LocalPage.DrawFillBox(0, 0, LocalPage.width, LocalPage.height, RGBmix(31,0,31));
		DrawAvatarToPage(&LocalPage, ox, oy, AvatarRes[h], bMirror, FALSE, 555);
		
		COLORREF colorTransparent = RGB(255,0,255);
		
		// 이전 오브젝트 저장용
		HBITMAP hOldMaskBmp = NULL;
		HBITMAP hOldMemBmp = NULL;

		HDC hdcMask = ::CreateCompatibleDC(hdcDest);
		if(!hdcMask) return FALSE;
		HDC hdcMem = ::CreateCompatibleDC(hdcDest);
		if(!hdcMem) return FALSE;

		hOldMemBmp = (HBITMAP)::SelectObject(hdcMem, LocalPage.hBmp);
		
		// 마스크를 위한 흑백 비트맵 생성
		HBITMAP hbmpMask = ::CreateBitmap(nWidth, nHeight, 1, 1, NULL);
		if(!hbmpMask) return FALSE;
		hOldMaskBmp = (HBITMAP)::SelectObject(hdcMask, hbmpMask);
		::SetBkColor(hdcMem, colorTransparent);
		
		// 메모리 DC로부터 마스크 생성
		::BitBlt(hdcMask, 0, 0, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY );
		
		// memDC의 백그라운드를 검은색으로 세팅한다.
		// 검은색으로 SRCPAINT를 사용하여 다른색은 나타나도록 만들어 검은색이 투명색이된다.
		::SetBkColor(hdcMem, RGB(0,0,0));
		::SetTextColor(hdcMem, RGB(255,255,255));
		::BitBlt(hdcMem, 0, 0, nWidth, nHeight, hdcMask, 0, 0, SRCAND);
		
		// 포그라운드를 검은색으로 세팅한다
		::SetBkColor(hdcDest, RGB(255,255,255));
		::SetTextColor(hdcDest, RGB(0,0,0));
		::BitBlt(hdcDest, xp, yp, nWidth, nHeight, hdcMask, 0, 0, SRCAND);
		
		// 포그라운드로 백그라운드를 융합
		::BitBlt(hdcDest, xp, yp, nWidth, nHeight, hdcMem, 0, 0, SRCPAINT);

		if(hOldMaskBmp) ::SelectObject(hdcMask, hOldMaskBmp );
		if(hOldMemBmp) ::SelectObject(hdcMem, hOldMemBmp );

		// 사용이 끝난 DC 및 비트맵 해제
		::DeleteDC(hdcMask);
		::DeleteDC(hdcMem);
		::DeleteObject(hbmpMask);
	}
	else {
		//
		// 백그라운드 이미지용 비트맵이 설정된 경우는 배경으로 비트맵을 사용
		//

		/*
		HDC hdcMem = ::CreateCompatibleDC(hdcDest);
		if(!hdcMem) return FALSE;

		HBITMAP hOldDestBmp = (HBITMAP)::SelectObject(hdcDest, LocalPage.hBmp);

		// 배경 비트맵 이미지 복사
		HBITMAP hOldMemBmp = (HBITMAP)::SelectObject(hdcMem, AvatarRes[h]->hBackImage);
		::BitBlt(hdcDest, xp, yp, nWidth, nHeight, hdcMem, AvatarRes[h]->BackImageSrcX, AvatarRes[h]->BackImageSrcY, SRCCOPY );

		// 아바트를 그림
		DrawAvatarToPage(&LocalPage, ox, oy, AvatarRes[h], bMirror, FALSE, 555);

		::SelectObject(hdcMem, hOldMemBmp );
		::SelectObject(hdcDest, hOldDestBmp );

		// 사용이 끝난 DC 및 비트맵 해제
		::DeleteDC(hdcMem);
		*/

		HDC hdcMem = ::CreateCompatibleDC(hdcDest);
		if(!hdcMem) return FALSE;
		HDC hdcMem2 = ::CreateCompatibleDC(hdcMem);
		if(!hdcMem2) return FALSE;


		HBITMAP hOldMemBmp = (HBITMAP)::SelectObject(hdcMem, LocalPage.hBmp);

		// 배경 비트맵 이미지 복사
		HBITMAP hOldMem2Bmp = (HBITMAP)::SelectObject(hdcMem2, AvatarRes[h]->hBackImage);
		::BitBlt(hdcMem, 0, 0, nWidth, nHeight, hdcMem2, AvatarRes[h]->BackImageSrcX, AvatarRes[h]->BackImageSrcY, SRCCOPY );

		// 아바트를 그림
		DrawAvatarToPage(&LocalPage, ox, oy, AvatarRes[h], bMirror, FALSE, 555);
		
		::BitBlt(hdcDest, xp, yp, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY );

		::SelectObject(hdcMem, hOldMemBmp );
		::SelectObject(hdcMem2, hOldMem2Bmp );

		// 사용이 끝난 DC 및 비트맵 해제
		::DeleteDC(hdcMem);
		::DeleteDC(hdcMem2);
	}

	return TRUE;
}

// 아바타를 DC에 그리되 원하는 사이즈로 부드러운 확대 축소를 적용하여 그림
 BOOL DrawAvatarToDC_SmoothZoom(int xp, int yp, HAVATAR h, HDC hdcDest, int tx, int ty, COLORREF crBackColor, BOOL bMirror)
{
	if(!hdcDest) return FALSE;
	if(h<1 || h>=MAX_AVATARRESOURCE) return FALSE;
	if(!AvatarRes[h]) return FALSE;

	int ox = AvatarRes[h]->OffXp;
	int oy = AvatarRes[h]->OffYp;

	int nWidth = AvatarRes[h]->Width;
	int nHeight = AvatarRes[h]->Height;

	if(nWidth > LocalPage.width) nWidth = LocalPage.width;
	if(nHeight > LocalPage.height) nHeight = LocalPage.height;

	static CPage TarPage, SrcPage;

	if(!TarPage.lpBit) {
		TarPage.Init(tx, ty, 16);
	}

	if(TarPage.lpBit && (TarPage.width != tx || TarPage.height != ty)) {
		TarPage.Destroy();
		TarPage.Init(tx, ty, 16);
	}

	if(!SrcPage.lpBit) {
		SrcPage.Init(nWidth, nHeight, 16);
	}

	if(SrcPage.lpBit && (SrcPage.width != nWidth || SrcPage.height != nHeight)) {
		SrcPage.Destroy();
		SrcPage.Init(nWidth, nHeight, 16);
	}

	WORD wBackColor = RGBmix(GetRValue(crBackColor)>>3, GetGValue(crBackColor)>>3, GetBValue(crBackColor)>>3);
	SrcPage.DrawFillBox(0, 0, SrcPage.width, SrcPage.height, wBackColor);
	DrawAvatarToPage(&SrcPage, ox, oy, AvatarRes[h], bMirror, FALSE, 555);

	TarPage.SmoothZoomFromPage(0, 0, &SrcPage);

	HDC hdcMem = ::CreateCompatibleDC(hdcDest);
	if(!hdcMem) return FALSE;

	HBITMAP hOldMemBmp = (HBITMAP)::SelectObject(hdcMem, TarPage.hBmp);

	::BitBlt(hdcDest, xp, yp, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY);

	if(hOldMemBmp) ::SelectObject(hdcMem, hOldMemBmp );

	// 사용이 끝난 DC 및 비트맵 해제
	::DeleteDC(hdcMem);

	return TRUE;
}

 char* GetAvatarStatusString()
{
	static CString sStatus;
	CString str;

	sStatus = "";

	if(!g_bAvatarInited) {
		sStatus += "Avatar-Display module is not initialized.\n";
		return (char*)((LPCTSTR)sStatus);
	}

	// 활동중인 쓰레드 개수 구하기
	int working_thread_num = 0;
	for(int i=0; i<MAX_DOWNTHREAD; i++) {
		if(ResMan.m_DownThread[i].m_bBusy) working_thread_num++;
	}

	int tot_downlist_filenum = 0; // == ResMan.m_FileList.m_DownMap.GetCount();
	int work_downlist_filenum = 0;
	int finished_downlist_filenum = 0;

	///////////////////////////////

	ResMan.m_FileList.Lock();	////// LOCK

	POSITION pos = ResMan.m_FileList.m_DownMap.GetStartPosition();
	while (pos != NULL)
	{
		CString sKey;
		CFileInfo* pFileInfo;
		ResMan.m_FileList.m_DownMap.GetNextAssoc(pos, sKey, pFileInfo);
		if(pFileInfo) {
			if(ResMan.m_FileList.m_DownMap[sKey]->m_WorkStatus == 1) work_downlist_filenum++;
			if(ResMan.m_FileList.m_DownMap[sKey]->m_WorkStatus == 100) finished_downlist_filenum++;
		}
		tot_downlist_filenum++;
	}

	ResMan.m_FileList.Unlock();	////// UNLOCK

	////////////////////////////////

	sStatus += ">>> AVATAR DISPLAY DLL STATUS <<<\n";
	str.Format("* Avatar download URL : %s\n", DEFAULT_REMOTEURL); sStatus += str;

	sStatus += "\n>>> ROADED RESOURCE STATUS <<<\n";
	str.Format("* Opened sprite file num : %d\n", ResMan.m_ResList.GetCount()); sStatus += str;

	sStatus += "\n>>> DOWNLOAD WORKING STATUS <<<\n";
	str.Format("* Working thread num : %d/%d\n", working_thread_num, MAX_DOWNTHREAD); sStatus += str;
	str.Format("* Download list file num : %d(work %d, finished %d)\n", tot_downlist_filenum, work_downlist_filenum, finished_downlist_filenum); sStatus += str;

	return (char*)((LPCTSTR)sStatus);
}