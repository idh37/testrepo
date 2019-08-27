#include "stdafx.h"
#include "Sprite.h"
#include "SpriteScene.h"
#include "../../Functor/Message.h"
#include "../Base/DumyImage.h"
#include "../ObjectMan.h"
#include "../../tinyxml/tinyxml.h"

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
CSprite::CSprite(void)
:	m_pSprite(NULL),
	m_nSceneCount(0),
	m_nMultiSceneCount(0),
	m_nRepeatCount(0),
	m_bAfterHide(false),
	m_nPlayIndex(-1),
	m_bUseMultiScene(false),
	m_bAnimation(false),
	m_dwTotalPlayTime(0),
	m_dwPlayTime(0),
	m_dwTotalCurTime(0),
	m_dwCurTime(0),
	m_pMScene(NULL),
	m_pScene(NULL),
	m_bMakeScene(false),
	m_dwCurRepeatCount(0),
	m_fMultiple(1.0f),
	m_dwDelayTime(0),
	m_pRectImage(NULL),
	m_bMessageLogic(false),
	m_bFirst(false)
{
	if(IsTool())
	{
		m_pRectImage = new CDumyImage;
		m_pRectImage->SetLocalAlpha(0.5f);
		m_pRectImage->Create(25, 25);
		m_pRectImage->ClearColor(RGB(128, 128, 20));
		SetLocalRect(m_pRectImage->GetLocalRect());
		AddLocalChild(m_pRectImage);
	}
}

CSprite::~CSprite()
{
	if(m_pSprite)
	{
		RM().RemoveSprite(m_pSprite);
		m_pSprite = NULL;
	}
	RemoveAllScene();
}

CObjectBase *CSprite::NewObject(void)
{
	return new CSprite;
}

CObjectBase *CSprite::Copy(CObjectBase *pObject)
{
	CObjectBase::Copy(pObject);

	CSprite *pSprite			= (CSprite *)pObject;
	if(m_pSprite) pSprite->m_pSprite = RM().Clone(m_pSprite);
	pSprite->m_nSceneCount		= m_nSceneCount;
	pSprite->m_nMultiSceneCount	= m_nMultiSceneCount;
	pSprite->m_nRepeatCount		= m_nRepeatCount;
	pSprite->m_bAfterHide		= m_bAfterHide;
	pSprite->m_nPlayIndex		= m_nPlayIndex;
	pSprite->m_bUseMultiScene	= m_bUseMultiScene;
	pSprite->m_fMultiple		= m_fMultiple;
	pSprite->GetTempPlayTime();
	pSprite->m_bMakeScene  = true;
	if(m_bAnimation) pSprite->StartAnimation(m_dwDelayTime);
	return pObject;
}

size_t CSprite::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(size_t);		//파일명 크기
	if(m_pSprite) nRet += m_pSprite->GetFilePath().size();	//파일명
	nRet += sizeof(int);		//m_nSceneCount
	nRet += sizeof(int);		//m_nMultiSceneCount
	nRet += sizeof(int);		//m_nRepeatCount
	nRet += sizeof(bool);		//m_bAfterHide
	nRet += sizeof(int);		//m_nPlayIndex
	nRet += sizeof(bool);		//m_bUseMultiScene
	nRet += sizeof(float);		//m_fMultiple
	nRet += sizeof(bool);		//m_bAnimation
	return nRet;
}

size_t CSprite::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	size_t nSize = 0;
	if(m_pSprite) nSize = m_pSprite->GetFilePath().size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);		//파일명 크기
	if(m_pSprite)
	{
		memcpy(pDest+nRet, m_pSprite->GetFilePath().c_str(), nSize);
		nRet += nSize;	//파일명
	}
	memcpy(pDest+nRet, &m_nSceneCount, sizeof(int));
	nRet += sizeof(int);		//m_nSceneCount
	memcpy(pDest+nRet, &m_nMultiSceneCount, sizeof(int));
	nRet += sizeof(int);		//m_nMultiSceneCount
	memcpy(pDest+nRet, &m_nRepeatCount, sizeof(int));
	nRet += sizeof(int);		//m_nRepeatCount
	memcpy(pDest+nRet, &m_bAfterHide, sizeof(bool));
	nRet += sizeof(bool);		//m_bAfterHide
	memcpy(pDest+nRet, &m_nPlayIndex, sizeof(int));
	nRet += sizeof(int);		//m_nPlayIndex
	memcpy(pDest+nRet, &m_bUseMultiScene, sizeof(bool));
	nRet += sizeof(bool);		//m_bUseMultiScene
	memcpy(pDest+nRet, &m_fMultiple, sizeof(float));
	nRet += sizeof(float);		//m_fMultiple
	memcpy(pDest+nRet, &m_bAnimation, sizeof(bool));
	nRet += sizeof(bool);		//m_bAnimation
	return nRet;
}

