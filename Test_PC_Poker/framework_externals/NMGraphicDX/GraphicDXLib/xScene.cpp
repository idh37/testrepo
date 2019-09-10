
#include "StdAfx.h"
#include "xScene.h"

//////////////////////////////////// xScene Class ///////////////////////////////////

#define MIN_FRAMENUM 4

xScene::xScene()
{
	SceneVer = 0;
	TotSceneNum = 0;
	memset(Reserved, 0, sizeof(Reserved));

	ppScene = NULL;
}

xScene::~xScene()
{
	Destroy();
}

void xScene::Destroy()
{
	if(ppScene) 
	{
		for(int i=0; i<TotSceneNum; i++) 
			if(ppScene[i] != NULL) { delete(ppScene[i]); ppScene[i]=NULL; }
			free(ppScene);
			ppScene= NULL;
	}

	TotSceneNum = 0;
}

void xScene::AddScene(char* name)
{
	TotSceneNum += 1;

	SceneData** ppNew = (SceneData**) malloc(sizeof(SceneData*) * TotSceneNum);

	if(ppScene) 
	{
		memcpy(ppNew, ppScene, sizeof(SceneData*) * (TotSceneNum-1));
		free(ppScene);
	}
	ppScene = ppNew;

	ppScene[TotSceneNum-1] = new SceneData;
	(ppScene[TotSceneNum-1])->Set(name);
}

void xScene::DelScene(int num)
{
	if(TotSceneNum <= 0) return;
	if(num < 0 || num >= TotSceneNum) return;
	if(TotSceneNum == 1) { Destroy(); return; }

	TotSceneNum -= 1;
	SceneData** ppNew = (SceneData**) malloc(sizeof(SceneData*) * TotSceneNum);

	int i;
	for(i=0; i<num; i++) ppNew[i] = ppScene[i];
	for(i=num; i<TotSceneNum; i++) ppNew[i] = ppScene[i+1];

	delete(ppScene[num]);
	if(ppScene) free(ppScene);
	ppScene = ppNew;
}

void xScene::MovScene(int num, int tnum)
{
	if(TotSceneNum <= 1) return;
	if(tnum < 0 || tnum >= TotSceneNum) return;
	if(num < 0 || num >= TotSceneNum) return;
	if(num == tnum) return;

	SceneData** ppNew = (SceneData**) malloc(sizeof(SceneData*) * TotSceneNum);

	int t,s;
	for(t=0, s=0; t<TotSceneNum; t++, s++)
	{
		if(t == tnum) t++;
		if(s == num) s++;
		if(t < TotSceneNum) ppNew[t] = ppScene[s];
	}
	ppNew[tnum] = ppScene[num];

	if(ppScene) free(ppScene);
	ppScene = ppNew;
}

// ����� �����͸� �̸����� ��´�(*pos = �ε��� ��ȣ�� ���� ������)
SceneData* xScene::GetScenePt(char* name, int* pos)
{
	if(TotSceneNum==0) return NULL;
	int i, Num=-1;
	for(i=0; i<TotSceneNum; i++)
		if(stricmp(name,(ppScene[i])->SceneName)==0) { Num=i; break; }

		if(Num == -1) return NULL;

		if(pos!=NULL) *pos=Num;
		return ppScene[Num];
}

// ����� �����͸� �ε����� ��´� 
SceneData* xScene::GetScenePt(int num)
{
	if(num < 0 || num >= TotSceneNum) return NULL;
	return ppScene[num];
}

FrameData* xScene::GetFramePt(int SceneNum, int FrameNum)
{
	if(SceneNum < 0 || SceneNum >= TotSceneNum) return NULL;
	if(FrameNum < 0 || FrameNum >= ppScene[SceneNum]->TotFrameNum) return NULL;
	return ppScene[SceneNum]->ppFrame[FrameNum];
}

CellData* xScene::GetCellPt(int SceneNum, int FrameNum, int CellNum)
{
	if(SceneNum < 0 || SceneNum >= TotSceneNum) return NULL;
	if(FrameNum < 0 || FrameNum >= ppScene[SceneNum]->TotFrameNum) return NULL;
	if(CellNum  < 0 || CellNum  >= ppScene[SceneNum]->ppFrame[FrameNum]->TotCellNum) return NULL;
	return ppScene[SceneNum]->ppFrame[FrameNum]->ppCell[CellNum];
}

