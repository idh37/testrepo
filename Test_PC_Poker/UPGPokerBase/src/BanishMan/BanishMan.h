#pragma once

class CSeatKeeper;
class CObserverMan;

class UPG_GAMEBASE_EXPORT CBanishManager
{	
protected:
	int				m_nBanismentcount;
	CSeatKeeper		*m_pSeatKeepr;
	CObserverMan	*m_pObserverMan;

public:
	CBanishManager();
	virtual ~CBanishManager();

	void Init(CSeatKeeper *pSeatKeeper, CObserverMan *pObserverMan);

	int		GetBanishmentCount();
	void	set_BanishmentCount(int cnt );
	void	Clear();
	int		GetMaxBnishCount();

	LONG OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData);
	void ReceiveSV_NOTIFY_BANISHCOUNT(int nTotSize, char *lpData);
	void ReceiveSV_ASK_BANISHVOTE(int nTotSize, char *lpData);

	LRESULT OnKickOut(WPARAM &wParam, LPARAM &lParam);
	BOOL IsImmuneToBanish(int nPnum_Local, char *ErrMsg);	//추방을 당하지 않는 유저 체크 
};