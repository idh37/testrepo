#pragma once

#include "../Base/ObjectBase.h"

namespace LSY
{
class CEdit;
class CTextList;
class CButton;
class CDumyImage;

class UPG_LSYLIB_EXPORT CCombo : public CObjectBase
{
protected:
	CEdit		*m_pEdit;
	CTextList	*m_pList;
	CButton		*m_pButton;

	int			m_nCurSel;
	CDumyImage	*m_pRectImage;
	bool		m_bAttachListShow;
public:
	CCombo(void);
	virtual ~CCombo();

	virtual bool Create(void);
	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_COMBO;}

	//==================================================================================================
	//에디트 관련 함수들
	virtual void SetText(const std::string &strText);
	virtual const std::string GetText(void);
	//==================================================================================================

	//==================================================================================================
	//텍스트리스트 관련 함수들
	//추가 삭제 관련 함수들
	virtual int AddText(const std::string &strText);
	virtual int InsertText(const int &nIndex, const std::string &strText);

	virtual bool RemoveText(const std::string &strText);
	virtual bool RemoveText(const int &nIndex);
	virtual bool RemoveText(const LPARAM &lParam);
	virtual void RemoveAll(void);

	//아이템 데이터 관련 함수들
	virtual void SetItemData(const int &nIndex, const LPARAM &lParam);
	virtual const LPARAM GetItemData(const int nIndex);

	//아이템의 총 갯수를 구한다.
	virtual const int GetItemCount(void);
	//nIndex의 아이템 텍스트를 구한다.
	virtual const std::string GetItemString(const int &nIndex);
	virtual void SetItemString(const int &nIndex, const std::string &strText);
	virtual void SetItemColor(const int &nIndex, const COLORREF &clrColor);

	//nIndex 아이템의 화면의 uFormat 위치에 보이도록 셋팅한다.
	//uFormat값은 DT_VCENTER, DT_TOP, DT_BOTTOM 이다.
	virtual void SetVisibleItem(int nIndex, UINT uFormat);
	//nIndex 아이템을 화면에 보이게 한다. 이미 보이면 그래도 두고
	//보이는쪽 보다 윗쪽에 있으면 TOP으로 아래에 있으면 Bottom으로 보이도록 한다.
	virtual void SetVisibleItem(int nIndex);

	virtual void SetLineScroll(const bool &bLineScroll);
	virtual const bool IsLineScroll(void);

	//아이템 선택 관련 함수들
	virtual const int &GetCurSelectItem(void);
	virtual void SetCurSelectItem(const int &nSel);

	//라인 높이 관련 함수들
	virtual void SetLineHeight(const int &nLineHeight);
	virtual const int GetLineHeight(void);

	//nIndex 아이템이 화면에 보이는지 알아보는 함수
	virtual const bool IsVisualItem(const int &nIndex);
	//화면에 보이는 아이템중 ptPos에 있는 아이템이 어떤것인지 알아보는 함수
	virtual const int  HitTest(const CPoint &ptPos);

	//화면에 처음으로 보이는 아이템, 마지막으로 보이는 아이템을 알아내는 함수
	virtual const int GetFirstVisibleItem(void);
	virtual const int GetLastVisibleItem(void);

	virtual void SetDefaultFindType(bool bUserData);

	//기본 찾기는 스트링으로 검색한다.
	virtual int FirstFind(const LPARAM &lUserData);
	virtual int NextFind(const LPARAM &lUserData);

	//라인높이를 이용하여 리스트의 크기를 결정하는 함수
	//nTotalLine * m_nLineHeight를 높이로 설정한다.
	virtual void SetTotalHeightUseLineHeight(int nTotalLine);
	//==================================================================================================

	virtual void AttachEdit(CEdit *pEdit);
	virtual const CEdit *GetEdit(void){return m_pEdit;}
	virtual void DettachEdit(void);
	virtual void AttachButton(CButton *pButton);
	virtual const CButton *GetButton(void){return m_pButton;}
	virtual void DettachButton(void);
	virtual void AttachList(CTextList *pList);
	virtual const CTextList *GetList(void){return m_pList;}
	virtual void DettachList(void);
public:
	virtual LRESULT OnLocalChildNotify(CMessage *msg);

	virtual LRESULT OnClickButton(CMessage *pMsg);
	virtual LRESULT OnEditReturn(CMessage *pMsg);

	LRESULT OnSelChange(CMessage *pMsg);
	LRESULT OnClickList(CMessage *pMsg);

	virtual bool OnClosePopup(CObjectBase *pFocusObject);
public:
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);
};
} //namespace LSY