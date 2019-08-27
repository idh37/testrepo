#pragma 

class UPG_LOBBY_EXPORT CVIPJackpotRankDlg : public LSY::CWindows
{
protected:
	LSY::CList				*m_pListWin;
	LSY::CList				*m_pListTop;

	bool					m_bLoadList;		//List °»½Å¿ë.
public:
	CVIPJackpotRankDlg();
	virtual ~CVIPJackpotRankDlg();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);

	virtual void SetShow(const bool &bShow);

	
	void AddListItemWin();
	void AddListItemTop();
	void AddListItem();

public:
	void SetLoadListFlag(bool bFlag_ )	{ m_bLoadList = bFlag_; }
};