// ���ϸ��̼��� �о���δ�
BOOL xScene::Load(FILE* fp)
{
	int n0=0, n1=0, n2=0;
	int cnt0=0, cnt1=0, cnt2=0;

	Destroy();

	// ���� �������� ȣȯ���� ���Ͽ� 2����Ʈ ����
	{
		// ���� ���������� ù 2����Ʈ�� �� ĳ���� ������
		// �� ���������� ���� ���� ������ �̹Ƿ� �׳� �б⸸ ��
		short tempread = 0;
		fread(&tempread, sizeof(tempread), 1, fp);
	}

	// �ִϸ��̼� �������� ����� ����
	char idstr[8] = {0,};
	fread(idstr, 8, 1, fp);
	if(strncmp(idstr, "ANI_DATA", 8) != 0)
		return FALSE;;

	fread(&SceneVer, sizeof(SceneVer), 1, fp);
	fread(&cnt0, sizeof(TotSceneNum), 1, fp);
	fread(Reserved, sizeof(Reserved), 1, fp);

	if(SceneVer != 100) {
		// ������ ��ġ���� ����
		return FALSE;
	}

	for(n0=0; n0<cnt0; n0++) // ���
	{		
		SceneData scene;

		fread(scene.SceneName, sizeof(scene.SceneName), 1, fp);
		fread(&scene.TotFrameNum, sizeof(scene.TotFrameNum), 1, fp);
		fread(scene.Reserved, sizeof(scene.Reserved), 1, fp);

		AddScene(scene.SceneName);

		cnt1 = scene.TotFrameNum;

		for(n1=0; n1<cnt1; n1++) // ������
		{
			FrameData frame;

			fread(&frame.TotCellNum, sizeof(frame.TotCellNum), 1, fp);
			fread(&frame.Delay, sizeof(frame.Delay), 1, fp);
			fread(frame.Reserved, sizeof(frame.Reserved), 1, fp);
			//if(GetScenePt(n0)==NULL) { MsgBox(NULL, "��� ���� ����"); continue; }
			if(GetScenePt(n0)==NULL) 
				return FALSE;

			GetScenePt(n0)->AddFrame(frame.Delay);

			cnt2 = frame.TotCellNum;

			for(n2=0; n2<cnt2; n2++) // ��
			{
				CellData cell;

				fread(&cell.SprNo, sizeof(cell.SprNo), 1, fp);
				fread(&cell.Rx, sizeof(cell.Rx), 1, fp);
				fread(&cell.Ry, sizeof(cell.Ry), 1, fp);
				fread(&cell.Effect, sizeof(cell.Effect), 1, fp);
				fread(&cell.Grade, sizeof(cell.Grade), 1, fp);
				fread(&cell.Color, sizeof(cell.Color), 1, fp);
				fread(&cell.Mirror, sizeof(cell.Mirror), 1, fp);
				fread(&cell.Interpol, sizeof(cell.Interpol), 1, fp);
				fread(&cell.ZoomX, sizeof(cell.ZoomX), 1, fp);
				fread(&cell.ZoomY, sizeof(cell.ZoomY), 1, fp);
				fread(&cell.Bias, sizeof(cell.Bias), 1, fp);
				fread(cell.Reserved1, sizeof(cell.Reserved1), 1, fp);	// ���� ����Ʈ
				fread(&cell.RotAngle, sizeof(cell.RotAngle), 1, fp);
				fread(cell.Reserved2, sizeof(cell.Reserved2), 1, fp);	// ���� ����Ʈ

				FrameData* pFrame = GetFramePt(n0, n1);
				if(!pFrame)
					return FALSE;

				pFrame->AddCell(cell.SprNo, cell.Rx, cell.Ry, cell.Effect, 
					cell.Grade, cell.Color, cell.Mirror, cell.ZoomX, cell.ZoomY, cell.RotAngle, cell.Interpol, cell.Bias);
			}
		}

		// ������ ��� Ʈ���� ����
		GetScenePt(n0)->BuildFrameTree();
	}

	return TRUE;
}

