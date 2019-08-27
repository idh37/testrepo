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
	//Create 전에 호출해 주어야 하는 함수
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
	//사용자 Object를 등록할 때 상속받아서 구현해주면 된다.
	virtual CObjectBase *OnCallCreateObject(const int &nID, const std::string &strName);
	//모든 Object가 모두 로딩되었을 때 호출되어지는 콜백함수
	//상속 받아서 이벤트 처리등을 등록해주면 된다.
	virtual bool OnCreatedProject(void);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void SetFPS(float fps);
	afx_msg void OnDestroy();

	virtual void OnCancel();
};
}