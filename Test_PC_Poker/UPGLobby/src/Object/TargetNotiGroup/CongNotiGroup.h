#pragma once

struct CongData
{
	std::string id;
	std::string message;
};

class UPG_LOBBY_EXPORT CCongNotiGroup : public LSY::CGroup
{
protected:
	LSY::CText				*m_pTextID;
	LSY::CText				*m_pTextMessage;
	queue<CongData>			m_QueData;

	CRITICAL_SECTION		m_cLockQueue;
public:
	CCongNotiGroup();
	virtual ~CCongNotiGroup();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	void SetTextInfo(std::string id, std::string msg, int time);
	LRESULT OnTimer(LSY::CMessage *msg) ;

protected:

};