// ���ϸ��̼��� ���Ͽ� �����Ѵ�(���� ���������Ϳ� �̾ ���)
BOOL xScene::Save(FILE* fp)
{
	if(TotSceneNum == 0) return TRUE;

	SceneVer = 100;

	// ���� �������� ȣȯ���� ���Ͽ� 2����Ʈ ����
	{
		// ���� ���������� ù 2����Ʈ�� �� ĳ���� ������
		// ���� �������� ������ ĳ���� ������ 0�̵ǵ��� ó��
		short tempwrite = 0;
		fwrite(&tempwrite, sizeof(tempwrite), 1, fp);
	}

	// ��� ���� ��� ����
	char idstr[8+1] = {0,};
	strncpy(idstr, "ANI_DATA", 8);
	fwrite(idstr, 8, 1, fp);
	fwrite(&SceneVer, sizeof(SceneVer), 1, fp);
	fwrite(&TotSceneNum, sizeof(TotSceneNum), 1, fp);
	fwrite(Reserved, sizeof(Reserved), 1, fp);

	int n0, n1, n2;
	int cnt0, cnt1, cnt2;

	cnt0 = TotSceneNum;

	for(n0=0; n0<cnt0; n0++) // ĳ����
	{		
		SceneData* pScene = GetScenePt(n0);
		if(pScene == NULL) return FALSE;

		// ������ ��� Ʈ���� ����
		pScene->BuildFrameTree();

		fwrite(pScene->SceneName, sizeof(pScene->SceneName), 1, fp);
		fwrite(&pScene->TotFrameNum, sizeof(pScene->TotFrameNum), 1, fp);
		fwrite(pScene->Reserved, sizeof(pScene->Reserved), 1, fp);

		cnt1 = pScene->TotFrameNum;

		for(n1=0; n1<cnt1; n1++) // ������
		{
			FrameData* pFrame = GetFramePt(n0, n1);
			if(pFrame == NULL) return FALSE;
			fwrite(&pFrame->TotCellNum, sizeof(pFrame->TotCellNum), 1, fp);
			fwrite(&pFrame->Delay, sizeof(pFrame->Delay), 1, fp);
			fwrite(pFrame->Reserved, sizeof(pFrame->Reserved), 1, fp);

			cnt2 = pFrame->TotCellNum;

			for(n2=0; n2<cnt2; n2++) // ��
			{
				CellData* pCell = GetCellPt(n0, n1, n2);
				if(pCell == NULL) return FALSE;
				fwrite(&pCell->SprNo, sizeof(pCell->SprNo), 1, fp);
				fwrite(&pCell->Rx, sizeof(pCell->Rx), 1, fp);
				fwrite(&pCell->Ry, sizeof(pCell->Ry), 1, fp);
				fwrite(&pCell->Effect, sizeof(pCell->Effect), 1, fp);
				fwrite(&pCell->Grade, sizeof(pCell->Grade), 1, fp);
				fwrite(&pCell->Color, sizeof(pCell->Color), 1, fp);
				fwrite(&pCell->Mirror, sizeof(pCell->Mirror), 1, fp);
				fwrite(&pCell->Interpol, sizeof(pCell->Interpol), 1, fp);
				fwrite(&pCell->ZoomX, sizeof(pCell->ZoomX), 1, fp);
				fwrite(&pCell->ZoomY, sizeof(pCell->ZoomY), 1, fp);
				fwrite(&pCell->Bias, sizeof(pCell->Bias), 1, fp);
				fwrite(pCell->Reserved1, sizeof(pCell->Reserved1), 1, fp);	// ���� ����Ʈ
				fwrite(&pCell->RotAngle, sizeof(pCell->RotAngle), 1, fp);				
				fwrite(pCell->Reserved2, sizeof(pCell->Reserved2), 1, fp);	// ���� ����Ʈ
			}
		}
	}

	return TRUE;
}

// ��������Ʈ�� �����ɶ� ���ϸ��̼� ������ ���Ž�Ų��
void xScene::OnDeleteSpr(int snum)
{
	int n0, n1, n2;
	int cnt0, cnt1, cnt2;

	cnt0 = TotSceneNum;

	for(n0=0; n0<cnt0; n0++) // ���
	{		
		SceneData* pScene = GetScenePt(n0);
		if(pScene == NULL) return;
		cnt1 = pScene->TotFrameNum;

		for(n1=0; n1<cnt1; n1++) // ������
		{
			FrameData* pFrame = GetFramePt(n0, n1);
			if(pFrame == NULL) return;
			cnt2 = pFrame->TotCellNum;

			for(n2=0; n2<cnt2; n2++) // ��
			{
				CellData* pCell = GetCellPt(n0, n1, n2);
				if(pCell == NULL) return;
				if(pCell->SprNo == snum) {pFrame->DelCell(n2); cnt2--;}
				else if(pCell->SprNo > snum) pCell->SprNo--;
			}
		}
	}
}

