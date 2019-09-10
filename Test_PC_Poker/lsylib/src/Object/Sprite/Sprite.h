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

	bool			m_bMakeScene;			//����� �����Ҷ� Scene�� ���������� ���θ� ��Ÿ��

	int				m_nRepeatCount;			//�ݺ� Ƚ��, 0�̸� ���� �ݺ�
	bool			m_bAfterHide;			//����� ����
	int				m_nPlayIndex;			//��� �ε��� ��ȣ
	bool			m_bUseMultiScene;		//��Ƽ���� ����ϴ����� ����

	bool			m_bAnimation;			//��� ����
	DWORD			m_dwTotalPlayTime;		//��ü ��� �ð�, 0�̸� ���� �ݺ�
	DWORD			m_dwPlayTime;			//1ȸ ����� ��� �ð�

	DWORD			m_dwTotalCurTime;		//���� �ð�(����� ���� �ð�)
	DWORD			m_dwCurTime;			//���� �ð�(���� ���� ���� �ð�)
	DWORD			m_dwDelayTime;			//����� ��� �ð�
	float			m_fMultiple;			//��� ���

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