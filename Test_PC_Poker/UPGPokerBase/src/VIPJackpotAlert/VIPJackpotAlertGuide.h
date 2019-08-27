#pragma once

class UPG_GAMEBASE_EXPORT CVIPJackpotAlertGuide : public LSY::CWindows
{
protected:
	std::map<int, LSY::CButton*>	m_mapEvent;
public:
	CVIPJackpotAlertGuide();
	virtual ~CVIPJackpotAlertGuide();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);

};