size_t CSprite::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	size_t nSize = 0;
	std::string strFileName;
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);		//파일명 크기
	if(nSize)
	{
		GetPointerToString(pDest+nRet, nSize, strFileName);
		nRet += nSize;	//파일명
	}
	memcpy(&m_nSceneCount, pDest+nRet, sizeof(int));
	nRet += sizeof(int);		//m_nSceneCount
	memcpy(&m_nMultiSceneCount, pDest+nRet, sizeof(int));
	nRet += sizeof(int);		//m_nMultiSceneCount
	memcpy(&m_nRepeatCount, pDest+nRet, sizeof(int));
	nRet += sizeof(int);		//m_nRepeatCount
	memcpy(&m_bAfterHide, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);		//m_bAfterHide
	memcpy(&m_nPlayIndex, pDest+nRet, sizeof(int));
	nRet += sizeof(int);		//m_nPlayIndex
	memcpy(&m_bUseMultiScene, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);		//m_bUseMultiScene
	memcpy(&m_fMultiple, pDest+nRet, sizeof(float));
	nRet += sizeof(float);		//m_fMultiple
	memcpy(&m_bAnimation, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);		//m_bAnimation
	Create(strFileName.c_str());
	if(IsTool()) SetMessage(true);
	m_bMakeScene = true;
	if(m_bAnimation) StartAnimation();
	return nRet;
}

bool CSprite::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	TiXmlElement xmlInheritance("Inheritance");
	xmlInheritance.SetAttribute("path",  m_pSprite?m_pSprite->GetFilePath().c_str():"");
	xmlInheritance.SetAttribute("index",  m_nPlayIndex);
	xmlInheritance.SetAttribute("repeat",  m_nRepeatCount);
	xmlInheritance.SetAttribute("afterhide",  m_bAfterHide?1:0);
	xmlInheritance.SetAttribute("usemultiscene",  m_bUseMultiScene?1:0);
	xmlInheritance.SetDoubleAttribute("multiple",  m_fMultiple);
	pXmlObject->InsertEndChild(xmlInheritance);

	return true;
}

bool CSprite::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	std::string strTemp;

	TiXmlElement *pXmlInheritance = pXmlObject->FirstChildElement("Inheritance");
	GetAttribute(pXmlInheritance, "path", strTemp);
	GetAttribute(pXmlInheritance, "index", m_nPlayIndex);
	GetAttribute(pXmlInheritance, "repeat", m_nRepeatCount);
	GetAttribute(pXmlInheritance, "afterhide", m_bAfterHide);
	GetAttribute(pXmlInheritance, "usemultiscene", m_bUseMultiScene);
	GetAttribute(pXmlInheritance, "multiple", m_fMultiple);

	Create(strTemp.c_str());
	m_bMakeScene = true;

	if(IsTool()) SetMessage(true);

	if(m_nRepeatCount == 0) StartAnimation();

	return true;
}


void CSprite::RemoveAllScene(void)
{
	LISTSCENEITER start = m_listScene.begin();
	LISTSCENEITER end = m_listScene.end();

	for(;start != end;++start)
	{
		delete *start;
	}

	m_listScene.clear();
}

const int &CSprite::GetTotalNum(void)
{
	if(m_bUseMultiScene) return m_nMultiSceneCount;
	return m_nSceneCount;
}