// ��������Ʈ�� ������ �ٲܶ� ���ϸ��̼� ������ ���Ž�Ų��
void xScene::OnMoveSpr(int snum, int tnum)
{
	int n0, n1, n2;
	int cnt0, cnt1, cnt2;

	cnt0 = TotSceneNum;

	for(n0=0; n0<cnt0; n0++) // ���
	{		
		SceneData* pScene = GetScenePt(n0);
		if(pScene == NULL) return;
		cnt1 = pScene->TotFrameNum;

		for(n1=0; n1<cnt1; n1++) // ������
		{
			FrameData* pFrame = GetFramePt(n0, n1);
			if(pFrame == NULL) return;
			cnt2 = pFrame->TotCellNum;

			for(n2=0; n2<cnt2; n2++) // ��
			{
				CellData* pCell = GetCellPt(n0, n1, n2);
				if(pCell == NULL) return;

				if(snum > tnum) {
					if(pCell->SprNo == snum) pCell->SprNo = tnum;
					else if(pCell->SprNo >= tnum && pCell->SprNo < snum) pCell->SprNo++;
				}

				if(snum < tnum) {
					if(pCell->SprNo == snum) pCell->SprNo = tnum;
					else if(pCell->SprNo > snum && pCell->SprNo <= tnum) pCell->SprNo--;
				}
			}
		}
	}
}



//////////////////////////////////// SceneData Class ///////////////////////////////////

SceneData::SceneData()
{
	memset(SceneName, 0, sizeof(SceneName));
	TotFrameNum = 0;
	memset(Reserved, 0, sizeof(Reserved));

	ppFrame = NULL;

	TotPlayTime = 0;

	pFrameNode = NULL;
	pTimeTable = NULL;
}

SceneData::~SceneData()
{
	Destroy();
}

void SceneData::Destroy()
{
	if(ppFrame) 
	{
		for(int i=0; i<TotFrameNum; i++) 
			if(ppFrame[i] != NULL) { delete(ppFrame[i]); ppFrame[i]=NULL; }
			free(ppFrame);
			ppFrame = NULL;
	}

	memset(SceneName, 0, sizeof(SceneName));
	TotFrameNum = 0;

	TotPlayTime = 0;
	if(pFrameNode) { delete pFrameNode; pFrameNode = NULL; }
	if(pTimeTable) { delete pTimeTable; pTimeTable = NULL; }
}

void SceneData::Set(char* name)
{
	memset(SceneName, 0, sizeof(SceneName));
	strncpy(SceneName, name, 31);
}

int SceneData::AddFrame(DWORD delay)
{
	TotFrameNum += 1;

	FrameData** ppNew = (FrameData**) malloc(sizeof(FrameData*) * TotFrameNum);

	if(ppFrame)
	{
		if(TotFrameNum > 1) 
			memcpy(ppNew, ppFrame, sizeof(FrameData*) * (TotFrameNum-1));
		free(ppFrame);
	}
	ppFrame = ppNew;

	ppFrame[TotFrameNum-1] = new FrameData;
	(ppFrame[TotFrameNum-1])->Set(delay);

	return TotFrameNum-1;
}

int SceneData::AddFrame(FrameData* pFrame)
{
	TotFrameNum += 1;

	FrameData** ppNew = (FrameData**) malloc(sizeof(FrameData*) * TotFrameNum);

	if(ppFrame)
	{
		if(TotFrameNum > 1) 
			memcpy(ppNew, ppFrame, sizeof(FrameData*) * (TotFrameNum-1));
		free(ppFrame);
	}
	ppFrame = ppNew;

	ppFrame[TotFrameNum-1] = new FrameData;
	(ppFrame[TotFrameNum-1])->CopyFrom(pFrame);

	return TotFrameNum-1;
}

void SceneData::DelFrame(int num)
{
	if(TotFrameNum <= 0) return;
	if(num < 0 || num >= TotFrameNum) return;
	//if(TotFrameNum == 1) { Destroy(); return; }

	TotFrameNum -= 1;
	FrameData** ppNew = (FrameData**) malloc(sizeof(FrameData*) * TotFrameNum);

	int i;
	for(i=0; i<num; i++) ppNew[i] = ppFrame[i];
	for(i=num; i<TotFrameNum; i++) ppNew[i] = ppFrame[i+1];

	delete(ppFrame[num]);
	if(ppFrame) free(ppFrame);
	ppFrame = ppNew;
}

