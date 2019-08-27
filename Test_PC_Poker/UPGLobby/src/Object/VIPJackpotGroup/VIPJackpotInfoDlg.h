#pragma 

class UPG_LOBBY_EXPORT CVIPJackpotInfoDlg : public LSY::CWindows
{
protected:
	std::map<int, LSY::CButton*>	m_mapEvent;


public:
	CVIPJackpotInfoDlg();
	virtual ~CVIPJackpotInfoDlg();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);

};