void CSprite::GetTempPlayTime(void)
{
	int nMaxIndex = m_nSceneCount;
	if(m_bUseMultiScene)
	{
		nMaxIndex = m_nMultiSceneCount;
	}

	if(m_pSprite == NULL || m_nPlayIndex < 0 || m_nPlayIndex > nMaxIndex)
	{
		m_dwPlayTime = 0;
	}
	else if(m_bUseMultiScene)
	{
		m_dwPlayTime = m_pSprite->GetMScenePlayTime(m_nPlayIndex);
	}
	else
	{
		m_dwPlayTime = m_pSprite->GetScenePlayTime(m_nPlayIndex);
	}

	m_dwTotalPlayTime = m_dwPlayTime * m_nRepeatCount;
}

void CSprite::SetPlayIndex(const int &nPlayIndex)
{
	if(m_nPlayIndex == nPlayIndex) return;
	m_nPlayIndex = nPlayIndex;
	m_bMakeScene = true;
	GetTempPlayTime();
}

void CSprite::SetRepeatCount(const int &nRepeatCount)
{
	if(m_nRepeatCount == nRepeatCount) return;
	m_nRepeatCount = nRepeatCount;
	GetTempPlayTime();
}

void CSprite::SetUseMultiScene(const bool &bUseMultiScene)
{
	if(m_bUseMultiScene == bUseMultiScene) return;
	m_bUseMultiScene = bUseMultiScene;

	if(m_bUseMultiScene)
	{
		if(m_nPlayIndex >= m_nMultiSceneCount)
		{
			if(m_nMultiSceneCount) m_nPlayIndex = 0;
			else m_nPlayIndex = -1;
		}
	}
	else
	{
		if(m_nPlayIndex >= m_nSceneCount)
		{
			if(m_nSceneCount) m_nPlayIndex = 0;
			else m_nPlayIndex = -1;
		}
	}

	m_bMakeScene = true;
	GetTempPlayTime();
}

bool CSprite::Create(LPCTSTR strPath)
{
	if(m_pSprite)
	{
		if(m_pSprite->GetFilePath() == strPath) return true;
		else RM().RemoveSprite(m_pSprite);

		m_pSprite = NULL;
		m_nSceneCount = 0;
		m_nMultiSceneCount = 0;
	}

	m_pSprite = RM().GetSprite(strPath);
	if(m_pSprite == NULL) return false;

	m_nSceneCount = m_pSprite->scene.TotSceneNum;
	m_nMultiSceneCount = m_pSprite->multiscene.TotMSceneNum;

	if(m_bUseMultiScene)
	{
		if(m_nPlayIndex >= m_nMultiSceneCount) m_nPlayIndex = 0;
	}
	else
	{
		if(m_nPlayIndex >= m_nSceneCount) m_nPlayIndex = 0;
	}

	GetTempPlayTime();

	return true;
}

const std::string CSprite::GetFileName(void)
{
	return m_pSprite?m_pSprite->GetFilePath():"";
}

bool CSprite::StartAnimation(DWORD dwDelayTime/* = 0*/)
{
	int nMaxIndex = m_nSceneCount;
	if(m_bUseMultiScene)
	{
		nMaxIndex = m_nMultiSceneCount;
	}

	if(m_nPlayIndex < 0 || nMaxIndex <= m_nPlayIndex)
	{
		StopAnimation(true);
		if(m_listScene.size())
		{
			if(m_bUseMultiScene)
			{
				LISTSCENEITER start = m_listScene.begin();
				LISTSCENEITER end = m_listScene.end();

				for(;start != end;++start)
				{
					(*start)->SetShow(false);
				}
			}
			else
			{
				m_pScene = *(m_listScene.begin());
				m_pScene->SetShow(false);
			}
		}
		return false;
	}

	if(m_bAnimation) StopAnimation(false);

	m_bAnimation = true;
	m_dwDelayTime = dwDelayTime;
	
	if(m_bUseMultiScene)
	{
		m_pMScene = m_pSprite->multiscene.GetMScenePt(m_nPlayIndex);
		m_pMScene->UpdateAffectClipboxTrack();
	}

	if(m_bMakeScene) MakeScene();

	if(m_bUseMultiScene)
	{
		if(m_listScene.size())
		{
			LISTSCENEITER start = m_listScene.begin();
			LISTSCENEITER end = m_listScene.end();

			for(;start != end;++start)
			{
				(*start)->SetShow(false);
			}
		}
	}
	else
	{
		m_pScene = *(m_listScene.begin());
		m_pScene->SetShow(false);
	}

	m_listPlaySoundTrack.clear();

	m_dwCurTime = 0;
	m_dwCurRepeatCount = 0;

	m_bFirst = true;

	SetUseLogic(true);
	SetShow(true);

	return true;
}

