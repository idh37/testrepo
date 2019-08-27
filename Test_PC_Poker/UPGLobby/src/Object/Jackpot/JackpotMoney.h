#pragma once

class UPG_LOBBY_EXPORT CJackpotMoney : public LSY::CImageNumber
{
public:
	CJackpotMoney();
	virtual ~CJackpotMoney();
	
	virtual void Logic(const DWORD &dwTime);
	virtual void OnCreated(void);
};

class UPG_LOBBY_EXPORT CHalfJackpotMoney : public LSY::CImageNumber
{
public:
	CHalfJackpotMoney();
	virtual ~CHalfJackpotMoney();

	virtual void Logic(const DWORD &dwTime);
	virtual void OnCreated(void);
};