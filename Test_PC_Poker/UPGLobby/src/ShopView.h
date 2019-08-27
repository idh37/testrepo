#pragma once


class CShopView :
	public CSSOWebWidget
{

	CString m_strURL;

public:
	CShopView(void);
	~CShopView(void);


	virtual void EnterWidget();
	virtual void ExitWidget();
};