bool CSprite::SetDeltaTime(DWORD dwDelta)
{
	int nMaxIndex = m_nSceneCount;
	if(m_bUseMultiScene)
	{
		nMaxIndex = m_nMultiSceneCount;
	}

	if(nMaxIndex <= m_nPlayIndex) return false;

	m_bAnimation = true;
	m_dwDelayTime = 0;

	if(m_bUseMultiScene)
	{
		m_pMScene = m_pSprite->multiscene.GetMScenePt(m_nPlayIndex);
		m_pMScene->UpdateAffectClipboxTrack();
	}

	if(m_bMakeScene) MakeScene();

	if(m_bUseMultiScene)
	{
		if(m_listScene.size())
		{
			LISTSCENEITER start = m_listScene.begin();
			LISTSCENEITER end = m_listScene.end();

			for(;start != end;++start)
			{
				(*start)->SetShow(false);
			}
		}
	}
	else
	{
		m_pScene = *(m_listScene.begin());
		m_pScene->SetShow(false);
	}

	m_listPlaySoundTrack.clear();

	m_dwCurTime = 0;
	m_dwCurRepeatCount = 0;

	m_bFirst = true;

	Logic(dwDelta);

	return true;
}

void CSprite::StopAnimation(bool bHide/* = true*/)
{
	if(m_bAnimation)
	{
		m_bAnimation = false;
		m_listPlaySoundTrack.clear();
		SetUseLogic(false);
	}

	if(bHide) SetShow(false);
}

void CSprite::MakeScene(void)
{
	RemoveAllScene();
	CSpriteScene *pSScene = NULL;
	if(m_bUseMultiScene)
	{
		int nTotalLayer = m_pMScene->TotTrackNum;
		NMBASE::GRAPHICGDI::TrackData* pTrack = NULL;
		for(int track=0; track<nTotalLayer; ++track) 
		{
			pTrack = m_pMScene->GetPtrTrack(track);
			if(pTrack)
			{
				// 숨김 트랙이면 스킵
				if(pTrack->bHideTrack)
					continue;

				if(pTrack->TrackType == NMBASE::GRAPHICGDI::TTYPE_SCENE)
				{
					NMBASE::GRAPHICGDI::TRACK_SCENE* pScnData = (NMBASE::GRAPHICGDI::TRACK_SCENE*)pTrack->pData;
					pSScene = new CSpriteScene;
					if(pSScene->Create(m_pSprite, pScnData->SceneNo) == false)
					{
						delete pSScene;
						return;
					}

					pSScene->SetLocalClipUseParentPos(true);
					m_listScene.push_back(pSScene);
					AddLocalChild(pSScene);
				}
			}
		}
	}
	else
	{
		pSScene = new CSpriteScene;
		if(pSScene->Create(m_pSprite, m_nPlayIndex) == false)
		{
			delete pSScene;
			return;
		}

		pSScene->SetLocalClipUseParentPos(true);
		m_listScene.push_back(pSScene);
		AddLocalChild(pSScene);
	}
}

