
#include "StdAfx.h"
#include <io.h>

#include "xMultiScene.h"




#include <math.h>
#define TO_RADIAN(x) ((x) * 0.0174773f)
#define FLOATROUNDINT(x) ((int)((x)+0.5f))


//////////////////////////////////// xMultiScene Class ///////////////////////////////////


#define MIN_MFRAMENUM 4



xMultiScene::xMultiScene()
{
	MSceneVer = 0;
	TotMSceneNum = 0;
	memset(Reserved, 0, sizeof(Reserved));

	ppMScene = NULL;
}

xMultiScene::~xMultiScene()
{
	Destroy();
}

void xMultiScene::Destroy()
{
	if(ppMScene) 
	{
		for(int i=0; i<TotMSceneNum; i++) 
			if(ppMScene[i] != NULL) { delete(ppMScene[i]); ppMScene[i]=NULL; }
			free(ppMScene);
			ppMScene= NULL;
	}

	TotMSceneNum = 0;
}

int xMultiScene::AddMScene(char* name)
{
	int rtn = TotMSceneNum;

	TotMSceneNum += 1;

	MSceneData** ppNew = (MSceneData**) malloc(sizeof(MSceneData*) * TotMSceneNum);

	if(ppMScene) 
	{
		memcpy(ppNew, ppMScene, sizeof(MSceneData*) * (TotMSceneNum-1));
		free(ppMScene);
	}
	ppMScene = ppNew;

	ppMScene[TotMSceneNum-1] = new MSceneData;
	(ppMScene[TotMSceneNum-1])->Set(name);

	return rtn;
}

void xMultiScene::DelMScene(int num)
{
	if(TotMSceneNum <= 0) return;
	if(num < 0 || num >= TotMSceneNum) return;
	if(TotMSceneNum == 1) { Destroy(); return; }

	TotMSceneNum -= 1;
	MSceneData** ppNew = (MSceneData**) malloc(sizeof(MSceneData*) * TotMSceneNum);

	int i;
	for(i=0; i<num; i++) ppNew[i] = ppMScene[i];
	for(i=num; i<TotMSceneNum; i++) ppNew[i] = ppMScene[i+1];

	delete(ppMScene[num]);
	if(ppMScene) free(ppMScene);
	ppMScene = ppNew;
}

void xMultiScene::MovMScene(int num, int tnum)
{
	if(TotMSceneNum <= 1) return;
	if(tnum < 0 || tnum >= TotMSceneNum) return;
	if(num < 0 || num >= TotMSceneNum) return;
	if(num == tnum) return;

	MSceneData** ppNew = (MSceneData**) malloc(sizeof(MSceneData*) * TotMSceneNum);

	int t,s;
	for(t=0, s=0; t<TotMSceneNum; t++, s++)
	{
		if(t == tnum) t++;
		if(s == num) s++;
		if(t < TotMSceneNum) ppNew[t] = ppMScene[s];
	}
	ppNew[tnum] = ppMScene[num];

	if(ppMScene) free(ppMScene);
	ppMScene = ppNew;
}

// 멀티씬의 포인터를 이름으로 얻는다(*pos = 인덱스 번호를 받을 포인터)
MSceneData* xMultiScene::GetMScenePt(char* name, int* pos)
{
	if(TotMSceneNum==0) return NULL;
	int i, Num=-1;
	for(i=0; i<TotMSceneNum; i++)
		if(stricmp(name,(ppMScene[i])->MSceneName)==0) { Num=i; break; }

		if(Num == -1) return NULL;

		if(pos!=NULL) *pos=Num;
		return ppMScene[Num];
}

// 멀티씬의 포인터를 인덱스로 얻는다 
MSceneData* xMultiScene::GetMScenePt(int num)
{
	if(num < 0 || num >= TotMSceneNum) return NULL;
	return ppMScene[num];
}

TrackData* xMultiScene::GetTrackPt(int MSceneNum, int TrackNum)
{
	if(MSceneNum < 0 || MSceneNum >= TotMSceneNum) return NULL;
	if(TrackNum < 0 || TrackNum >= ppMScene[MSceneNum]->TotTrackNum) return NULL;
	return ppMScene[MSceneNum]->ppTrack[TrackNum];
}

// 멀티씬 데이터를 읽어들인다
BOOL xMultiScene::Load(FILE* fp)
{	
	Destroy();

	long flen = _filelength(_fileno(fp));
	fpos_t pos;
	if(fgetpos(fp, &pos) == 0) {
		// 멀티씬 데이터 공간이 있는지 검사
		if(pos >= flen-8)
			return FALSE;
	}
	else {
		// 멀티씬 데이터가 존재하지 않는다
		return FALSE;
	}

	int n0=0, n1=0;
	int cnt0=0, cnt1=0;

	// 멀티씬 데이터의 헤더를 읽음
	char idstr[8] = {0,};
	fread(idstr, 8, 1, fp);
	if(strncmp(idstr, "MULTISCN", 8) != 0) {
		// 멀티씬 데이터가 아니므로 파일 포인터를 원래 위치로 되돌림
		fsetpos(fp, &pos);
		return FALSE;
	}

	fread(&MSceneVer, sizeof(MSceneVer), 1, fp);
	fread(&cnt0, sizeof(TotMSceneNum), 1, fp);
	fread(Reserved, sizeof(Reserved), 1, fp);

	if(MSceneVer < 100 || MSceneVer > MULTISCENE_VERSION) {
		// 버전이 일치하지 않음
		return FALSE;
	}

	for(n0=0; n0<cnt0; n0++) // 멀티씬
	{		
		MSceneData scene;

		fread(scene.MSceneName, sizeof(scene.MSceneName), 1, fp);
		fread(&scene.TotTrackNum, sizeof(scene.TotTrackNum), 1, fp);
		fread(scene.Reserved, sizeof(scene.Reserved), 1, fp);

		AddMScene(scene.MSceneName);

		cnt1 = scene.TotTrackNum;

		for(n1=0; n1<cnt1; n1++) // 트랙
		{
			int trackno = GetMScenePt(n0)->AddEmptyTrack();
			if(!GetMScenePt(n0)->GetPtrTrack(trackno)->Read(fp)) {
				Destroy();
				return FALSE;
			}

			if(MSceneVer == 100) {
				// 버전이 낮으면 상위 버전으로 컨버팅
				TrackData* pTrack = GetMScenePt(n0)->GetPtrTrack(trackno);
				if(pTrack->TrackType == TTYPE_SCENE) {
					TRACK_SCENE* pSceneData = (TRACK_SCENE*)pTrack->pData;
					pSceneData->Alpha = 31;
					pSceneData->ZoomX = 100;
					pSceneData->ZoomY = 100;
					pSceneData->Angle = 0;
					pSceneData->LifeTime = 0;

					for(int i=0; i<pSceneData->CpiNum; i++) {
						pTrack->pCpiScene[i].Alpha = 31;
						pTrack->pCpiScene[i].ZoomX = 100;
						pTrack->pCpiScene[i].ZoomY = 100;
						pTrack->pCpiScene[i].Angle = 0;
					}
				}
			}

			GetMScenePt(n0)->UpdateAffectClipboxTrack();
		}
	}

	return TRUE;
}

