#pragma once

class UPG_LOBBY_EXPORT CShopTabControl : public LSY::CTabControl
{
protected:
	LSY::CWeb	*m_pWebShop;
	LSY::CWeb	*m_pWebEvent;
public:
	CShopTabControl();
	virtual ~CShopTabControl();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	virtual void SetEventURL(std::string strURL);

	void RefreshWeb();

	LRESULT OnSelTabChange(LSY::CMessage *msg);

	//virtual void Logic(const DWORD &dwTime);
};