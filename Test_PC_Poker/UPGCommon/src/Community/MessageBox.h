#pragma once

#include "CommunityManager.h"

typedef CCommunityManager::CMemoCompare::ENUM_MEMO_COMPARE_TYPE MEMO_COMPARE_TYPE;

class UPG_COMMON_EXPORT CMessageBox : public LSY::CLSYLibDialogBase
{
protected:
	int				m_nSelID;
	int				m_nCurPage;

	LSY::CGroup			*m_pGrpAll;
	LSY::CSprite		*m_pLoading;
	LSY::CCheckButton	*m_pAllCheck;

	LSY::CButton	*m_pBtnGoFirst;
	LSY::CButton	*m_pBtnGoPrev;
	LSY::CButton	*m_pBtnGoNext;
	LSY::CButton	*m_pBtnGoLast;

	bool			m_bRecevieMemoBox;
	bool			m_bSendMemoBox;
	bool			m_bSuccessMemoBox;

	bool			m_bRecevieStorageMemoBox;
	bool			m_bSendStorageMemoBox;
	bool			m_bSuccessStorageMemoBox;

	int				m_nSendRemoveCount;
	int				m_nSendMemoListToStorageCount;

	LSY::CButton		*m_pListBtnGoNumber[5];
	LSY::CTextButton	*m_pListTxtBtnGoNumber[5];

	bool				m_bSearch;
	bool				m_bSearchNickName;
	std::string			m_strSearch;

	MEMO_COMPARE_TYPE	m_nCompareType;
	bool				m_bIncrement;
public:
	CMessageBox(CWnd* pParent = NULL);
	virtual ~CMessageBox(){}

	virtual bool OnCreatedProject(void);
	virtual LSY::CObjectBase *OnCallCreateObject(const int &nID, const std::string &strName);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnMouseOver(LSY::CMessage *pMsg);
	LRESULT OnMouseLeave(LSY::CMessage *pMsg);
	LRESULT OnTabChildNotify(LSY::CMessage *pMsg);
	LRESULT OnLButtonDbClick(LSY::CMessage *pMsg);
	LRESULT OnReturn(LSY::CMessage *pMsg);
	LRESULT OnCheckChange(LSY::CMessage *pMsg);
	LRESULT OnListItemNickNameDbClick(LSY::CMessage *pMsg);
	LRESULT OnComboSearchTypeChange(LSY::CMessage *pMsg);

	virtual INT_PTR DoModal(CWnd *pParent);

	afx_msg void OnDestroy();

	void SetTabChange(int nID, bool bCheckSameID = true);
	void SetChangePage(int nPage);
public:
	void CheckMessageCount(const int &nMemoCount, const int &nStorageCount);
	void CheckTabColor(const int &nMemoCount, const int &nStorageCount);
	void CheckGoButton(const int &nCount);
	void OnSearch(void);
	void OnReport(void);
	void OnReadMemoList(void);
	void ReceiveMemoList(bool bMemoBox);
	void ResultRemoveMemo(const bool &bSuccess);
	void ResultMoveBoxToStrageMemo(const bool &bSuccess);
	void DisableList(void);
protected:
	DECLARE_MESSAGE_MAP()
};