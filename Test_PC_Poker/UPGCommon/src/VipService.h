// VipService.h: interface for the CVipService class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIPSERVICE_H__BAF495D4_C3E7_41E6_BF7D_FD9042F5933B__INCLUDED_)
#define AFX_VIPSERVICE_H__BAF495D4_C3E7_41E6_BF7D_FD9042F5933B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class UPG_COMMON_EXPORT CVipService  
{
public:
	CVipService();
	virtual ~CVipService();

public:
	CVipServiceDlg		m_ServiceDlg;

	BOOL				m_bLoaded;
	NMBASE::GRAPHICGDI::xSprite				m_spr;

public:
	static CVipService& GetInstance();

	void DoModalServiceDlg();

	void OnLogin();

	void OnEnterGame();	

	BOOL IsService();

};

UPG_COMMON_EXPORT CVipService& VipService();

#endif // !defined(AFX_VIPSERVICE_H__BAF495D4_C3E7_41E6_BF7D_FD9042F5933B__INCLUDED_)
