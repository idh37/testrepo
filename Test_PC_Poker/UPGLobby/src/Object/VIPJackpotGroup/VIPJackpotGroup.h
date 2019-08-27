#pragma 

class CJackpotMoney;
class LSY::CButton;
class CVIPJackpotGroupDisplay;

class UPG_LOBBY_EXPORT CVIPJackpotGroup : public LSY::CGroup
{
protected:
	CJackpotMoney			*m_pJackpotMoney;
	LSY::CButton			*m_pBtnJackpotDetail;
	CVIPJackpotGroupDisplay *m_pDisplay;

public:
	CVIPJackpotGroup();
	virtual ~CVIPJackpotGroup();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);

	LRESULT OnClickButton(LSY::CMessage *msg);

};