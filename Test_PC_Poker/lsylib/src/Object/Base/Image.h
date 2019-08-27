#pragma once

#include "ObjectBase.h"

namespace LSY
{
class UPG_LSYLIB_EXPORT CImage :public CObjectBase
{
protected:
	NMBASE::GRAPHICGDI::xSprite		*m_pSprite;
	int			m_nIndex;			//이미지 인덱스
	int			m_nTotalNum;

	//조합형 이미지(Attach된 이미지들을 가지고 이미지를 조합하여 하나의 이미지로 사용하는 것을 말한다)
	//수직형일때 현재 이미지는 상단을 Middle이미지는 중간의 이미지를, End는 하단의 이미지를 나타낸다.
	//수평형일때 현재 이미지는 좌측을 Middle이미지는 중간의 이미지를, End는 우측의 이미지를 나타낸다.
	//MiddleCount는 중간이미지를 몇번 반복할지를 나타낸다.
	CImage		*m_pMiddleImage;
	CImage		*m_pEndImage;
	int			m_nMiddleCount;		//준간 이미지 갯수
	bool		m_bHorizontality;	//수평형 인지의 여부, 수평형일때
	LISTOBJECT	m_listMiddleImage;
public:
	CImage(void);
	virtual ~CImage();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_IMAGE;}

	bool Create(LPCTSTR lpPath);
	bool Create(NMBASE::GRAPHICGDI::xSprite *pSprite);

	virtual void DrawLocal(NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawMy(NMBASE::GRAPHICGDI::CPage *pPage);

	//화면에 표시할 이미지 인덱스 지정
	void SetIndex(int nIndex, bool bForce = false);
	//현재 표시되는 인덱스를 가져온다.
	const int &GetIndex(void){return m_nIndex;};
	//전체 이미지 갯수를 가져온다.
	const int &GetTotalNum(void);
	const CRect GetImageRect(int nIndex);

	void AttachMiddle(CImage *pImage);
	const CImage *GetMiddle(void){return m_pMiddleImage;}
	void DettachMiddle(void);

	void AttachEnd(CImage *pImage);
	const CImage *GetEnd(void){return m_pEndImage;}
	void DettachEnd(void);

	void SetMiddleCount(const int &nMiddleCount);
	const int &GetMiddleCount(void){return m_nMiddleCount;}

	void SetHorizontality(const bool &bHorizontality);
	const bool &IsHorizontality(void){return m_bHorizontality;}

	std::string GetFileName(void);
	bool IsPossibilityIndex(const int &nIndex);

	CImage *GetMiddleImage(int nIndex);
	int GetMiddleImageToIndex(CImage *pImage);
	NMBASE::GRAPHICGDI::xSprite	*GetSprite(void){return m_pSprite;}

	virtual void CheckEventDraw(void);
public:
	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);

protected:
	void MakeMiddleEndImage(void);
	void ReleaseMiddleEndImage(void);
};
} //namespace LSY