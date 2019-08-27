// PlayTimeWarning.h: interface for the CPlayTimeWarning class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYTIMEWARNING_H__571740BC_45A3_4C85_8DEB_D526EE585B02__INCLUDED_)
#define AFX_PLAYTIMEWARNING_H__571740BC_45A3_4C85_8DEB_D526EE585B02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlayTimeWarning  
{
public:
	CPlayTimeWarning();
	virtual ~CPlayTimeWarning();

private:
	int m_nFirstPrint;
	DWORD m_dwLoginTime;
	int m_nLastPrintTime;

	CString m_strNotice[2];


	void PrintPerHour( int nHour );

public:
	void Login();
	void EnterTheGame();
	void OnTimer();
};

#endif // !defined(AFX_PLAYTIMEWARNING_H__571740BC_45A3_4C85_8DEB_D526EE585B02__INCLUDED_)