void SceneData::MovFrame(int num, int tnum)
{
	if(TotFrameNum <= 1) return;
	if(tnum < 0 || tnum >= TotFrameNum) return;
	if(num < 0 || num >= TotFrameNum) return;
	if(num == tnum) return;

	FrameData** ppNew = (FrameData**) malloc(sizeof(FrameData*) * TotFrameNum);

	int t,s;
	for(t=0, s=0; t<TotFrameNum; t++, s++)
	{
		if(t == tnum) t++;
		if(s == num) s++;
		if(t < TotFrameNum) ppNew[t] = ppFrame[s];
	}
	ppNew[tnum] = ppFrame[num];

	if(ppFrame) free(ppFrame);
	ppFrame = ppNew;
}

int SceneData::MovFrames(int totnum, int* plist, int tnum)
{
	if(TotFrameNum <= 0) return -1;
	if(totnum <= 0 || totnum > TotFrameNum || !plist) return -1;
	if(tnum < 0 || tnum > TotFrameNum) return -1;

	FrameData** ppNew = (FrameData**) malloc(sizeof(FrameData*) * TotFrameNum);

	int pos = -1;

	int t=0;
	for(int s=0; s<TotFrameNum; s++)
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
				ppNew[t] = ppFrame[plist[i]];
				t++;
			}
		}

		if(!bFind) {
			ppNew[t] = ppFrame[s];
			t++;
		}
	}

	if(tnum == TotFrameNum) {
		pos = t;
		for(int i=0; i<totnum; i++) {
			ppNew[t] = ppFrame[plist[i]];
			t++;
		}
	}

	if(ppFrame) free(ppFrame);
	ppFrame = ppNew;

	// ���������� ���Ե� ��ġ�� ����
	return pos;
}

int SceneData::CopyFrames(int totnum, int* plist, int tnum)
{
	if(TotFrameNum <= 0) return -1;
	if(totnum <= 0 || totnum > TotFrameNum || !plist) return -1;
	if(tnum < 0 || tnum > TotFrameNum) return -1;

	int newtot = TotFrameNum + totnum;

	FrameData** ppNew = (FrameData**) malloc(sizeof(FrameData*) * newtot);

	int pos = -1;

	int t=0;
	for(int s=0; s<=TotFrameNum; s++)
	{
		if(s == tnum) {
			pos = t;
			for(int i=0; i<totnum; i++) {
				ppNew[t] = new FrameData;

				// �������� ����
				ppNew[t]->CopyFrom(ppFrame[plist[i]]);

				t++;
			}
		}

		// Ÿ�� ��ġ�� ������ �������� �Ѿ ����� ���⼭ ����
		if(s == TotFrameNum)
			break;

		ppNew[t] = ppFrame[s];
		t++;
	}


	TotFrameNum += totnum;

	if(ppFrame) free(ppFrame);
	ppFrame = ppNew;

	// ���������� ���Ե� ��ġ�� ����
	return pos;
}

// �������� �����͸� �ε����� ��´�
FrameData* SceneData::GetPtrFrame(int num)
{
	if(num<0 || num >= TotFrameNum) return NULL;
	return ppFrame[num];
}

int SceneData::SearchFrame(DWORD time, BOOL bLoop)
{
	if(!pFrameNode || !pTimeTable)
		return -1;

	if(TotFrameNum == 0)
		return -1;

	if(TotPlayTime == 0) {
		// �� �÷��� Ÿ���� 0�� ���� ù��° �������� ����
		return 0;
	}

	if(bLoop) {
		if(time > 0 ) {
			time = time % TotPlayTime;
			if(time == 0)
				time = TotPlayTime;
		}
		else {
			time = time % TotPlayTime;
		}
	}

	if(time == 0) return 0;
	if(time >= TotPlayTime) return TotFrameNum-1;

	DWORD* pdata = pFrameNode;

	int pos=0;
	for(;;)
	{
		if(pdata[pos] > 0) {
			// ������ ���� ����� ���
			int cnt = pdata[pos++];
			for(int n=0; n<cnt; ++n) {
				// �� �������� ���� �ð��� ��
				if(time <= pTimeTable[pdata[pos]])
					return pdata[pos];
				++pos;
			}
			break;
		}
		else {
			// ���ø� ����� ���
			++pos;
			// ���ø� �ð� ������ �������� ��/���� ���� �̵�
			if(time <= pdata[pos]) {
				// front node
				pos = pdata[pos+1];
			}
			else {
				// back node
				pos = pdata[pos+2];
			}

			continue;
		}
	}

	return -1;
}