// 멀티씬 데이터를 파일에 저장한다(받은 파일포인터에 이어서 기록)
BOOL xMultiScene::Save(FILE* fp)
{
	if(TotMSceneNum == 0) return TRUE;

	MSceneVer = MULTISCENE_VERSION;

	// 장면 파일 헤더 저장
	char idstr[8+1] = {0,};
	strncpy(idstr, "MULTISCN", 8);
	fwrite(idstr, 8, 1, fp);
	fwrite(&MSceneVer, sizeof(MSceneVer), 1, fp);
	fwrite(&TotMSceneNum, sizeof(TotMSceneNum), 1, fp);
	fwrite(Reserved, sizeof(Reserved), 1, fp);

	int n0, n1;
	int cnt0, cnt1;

	cnt0 = TotMSceneNum;

	for(n0=0; n0<cnt0; n0++) // 멀티씬
	{		
		MSceneData* pMScene = GetMScenePt(n0);
		if(pMScene == NULL) return FALSE;

		fwrite(pMScene->MSceneName, sizeof(pMScene->MSceneName), 1, fp);
		fwrite(&pMScene->TotTrackNum, sizeof(pMScene->TotTrackNum), 1, fp);
		fwrite(pMScene->Reserved, sizeof(pMScene->Reserved), 1, fp);

		cnt1 = pMScene->TotTrackNum;

		for(n1=0; n1<cnt1; n1++) // 트랙
		{
			TrackData* pTrack = GetTrackPt(n0, n1);
			if(pTrack == NULL) return FALSE;
			pTrack->Write(fp);
		}
	}

	return TRUE;
}

// 장면이 삭제될때 멀티씬 정보도 갱신시킨다
void xMultiScene::OnDeleteScene(int scn)
{
	int n0, n1;
	int cnt0, cnt1;

	cnt0 = TotMSceneNum;

	for(n0=0; n0<cnt0; n0++) // 멀티씬
	{		
		MSceneData* pScene = GetMScenePt(n0);
		if(pScene == NULL) continue;
		cnt1 = pScene->TotTrackNum;

		for(n1=0; n1<cnt1; n1++) // 트랙
		{
			TrackData* pTrack = GetTrackPt(n0, n1);
			if(pTrack == NULL) continue;

			if(pTrack->TrackType == TTYPE_SCENE) {
				TRACK_SCENE* pScnData = (TRACK_SCENE*)pTrack->pData;
				if(pScnData->SceneNo == scn)
					pScnData->SceneNo = -1;
				// 한 칸씩 당겨짐
				if(pScnData->SceneNo > scn)
					pScnData->SceneNo -= 1;
			}
		}
	}
}

// 장면의 순서를 바꿀때 멀티씬 정보도 갱신시킨다
void xMultiScene::OnMoveScene(int snum, int tnum)
{
	int n0, n1;
	int cnt0, cnt1;

	cnt0 = TotMSceneNum;

	for(n0=0; n0<cnt0; n0++) // 멀티씬
	{		
		MSceneData* pScene = GetMScenePt(n0);
		if(pScene == NULL) continue;
		cnt1 = pScene->TotTrackNum;

		for(n1=0; n1<cnt1; n1++) // 트랙
		{
			TrackData* pTrack = GetTrackPt(n0, n1);
			if(pTrack == NULL) continue;

			if(pTrack->TrackType == TTYPE_SCENE) {
				TRACK_SCENE* pScnData = (TRACK_SCENE*)pTrack->pData;
				if(pScnData == NULL) continue;

				if(snum > tnum) {
					if(pScnData->SceneNo == snum) pScnData->SceneNo = tnum;
					else if(pScnData->SceneNo >= tnum && pScnData->SceneNo < snum) pScnData->SceneNo++;
				}

				if(snum < tnum) {
					if(pScnData->SceneNo == snum) pScnData->SceneNo = tnum;
					else if(pScnData->SceneNo > snum && pScnData->SceneNo <= tnum) pScnData->SceneNo--;
				}
			}
		}
	}
}



//////////////////////////////////// MSceneData Class ///////////////////////////////////

MSceneData::MSceneData()
{
	memset(MSceneName, 0, sizeof(MSceneName));
	TotTrackNum = 0;
	memset(Reserved, 0, sizeof(Reserved));

	ppTrack = NULL;

	TotPlayTime = 0;
}

MSceneData::~MSceneData()
{
	Destroy();
}

void MSceneData::Destroy()
{
	if(ppTrack)
	{
		for(int i=0; i<TotTrackNum; i++) 
		{
			if(ppTrack[i] != NULL) 
			{
				delete(ppTrack[i]); 
				ppTrack[i]=NULL; 
			}
		}

		free(ppTrack);
		ppTrack = NULL;
	}

	memset(MSceneName, 0, sizeof(MSceneName));
	TotTrackNum = 0;
	memset(Reserved, 0, sizeof(Reserved));

	TotPlayTime = 0;
}

