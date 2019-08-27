#pragma 

class CVIPJackpotInfoDlg;
class CVIPJackpotRankDlg;
class UPG_LOBBY_EXPORT CVIPJackpotGroupDisplay : public LSY::CGroup
{
protected:
	LSY::CTabControl		*m_pTabMenu;

	CVIPJackpotInfoDlg		*m_pInfoDlg;
	CVIPJackpotRankDlg		*m_pRankDlg;

public:
	CVIPJackpotGroupDisplay();
	virtual ~CVIPJackpotGroupDisplay();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);

	LRESULT OnClickButton(LSY::CMessage *msg);
	LRESULT OnRoomTypeSelChange(LSY::CMessage *msg);

	void ShowDetailInfo();
};