float SceneData::GetFrameDelta(int frame, DWORD time, BOOL bLoop)
{
	if(frame < 0)
		return 0;

	float delta = 0.0f;

	if(frame < TotFrameNum) {
		if(bLoop && TotPlayTime > 0) {
			if(time > 0 ) {
				time = time % TotPlayTime;
				if(time == 0)
					time = TotPlayTime;
			}
			else {
				time = time % TotPlayTime;
			}
		}

		DWORD st, et;
		if(GetFrameTime(frame, &st, &et)) {
			if(time <= st) {
				delta = 0.0f;
			}
			else if(time >= et) {
				delta = 1.0f;
			}
			else {
				float wt = (float)(et - st);
				float dt = (float)(time - st);
				if(wt != 0) delta = dt / wt;
			}
		}
	}

	return delta;
}

BOOL SceneData::GetFrameTime(int frame, DWORD* pSTime, DWORD* pETime)
{
	if(TotFrameNum == 0)
		return FALSE;
	if(frame < 0 || frame >= TotFrameNum)
		return FALSE;
	if(!pTimeTable)
		return FALSE;

	// �־��� �������� ���� �ð��� �� �ð��� ����
	if(pSTime) *pSTime = pTimeTable[frame] - ppFrame[frame]->Delay;
	if(pETime) *pETime = pTimeTable[frame];

	return TRUE;
}

BOOL SceneData::BuildFrameTree()
{
	if(TotFrameNum == 0)
		return FALSE;

	if(pFrameNode) { delete pFrameNode; pFrameNode = NULL; }
	if(pTimeTable) { delete pTimeTable; pTimeTable = NULL; }

	pTimeTable = new DWORD[TotFrameNum];

	// ����� �� �÷��� Ÿ�Ӱ� �����Ӻ� ���� ������ ���̺� ����
	TotPlayTime = 0;
	for(int i=0; i<TotFrameNum; i++) {
		TotPlayTime += ppFrame[i]->Delay;
		pTimeTable[i] = TotPlayTime;	// �����Ӻ� ���� ������ ���̺�
	}

	// �ӽù��۸� �󸶳� �Ҵ����� ���
	int totframe = TotFrameNum;
	int aloccnt = 0;
	int magcnt = 1;
	for(;;) 
	{
		if(totframe > MIN_FRAMENUM) {
			totframe /= 2;
			magcnt *= 2;
			aloccnt += magcnt;
		}
		else {
			magcnt += magcnt * MIN_FRAMENUM;
			aloccnt += magcnt;
			break;
		}
	}
	aloccnt *= 2;	// ������

	// �ӽ� ���� �Ҵ�
	DWORD* pdata = new DWORD[aloccnt];

	// ���ȣ��� 2�� Ʈ�� ����
	int length = 0;
	int totblock = RecursiveBuildTree(0, TotFrameNum-1, pdata, length, pTimeTable);

	if(totblock > 0) {
		pFrameNode = new DWORD[length];
		memcpy(pFrameNode, pdata, sizeof(DWORD)*length);
	}

	delete [] pdata;

	return (totblock > 0);
}

int SceneData::RecursiveBuildTree(int si, int ei, DWORD* pdata, int& pos, DWORD* pdelay)
{
	// ==== ������ ���� ====

	// [���ø� ����� ���]
	// 1st: ����� ����(0:���ø����, 0����ŭ:������ ���)
	// 2nd: ���ҵ� ���� �ð�
	// 3rd: ����Ʈ ����� �ε��� ��ġ
	// 4th: �� ����� �ε��� ��ġ

	// [������ ����� ���]
	// 1st: ����� ����(0:���ø����, 0����ŭ:������ ���)
	// 2nd: ������ ����
	// 3rd: ������ ����
	// 4th: (.........) <- ������ ������ ����� ������ ����� ���ڸ�ŭ ���ӵ�

	// =====================


	int totblock = 0;

	int cnt = ei - si + 1;
	if(cnt > MIN_FRAMENUM) {

		// ���ҵ� �߰� ����(���� �ִ� �������� ����)
		int split = si + cnt/2;

		// ���� ���
		int f1 = si;
		int f2 = split;

		// ������ ���
		int b1 = split + 1;
		int b2 = ei;

		pdata[pos++] = 0;				// ī��Ʈ�� 0�̸� ���ø� �����
		pdata[pos++] = pdelay[split];	// ���ҵ� ���� �ð�(�� �����ӱ����� ���� ������)

		int frontpos = pos;
		pdata[pos++] = 0;				// ����Ʈ ����� �ε��� ��ġ

		int backpos = pos;				// �� ����� �ε��� ��ġ
		pdata[pos++] = 0;

		pdata[frontpos] = pos;
		totblock += RecursiveBuildTree(f1, f2, pdata, pos, pdelay);

		pdata[backpos] = pos;
		totblock += RecursiveBuildTree(b1, b2, pdata, pos, pdelay);
	}
	else {
		pdata[pos++] = cnt;				// ī��Ʈ�� 0�� �ƴϸ� ������ �����
		for(int i=0; i<cnt; i++) {
			pdata[pos++] = si+i;
		}

		totblock++;
	}

	return totblock;
}