void CSprite::GetTrackPlayTime(NMBASE::GRAPHICGDI::TrackData* pTrack, NMBASE::GRAPHICGDI::SceneData **ppScene, DWORD &dwStart, DWORD &dwEnd)
{
	if(pTrack == NULL) return;
	*ppScene = NULL;
	switch(pTrack->TrackType)
	{
	case NMBASE::GRAPHICGDI::TTYPE_SCENE:
		{
			NMBASE::GRAPHICGDI::TRACK_SCENE* pScnData = (NMBASE::GRAPHICGDI::TRACK_SCENE*)pTrack->pData;
			dwStart = pTrack->Delay;
			*ppScene = m_pSprite->scene.GetScenePt(pScnData->SceneNo);

			// 씬의 경우 라이프타임이 0인 경우 반복 회수에 따라 자동으로 시간이 결정됨
			if(pScnData->LifeTime == 0)
				dwEnd = pTrack->Delay + ((*ppScene)->TotPlayTime * pScnData->RepeatCnt);
			else 
				dwEnd = pTrack->Delay + pScnData->LifeTime;
		} 
		break;
	case NMBASE::GRAPHICGDI::TTYPE_SOUND:
		{
			NMBASE::GRAPHICGDI::TRACK_SOUND* pSndData = (NMBASE::GRAPHICGDI::TRACK_SOUND*)pTrack->pData;
			dwStart = pTrack->Delay;
			dwEnd = pTrack->Delay;
		} 
		break;
	case NMBASE::GRAPHICGDI::TTYPE_CLIPBOX:
		{
			NMBASE::GRAPHICGDI::TRACK_CLIPBOX* pCbxData = (NMBASE::GRAPHICGDI::TRACK_CLIPBOX*)pTrack->pData;
			dwStart = pTrack->Delay;
			dwEnd = pTrack->Delay + pCbxData->LifeTime;
		} 
		break;
	case NMBASE::GRAPHICGDI::TTYPE_SCRIPT:
		{
			NMBASE::GRAPHICGDI::TRACK_SCRIPT* pScrData = (NMBASE::GRAPHICGDI::TRACK_SCRIPT*)pTrack->pData;
			dwStart = pTrack->Delay;
			dwEnd = pTrack->Delay;
		} 
		break;
	default:
		{
			dwStart = pTrack->Delay;
			dwEnd = pTrack->Delay;
		} 
		break;
	}	
}

