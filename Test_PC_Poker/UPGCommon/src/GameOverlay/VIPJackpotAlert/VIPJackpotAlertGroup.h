#pragma once

class CVIPJackpotAlertGuide;

class UPG_COMMON_EXPORT CVIPJackpotAlertGroup : public LSY::CGroup
{
protected:
	LSY::CButton*				m_pBtnGuide;
	CVIPJackpotAlertGuide*		m_pGuide;

public:
	CVIPJackpotAlertGroup();
	virtual ~CVIPJackpotAlertGroup();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);

	LRESULT OnClickButton(LSY::CMessage *msg);
};