//////////////////////////////////// FrameData Class ///////////////////////////////////

FrameData::FrameData()
{
	TotCellNum = 0;
	Delay = 0;
	UserVal = 0;
	memset(Reserved, 0, sizeof(Reserved));

	ppCell = NULL;
}

FrameData::~FrameData()
{
	Destroy();
}

void FrameData::Destroy()
{
	if(ppCell) {
		for(int i=0; i<TotCellNum; i++) 
			if(ppCell[i] != NULL) { delete(ppCell[i]); ppCell[i]=NULL; }
			free(ppCell);
			ppCell = NULL;
	}

	TotCellNum = 0;
	Delay = 0;
}

void FrameData::Set(int delay)
{
	Delay = delay;
}

int FrameData::AddCell(int sprno, int rx, int ry, char effect, char grade, WORD color, char mirror, int zoomx, int zoomy, int angle, char interpol, char bias)
{
	TotCellNum += 1;

	CellData** ppNew = (CellData**) malloc(sizeof(CellData*) * TotCellNum);

	if(ppCell) {
		if(TotCellNum > 1) 
			memcpy(ppNew, ppCell, sizeof(CellData*) * (TotCellNum-1));
		free(ppCell);
	}
	ppCell = ppNew;

	ppCell[TotCellNum-1] = new CellData;
	ppCell[TotCellNum-1]->Set(sprno, rx, ry, effect, grade, color, mirror, zoomx, zoomy, angle, interpol, bias);

	return TotCellNum-1;
}

int FrameData::AddCell(CellData* pCell)
{
	TotCellNum += 1;

	CellData** ppNew = (CellData**) malloc(sizeof(CellData*) * TotCellNum);

	if(ppCell) {
		if(TotCellNum > 1) 
			memcpy(ppNew, ppCell, sizeof(CellData*) * (TotCellNum-1));
		free(ppCell);
	}
	ppCell = ppNew;

	ppCell[TotCellNum-1] = new CellData;
	ppCell[TotCellNum-1]->CopyFrom(pCell);

	return TotCellNum-1;
}

void FrameData::DelCell(int num)
{
	if(TotCellNum <= 0) return;
	if(num < 0 || num >= TotCellNum) return;

	TotCellNum -= 1;
	CellData** ppNew = (CellData**) malloc(sizeof(CellData*) * TotCellNum);

	int i;
	for(i=0; i<num; i++) ppNew[i] = ppCell[i];
	for(i=num; i<TotCellNum; i++) ppNew[i] = ppCell[i+1];

	delete(ppCell[num]);
	if(ppCell) free(ppCell);
	ppCell = ppNew;
}

void FrameData::MovCell(int num, int tnum)
{
	if(TotCellNum <= 1) return;
	if(tnum < 0 || tnum >= TotCellNum) return;
	if(num < 0 || num >= TotCellNum) return;
	if(num == tnum) return;

	CellData** ppNew = (CellData**) malloc(sizeof(CellData*) * TotCellNum);

	int t,s;
	for(t=0, s=0; t<TotCellNum; t++, s++)
	{
		if(t == tnum) t++;
		if(s == num) s++;
		if(t < TotCellNum) ppNew[t] = ppCell[s];
	}
	ppNew[tnum] = ppCell[num];

	if(ppCell) free(ppCell);
	ppCell = ppNew;
}

