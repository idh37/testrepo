#pragma once

#include "../Base/ObjectBase.h"
#include <set>

class NMBASE::GRAPHICGDI::MSceneData;
class NMBASE::GRAPHICGDI::TrackData;
class NMBASE::GRAPHICGDI::SceneData;

namespace LSY
{
class CSpriteScene;
class CDumyImage;
class UPG_LSYLIB_EXPORT CSprite : public CObjectBase
{
protected:
	NMBASE::GRAPHICGDI::xSprite			*m_pSprite;

	int				m_nSceneCount;
	int				m_nMultiSceneCount;

	bool			m_bMakeScene;			//재생을 시작할때 Scene을 생성할지의 여부를 나타냄

	int				m_nRepeatCount;			//반복 횟수, 0이면 무한 반복
	bool			m_bAfterHide;			//재생후 숨김
	int				m_nPlayIndex;			//재생 인덱스 번호
	bool			m_bUseMultiScene;		//멀티씬을 재생하는지의 여부

	bool			m_bAnimation;			//재생 여부
	DWORD			m_dwTotalPlayTime;		//전체 재생 시간, 0이면 무한 반복
	DWORD			m_dwPlayTime;			//1회 재생시 재생 시간

	DWORD			m_dwTotalCurTime;		//현재 시간(재생후 지난 시간)
	DWORD			m_dwCurTime;			//현재 시간(리핏 된후 지난 시간)
	DWORD			m_dwDelayTime;			//재생시 대기 시간
	float			m_fMultiple;			//재생 배속

	bool			m_bFirst;

	DWORD			m_dwCurRepeatCount;
	bool			m_bMessageLogic;

	NMBASE::GRAPHICGDI::MSceneData		*m_pMScene;
	CSpriteScene	*m_pScene;

	typedef std::list<CSpriteScene *>	LISTSCENE;
	typedef LISTSCENE::iterator			LISTSCENEITER;

	std::set<int>	m_listPlaySoundTrack;

	LISTSCENE		m_listScene;
	CDumyImage		*m_pRectImage;
public:
	CSprite(void);
	virtual ~CSprite();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_SPRITE;}

	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);

	virtual void Logic(const DWORD &dwTime);
	virtual void LogicMultiScene(NMBASE::GRAPHICGDI::MSceneData *pMScene);

	virtual bool Create(LPCTSTR strPath);
	virtual const std::string GetFileName(void);

	virtual void SetRepeatCount(const int &nRepeatCount);
	virtual const int &GetRepeatCount(void){return m_nRepeatCount;}

	virtual void SetAfterHide(const bool &bAfterHide){m_bAfterHide = bAfterHide;}
	virtual const bool &IsAfterHide(void){return m_bAfterHide;}

	virtual const int &GetTotalNum(void);
	const CRect GetImageRect(int nIndex);

	virtual void SetPlayIndex(const int &nPlayIndex);
	virtual const int &GetPlayIndex(void){return m_nPlayIndex;}

	virtual void SetUseMultiScene(const bool &bUseMultiScene);
	virtual const bool &IsUseMultiScene(void){return m_bUseMultiScene;}

	virtual bool IsHaveMultiScene(void){return m_nMultiSceneCount?true:false;}

	virtual void SetMultiple(const float &fMultiple){m_fMultiple = fMultiple;}
	virtual const float &GetMultiple(void){return m_fMultiple;}

	virtual bool StartAnimation(DWORD dwDelayTime = 0);
	virtual void StopAnimation(bool bHide = true);

	virtual const bool &IsAnimation(void){return m_bAnimation;}

	virtual void  SetCurTime(DWORD curtime){m_dwCurTime = curtime;}
	virtual const DWORD &GetCurTime(void){return m_dwCurTime;}
	virtual const DWORD &GetPlayTime(void){return m_dwPlayTime;}
	virtual const DWORD &GetTotalPlayTime(void){return m_dwTotalPlayTime;}
	virtual const DWORD &GetCurrentRepeatCount(void){return m_dwCurRepeatCount;}

	virtual bool SetDeltaTime(DWORD dwDelta);
	virtual bool IsIn(IN CPoint &ptPos, OUT bool &bIn);

	virtual bool AddHandler(int nCommond, IFunctor *pFunctor);
	virtual bool RemoveHandler(int nCommond);
protected:
	virtual void RemoveAllScene(void);
	virtual void MakeScene(void);
	virtual void GetTrackPlayTime(NMBASE::GRAPHICGDI::TrackData* pTrack, NMBASE::GRAPHICGDI::SceneData **ppScene, DWORD &dwStart, DWORD &dwEnd);
	virtual void GetTempPlayTime(void);

public:
	void SetTrackAni(int nTrack_ ,int nAniPlayIndex_ );
	void SetShowTrack(int nTrack_ ,bool bShow_ );
	bool GetTrackPos(int nTrack_, CPoint &cpPos);
	void SetTrackPos(int nTrack_, CPoint cpPos);
	bool GetClipBox(int nTrack_, CRect &crArea);
	void SetClipBox(int nTrack_, CRect crArea);

public:
	virtual void ChangeMultiScene(int nTrack, int nSceneNum);
};
} //namespace LSY