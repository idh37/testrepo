#pragma once

#include "Resource.h"
#include <string>
namespace LSY
{
class CObjectMan;
class CMessage;
class CObjectBase;
class UPG_LSYLIB_EXPORT CLSYLibWindowsBase : public CWnd
{
protected:
	CObjectMan		*m_pObjectMan;

	std::string		m_strFileName;
	std::string		m_strProjectName;
	int				m_nID;
	CSize			m_szSize;
	bool			m_bEventDraw;
	bool			m_bFirstPaint;
	CWnd			*m_pReceiveWnd;

	DECLARE_DYNAMIC(CLSYLibWindowsBase)
public:
	CLSYLibWindowsBase();
	virtual ~CLSYLibWindowsBase();

protected:
	DECLARE_DISPATCH_MAP()
	DECLARE_MESSAGE_MAP()
public:
	//Create ���� ȣ���� �־�� �ϴ� �Լ�
	void Init(CObjectMan *pMainObjectMan, int nID, bool bEventDraw = true);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	LRESULT OnCallCreateObject(CMessage *msg);

	CObjectBase *GetObject(const int &nID);
	CObjectBase *GetObject(const std::string &strName);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	bool SetRegion(void);
public:
	//����� Object�� ����� �� ��ӹ޾Ƽ� �������ָ� �ȴ�.
	virtual CObjectBase *OnCallCreateObject(const int &nID, const std::string &strName);
	//��� Object�� ��� �ε��Ǿ��� �� ȣ��Ǿ����� �ݹ��Լ�
	//��� �޾Ƽ� �̺�Ʈ ó������ ������ָ� �ȴ�.
	virtual bool OnCreatedProject(void);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void SetFPS(float fps);
	afx_msg void OnDestroy();

	virtual void OnCancel();
};
}