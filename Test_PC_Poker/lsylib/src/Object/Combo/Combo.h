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
	//����Ʈ ���� �Լ���
	virtual void SetText(const std::string &strText);
	virtual const std::string GetText(void);
	//==================================================================================================

	//==================================================================================================
	//�ؽ�Ʈ����Ʈ ���� �Լ���
	//�߰� ���� ���� �Լ���
	virtual int AddText(const std::string &strText);
	virtual int InsertText(const int &nIndex, const std::string &strText);

	virtual bool RemoveText(const std::string &strText);
	virtual bool RemoveText(const int &nIndex);
	virtual bool RemoveText(const LPARAM &lParam);
	virtual void RemoveAll(void);

	//������ ������ ���� �Լ���
	virtual void SetItemData(const int &nIndex, const LPARAM &lParam);
	virtual const LPARAM GetItemData(const int nIndex);

	//�������� �� ������ ���Ѵ�.
	virtual const int GetItemCount(void);
	//nIndex�� ������ �ؽ�Ʈ�� ���Ѵ�.
	virtual const std::string GetItemString(const int &nIndex);
	virtual void SetItemString(const int &nIndex, const std::string &strText);
	virtual void SetItemColor(const int &nIndex, const COLORREF &clrColor);

	//nIndex �������� ȭ���� uFormat ��ġ�� ���̵��� �����Ѵ�.
	//uFormat���� DT_VCENTER, DT_TOP, DT_BOTTOM �̴�.
	virtual void SetVisibleItem(int nIndex, UINT uFormat);
	//nIndex �������� ȭ�鿡 ���̰� �Ѵ�. �̹� ���̸� �׷��� �ΰ�
	//���̴��� ���� ���ʿ� ������ TOP���� �Ʒ��� ������ Bottom���� ���̵��� �Ѵ�.
	virtual void SetVisibleItem(int nIndex);

	virtual void SetLineScroll(const bool &bLineScroll);
	virtual const bool IsLineScroll(void);

	//������ ���� ���� �Լ���
	virtual const int &GetCurSelectItem(void);
	virtual void SetCurSelectItem(const int &nSel);

	//���� ���� ���� �Լ���
	virtual void SetLineHeight(const int &nLineHeight);
	virtual const int GetLineHeight(void);

	//nIndex �������� ȭ�鿡 ���̴��� �˾ƺ��� �Լ�
	virtual const bool IsVisualItem(const int &nIndex);
	//ȭ�鿡 ���̴� �������� ptPos�� �ִ� �������� ������� �˾ƺ��� �Լ�
	virtual const int  HitTest(const CPoint &ptPos);

	//ȭ�鿡 ó������ ���̴� ������, ���������� ���̴� �������� �˾Ƴ��� �Լ�
	virtual const int GetFirstVisibleItem(void);
	virtual const int GetLastVisibleItem(void);

	virtual void SetDefaultFindType(bool bUserData);

	//�⺻ ã��� ��Ʈ������ �˻��Ѵ�.
	virtual int FirstFind(const LPARAM &lUserData);
	virtual int NextFind(const LPARAM &lUserData);

	//���γ��̸� �̿��Ͽ� ����Ʈ�� ũ�⸦ �����ϴ� �Լ�
	//nTotalLine * m_nLineHeight�� ���̷� �����Ѵ�.
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