void MSceneData::Set(char* name)
{
	memset(MSceneName, 0, sizeof(MSceneName));
	strncpy(MSceneName, name, 31);
}

int MSceneData::AddEmptyTrack()
{
	TotTrackNum += 1;

	TrackData** ppNew = (TrackData**) malloc(sizeof(TrackData*) * TotTrackNum);

	if(ppTrack)
	{
		if(TotTrackNum > 1) 
			memcpy(ppNew, ppTrack, sizeof(TrackData*) * (TotTrackNum-1));
		free(ppTrack);
	}
	ppTrack = ppNew;

	ppTrack[TotTrackNum-1] = new TrackData;
	(ppTrack[TotTrackNum-1])->SetBase(TTYPE_NONE);

	return TotTrackNum-1;
}

int MSceneData::AddTrack(TrackData* pTrack)
{
	TotTrackNum += 1;

	TrackData** ppNew = (TrackData**) malloc(sizeof(TrackData*) * TotTrackNum);

	if(ppTrack)
	{
		if(TotTrackNum > 1) 
			memcpy(ppNew, ppTrack, sizeof(TrackData*) * (TotTrackNum-1));
		free(ppTrack);
	}
	ppTrack = ppNew;

	ppTrack[TotTrackNum-1] = new TrackData;
	(ppTrack[TotTrackNum-1])->CopyFrom(pTrack);

	return TotTrackNum-1;
}

void MSceneData::DelTrack(int num)
{
	if(TotTrackNum <= 0) return;
	if(num < 0 || num >= TotTrackNum) return;
	//if(TotTrackNum == 1) { DestroyAllTrack(); return; }

	TotTrackNum -= 1;
	TrackData** ppNew = (TrackData**) malloc(sizeof(TrackData*) * TotTrackNum);

	int i;
	for(i=0; i<num; i++) ppNew[i] = ppTrack[i];
	for(i=num; i<TotTrackNum; i++) ppNew[i] = ppTrack[i+1];

	delete(ppTrack[num]);
	if(ppTrack) free(ppTrack);
	ppTrack = ppNew;
}

void MSceneData::MovTrack(int num, int tnum)
{
	if(TotTrackNum <= 1) return;
	if(tnum < 0 || tnum >= TotTrackNum) return;
	if(num < 0 || num >= TotTrackNum) return;
	if(num == tnum) return;

	TrackData** ppNew = (TrackData**) malloc(sizeof(TrackData*) * TotTrackNum);

	int t,s;
	for(t=0, s=0; t<TotTrackNum; t++, s++)
	{
		if(t == tnum) t++;
		if(s == num) s++;
		if(t < TotTrackNum) ppNew[t] = ppTrack[s];
	}
	ppNew[tnum] = ppTrack[num];

	if(ppTrack) free(ppTrack);
	ppTrack = ppNew;
}

int MSceneData::MovTracks(int totnum, int* plist, int tnum)
{
	if(TotTrackNum <= 0) return -1;
	if(totnum <= 0 || totnum > TotTrackNum || !plist) return -1;
	if(tnum < 0 || tnum > TotTrackNum) return -1;

	TrackData** ppNew = (TrackData**) malloc(sizeof(TrackData*) * TotTrackNum);

	int pos = -1;

	int t=0;
	for(int s=0; s<TotTrackNum; s++)
	{
		BOOL bFind = FALSE;
		for(int i=0; i<totnum; i++) {
			if(s == plist[i]) {
				bFind = TRUE;
				break;
			}
		}

		if(s == tnum) {
			pos = t;
			for(int i=0; i<totnum; i++) {
				ppNew[t] = ppTrack[plist[i]];
				t++;
			}
		}

		if(!bFind) {
			ppNew[t] = ppTrack[s];
			t++;
		}
	}

	if(tnum == TotTrackNum) {
		pos = t;
		for(int i=0; i<totnum; i++) {
			ppNew[t] = ppTrack[plist[i]];
			t++;
		}
	}

	if(ppTrack) free(ppTrack);
	ppTrack = ppNew;

	// 최종적으로 삽입된 위치를 리턴
	return pos;
}

int MSceneData::CopyTracks(int totnum, int* plist, int tnum)
{
	if(TotTrackNum <= 0) return -1;
	if(totnum <= 0 || totnum > TotTrackNum || !plist) return -1;
	if(tnum < 0 || tnum > TotTrackNum) return -1;

	int newtot = TotTrackNum + totnum;

	TrackData** ppNew = (TrackData**) malloc(sizeof(TrackData*) * newtot);

	int pos = -1;

	int t=0;
	for(int s=0; s<=TotTrackNum; s++)
	{
		if(s == tnum) 
		{
			pos = t;
			for(int i=0; i<totnum; i++)
			{
				ppNew[t] = new TrackData;

				// 트랙을 복사
				ppNew[t]->CopyFrom(ppTrack[plist[i]]);

				t++;
			}
		}

		// 타겟 위치가 마지막 트랙을 넘어선 경우라면 여기서 종료
		if(s == TotTrackNum)
			break;

		ppNew[t] = ppTrack[s];
		t++;
	}


	TotTrackNum += totnum;

	if(ppTrack)
	{
		free(ppTrack);
	}

	ppTrack = ppNew;

	// 최종적으로 삽입된 위치를 리턴
	return pos;
}

// 프레임의 포인터를 인덱스로 얻는다
TrackData* MSceneData::GetPtrTrack(int num)
{
	if(num<0 || num >= TotTrackNum) return NULL;
	return ppTrack[num];
}

// 클립박스 트랙에 영향 받는 트랙 정보를 업데이트
void MSceneData::UpdateAffectClipboxTrack()
{
	int cliptrack = -1;
	for(int i=TotTrackNum-1; i>=0; i--) {
		TrackData* pTrack = GetPtrTrack(i);
		if(!pTrack)
			continue;

		if(pTrack->TrackType == TTYPE_CLIPBOX) 
			cliptrack = i;

		if(pTrack->TrackType == TTYPE_SCENE) {
			TRACK_SCENE* pScnData = (TRACK_SCENE*)pTrack->pData;
			if(pScnData->cbClipping == 1) {
				pTrack->AffectClipboxTrack = cliptrack;
				continue;
			}
		}

		pTrack->AffectClipboxTrack = -1;
	}
}


