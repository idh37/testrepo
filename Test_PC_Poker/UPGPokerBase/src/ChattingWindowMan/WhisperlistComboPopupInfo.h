#pragma once

#define _DEF_GAME_WHISPERLIST_ALL_USER "일반대화"

class UPG_GAMEBASE_EXPORT CWhisperlistComboPopupInfo : public LSY::CLSYLibWindowsBase
{
protected:
	DECLARE_DYNAMIC(CWhisperlistComboPopupInfo)
	DECLARE_DISPATCH_MAP()
	DECLARE_MESSAGE_MAP()

public:
	CWhisperlistComboPopupInfo();
	virtual ~CWhisperlistComboPopupInfo();

	bool Create(int x, int y, CWnd *pParent);
	void SetShow(bool bShow);
	bool IsShow(void);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};