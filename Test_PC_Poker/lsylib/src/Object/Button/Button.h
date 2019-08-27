#pragma once

#include "../Base/Image.h"

namespace LSY
{
class CTextButton;
class UPG_LSYLIB_EXPORT CButton : public CObjectBase
{
protected:
	ENUM_BUTTON_STATUS		m_nStatus;					//상태별
	ENUM_BUTTON_STATUS_TYPE	m_nStatusType;				//버튼의 종류
	int						m_listStatusIndex[EBS_MAX];	//상태별 이미지 인덱스

	CImage					*m_pImage;
	CTextButton				*m_pTextButton;
public:
	CButton(void);
	virtual ~CButton();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_BUTTON;}

	virtual bool Create(LPCTSTR lpPath);
public:
	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual void AttachTextButton(CTextButton *pButton);
	virtual const CTextButton *GetTextButton(void){return m_pTextButton;}
	virtual void DettachTextButton(void);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);

	virtual ENUM_BUTTON_STATUS GetButtonStatus(void){return m_nStatus;}
	std::string GetFileName(void);

	virtual void SetFourStatusButton(int nStartIndex);
	virtual void SetThreeStatusButton(int nStartIndex);
	virtual void SetTwoStatusButton(int nStartIndex);
	virtual void SetOneStatusButton(int nStartIndex);
	virtual void SetFreeStatusButton(int *plistStatusIndex);

	//자유롭게 버튼 인덱스를 정해줄수 있을때만 사용가능
	virtual void SetButtonStatusIndex(ENUM_BUTTON_STATUS nStatus, int nIndex);
	ENUM_BUTTON_STATUS_TYPE GetButtonStatusType(void){return m_nStatusType;}
	int GetButtonStatusIndex(ENUM_BUTTON_STATUS nStatus);
	void GetButtonStatusIndex(int *pListIndex);
	int *GetButtonStatusIndexList(void){return m_listStatusIndex;}

	const int GetTotalNum(void);

	virtual void SetEnable(const bool &bEnable);
	virtual void SetEnableRef(const bool &bEnableRef);
	virtual void OnMouseOver(void);
	virtual void OnMouseLeave(void);
	virtual bool OnLButtonDown(const CPoint &ptPos);
	virtual bool OnLButtonDbClick(const CPoint &ptPos);
	virtual bool OnLButtonUP(const CPoint &ptPos);

	virtual const CRect &GetImageRect(void);

	virtual void CheckEventDraw(void);
	virtual void SetTempEventDraw(const bool &bEventDraw);

	void ForceChangStatusNomal();
	void ForceChangStatusOver();
protected:
	virtual void SetStatusIndex(void);
	virtual void SetButtonStatus(const ENUM_BUTTON_STATUS &nStatus);
	void InitButtonStatus(int nIndex = -1);

	CImage *GetImage(void){return m_pImage;}

	friend class CSlideBar;
};
} //namespace LSY