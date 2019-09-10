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

// ���̾� Define

#define LN_LAYER0		0		// ���̾�0 - ���ӿ����� ������ �ʴ� ���(���̳� �޽��������� ����)
#define LN_BACKGROUND1	1		// ©���� ���
#define LN_BACKGROUND2	2		// ��©���� ���
#define LN_BACKCOAT		3		// ��Ʈ(��)
#define LN_HEAD			4		// �Ӹ�
#define LN_BODY			5		// ���� -  (Ȧ��:���ڸ���  ¦��:���ڸ���)
#define LN_FACE			6		// ǥ��(��) - (�⺻1,���2,����3,ȭ��4,Ȳ��5)
#define LN_BACKHAIR		7		// ���(�޸Ӹ�)
#define LN_SHOES1		8		// �Ź�1
#define LN_SLACKS		9		// ����, ġ��
#define LN_SUPERTHEME	10		// �󱼾��� �׸�(�׸����̾�� ������ ���� �׸��� ����)
#define LN_SKIRT		11		// ġ��	- ����� ����Ī �����۵����� ��� -> ������ ���� ������ ���̾�� ��� ��
#define LN_JACKET		12		// ����
#define LN_SETCLOTHES	13		// ��Ʈ��
#define LN_THEME		14		// �׸���
#define LN_FRONTCOAT	15		// ��Ʈ(��)
#define LN_FRONTHAIR	16		// ���(�ոӸ�)
#define LN_ACCESSORY1 	17		// �Ǽ��縮1(�ͱݼӷ� - �Ͱ���, ����̵� ��ű�)
#define LN_ACCESSORY2 	18		// �Ǽ��縮2(�� ������1)
#define LN_ACCESSORY3 	19		// �Ǽ��縮3(�� ������2)
#define LN_ACCESSORY4 	20		// �Ǽ��縮4(�Ȱ� Ȥ�� ����ũ, Ű����ũ ���)
#define LN_ACCESSORY5	21		// �Ǽ��縮5(©���� �Ǽ��縮 ������)
#define LN_ACCESSORY6	22		// �Ǽ��縮6(�ֿϵ����� �μտ� ������)
#define LN_LAYER23		23		// ���̾�23
#define LN_LAYER24		24		// ���̾�24
#define LN_LAYER25		25		// ���̾�25 (������ �����ܿ� - ������ �ʰ� �׻� �ȹٷ� ���)

// ��������Ʈ ���ҽ� �޴���
static CResMan ResMan;

// ���̾� ���� ����ü
struct LAYERINFO
{
	CSprFile*	pSprFile;
	int			LayerItem;
	WORD		NowFrameTick;
};


// �ƹ�Ÿ ���ҽ� Ŭ����
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


// DLL �ڵ� �ı� Ŭ����
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
// ���� ��� �Լ���

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

	// GIF�� ��������Ʈ�� ��ȯ
	BOOL bRetValue = ConvertGif2SprFromMemory(pDest, dwSize, pSprite);

	GlobalFree(hGlobal);
	return bRetValue;
}


BOOL OpenSprToLayer(LAYERINFO* pLInfo, int layer, int itemno, BOOL bSmall)
{
	if(!pLInfo) return FALSE;

	// ������ �ִ� ��������Ʈ Ŭ���� �����͸� ����(������ �������� �ʴ´�)
	CSprFile* pOldSprFile = pLInfo->pSprFile;

	pLInfo->pSprFile = NULL;
	pLInfo->NowFrameTick = 0;
	pLInfo->LayerItem = 0;

	if(itemno > 0) {
		// ������ ��ȣ�� ����
		pLInfo->LayerItem = itemno;
		// ��������Ʈ�� �����Ѵ�
		pLInfo->pSprFile = ResMan.OpenSpr(layer, itemno, bSmall);
	}

	if(pOldSprFile) {
		// �̹� �ٸ� ��������Ʈ�� ���µǾ� �־��ٸ� �����Ѵ�
		// ���߿� �����ϴ� ������ ���� ���µǴ� ������ ���� ���ŵǴ� ���ϰ� ���� ��� 
		// ���� ī���͸� �����Ͽ� ������ ���µǵ��� �ϱ� ����
		ResMan.CloseSpr(pOldSprFile);
	}

	if(!pLInfo->pSprFile) return FALSE;
	else return TRUE;
}

