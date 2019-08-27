#pragma once

class CUIInfoData;
#define IDM_ROUNDINFO (WM_USER+6302)

class UPG_GAMEBASE_EXPORT CBackGround
{
protected:
	CUIInfoData	*m_pUIInfoData;
	NMBASE::GRAPHICGDI::xSprite		m_sprBack;

	DWORD		m_dwStartTime;
	bool		m_bVipRoom;
	bool		m_bResult;
	int			m_nPlayIndex;
	int			m_nDeckIndex;
	DWORD		m_dwBackPlayTotalTime;
public:
	CBackGround();
	virtual ~CBackGround();

	virtual void LoadImage(LPCTSTR lpBackPath);
	virtual void Init(CUIInfoData *pUIInfoData, CGameViewBase *pView);
	virtual void DrawBottom(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC);
	virtual void DrawMiddle(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC);
	virtual void DrawTop(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC);

	virtual void OnEnterRoom(void);
};