//////////////////////////////////// TrackData Class ///////////////////////////////////

TrackData::TrackData()
{
	ClearMember();
}

TrackData::~TrackData()
{
	Destroy();
}

void TrackData::ClearMember()
{
	// 파일 저장용 데이터
	TrackType = TTYPE_NONE;
	Delay = 0;
	memset(Reserved, 0, sizeof(Reserved));
	DataSize = 0;

	// 클래스 내부 사용 데이터
	bHideTrack = FALSE;
	pData = NULL;

	AffectClipboxTrack = -1;
	pCpiScene = NULL;

	pCpiClipbox = NULL;

	pstrScript = "";
}

void TrackData::Destroy()
{
	if(pData) {
		delete pData;
	}

	ClearMember();
}

// 트랙 데이터의 정보 공간을 확보한다
void TrackData::SetBase(ENUM_TRACKTYPE type)
{
	Destroy();

	switch(type)
	{
	case TTYPE_NONE :
		pData = NULL;
		DataSize = 0;
		break;

	case TTYPE_SCENE :
		pData = new TRACK_SCENE;
		DataSize = sizeof(TRACK_SCENE);
		memset(pData, 0, DataSize);
		((TRACK_SCENE*)pData)->Alpha = 31;
		((TRACK_SCENE*)pData)->ZoomX = 100;
		((TRACK_SCENE*)pData)->ZoomY = 100;
		((TRACK_SCENE*)pData)->Angle = 0;
		break;

	case TTYPE_SOUND :
		pData = new TRACK_SOUND;
		DataSize = sizeof(TRACK_SOUND);
		memset(pData, 0, DataSize);
		break;

	case TTYPE_CLIPBOX :
		pData = new TRACK_CLIPBOX;
		DataSize = sizeof(TRACK_CLIPBOX);
		memset(pData, 0, DataSize);
		break;

	case TTYPE_SCRIPT :
		pData = new TRACK_SCRIPT;
		DataSize = sizeof(TRACK_SCRIPT);
		memset(pData, 0, DataSize);
		break;

	default :
		return;
	}

	TrackType = type;
	Delay = 0;

	RebuildInfo();
}

BOOL TrackData::RebuildInfo()
{
	if(TrackType != TTYPE_NONE && pData == NULL)
		return FALSE;

	pCpiScene = NULL;
	pCpiClipbox = NULL;
	pstrScript = "";

	int dsize = 0;

	switch(TrackType)
	{
	case TTYPE_NONE :
		{
			dsize = 0; 
		} break;

	case TTYPE_SCENE : 
		{
			dsize = sizeof(TRACK_SCENE);
			TRACK_SCENE* pScnData = (TRACK_SCENE*)pData;
			if(pScnData->CpiNum > 0) {
				pCpiScene = (CPI_SCENE*)((char*)pData + dsize);
				dsize += sizeof(CPI_SCENE) * pScnData->CpiNum;
			}
		} break;

	case TTYPE_SOUND :	
		{
			dsize = sizeof(TRACK_SOUND); 
		} break;

	case TTYPE_CLIPBOX :
		{
			dsize = sizeof(TRACK_CLIPBOX);
			TRACK_CLIPBOX* pCbxData = (TRACK_CLIPBOX*)pData;
			if(pCbxData->CpiNum > 0) {
				pCpiClipbox = (CPI_CLIPBOX*)((char*)pData + dsize);
				dsize += sizeof(CPI_CLIPBOX) * pCbxData->CpiNum;
			}

		} break;

	case TTYPE_SCRIPT :
		{
			dsize = sizeof(TRACK_SCRIPT);
			TRACK_SCRIPT* pScrData = (TRACK_SCRIPT*)pData;
			if(pScrData->ScriptBufLen > 0) {
				pstrScript = (char*)pData + dsize;
				dsize += pScrData->ScriptBufLen;
			}
		} break;

	default :	
		return FALSE;
	}

	if(DataSize != dsize)
		return FALSE;

	return TRUE;
}

// 같은 트랙 클래스로부터 복사해온다
void TrackData::CopyFrom(TrackData* pSrc)
{
	Destroy();

	// 트랙을 복사
	TrackType = pSrc->TrackType;
	Delay = pSrc->Delay;

	if(pSrc->DataSize > 0 && pSrc->pData) {
		pData = new BYTE[pSrc->DataSize];
		DataSize = pSrc->DataSize;
		memcpy(pData, pSrc->pData, pSrc->DataSize);
	}
	else {
		DataSize = 0;
		pData = NULL;
	}

	RebuildInfo();
}