BOOL PrepareResource(CAvatarResource* pRes, int *item, BOOL bSmall)
{
	if(!pRes || !item) return FALSE;

	// ��� ���̾��� 0�� ���̾ �ϴ� ������(�������� ������ �ٿ�ε�)
	for(int i=0; i<MAX_AVATARLAYER; i++) {
		// item��ȣ�� 0���̾ ȣ��(���� ���ҽ��� �����ϱ� ����)
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
	
	// �ӽ� Ŭ������ �����Ǿ� �ִٸ�
	if(bTempClip) {
		opt |= TEMPCLIP;
	}

	// ������ ��� ����̸�
	if(bMirror) {
		// 2009.02.27 �̷� ��� ���� ���� By �����
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
			// ��������Ʈ�� ���ε� �ؾ��� �����̶�� �ٽ� �ε�(�÷��״� �ʱ�ȭ)
			pRes->LInfo[i].pSprFile->m_bReloadSpr = FALSE;
			// ��������Ʈ ���ε�
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
					// �� ������ ƽ�� 0�� ���� ��� 1msec�� 1�������� �׸����� �Ѵ�
					pRes->LInfo[i].NowFrameTick %= totspn;
				}

				// �ð��� ���� ���� �����ӹ�ȣ�� ���
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

			// 25�� ���̾�� ������ �ʰ� ������ �������� �׸�(������ �����ܿ� ���̾�)
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

		// ��������Ʈ�� �ε����̶�� ǥ��

		opt &= ~MIRROR; // �̷� �ɼ��� ����

		static UINT tottick = 0;
		if(tottick == 0) {
			for(int i=0; i<LoadingSpr.sfhead.TotalSprNum; i++) 
				tottick += LoadingSpr.spr[i].byUserVal * 100;
		}

		int totspn = LoadingSpr.sfhead.TotalSprNum;
		int spn = 0;

		if(totspn > 1 && tottick > 0) {
			// �ð��� ���� ���� �����ӹ�ȣ�� ���
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

	// �׷��� ���̺귯�� �ʱ�ȭ
	InitGrpLib(555);
	InitAlphaTable(555);

	// �⺻ ���丮 ����
	strcpy(BaseAvatarDir, pstrBaseDir);

	// ������ �ʱ�ȭ
	LocalPage.Init(DEFAVATAR_WIDTH, DEFAVATAR_HEIGHT, 16);

	// ���ҽ� ������ �ʱ�ȭ
	ResMan.Init(BaseAvatarDir);

	// �ε� ��������Ʈ �б�
	OpenLoadingSprite(&LoadingSpr);

	// �ڵ� �ı��� �ʱ�ȭ
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
	
	// �������� �ִٸ� 0���� ó���Ѵ�
	for(int i=0; i<MAX_AVATARLAYER; i++) 
		if(item[i] < 0) item[i] = 0;
		
		// �⺻���� ��Ұ� ���ǵ��� �ʾҴٸ� �����ȣ�� �������� �⺻ ������
		if(item[LN_HEAD] <= 0) item[LN_HEAD] = defitem;
		if(item[LN_FACE] <= 0) item[LN_FACE] = ((defitem==1)?(1):(6));	// ǥ���� 5���� �׷���
		if(item[LN_FRONTHAIR] <= 0) item[LN_FRONTHAIR] = defitem;
		
		// ���� �׸����� �Ծ��ٸ� Ư�� ���̾�ܿ��� �׸��� �ʴ´�(�Ϲ� �׸����� �켱��)
		if(item[LN_SUPERTHEME]) {
			for(int i=LN_BACKCOAT; i<=LN_ACCESSORY4; i++) {
				if(i != LN_SUPERTHEME) item[i]=0;
			}
		}
		
		// �׸����� �Ծ��ٸ� Ư�� ���̾�ܿ��� �׸��� �ʴ´�
		if(item[LN_THEME] > 0) {
			for(int i=LN_BACKCOAT; i<=LN_ACCESSORY4; i++) {
				if(!(i==LN_HEAD || i==LN_FACE || i==LN_THEME)) item[i]=0;
			}
		}
		// ��Ʈ���� ������ �����Ǹ� �׸��� �ʴ´�
		if(item[LN_SETCLOTHES] > 0) {
			item[LN_JACKET] = 0;
			item[LN_SLACKS] = 0;
			//item[LN_SKIRT] = 0; // ���� ���̾�� ����ϴ� ������ ����
		}
		// ����, ��Ʈ��, �׸�, �����׸����� ������ �⺻ ���Ǹ� ������.
		if(item[LN_JACKET]==0 && item[LN_SETCLOTHES]==0 && item[LN_THEME]==0 && item[LN_SUPERTHEME]==0) {
			item[LN_JACKET] = defitem;
		}
		// ����, ġ��, ��Ʈ��, �׸�, �����׸����� ������ �⺻ ���Ǹ� ������.
		//if(item[LN_SLACKS]==0 && item[LN_SKIRT]==0 && item[LN_SETCLOTHES]==0 && item[LN_THEME]==0 && item[LN_SUPERTHEME]==0) {
		// ����, ��Ʈ��, �׸�, �����׸����� ������ �⺻ ���Ǹ� ������.
		if(item[LN_SLACKS]==0 && item[LN_SETCLOTHES]==0 && item[LN_THEME]==0 && item[LN_SUPERTHEME]==0) {
			item[LN_SLACKS] = defitem;
		}
		
		// ������ �׷����� �����۵�(���� ���̾ ������ ������ ��ȣ�� �����Ѵ�)
		item[LN_BACKHAIR] = item[LN_FRONTHAIR];
		item[LN_BACKCOAT] = item[LN_FRONTCOAT];
		
		// ū �ƹ�Ÿ �̹����� �غ�
		return PrepareResource(h, item, FALSE);
}

 BOOL SetAvatarUnsignedShort(HAVATAR h, unsigned short *pLayerItem)
{
	if(!pLayerItem) return FALSE;

	int item[MAX_AVATARLAYER];

	// �������� �ִٸ� 0���� ó���Ѵ�
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

// 	// �������� �ִٸ� 0���� ó���Ѵ�
// 	for(int i=0; i<MAX_AVATARLAYER; i++) 
// 		if(item[i] < 0) item[i] = 0;

	// ������ �׷����� �����۵�(���� ���̾ ������ ������ ��ȣ�� �����Ѵ�)
	if(bAutoPair) {
		item[LN_BACKHAIR] = item[LN_FRONTHAIR];
		item[LN_BACKCOAT] = item[LN_FRONTCOAT];
	}

	// ū �ƹ�Ÿ �̹����� �غ�
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
		// ���� �׸����� �Ծ��ٸ� �ٸ� ���̾�� ��� �׸��� ����
		item[LN_SUPERTHEME] = pLayerItem[LN_SUPERTHEME];
	}
	else {
		item[LN_HEAD]      = max(0, pLayerItem[LN_HEAD]);
		item[LN_FACE]      = max(0, pLayerItem[LN_FACE]);
		item[LN_FRONTHAIR] = max(0, pLayerItem[LN_FRONTHAIR]);
		item[LN_THEME]     = max(0, pLayerItem[LN_THEME]);

		// �⺻���� ��Ұ� ���ǵ��� �ʾҴٸ� �����ȣ�� �������� �⺻ ������
		if(item[LN_HEAD] <= 0) item[LN_HEAD] = defitem;
		if(item[LN_FACE] <= 0) item[LN_FACE] = ((defitem==1)?(1):(6));	// ǥ���� 5���� �׷���
		if(item[LN_FRONTHAIR] <= 0) item[LN_FRONTHAIR] = defitem;

		if(item[LN_THEME] > 0) {
			// �Ϲ� �׸����� �Ծ��ٸ� �ոӸ� ���̾�� �׸��� ����(�׸� ���̾ �׸��Ե�)
			item[LN_FRONTHAIR] = 0;
		}
	}

	// ���� �ƹ�Ÿ �̹����� �غ�
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
	
	// ���� ������Ʈ �����
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
	
	// ����ũ�� ���� ��� ��Ʈ�� ����
	HBITMAP hbmpMask = ::CreateBitmap(nWidth, nHeight, 1, 1, NULL);
	if(!hbmpMask) return FALSE;
	hOldMaskBmp = (HBITMAP)::SelectObject(hdcMask, hbmpMask);
	::SetBkColor(hdcMem, colorTransparent);
	
	// �޸� DC�κ��� ����ũ ����
	::BitBlt(hdcMask, 0, 0, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY );
	
	// memDC�� ��׶��带 ���������� �����Ѵ�.
	// ���������� SRCPAINT�� ����Ͽ� �ٸ����� ��Ÿ������ ����� �������� ������̵ȴ�.
	::SetBkColor(hdcMem, RGB(0,0,0));
	::SetTextColor(hdcMem, RGB(255,255,255));
	::BitBlt(hdcMem, 0, 0, nWidth, nHeight, hdcMask, 0, 0, SRCAND);
	
	// ���׶��带 ���������� �����Ѵ�
	::SetBkColor(hdcDest, RGB(255,255,255));
	::SetTextColor(hdcDest, RGB(0,0,0));
	::BitBlt(hdcDest, xp, yp, nWidth, nHeight, hdcMask, 0, 0, SRCAND);
	
	// ���׶���� ��׶��带 ����
	::BitBlt(hdcDest, xp, yp, nWidth, nHeight, hdcMem, 0, 0, SRCPAINT);

	if(hOldMaskBmp) ::SelectObject(hdcMask, hOldMaskBmp );
	if(hOldMemBmp) ::SelectObject(hdcMem, hOldMemBmp );
	if(hOldTempBmp) ::SelectObject(hdcTemp, hOldTempBmp);

	// ����� ���� DC �� ��Ʈ�� ����
	::DeleteDC(hdcMask);
	::DeleteDC(hdcMem);
	::DeleteDC(hdcTemp);
	::DeleteObject(hbmpImage);
	::DeleteObject(hbmpMask);

	return TRUE;
}
*/

// �ణ �� ���� ����
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
		// ��׶��� �̹����� ��Ʈ���� �������� ���� ���� �÷�Ű�� �����Ͽ� �ƹ�Ÿ ���(�ӵ��� ������)
		//

		if(nWidth > LocalPage.width) nWidth = LocalPage.width;
		if(nHeight > LocalPage.height) nHeight = LocalPage.height;

		LocalPage.DrawFillBox(0, 0, LocalPage.width, LocalPage.height, RGBmix(31,0,31));
		DrawAvatarToPage(&LocalPage, ox, oy, AvatarRes[h], bMirror, FALSE, 555);
		
		COLORREF colorTransparent = RGB(255,0,255);
		
		// ���� ������Ʈ �����
		HBITMAP hOldMaskBmp = NULL;
		HBITMAP hOldMemBmp = NULL;

		HDC hdcMask = ::CreateCompatibleDC(hdcDest);
		if(!hdcMask) return FALSE;
		HDC hdcMem = ::CreateCompatibleDC(hdcDest);
		if(!hdcMem) return FALSE;

		hOldMemBmp = (HBITMAP)::SelectObject(hdcMem, LocalPage.hBmp);
		
		// ����ũ�� ���� ��� ��Ʈ�� ����
		HBITMAP hbmpMask = ::CreateBitmap(nWidth, nHeight, 1, 1, NULL);
		if(!hbmpMask) return FALSE;
		hOldMaskBmp = (HBITMAP)::SelectObject(hdcMask, hbmpMask);
		::SetBkColor(hdcMem, colorTransparent);
		
		// �޸� DC�κ��� ����ũ ����
		::BitBlt(hdcMask, 0, 0, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY );
		
		// memDC�� ��׶��带 ���������� �����Ѵ�.
		// ���������� SRCPAINT�� ����Ͽ� �ٸ����� ��Ÿ������ ����� �������� ������̵ȴ�.
		::SetBkColor(hdcMem, RGB(0,0,0));
		::SetTextColor(hdcMem, RGB(255,255,255));
		::BitBlt(hdcMem, 0, 0, nWidth, nHeight, hdcMask, 0, 0, SRCAND);
		
		// ���׶��带 ���������� �����Ѵ�
		::SetBkColor(hdcDest, RGB(255,255,255));
		::SetTextColor(hdcDest, RGB(0,0,0));
		::BitBlt(hdcDest, xp, yp, nWidth, nHeight, hdcMask, 0, 0, SRCAND);
		
		// ���׶���� ��׶��带 ����
		::BitBlt(hdcDest, xp, yp, nWidth, nHeight, hdcMem, 0, 0, SRCPAINT);

		if(hOldMaskBmp) ::SelectObject(hdcMask, hOldMaskBmp );
		if(hOldMemBmp) ::SelectObject(hdcMem, hOldMemBmp );

		// ����� ���� DC �� ��Ʈ�� ����
		::DeleteDC(hdcMask);
		::DeleteDC(hdcMem);
		::DeleteObject(hbmpMask);
	}
	else {
		//
		// ��׶��� �̹����� ��Ʈ���� ������ ���� ������� ��Ʈ���� ���
		//

		/*
		HDC hdcMem = ::CreateCompatibleDC(hdcDest);
		if(!hdcMem) return FALSE;

		HBITMAP hOldDestBmp = (HBITMAP)::SelectObject(hdcDest, LocalPage.hBmp);

		// ��� ��Ʈ�� �̹��� ����
		HBITMAP hOldMemBmp = (HBITMAP)::SelectObject(hdcMem, AvatarRes[h]->hBackImage);
		::BitBlt(hdcDest, xp, yp, nWidth, nHeight, hdcMem, AvatarRes[h]->BackImageSrcX, AvatarRes[h]->BackImageSrcY, SRCCOPY );

		// �ƹ�Ʈ�� �׸�
		DrawAvatarToPage(&LocalPage, ox, oy, AvatarRes[h], bMirror, FALSE, 555);

		::SelectObject(hdcMem, hOldMemBmp );
		::SelectObject(hdcDest, hOldDestBmp );

		// ����� ���� DC �� ��Ʈ�� ����
		::DeleteDC(hdcMem);
		*/

		HDC hdcMem = ::CreateCompatibleDC(hdcDest);
		if(!hdcMem) return FALSE;
		HDC hdcMem2 = ::CreateCompatibleDC(hdcMem);
		if(!hdcMem2) return FALSE;


		HBITMAP hOldMemBmp = (HBITMAP)::SelectObject(hdcMem, LocalPage.hBmp);

		// ��� ��Ʈ�� �̹��� ����
		HBITMAP hOldMem2Bmp = (HBITMAP)::SelectObject(hdcMem2, AvatarRes[h]->hBackImage);
		::BitBlt(hdcMem, 0, 0, nWidth, nHeight, hdcMem2, AvatarRes[h]->BackImageSrcX, AvatarRes[h]->BackImageSrcY, SRCCOPY );

		// �ƹ�Ʈ�� �׸�
		DrawAvatarToPage(&LocalPage, ox, oy, AvatarRes[h], bMirror, FALSE, 555);
		
		::BitBlt(hdcDest, xp, yp, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY );

		::SelectObject(hdcMem, hOldMemBmp );
		::SelectObject(hdcMem2, hOldMem2Bmp );

		// ����� ���� DC �� ��Ʈ�� ����
		::DeleteDC(hdcMem);
		::DeleteDC(hdcMem2);
	}

	return TRUE;
}

// �ƹ�Ÿ�� DC�� �׸��� ���ϴ� ������� �ε巯�� Ȯ�� ��Ҹ� �����Ͽ� �׸�
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

	// ����� ���� DC �� ��Ʈ�� ����
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

	// Ȱ������ ������ ���� ���ϱ�
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