int FrameData::MovCells(int totnum, int* plist, int tnum)
{
	if(TotCellNum <= 0) return -1;
	if(totnum <= 0 || totnum > TotCellNum || !plist) return -1;
	if(tnum < 0 || tnum > TotCellNum) return -1;

	CellData** ppNew = (CellData**) malloc(sizeof(CellData*) * TotCellNum);

	int pos = -1;

	int t=0;
	for(int s=0; s<TotCellNum; s++)
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
				ppNew[t] = ppCell[plist[i]];
				t++;
			}
		}

		if(!bFind) {
			ppNew[t] = ppCell[s];
			t++;
		}
	}

	if(tnum == TotCellNum) 
	{
		pos = t;
		for(int i=0; i<totnum; i++) 
		{
			ppNew[t] = ppCell[plist[i]];
			t++;
		}
	}

	if(ppCell)
		free(ppCell);
	ppCell = ppNew;

	// ���������� ���Ե� ��ġ�� ����
	return pos;
}

int FrameData::CopyCells(int totnum, int* plist, int tnum)
{
	if(TotCellNum <= 0)
		return -1;
	if(totnum <= 0 || totnum > TotCellNum || !plist)
		return -1;
	if(tnum < 0 || tnum > TotCellNum)
		return -1;

	int newtot = TotCellNum + totnum;

	CellData** ppNew = (CellData**) malloc(sizeof(CellData*) * newtot);

	int pos = -1;

	int t=0;
	for(int s=0; s<=TotCellNum; s++)
	{
		if(s == tnum)
		{
			pos = t;
			for(int i=0; i<totnum; i++)
			{
				ppNew[t] = new CellData;
				CellData* pSrc = ppCell[plist[i]];

				// ���� ����
				ppNew[t]->CopyFrom(pSrc);

				t++;
			}
		}

		// Ÿ�� ��ġ�� ������ �������� �Ѿ ����� ���⼭ ����
		if(s == TotCellNum)
			break;

		ppNew[t] = ppCell[s];
		t++;
	}


	TotCellNum += totnum;

	if(ppCell)
		free(ppCell);

	ppCell = ppNew;

	// ���������� ���Ե� ��ġ�� ����
	return pos;
}

// ���� ������ Ŭ�����κ��� �����ؿ´�
void FrameData::CopyFrom(FrameData* pSrc)
{
	Destroy();

	// �������� ����
	Set(pSrc->Delay);

	if(pSrc->TotCellNum > 0) 
	{
		for(int i=0; i<pSrc->TotCellNum; i++)
		{
			CellData *pCell = pSrc->GetPtrCell(i);
			if(pCell)
			{
				AddCell(pCell->SprNo, pCell->Rx, pCell->Ry, pCell->Effect, pCell->Grade, pCell->Color, 
					pCell->Mirror, pCell->ZoomX, pCell->ZoomY, pCell->RotAngle, pCell->Interpol, pCell->Bias);
			}
		}
	}
}

// ���� �����͸� �ε����� ��´�
CellData* FrameData::GetPtrCell(int num)
{
	if(num < 0 || num >= TotCellNum) 
		return NULL;
	return ppCell[num];
}


//////////////////////////////////// Cel Class ///////////////////////////////////

CellData::CellData()
{
	ClearMember();
}

CellData::~CellData()
{
	Destroy();
}

void CellData::ClearMember()
{
	SprNo = 0;
	Rx = Ry = 0;
	Effect = 0;
	Grade = 0;
	Color = 0;
	Mirror = 0;
	Interpol = 0;
	ZoomX = 0;
	ZoomY = 0;
	Bias = 0;
	memset(Reserved1, 0, sizeof(Reserved1));
	RotAngle = 0;
	memset(Reserved2, 0, sizeof(Reserved2));
}

void CellData::Destroy()
{
	ClearMember();
}

void CellData::Set(int sprno, int rx, int ry, char effect, char grade, WORD color, char mirror, int zoomx, int zoomy, int angle, char interpol, char bias)
{
	SprNo = (short)sprno;
	Rx = (short)rx;
	Ry = (short)ry;
	Effect = effect;
	Grade = grade;
	Color = color;
	Mirror = mirror;
	Interpol = interpol;
	ZoomX = (short)zoomx;
	ZoomY = (short)zoomy;
	RotAngle = (short)angle;
	Bias = bias;
}

void CellData::CopyFrom(CellData* pSrc)
{
	Destroy();

	// ���� ����
	Set(pSrc->SprNo, pSrc->Rx, pSrc->Ry, pSrc->Effect, pSrc->Grade, pSrc->Color, 
		pSrc->Mirror, pSrc->ZoomX, pSrc->ZoomY, pSrc->RotAngle, pSrc->Interpol, pSrc->Bias);
}
