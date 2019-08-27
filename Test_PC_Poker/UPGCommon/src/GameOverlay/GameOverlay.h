#pragma once

class CVIPJackpotAlertGroup;

class UPG_COMMON_EXPORT CGameOverlay
{
protected:

	CVIPJackpotAlertGroup			*m_pVIPJackpotAlertGroup;

public:
	CGameOverlay();
	~CGameOverlay();

	BOOL Init( CWnd* pWnd_ );

	LRESULT OnCallCreateObject(LSY::CMessage *msg);
protected:
	static LRESULT	_OnCallCreateObject_VIPJackpotAlert(LSY::CMessage *msg_ , CGameOverlay* pObj_ );


	
};