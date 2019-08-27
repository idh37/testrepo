#pragma once

#include "PromotionBase.h"

class UPG_GAMEBASE_EXPORT CPromotionPokerTime : public CPromotionBase
{
public:
	enum ENUM_POKERTIME_STATUS
	{
		EPTS_END = 0,		//이벤트 종료
		EPTS_SEASON,		//이벤트 기간
		EPTS_POKERTIME,		//이벤트 진쟁중
	};
protected:
	class EFFECT_PLAY_INFO
	{
	public:
		DWORD		m_dwStartTime;
		DWORD		m_dwEndTime;
		CString		m_strJokbo;
		CString		m_strMoney;
	public:
		EFFECT_PLAY_INFO():m_dwStartTime(0),m_dwEndTime(0){}
		virtual ~EFFECT_PLAY_INFO(){}
	};

	NMBASE::GRAPHICGDI::xSprite						m_sprEffect; // 포커 타임 용
	NMBASE::GRAPHICGDI::xSprite						m_sprTab;

	NMBASE::GRAPHICGDI::xSprite						m_sprJulyEffect; // 7월 올림픽 프로모션 용
	NMBASE::GRAPHICGDI::xSprite						m_sprJulyTab;

	ENUM_POKERTIME_STATUS		m_nStatus;
	map<int, EFFECT_PLAY_INFO>	m_mapEffectPlayInfo;

	DWORD	m_dwPokerTimeEffectStartTime;

public:
	CPromotionPokerTime(CPromotionManager *pManager, int nType);
	virtual ~CPromotionPokerTime();

	void SetEventState(const ENUM_POKERTIME_STATUS &nStatus){m_nStatus = nStatus;}
	const ENUM_POKERTIME_STATUS &GetEvnetState(){return m_nStatus;}

	CString GetRuleName(int nJokbo, int nValue);
	void ShowAniEffect(int p, CString strJokbo, CString strMoney);

	//virtual const bool IsShowTab(void){	return false;}

	virtual BOOL OnLButtonDownBottom(int &x , int &y);
	virtual BOOL OnLButtonUpBottom(int &x , int &y);
	virtual BOOL OnMouseMoveBottom(int &x , int &y);

	virtual BOOL OnLButtonDownMiddle(int &x , int &y);
	virtual BOOL OnLButtonUpMiddle(int &x , int &y);
	virtual BOOL OnMouseMoveMiddle(int &x , int &y);

	virtual BOOL OnLButtonDownTop(int &x , int &y);
	virtual BOOL OnLButtonUpTop(int &x , int &y);
	virtual BOOL OnMouseMoveTop(int &x , int &y);

	virtual BOOL OnEventTabLButtonDown(int &x , int &y);
	virtual BOOL OnEventTabLButtonUp(int &x , int &y);
	virtual BOOL OnEventTabMouseMove(int &x , int &y);

	virtual BOOL OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData);
	virtual BOOL OnCommand(WPARAM &wParam, LPARAM &lParam);
	
	virtual void DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);

	virtual void DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart);
	virtual int GetPriorities(void);

	virtual void Accept_CreateRoom();
	virtual void Accept_EnterRoom();
};
