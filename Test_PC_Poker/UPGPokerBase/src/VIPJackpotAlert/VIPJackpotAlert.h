#pragma once

class CVIPJackpotAlertGuide;

class UPG_GAMEBASE_EXPORT CVIPJackpotAlert : public LSY::CLSYLibWindowsBase
{
protected:
	DECLARE_DYNAMIC(CVIPJackpotAlert)
	DECLARE_DISPATCH_MAP()
	DECLARE_MESSAGE_MAP()

	CVIPJackpotAlertGuide* m_pGuide;

public:
	CVIPJackpotAlert();
	virtual ~CVIPJackpotAlert();

	bool Create(int x, int y);
	void SetShow(bool bShow);
	bool IsShow(void);

	virtual bool OnCreatedProject(void);

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};