void CSprite::LogicMultiScene(NMBASE::GRAPHICGDI::MSceneData *pMScene)
{
	int nTotalLayer = m_pMScene->TotTrackNum;
	NMBASE::GRAPHICGDI::TrackData* pTrack = NULL;
	CRect rtClip = CRectZero;
	NMBASE::GRAPHICGDI::TrackData* pClipTrack = NULL;
	CImage *pImage = NULL;
	NMBASE::GRAPHICGDI::SceneData *pScene = NULL;

	LISTSCENEITER start = m_listScene.begin();
	LISTSCENEITER end = m_listScene.end();

	LISTSCENE listTempScene;
	DWORD stime, etime;

	for(int track=0; track<nTotalLayer; ++track) 
	{
		pTrack = m_pMScene->GetPtrTrack(track);
		if(pTrack->bHideTrack)
			continue;

		switch(pTrack->TrackType)
		{
		case NMBASE::GRAPHICGDI::TTYPE_SCENE:
			{
				ASSERT(start != end);
				CSpriteScene *pSScene = *start;
				GetTrackPlayTime(pTrack, &pScene, stime, etime);
				if(m_dwCurTime < stime || m_dwCurTime > etime) 
				{
					pSScene->SetShow(false);
					++start;
					continue;
				}

				NMBASE::GRAPHICGDI::TRACK_SCENE* pScnData = (NMBASE::GRAPHICGDI::TRACK_SCENE*)pTrack->pData;
				if(pScnData->cbClipping == 1)	listTempScene.push_back(pSScene);

				NMBASE::GRAPHICGDI::CPI_SCENE cpi;
				if(pTrack->GetInterpol_Scene(m_dwCurTime, &cpi)) 
				{
					BOOL bLoop = TRUE;
					if(pScnData->LifeTime != 0)
					{
						if(cpi.DeltaTime >  pScene->TotPlayTime * pScnData->RepeatCnt)
						{
							bLoop = FALSE;
						}
					}

					pSScene->SetPivot(pScnData->CenterX, pScnData->CenterY);
					pSScene->SetLocalPos((float)(cpi.Pos.x+pScnData->CenterX), (float)(cpi.Pos.y+pScnData->CenterY));
					pSScene->SetLocalScaleX((float)cpi.ZoomX/100.0f);
					pSScene->SetLocalScaleY((float)cpi.ZoomY/100.0f);
					pSScene->SetLocalRotation(TO_RADIAN(-cpi.Angle));
					pSScene->SetLocalAlpha((float)cpi.Alpha/31.0f);

					pSScene->Logic(cpi.DeltaTime, bLoop);
					pSScene->SetShow(true);
				}
				else pSScene->SetShow(false);

				++start;
			} 
			break;
		case NMBASE::GRAPHICGDI::TTYPE_SOUND:
			{
				if(m_listPlaySoundTrack.find(track) != m_listPlaySoundTrack.end()) break;
				if(pTrack->Delay <= m_dwCurTime)
				{
					NMBASE::GRAPHICGDI::TRACK_SOUND* pSndData = (NMBASE::GRAPHICGDI::TRACK_SOUND*)pTrack->pData;
					CMO_AniSound msg(this, pSndData->SndFile, pSndData->Volume);
					Dispatch(&msg);
					m_listPlaySoundTrack.insert(track);
				}
			} 
			break;
		case NMBASE::GRAPHICGDI::TTYPE_CLIPBOX:
			{
				pClipTrack = pTrack;
				rtClip = CRectZero;

				if(pClipTrack && !pClipTrack->bHideTrack)
				{
					NMBASE::GRAPHICGDI::TRACK_CLIPBOX* pCbxData = (NMBASE::GRAPHICGDI::TRACK_CLIPBOX*)pClipTrack->pData;
					stime = pClipTrack->Delay;
					etime = pClipTrack->Delay + pCbxData->LifeTime;
					if(m_dwCurTime >= stime && m_dwCurTime <= etime) {
						NMBASE::GRAPHICGDI::CPI_CLIPBOX cpi;
						if(pClipTrack->GetInterpol_Clipbox(m_dwCurTime, &cpi))
						{
							rtClip = cpi.Rect;
						}
					}
				}

				LISTSCENEITER start_temp = listTempScene.begin();
				LISTSCENEITER end_temp = listTempScene.end();

				for(;start_temp != end_temp;++start_temp)
				{
					(*start_temp)->SetLocalClipRect(rtClip);
				}

				listTempScene.clear();
			}
			break;
		case NMBASE::GRAPHICGDI::TTYPE_SCRIPT:
			{
				//NMBASE::GRAPHICGDI::TRACK_SCRIPT* pScrData = (NMBASE::GRAPHICGDI::TRACK_SCRIPT*)pTrack->pData;
			} 
			break;
		}
	}
}

void CSprite::Logic(const DWORD &dwTime)
{
	if(dwTime < m_dwDelayTime || m_dwPlayTime == 0)
	{
		return;
	}

	if(m_bFirst)
	{
		m_bFirst = false;
		CMO_AniStart msg(this, m_nPlayIndex);
		Dispatch(&msg);
	}

	DWORD dwDelta = dwTime - m_dwDelayTime;
	if(m_fMultiple != 1.0f)	dwDelta = (DWORD)((float)dwDelta * m_fMultiple);
	DWORD dwOldRepeatCount = m_dwCurRepeatCount;
	if(m_dwTotalPlayTime != 0 && m_dwTotalPlayTime <= dwDelta)
	{
		m_dwTotalCurTime = m_dwTotalPlayTime - 1;
		m_dwCurTime = m_dwPlayTime - 1;
		m_dwCurRepeatCount = m_nRepeatCount;
	}
	else
	{
		m_dwTotalCurTime = dwDelta;
		m_dwCurRepeatCount = dwDelta/m_dwPlayTime;
		m_dwCurTime = dwDelta - m_dwPlayTime * m_dwCurRepeatCount;
	}

	if(m_bMessageLogic)
	{
		CMO_AniLogic msg(this, m_dwCurTime);
		Dispatch(&msg);
	}

	//Animation 처리
	if(m_bUseMultiScene)
	{
		LogicMultiScene(m_pMScene);
	}
	else
	{
		m_pScene->SetShow(true);
		m_pScene->Logic(m_dwCurTime, FALSE);
	}

	//Repeat상태가 변경되었을때 메시지를 전달한다.
	if(dwOldRepeatCount != m_dwCurRepeatCount)
	{
		for(DWORD dwRepeat = dwOldRepeatCount+1;dwRepeat<m_dwCurRepeatCount;++dwRepeat)
		{
			CMO_AniRepeat msg(this, (int)dwRepeat);
			Dispatch(&msg);
		}

		m_listPlaySoundTrack.clear();
	}

	//Animation 종료 처리
	if(m_dwTotalPlayTime != 0 && m_dwTotalPlayTime <= dwDelta)
	{
		StopAnimation(false);
		if(m_bAfterHide) SetShow(false);
		CMO_AniEnd msg(this, m_nPlayIndex);
		Dispatch(&msg);
	}
}