BOOL TrackData::GetInterpol_Scene(DWORD playtime, CPI_SCENE* pCpi)
{
	TRACK_SCENE* pTrack = (TRACK_SCENE*)pData;
	if(Delay > playtime)
		return FALSE;

	DWORD deltatime = playtime - Delay;
	pCpi->DeltaTime = deltatime;

	if(deltatime == 0 || pTrack->CpiNum <= 0) {
		// 플레이타임이 트랙의 시작인 경우
		pCpi->Pos = pTrack->Pos;
		pCpi->Alpha = pTrack->Alpha;
		pCpi->ZoomX = pTrack->ZoomX;
		pCpi->ZoomY = pTrack->ZoomY;
		pCpi->Angle = pTrack->Angle;
	}
	else {
		// 트랙의 시작점을 지나서 플레이 되는 경우
		for(int i=0; i<pTrack->CpiNum; i++) {
			if(deltatime < pCpiScene[i].DeltaTime) {

				CPI_SCENE preCpi;

				int pre = i-1;
				if(pre < 0) {
					preCpi.DeltaTime = 0;
					preCpi.Pos = pTrack->Pos;
					preCpi.Alpha = pTrack->Alpha;
					preCpi.ZoomX = pTrack->ZoomX;
					preCpi.ZoomY = pTrack->ZoomY;
					preCpi.Angle = pTrack->Angle;
				}
				else {
					preCpi = pCpiScene[pre];
				}

				DWORD totgap = pCpiScene[i].DeltaTime - preCpi.DeltaTime;
				DWORD delgap = deltatime - preCpi.DeltaTime;

				if(totgap == 0)
					continue;

				float fdelta = (float)delgap / (float)totgap;

				/////////

				float delta = 0;
				float bias = (float)(pCpiScene[i].Bias / 100.0);

				if(bias < 0.0001f && bias > -0.0001f) bias = 0;
				if(bias > 1.0f) bias = 1.0f;
				if(bias < -1.0f) bias = -1.0f;

				if(bias == 0) {
					delta = fdelta;
				}
				else if(bias < 0) {
					float ang = fdelta * TO_RADIAN(90);
					float m = (float)(1.0 - cos(ang));
					delta = fdelta + ((fdelta - m) * bias);
				}
				else {
					float ang = fdelta * TO_RADIAN(90);
					float m = (float)sin(ang);
					delta = fdelta + ((m - fdelta) * bias);
				}

				/////////

				pCpi->Pos.x = preCpi.Pos.x + FLOATROUNDINT((pCpiScene[i].Pos.x - preCpi.Pos.x) * delta);
				pCpi->Pos.y = preCpi.Pos.y + FLOATROUNDINT((pCpiScene[i].Pos.y - preCpi.Pos.y) * delta);
				pCpi->Alpha = preCpi.Alpha + FLOATROUNDINT((pCpiScene[i].Alpha - preCpi.Alpha) * delta);
				pCpi->ZoomX = preCpi.ZoomX + FLOATROUNDINT((pCpiScene[i].ZoomX - preCpi.ZoomX) * delta);
				pCpi->ZoomY = preCpi.ZoomY + FLOATROUNDINT((pCpiScene[i].ZoomY - preCpi.ZoomY) * delta);
				pCpi->Angle = preCpi.Angle + FLOATROUNDINT((pCpiScene[i].Angle - preCpi.Angle) * delta);

				return TRUE;
			}
		}

		// 플레이 타임이 마지막 컨트롤 포인트를 지난 경우
		pCpi->Pos = pCpiScene[pTrack->CpiNum-1].Pos;
		pCpi->Alpha = pCpiScene[pTrack->CpiNum-1].Alpha;
		pCpi->ZoomX = pCpiScene[pTrack->CpiNum-1].ZoomX;
		pCpi->ZoomY = pCpiScene[pTrack->CpiNum-1].ZoomY;
		pCpi->Angle = pCpiScene[pTrack->CpiNum-1].Angle;
	}

	return TRUE;
}
BOOL TrackData::GetInterpol_Scene(DWORD playtime, CPI_SCENE2* pCpi)
{
	TRACK_SCENE* pTrack = (TRACK_SCENE*)pData;
	if(Delay > playtime)
		return FALSE;

	DWORD deltatime = playtime - Delay;
	pCpi->DeltaTime = deltatime;

	if(deltatime == 0 || pTrack->CpiNum <= 0) {
		// 플레이타임이 트랙의 시작인 경우
		pCpi->PosX = (float)pTrack->Pos.x;
		pCpi->PosY = (float)pTrack->Pos.y;

		pCpi->Alpha = pTrack->Alpha;
		pCpi->ZoomX = pTrack->ZoomX;
		pCpi->ZoomY = pTrack->ZoomY;
		pCpi->Angle = pTrack->Angle;
	}
	else {
		// 트랙의 시작점을 지나서 플레이 되는 경우
		for(int i=0; i<pTrack->CpiNum; i++) {
			if(deltatime < pCpiScene[i].DeltaTime) {

				CPI_SCENE2 preCpi;

				int pre = i-1;
				if(pre < 0) {
					preCpi.DeltaTime = 0;
					//	preCpi.Pos = pTrack->Pos;
					preCpi.PosX = (float)pTrack->Pos.x;
					preCpi.PosY = (float)pTrack->Pos.y;
					preCpi.Alpha = pTrack->Alpha;
					preCpi.ZoomX = pTrack->ZoomX;
					preCpi.ZoomY = pTrack->ZoomY;
					preCpi.Angle = pTrack->Angle;
				}
				else {
					preCpi.Alpha = pCpiScene[pre].Alpha;
					preCpi.Angle = pCpiScene[pre].Angle;
					preCpi.Bias = pCpiScene[pre].Bias;
					preCpi.DeltaTime = pCpiScene[pre].DeltaTime;
					preCpi.PosX = (float)pCpiScene[pre].Pos.x;
					preCpi.PosY = (float)pCpiScene[pre].Pos.y;
					memcpy( preCpi.Reserved1,pCpiScene[pre].Reserved1,sizeof(pCpiScene[pre].Reserved1));
					preCpi.ZoomX = pCpiScene[pre].ZoomX;
					preCpi.ZoomY = pCpiScene[pre].ZoomY;


				}

				DWORD totgap = pCpiScene[i].DeltaTime - preCpi.DeltaTime;
				DWORD delgap = deltatime - preCpi.DeltaTime;

				if(totgap == 0)
					continue;

				float fdelta = (float)delgap / (float)totgap;

				/////////

				float delta = 0;
				float bias = (float)(pCpiScene[i].Bias / 100.0);

				if(bias < 0.0001f && bias > -0.0001f) bias = 0;
				if(bias > 1.0f) bias = 1.0f;
				if(bias < -1.0f) bias = -1.0f;

				if(bias == 0) {
					delta = fdelta;
				}
				else if(bias < 0) {
					float ang = fdelta * TO_RADIAN(90);
					float m = (float)(1.0 - cos(ang));
					delta = fdelta + ((fdelta - m) * bias);
				}
				else {
					float ang = fdelta * TO_RADIAN(90);
					float m = (float)sin(ang);
					delta = fdelta + ((m - fdelta) * bias);
				}

				/////////

				pCpi->PosX = preCpi.PosX + ((pCpiScene[i].Pos.x - preCpi.PosX) * delta);
				pCpi->PosY = preCpi.PosY + ((pCpiScene[i].Pos.y - preCpi.PosY) * delta);
				pCpi->Alpha = preCpi.Alpha + FLOATROUNDINT((pCpiScene[i].Alpha - preCpi.Alpha) * delta);
				pCpi->ZoomX = preCpi.ZoomX + FLOATROUNDINT((pCpiScene[i].ZoomX - preCpi.ZoomX) * delta);
				pCpi->ZoomY = preCpi.ZoomY + FLOATROUNDINT((pCpiScene[i].ZoomY - preCpi.ZoomY) * delta);
				pCpi->Angle = preCpi.Angle + FLOATROUNDINT((pCpiScene[i].Angle - preCpi.Angle) * delta);

				return TRUE;
			}
		}

		// 플레이 타임이 마지막 컨트롤 포인트를 지난 경우
		pCpi->PosX = (float)pCpiScene[pTrack->CpiNum-1].Pos.x;
		pCpi->PosY = (float)pCpiScene[pTrack->CpiNum-1].Pos.y;
		pCpi->Alpha = pCpiScene[pTrack->CpiNum-1].Alpha;
		pCpi->ZoomX = pCpiScene[pTrack->CpiNum-1].ZoomX;
		pCpi->ZoomY = pCpiScene[pTrack->CpiNum-1].ZoomY;
		pCpi->Angle = pCpiScene[pTrack->CpiNum-1].Angle;
	}

	return TRUE;
}


