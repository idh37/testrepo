#pragma once

struct TargetData
{
	std::string id;
	std::string money;
};

class UPG_LOBBY_EXPORT CTargetNotiGroup : public LSY::CGroup
{
protected:
	LSY::CButton			*m_pBtnCong;
	LSY::CButton			*m_pBtnAddFriend;
	LSY::CText				*m_pTextID;
	LSY::CText				*m_pTextMoney;
	queue<TargetData>		m_QueData;

	CRITICAL_SECTION		m_cLockQueue;

public:
	CTargetNotiGroup();
	virtual ~CTargetNotiGroup();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	void SetTextInfo(std::string id, int money, int time);
	LRESULT OnClickButton(LSY::CMessage *msg);
	LRESULT OnTimer(LSY::CMessage *msg) ;

protected:

};