bool CSprite::IsIn(IN CPoint &ptPos, OUT bool &bIn)
{
	if(m_bUseMultiScene)
	{
		LISTSCENE::reverse_iterator start = m_listScene.rbegin();
		LISTSCENE::reverse_iterator end = m_listScene.rend();

		for(;start != end;++start)
		{
			if((*start)->IsShow())
			{
				if((*start)->IsIn(ptPos, bIn)) return true;
				if(bIn) return false;
			}
		}
	}
	else
	{
		if(m_pScene->IsShow())
		{
			if(m_pScene->IsIn(ptPos, bIn)) return true;
			if(bIn) return false;
		}
	}

	return __super::IsIn(ptPos, bIn);
}

const CRect CSprite::GetImageRect(int nIndex)
{
	if(m_pSprite == NULL) return CRectZero;
	ASSERT(nIndex >= 0 && nIndex < m_pSprite->GetTotalSpr());
	return CRect(0,0,(float)m_pSprite->spr[nIndex].xl,(float)m_pSprite->spr[nIndex].yl);
}

bool CSprite::AddHandler(int nCommond, IFunctor *pFunctor)
{
	bool bRet = __super::AddHandler(nCommond, pFunctor);
	if(bRet && (nCommond == EM_O_ANILOGIC)) m_bMessageLogic = true;
	return bRet;
}

bool CSprite::RemoveHandler(int nCommond)
{
	bool bRet = __super::RemoveHandler(nCommond);
	if(bRet && (nCommond == EM_O_ANILOGIC)) m_bMessageLogic = false;
	return bRet;
}

void CSprite::SetTrackAni( int nTrack_ ,int nAniPlayIndex_ )
{
	if (NULL == m_pSprite) return;
	if (-1 == m_nPlayIndex) return;
	NMBASE::GRAPHICGDI::MSceneData* ppMS = m_pSprite->multiscene.ppMScene[m_nPlayIndex];
	if (NULL == ppMS) return;
	if (ppMS->TotTrackNum <= nTrack_ ) return;
	NMBASE::GRAPHICGDI::TRACK_SCENE* pScnData = ppMS->ppTrack[nTrack_]->GetDataScene();
	if (NULL == pScnData) return;
	pScnData->SceneNo = nAniPlayIndex_;
}

void CSprite::SetShowTrack( int nTrack_ ,bool bShow_ )
{
	if (NULL == m_pSprite) return;
	if (-1 == m_nPlayIndex) return;
	NMBASE::GRAPHICGDI::MSceneData* ppMS = m_pSprite->multiscene.ppMScene[m_nPlayIndex];
	if (NULL == ppMS) return;
	if (ppMS->TotTrackNum <= nTrack_ ) return;
	ppMS->ppTrack[nTrack_]->bHideTrack = (!bShow_);
}


bool CSprite::GetTrackPos(int nTrack_, CPoint &cpPos)
{
	if (NULL == m_pSprite) return false;
	if (-1 == m_nPlayIndex) return false;
	NMBASE::GRAPHICGDI::MSceneData* ppMS = m_pSprite->multiscene.ppMScene[m_nPlayIndex];
	if (NULL == ppMS) return false;
	if (ppMS->TotTrackNum <= nTrack_ ) return false;
	NMBASE::GRAPHICGDI::TRACK_SCENE* pScnData = ppMS->ppTrack[nTrack_]->GetDataScene();
	cpPos.x=pScnData->Pos.x;
	cpPos.y=pScnData->Pos.y;
	return true;
}