BOOL TrackData::GetInterpol_Clipbox(DWORD playtime, CPI_CLIPBOX* pCpi)
{
	TRACK_CLIPBOX* pTrack = (TRACK_CLIPBOX*)pData;
	if(Delay > playtime)
		return FALSE;

	DWORD deltatime = playtime - Delay;
	pCpi->DeltaTime = deltatime;

	if(deltatime == 0 || pTrack->CpiNum <= 0) {
		pCpi->Rect = pTrack->Rect;
	}
	else {
		for(int i=0; i<pTrack->CpiNum; i++) {
			if(deltatime < pCpiClipbox[i].DeltaTime) {

				RECT preRect;
				DWORD preDelta;

				int pre = i-1;
				if(pre < 0) {
					preRect = pTrack->Rect;
					preDelta = 0;
				}
				else {
					preRect = pCpiClipbox[pre].Rect;
					preDelta = pCpiClipbox[pre].DeltaTime;
				}

				DWORD totgap = pCpiClipbox[i].DeltaTime - preDelta;
				DWORD delgap = deltatime - preDelta;

				if(totgap == 0)
					continue;

				float fdelta = (float)delgap / (float)totgap;

				/////////

				float delta = 0;
				float bias = (float)(pCpiClipbox[i].Bias / 100.0);

				if(bias < 0.0001f && bias > -0.0001f) bias = 0;
				if(bias > 1.0f) bias = 1.0f;
				if(bias < -1.0f) bias = -1.0f;

				if(bias == 0) {
					delta = fdelta;
				}
				else if(bias < 0) {
					float ang = fdelta * TO_RADIAN(90);
					float m = (float)(1.0 - cos(ang));
					delta = fdelta + ((fdelta - m) * bias);
				}
				else {
					float ang = fdelta * TO_RADIAN(90);
					float m = (float)sin(ang);
					delta = fdelta + ((m - fdelta) * bias);
				}

				/////////

				pCpi->Rect.left = preRect.left + FLOATROUNDINT((pCpiClipbox[i].Rect.left - preRect.left) * delta);
				pCpi->Rect.top = preRect.top + FLOATROUNDINT((pCpiClipbox[i].Rect.top - preRect.top) * delta);
				pCpi->Rect.right = preRect.right + FLOATROUNDINT((pCpiClipbox[i].Rect.right - preRect.right) * delta);
				pCpi->Rect.bottom = preRect.bottom + FLOATROUNDINT((pCpiClipbox[i].Rect.bottom - preRect.bottom) * delta);

				return TRUE;
			}
		}

		pCpi->Rect = pCpiClipbox[pTrack->CpiNum-1].Rect;
	}

	return TRUE;
}

int TrackData::AddCpi_Scene(CPI_SCENE* pcpi)
{
	if(TrackType != TTYPE_SCENE || !pData || DataSize == 0)
		return -1;

	// 트랙 시작점과 같을 수는 없다
	if(pcpi->DeltaTime == 0)
		return -1;

	TRACK_SCENE* pScnData = (TRACK_SCENE*)pData;

	int iInsert = 0;
	for(int i=0; i<pScnData->CpiNum; i++) {
		if(pcpi->DeltaTime == pCpiScene[i].DeltaTime) {
			// 이미 존재하는 컨트롤 포인트라면 기존 정보를 대체
			memcpy(&pCpiScene[i], pcpi, sizeof(CPI_SCENE));

			RebuildInfo();
			return i;
		}

		if(pcpi->DeltaTime > pCpiScene[i].DeltaTime) {
			iInsert = i+1;
		}
	}

	// 새로운 컨트롤 포인트를 추가
	int idx = -1;

	int newdsize = DataSize + sizeof(CPI_SCENE);
	char* pnewbuf = new char[newdsize];

	if(iInsert >= pScnData->CpiNum) {
		// 맨 뒤에 붙이기
		idx = pScnData->CpiNum;
		(pScnData->CpiNum)++;	// 포인터가 유효할 때 미리 카운트 증가

		memcpy(pnewbuf, pData, DataSize);
		memcpy(pnewbuf + DataSize, pcpi, sizeof(CPI_SCENE));
	}
	else {
		// 중간에 끼워넣기
		idx = iInsert;

		int cpn1 = iInsert;
		int cpn2 = pScnData->CpiNum - iInsert;
		(pScnData->CpiNum)++;	// 포인터가 유효할 때 미리 카운트 증가

		int frontsize = sizeof(TRACK_SCENE) + (sizeof(CPI_SCENE) * cpn1);

		// 끼워 넣기 이전까지의 정보 복사
		memcpy(pnewbuf, pData, frontsize);

		// 끼워 넣을 정보 복사
		memcpy(pnewbuf + frontsize, pcpi, sizeof(CPI_SCENE));		

		// 끼워 넣고 남은 정보 복사
		if(cpn2 > 0) {
			char* tar = pnewbuf + (frontsize + sizeof(CPI_SCENE));
			char* sor = (char*)pData + frontsize;
			memcpy(tar, sor, sizeof(CPI_SCENE) * cpn2);
		}
	}

	delete pData;

	pData = pnewbuf;
	DataSize = newdsize;

	RebuildInfo();

	return idx;
}

