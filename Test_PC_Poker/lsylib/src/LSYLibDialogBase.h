#pragma once

#include "Resource.h"
#include <string>

namespace LSY
{
class CObjectMan;
class CMessage;
class CObjectBase;
class UPG_LSYLIB_EXPORT CLSYLibDialogBase : public CDialog
{
public:
	CObjectMan		*m_pObjectMan;
protected:
	std::string		m_strFileName;
	std::string		m_strProjectName;
	int				m_nID;
	CSize			m_szSize;
	bool			m_bModal;
	bool			m_bEventDraw;
	CWnd			*m_pReceiveWnd;

	DECLARE_DYNAMIC(CLSYLibDialogBase)
public:
	CLSYLibDialogBase(CWnd* pParent = NULL);
	virtual ~CLSYLibDialogBase();

	enum { IDD = IDD_LSYLIB_DIALOG_BASE };
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_DISPATCH_MAP()
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	virtual INT_PTR DoModal(CObjectMan *pMainObjectMan, int nID, bool bEventDraw = true, CWnd* pParent = NULL);
	virtual BOOL Create(CObjectMan *pMainObjectMan, int nID, CWnd *pParent = NULL, bool bEventDraw = true);

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	LRESULT OnCallCreateObject(CMessage *msg);

	const bool &IsModal(void){return m_bModal;}

	CObjectBase *GetObject(const int &nID);
	CObjectBase *GetObject(const std::string &strName);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	bool SetRegion(void);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//����� Object�� ����� �� ��ӹ޾Ƽ� �������ָ� �ȴ�.
	virtual CObjectBase *OnCallCreateObject(const int &nID, const std::string &strName);
	//��� Object�� ��� �ε��Ǿ��� �� ȣ��Ǿ����� �ݹ��Լ�
	//��� �޾Ƽ� �̺�Ʈ ó������ ������ָ� �ȴ�.
	virtual bool OnCreatedProject(void);

	void SetFPS(float fps);
	void SetCaptionHeight(int nCaptionHeight);
	afx_msg void OnDestroy();

	friend CLSYLibDialogBase;
};
}