void CSprite::SetTrackPos(int nTrack_, CPoint cpPos)
{
	if (NULL == m_pSprite) return;
	if (-1 == m_nPlayIndex) return;
	NMBASE::GRAPHICGDI::MSceneData* ppMS = m_pSprite->multiscene.ppMScene[m_nPlayIndex];
	if (NULL == ppMS) return;
	if (ppMS->TotTrackNum <= nTrack_ ) return;
	NMBASE::GRAPHICGDI::TRACK_SCENE* pScnData = ppMS->ppTrack[nTrack_]->GetDataScene();
	pScnData->Pos.x=cpPos.x;
	pScnData->Pos.y=cpPos.y;
}

bool CSprite::GetClipBox(int nTrack_, CRect &crArea)
{
	if (NULL == m_pSprite) return false;
	if (-1 == m_nPlayIndex) return false;
	NMBASE::GRAPHICGDI::MSceneData* ppMS = m_pSprite->multiscene.ppMScene[m_nPlayIndex];
	if (NULL == ppMS) return false;
	if (ppMS->TotTrackNum <= nTrack_ ) return false;
	if(NMBASE::GRAPHICGDI::TTYPE_CLIPBOX!=ppMS->ppTrack[nTrack_]->TrackType) return false;
	NMBASE::GRAPHICGDI::TRACK_CLIPBOX* pCbxData = (NMBASE::GRAPHICGDI::TRACK_CLIPBOX*)(ppMS->ppTrack[nTrack_]->pData);
	crArea.left=pCbxData->Rect.left;
	crArea.right=pCbxData->Rect.right;
	crArea.top=pCbxData->Rect.top;
	crArea.bottom=pCbxData->Rect.bottom;
	return true;
}

void CSprite::SetClipBox(int nTrack_, CRect crArea)
{
	if (NULL == m_pSprite) return;
	if (-1 == m_nPlayIndex) return;
	NMBASE::GRAPHICGDI::MSceneData* ppMS = m_pSprite->multiscene.ppMScene[m_nPlayIndex];
	if (NULL == ppMS) return;
	if (ppMS->TotTrackNum <= nTrack_ ) return;
	if(NMBASE::GRAPHICGDI::TTYPE_CLIPBOX!=ppMS->ppTrack[nTrack_]->TrackType) return;
	NMBASE::GRAPHICGDI::TRACK_CLIPBOX* pCbxData = (NMBASE::GRAPHICGDI::TRACK_CLIPBOX*)(ppMS->ppTrack[nTrack_]->pData);
	pCbxData->Rect.left=crArea.left;
	pCbxData->Rect.right=crArea.right;
	pCbxData->Rect.top=crArea.top;
	pCbxData->Rect.bottom=crArea.bottom;
}

void CSprite::ChangeMultiScene(int nTrack, int nSceneNum)
{
	if(NULL==m_pSprite) return;
	NMBASE::GRAPHICGDI::MSceneData* ppMS=m_pSprite->multiscene.ppMScene[m_nPlayIndex];
	if(NULL==ppMS) return;
	if(ppMS->TotTrackNum<=nTrack) return;
	NMBASE::GRAPHICGDI::TRACK_SCENE* pScnData=(NMBASE::GRAPHICGDI::TRACK_SCENE*)ppMS->ppTrack[nTrack]->GetDataScene();
	if(NULL==pScnData) return;
	pScnData->SceneNo=nSceneNum;	
}

//#if defined(_DEBUG)
//		m_pListView->m_pSprTest=&m_sprTest;
//		NMBASE::GRAPHICGDI::MSceneData* ppMS=m_sprTest.multiscene.ppMScene[0];
//		if(NULL==ppMS) return;
//		if(NMBASE::GRAPHICGDI::TTYPE_CLIPBOX==ppMS->ppTrack[1]->TrackType){
//			NMBASE::GRAPHICGDI::TRACK_CLIPBOX* pCbxData = (NMBASE::GRAPHICGDI::TRACK_CLIPBOX*)(ppMS->ppTrack[1]->pData);
//			pCbxData->Rect.right=150;
//		}
//#endif

} //namespace LSY