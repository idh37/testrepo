#pragma once

class CVipJackpotMoney;
class CVipJackpotDlg;
class UPG_LOBBY_EXPORT CVipJackpotGroup : public LSY::CGroup
{
protected:
	CVipJackpotMoney		*m_pJackpotMoney;
	LSY::CImage				*m_pJackpotRelease;	
	LSY::CButton			*m_pBtnJackpotDetail;
	CVipJackpotDlg			*m_pVipJackpotDlg;
public:
	CVipJackpotGroup();
	virtual ~CVipJackpotGroup();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);

	LRESULT OnClickButton(LSY::CMessage *msg);
	LRESULT OnReleaseMouseOver(LSY::CMessage *msg);
	LRESULT OnReleaseMouseLeave(LSY::CMessage *msg);
	LRESULT OnDetailDlgLocalChildNotify(LSY::CMessage *msg);

	LRESULT OnTimer(LSY::CMessage *msg) ;

protected:

};