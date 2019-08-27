#pragma once

class CBuddyGroup;
class CBuddyFriend;

class CConnectUserListItem : public LSY::CListItem
{
protected:
	//친구 그룹 관련 객체 변수들
	LSY::CCheckButton	*m_pCheckSelect;
	LSY::CText			*m_pTxtNickName;
	LSY::CTextNumber	*m_pTxtnbHaveMoney;
	LSY::CImage			*m_pImgLocation;
	LSY::CText			*m_pTxtLocationGame;
	LSY::CText			*m_pTxtnbLocationRoomNumber;
public:
	CConnectUserListItem();
	virtual ~CConnectUserListItem();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	LRESULT OnCheckChange(LSY::CMessage *msg);

	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);

public:
	friend class CAddUserSelectDlg;
};