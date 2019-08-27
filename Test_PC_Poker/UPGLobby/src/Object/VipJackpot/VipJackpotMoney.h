#pragma once

class UPG_LOBBY_EXPORT CVipJackpotMoney : public LSY::CImageNumber
{
public:
	CVipJackpotMoney();
	virtual ~CVipJackpotMoney();
	
	virtual void Logic(const DWORD &dwTime);
	virtual void OnCreated(void);
};

