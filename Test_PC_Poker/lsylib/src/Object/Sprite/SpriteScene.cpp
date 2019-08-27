#include "stdafx.h"
#include "SpriteScene.h"
#include "../Base/Image.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TO_RADIAN(x) ((x) * 0.0174773f)
#define FLOATROUNDINT(x) ( (int)((x)+0.5f) )
#define FLOATROUND2(x) ( floor(10.0*((x)+0.05))/10.0 )

namespace LSY
{
CSpriteScene::CSpriteScene(void)
:	m_pSprite(NULL),
	m_pScene(NULL),
	m_nSceneIndex(-1),
	m_nMaxLayer(0)
{
}

CSpriteScene::~CSpriteScene()
{
	if(m_pSprite)
	{
		RM().RemoveSprite(m_pSprite);
		m_pSprite;
	}

	RemoveAllLayer();
}

void CSpriteScene::RemoveAllLayer(void)
{
	LISTLAYERITER start = m_listLayer.begin();
	LISTLAYERITER end = m_listLayer.end();

	for(;start != end;++start)
	{
		delete *start;
	}

	m_listLayer.clear();
	m_nMaxLayer = 0;
}

bool CSpriteScene::Create(NMBASE::GRAPHICGDI::xSprite *pSprite, int nSceneIndex)
{
	ASSERT(pSprite != NULL);
	if(m_pSprite)
	{
		RM().RemoveSprite(m_pSprite);
		m_pSprite = NULL;
	}

	m_pSprite = RM().Clone(pSprite);
	m_nSceneIndex = nSceneIndex;

	m_pScene = m_pSprite->GetScenePt(m_nSceneIndex);

	RemoveAllLayer();

	NMBASE::GRAPHICGDI::FrameData *pFrame = NULL;
	int nMaxFrame = m_pScene->TotFrameNum;
	for(int i=0;i<nMaxFrame;++i)
	{
		pFrame = m_pScene->GetPtrFrame(i);
		if(m_nMaxLayer < pFrame->TotCellNum) m_nMaxLayer = pFrame->TotCellNum;
	}

	MakeLayer();
	return true;
}

void CSpriteScene::MakeLayer(void)
{
	CImage *pImage = NULL;

	for(int i=0;i<m_nMaxLayer;++i)
	{
		pImage = new CImage;
		if(pImage->Create(m_pSprite) == false)
		{
			delete pImage;
			return;
		}

		m_listLayer.push_back(pImage);
		AddLocalChild(pImage);
		pImage->SetShow(false);
	}
}

CObjectBase *CSpriteScene::NewObject(void)
{
	return new CSpriteScene;
}

CObjectBase *CSpriteScene::Copy(CObjectBase *pObject)
{
	CObjectBase::Copy(pObject);

	CSpriteScene *pSprite			= (CSpriteScene *)pObject;
	return pObject;
}

BOOL CSpriteScene::GetDrawingCellInfo(NMBASE::GRAPHICGDI::FrameData* pFrame, NMBASE::GRAPHICGDI::FrameData* pNextFrame, int cell, float fdelta, CImage *pLayer)
{
	if(pFrame == NULL || pNextFrame == NULL) return FALSE;

	NMBASE::GRAPHICGDI::CellData* pCell = pFrame->GetPtrCell(cell);
	if(!pCell) return FALSE;
	if(pCell->SprNo < 0 || pCell->SprNo >= m_pSprite->sfhead.TotalSprNum) return FALSE;

	float xp = (float)(pCell->Rx);
	float yp = (float)(pCell->Ry);
	int sprno = pCell->SprNo;

	NMBASE::GRAPHICGDI::SPRITE30 *pSprite = &m_pSprite->spr[sprno];

	float w = (float)pSprite->xl;
	float h = (float)pSprite->yl;

	int opt = 0;
	float grade = (float)pCell->Grade;
	WORD color = pCell->Color;

	ENUM_OBJECTEFFECTTYPE nEffect = EOET_NORMAL;
	switch(pCell->Effect) 
	{
	case 1 : nEffect = EOET_HALFALPHA; break;
	case 2 : nEffect = EOET_STEPALPHA; break;
	case 3 : nEffect = EOET_LIGHT; break;
	case 4 : nEffect = EOET_DARKEN; break;
	case 5 : nEffect = EOET_GLOW; break;
	case 6 : nEffect = EOET_MONO; break;
	case 7 : nEffect = EOET_GLAYSCALE; break;
	case 8 : nEffect = EOET_ALPHA; break;
	}

	pLayer->SetEffect(nEffect);

	bool bXFlip = false;
	bool bYFlip = false;
	if(pCell->Mirror == 1) {
		bXFlip = true;
		pLayer->SetXFlip(true);
	}
	if(pCell->Mirror == 2) {
		bYFlip = true;
		pLayer->SetYFlip(true);
	}

	if(pLayer->IsXFlip() != bXFlip) pLayer->SetXFlip(bXFlip);
	if(pLayer->IsYFlip() != bYFlip) pLayer->SetYFlip(bYFlip);

	float zoomx = pCell->ZoomX;
	float zoomy = pCell->ZoomY;
	float angle = (float)pCell->RotAngle;

	if(pCell->Interpol == 1) {
		NMBASE::GRAPHICGDI::CellData* pCell2 = pNextFrame->GetPtrCell(cell);

		if(pCell2) {

			float delta = 0;
			float bias = (float)(pCell->Bias / 100.0);

			if(bias < 0.0001f && bias > -0.0001f) bias = 0;
			if(bias > 1.0f) bias = 1.0f;
			if(bias < -1.0f) bias = -1.0f;

			if(bias == 0) {
				delta = fdelta;
			}
			else if(bias < 0) {
				float ang = fdelta * TO_RADIAN(90);
				float m = (float)(1.0 - _cos(ang));
				delta = fdelta + ((fdelta - m) * bias);
			}
			else {
				float ang = fdelta * TO_RADIAN(90);
				float m = (float)_sin(ang);
				delta = fdelta + ((m - fdelta) * bias);
			}

			// 좌표 보간
			xp += ((pCell2->Rx - pCell->Rx) * delta);
			yp += ((pCell2->Ry - pCell->Ry) * delta);

			// 알파값 보간
			grade += FLOATROUNDINT((pCell2->Grade - pCell->Grade) * delta);

			// 컬러 보간
			if(pCell2->Effect == 5 || pCell2->Effect == 6) {
				int r1 = (int)R555(pCell->Color);
				int g1 = (int)G555(pCell->Color);
				int b1 = (int)B555(pCell->Color);
				int r2 = (int)R555(pCell2->Color);
				int g2 = (int)G555(pCell2->Color);
				int b2 = (int)B555(pCell2->Color);

				int r3 = r1 + FLOATROUNDINT((r2 - r1) * delta);
				int g3 = g1 + FLOATROUNDINT((g2 - g1) * delta);
				int b3 = b1 + FLOATROUNDINT((b2 - b1) * delta);

				color = NMBASE::GRAPHICGDI::GetRGBmix(r3, g3, b3);
			}

			// 줌 비율 보간
			zoomx += ((pCell2->ZoomX - pCell->ZoomX) * delta);
			zoomy += ((pCell2->ZoomY - pCell->ZoomY) * delta);

			// 회전 비율 보간
			angle += ((pCell2->RotAngle - pCell->RotAngle) * delta);
		}
	}

	pLayer->SetPivot((float)pSprite->mx, (float)pSprite->my);
	pLayer->SetLocalPos((float)(xp + pSprite->mx), (float)(yp + pSprite->my));
	pLayer->SetLocalScaleX(zoomx/100.0f);
	pLayer->SetLocalScaleY(zoomy/100.0f);
	pLayer->SetLocalRotation(TO_RADIAN(-angle));
	pLayer->SetLocalAlpha((float)grade/31.0f);
	pLayer->SetIndex(sprno);
	pLayer->SetColor(color);

	return TRUE;
}

void CSpriteScene::Logic(const DWORD &dwTime, const BOOL &bLoop)
{
	int frame = m_pScene->SearchFrame(dwTime, bLoop);
	if(frame < 0)
	{
		SetShow(false);
		return;
	}
	float delta = m_pScene->GetFrameDelta(frame, dwTime, bLoop);

	NMBASE::GRAPHICGDI::FrameData* pFrame = m_pScene->GetPtrFrame(frame);

	if(pFrame == NULL || pFrame->TotCellNum == 0)
	{
		SetShow(false);
		return;
	}

	int nNextframe = frame + 1;
	if(nNextframe >= m_pScene->TotFrameNum) nNextframe = 0;

	NMBASE::GRAPHICGDI::FrameData* pNextFrame = m_pScene->GetPtrFrame(nNextframe);

	LISTLAYERITER start = m_listLayer.begin();
	LISTLAYERITER end = m_listLayer.end();

	for(int i=0; i<m_nMaxLayer; ++i, ++start)
	{
		if(i<pFrame->TotCellNum)
		{
			(*start)->SetShow(true);
		}
		else
		{
			(*start)->SetShow(false);
			continue;
		}

		// 셀 출력 정보를 얻음
		if(!GetDrawingCellInfo(pFrame, pNextFrame, i, delta, *start))
		{
			(*start)->SetShow(false);
			continue;
		}

		//// 오버라이드 옵션이 설정되어 있다면 그 값으로 대체
		//if(n_opt != 0) {
		//	if(n_opt & LR || n_opt & UD || n_opt & TEMPCLIP || n_opt & MIRROR || n_opt & PERCENT || n_opt & RXY || n_opt & CENTER) {
		//		// 조합 가능한 부가 옵션인 경우
		//		desc.opt |= n_opt;
		//	}

		//	if(n_opt & HB || n_opt & GB || n_opt & GLOW || n_opt & GRAYSCL || n_opt & MONO || n_opt & ANTI || n_opt & LE || n_opt & DE || n_opt & AB) {
		//		// 조합이 불가능한 효과 옵션인 경우
		//		desc.opt = n_opt;
		//		desc.grade = n_grade; 
		//		desc.color = n_color;
		//	}
		//}

		//if(pXSpr->spr[desc.sprno].xl == desc.width && pXSpr->spr[desc.sprno].yl == desc.height && desc.angle == 0) {
		//	PutSprAuto((int)desc.xp, (int)desc.yp, pXSpr, desc.sprno, desc.opt, desc.grade, desc.color);
		//}
		//else if(desc.angle == 0){
		//	if(desc.width != 0 && desc.height != 0)
		//		ZoomSprAuto((int)desc.xp, (int)desc.yp, pXSpr, desc.sprno, (int)desc.width, (int)desc.height, desc.opt, desc.grade, desc.color);
		//} 
		//else {
		//	if(desc.width != 0 && desc.height != 0)
		//		RotateZoomSprAuto((int)desc.xp, (int)desc.yp, pXSpr, desc.sprno, (int)desc.width, (int)desc.height, (float)desc.angle, desc.opt, desc.grade, desc.color);
		//}
	}

	//SetShow(true);
}

bool CSpriteScene::IsIn(IN CPoint &ptPos, OUT bool &bIn)
{
	LISTLAYER::reverse_iterator start = m_listLayer.rbegin();
	LISTLAYER::reverse_iterator end = m_listLayer.rend();

	for(;start != end;++start)
	{
		if((*start)->IsShow())
		{
			if((*start)->IsIn(ptPos, bIn)) return true;
			if(bIn) return false;
		}
	}

	return __super::IsIn(ptPos, bIn);
}
} //namespace LSY