int TrackData::AddCpi_Clipbox(CPI_CLIPBOX* pcpi)
{
	if(TrackType != TTYPE_CLIPBOX || !pData || DataSize == 0)
		return -1;

	// 트랙 시작점과 같을 수는 없다
	if(pcpi->DeltaTime == 0)
		return -1;

	TRACK_CLIPBOX* pCbxData = (TRACK_CLIPBOX*)pData;

	int iInsert = 0;
	for(int i=0; i<pCbxData->CpiNum; i++) {
		if(pcpi->DeltaTime == pCpiClipbox[i].DeltaTime) {
			// 이미 존재하는 컨트롤 포인트라면 기존 정보를 대체
			memcpy(&pCpiClipbox[i], pcpi, sizeof(CPI_CLIPBOX));

			RebuildInfo();
			return i;
		}

		if(pcpi->DeltaTime > pCpiClipbox[i].DeltaTime) {
			iInsert = i+1;
		}
	}

	// 새로운 컨트롤 포인트를 추가
	int idx = -1;

	int newdsize = DataSize + sizeof(CPI_CLIPBOX);
	char* pnewbuf = new char[newdsize];

	if(iInsert >= pCbxData->CpiNum) {
		// 맨 뒤에 붙이기
		idx = pCbxData->CpiNum;
		(pCbxData->CpiNum)++;	// 포인터가 유효할 때 미리 카운트 증가

		memcpy(pnewbuf, pData, DataSize);
		memcpy(pnewbuf + DataSize, pcpi, sizeof(CPI_CLIPBOX));
	}
	else {
		// 중간에 끼워넣기
		idx = iInsert;

		int cpn1 = iInsert;
		int cpn2 = pCbxData->CpiNum - iInsert;
		(pCbxData->CpiNum)++;	// 포인터가 유효할 때 미리 카운트 증가

		int frontsize = sizeof(TRACK_CLIPBOX) + (sizeof(CPI_CLIPBOX) * cpn1);

		// 끼워 넣기 이전까지의 정보 복사
		memcpy(pnewbuf, pData, frontsize);

		// 끼워 넣을 정보 복사
		memcpy(pnewbuf + frontsize, pcpi, sizeof(CPI_CLIPBOX));		

		// 끼워 넣고 남은 정보 복사
		if(cpn2 > 0) {
			char* tar = pnewbuf + (frontsize + sizeof(CPI_CLIPBOX));
			char* sor = (char*)pData + frontsize;
			memcpy(tar, sor, sizeof(CPI_CLIPBOX) * cpn2);
		}
	}

	delete pData;

	pData = pnewbuf;
	DataSize = newdsize;

	RebuildInfo();

	return idx;
}

BOOL TrackData::SetCpi_Script(char* str)
{
	if(TrackType != TTYPE_SCRIPT || !pData || DataSize == 0)
		return FALSE;

	int slen = strlen(str);

	int buflen = slen + 1;

	if(slen == 0)
		buflen = 0;

	((TRACK_SCRIPT*)pData)->ScriptBufLen = buflen;

	int newdsize = sizeof(TRACK_SCRIPT) + buflen;
	char* pnewbuf = new char[newdsize];

	memcpy(pnewbuf, pData, sizeof(TRACK_SCRIPT));
	if(buflen > 0) {
		strcpy(pnewbuf+sizeof(TRACK_SCRIPT), str);
	}

	delete pData;

	pData = pnewbuf;
	DataSize = newdsize;

	RebuildInfo();

	return TRUE;
}

int TrackData::DelCpi_Scene(DWORD stime, DWORD etime)
{
	if(TrackType != TTYPE_SCENE || !pData || DataSize == 0)
		return 0;

	if(stime > etime)
		return 0;

	TRACK_SCENE* pScnData = (TRACK_SCENE*)pData;
	if(pScnData->CpiNum <= 0)
		return 0;

	int delcnt = 0;
	for(int i=pScnData->CpiNum-1; i>=0; i--) {
		// 지울때 마다 재할당되므로 포인터를 새로 지정
		pScnData = (TRACK_SCENE*)pData;

		if(pCpiScene[i].DeltaTime < stime || pCpiScene[i].DeltaTime > etime) 
			continue;

		// 단일 범위가 아닌 경우 마지막 시간은 포함하지 않음
		if(stime != etime && pCpiScene[i].DeltaTime >= etime)
			continue;

		int newdsize = DataSize - sizeof(CPI_SCENE);
		char* pnewbuf = new char[newdsize];

		//////////

		int cpn1 = i;
		int cpn2 = pScnData->CpiNum - (i+1);
		(pScnData->CpiNum)--;	// 포인터가 유효할 때 미리 카운트 감소

		int frontsize = sizeof(TRACK_SCENE) + (sizeof(CPI_SCENE) * cpn1);

		// 지우기 이전까지의 정보 복사
		memcpy(pnewbuf, pData, frontsize);

		// 지우고 남은 정보 복사
		if(cpn2 > 0) {
			char* tar = pnewbuf + frontsize;
			char* sor = (char*)pData + (frontsize + sizeof(CPI_SCENE));
			memcpy(tar, sor, sizeof(CPI_SCENE) * cpn2);
		}

		//////////

		delete pData;

		pData = pnewbuf;
		DataSize = newdsize;

		RebuildInfo();

		delcnt++;
	}

	return delcnt;
}

