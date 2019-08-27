// PlayTimeWarning.h: interface for the CPlayTimeWarning class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYTIMEWARNING_H__571740BC_45A3_4C85_8DEB_D526EE585B02__INCLUDED_)
#define AFX_PLAYTIMEWARNING_H__571740BC_45A3_4C85_8DEB_D526EE585B02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class UPG_COMMON_EXPORT CPlayTimeWarning  
{
public:
	CPlayTimeWarning();
	virtual ~CPlayTimeWarning();

private:
	int m_nFirstPrint;
	DWORD m_dwLoginTime;
	int m_nLastPrintTime;
	int m_nLastPrintTimeMin;

	BOOL m_bDisplay30MinWarnig;

	int m_nTotalMin;

	CString m_strNotice[2];

	bool m_bShowTimeMessageAtEnterRoom;
	int m_nHourToShow;
	int m_nMinToShow;


	void PrintPerHour( int nHour, int nMin = 0 );

public:
	void Login();
	void EnterTheGame();
	void OnTimer();
	LRESULT  OnChangeWaringType(WPARAM &wParam, LPARAM &lParam);
	
};

#endif // !defined(AFX_PLAYTIMEWARNING_H__571740BC_45A3_4C85_8DEB_D526EE585B02__INCLUDED_)
