#pragma once

class UPG_LOBBY_EXPORT CRoomInfoUserListItem : public LSY::CListItem
{
protected:
	LSY::CText			*m_pNickName;
	LSY::CTextNumber	*m_pHaveMoney;
public:
	CRoomInfoUserListItem();
	virtual ~CRoomInfoUserListItem();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	virtual LSY::CObjectBase *NewObject(void);
	void UpdateData(void);
};