int TrackData::DelCpi_Clipbox(DWORD stime, DWORD etime)
{
	if(TrackType != TTYPE_CLIPBOX || !pData || DataSize == 0)
		return 0;

	if(stime > etime)
		return 0;

	TRACK_CLIPBOX* pCbxData = (TRACK_CLIPBOX*)pData;
	if(pCbxData->CpiNum <= 0)
		return 0;

	int delcnt = 0;
	for(int i=pCbxData->CpiNum-1; i>=0; i--) {
		// 지울때 마다 재할당되므로 포인터를 새로 지정
		pCbxData = (TRACK_CLIPBOX*)pData;

		if(pCpiClipbox[i].DeltaTime < stime || pCpiClipbox[i].DeltaTime > etime) 
			continue;

		// 단일 범위가 아닌 경우 마지막 시간은 포함하지 않음
		if(stime != etime && pCpiClipbox[i].DeltaTime >= etime)
			continue;

		int newdsize = DataSize - sizeof(CPI_CLIPBOX);
		char* pnewbuf = new char[newdsize];

		//////////

		int cpn1 = i;
		int cpn2 = pCbxData->CpiNum - (i+1);
		(pCbxData->CpiNum)--;	// 포인터가 유효할 때 미리 카운트 감소

		int frontsize = sizeof(TRACK_CLIPBOX) + (sizeof(CPI_CLIPBOX) * cpn1);

		// 지우기 이전까지의 정보 복사
		memcpy(pnewbuf, pData, frontsize);

		// 지우고 남은 정보 복사
		if(cpn2 > 0) {
			char* tar = pnewbuf + frontsize;
			char* sor = (char*)pData + (frontsize + sizeof(CPI_CLIPBOX));
			memcpy(tar, sor, sizeof(CPI_CLIPBOX) * cpn2);
		}

		//////////

		delete pData;

		pData = pnewbuf;
		DataSize = newdsize;

		RebuildInfo();

		delcnt++;
	}

	return delcnt;
}

BOOL TrackData::InsertDelay(DWORD tartime, DWORD delay)
{
	if(!pData || DataSize == 0)
		return FALSE;

	if(tartime < Delay)
		return FALSE;

	tartime -= Delay;

	switch(TrackType)
	{
	case TTYPE_SCENE : 
		{
			TRACK_SCENE* pScnData = (TRACK_SCENE*)pData;
			if(pScnData->CpiNum > 0) {
				for(int i=0; i<pScnData->CpiNum; i++) {
					if(pCpiScene[i].DeltaTime >= tartime) {
						pCpiScene[i].DeltaTime += delay;
					}
				}
				return TRUE;
			}
		} break;

	case TTYPE_CLIPBOX :
		{
			TRACK_CLIPBOX* pCbxData = (TRACK_CLIPBOX*)pData;
			if(pCbxData->CpiNum > 0) {
				for(int i=0; i<pCbxData->CpiNum; i++) {
					if(pCpiClipbox[i].DeltaTime >= tartime) {
						pCpiClipbox[i].DeltaTime += delay;
					}
				}
				return TRUE;
			}

		} break;

	default :	
		return FALSE;
	}

	return FALSE;
}

BOOL TrackData::DeleteDelay(DWORD tartime, DWORD delay)
{
	if(!pData || DataSize == 0)
		return FALSE;

	if(tartime < Delay)
		return FALSE;

	tartime -= Delay;

	switch(TrackType)
	{
	case TTYPE_SCENE : 
		{
			TRACK_SCENE* pScnData = (TRACK_SCENE*)pData;
			if(pScnData->CpiNum > 0) {
				for(int i=0; i<pScnData->CpiNum; i++) {
					if(pCpiScene[i].DeltaTime >= tartime) {
						if(pCpiScene[i].DeltaTime - tartime >= delay) {
							pCpiScene[i].DeltaTime -= delay;
						}
						else {
							// 삭제할 수 없는 조건이 하나라도 있으면 실패
							return FALSE;
						}
					}
				}
				return TRUE;
			}
		} break;

	case TTYPE_CLIPBOX :
		{
			TRACK_CLIPBOX* pCbxData = (TRACK_CLIPBOX*)pData;
			if(pCbxData->CpiNum > 0) {
				for(int i=0; i<pCbxData->CpiNum; i++) {
					if(pCpiClipbox[i].DeltaTime >= tartime) {
						if(pCpiClipbox[i].DeltaTime - tartime >= delay) {
							pCpiClipbox[i].DeltaTime -= delay;
						}
						else {
							// 삭제할 수 없는 조건이 하나라도 있으면 실패
							return FALSE;
						}
					}
				}
				return TRUE;
			}

		} break;

	default :	
		return FALSE;
	}

	return FALSE;
}

// 장면 트랙인 경우 장면 정보 포인터를 리턴
TRACK_SCENE* TrackData::GetDataScene()
{
	if(TrackType != TTYPE_SCENE || DataSize == 0)
		return NULL;

	return (TRACK_SCENE*)pData;
}

// 사운드 트랙인 경우 사운드 정보 포인터를 리턴
TRACK_SOUND* TrackData::GetDataSound()
{
	if(TrackType != TTYPE_SOUND || DataSize == 0)
		return NULL;

	return (TRACK_SOUND*)pData;
}

BOOL TrackData::Read(FILE* fp)
{
	Destroy();

	if(!fp)
		return FALSE;

	fread(&TrackType, sizeof(TrackType), 1, fp);
	fread(&Delay, sizeof(Delay), 1, fp);
	fread(Reserved, sizeof(Reserved), 1, fp);
	fread(&DataSize, sizeof(DataSize), 1, fp);
	if(DataSize > 0) {
		pData = new BYTE[DataSize];
		fread(pData, DataSize, 1, fp);
	}
	else {
		pData = NULL;
	}

	if(!RebuildInfo()) {
		Destroy();
		return FALSE;
	}

	return TRUE;
}

BOOL TrackData::Write(FILE* fp)
{
	if(!fp)
		return FALSE;

	fwrite(&TrackType, sizeof(TrackType), 1, fp);
	fwrite(&Delay, sizeof(Delay), 1, fp);
	fwrite(Reserved, sizeof(Reserved), 1, fp);
	fwrite(&DataSize, sizeof(DataSize), 1, fp);
	if(DataSize > 0) {
		fwrite(pData, DataSize, 1, fp);
	}

	return TRUE;
}