#pragma once
#include <vector>

enum
{
	State_Scroll,
	State_Wait,
	State_Stop
};

class CSlideText
{
protected:
	NMBASE::GRAPHICGDI::CPage		*m_pPage;
	CRect		m_rtPageRect;

	CDC			*m_pDC;

	DWORD		m_dwWaitTime;
	int			m_nScrollSpeed;

	std::vector<CString> m_vecStringList;

	CString		m_strDefaultText;

	int			m_nShowTextIndex;

	CString		m_strShowingText;

	DWORD		m_dwPlayStartTime;
	DWORD		m_dwWaitStartTime;
	DWORD		m_dwCurrentTime;
	bool		m_bIsScroll;

	int			m_nScrollGab;
	int			m_nState;

public:
	CSlideText();
	virtual ~CSlideText();

	bool Create(int nPosx, int nPosy, int nWidth, int nHeight);
	void Draw(NMBASE::GRAPHICGDI::CPage *pPage);

	void SetDefaultString(CString strText);
	void AddScrollText(CString strText);

	void StartScroll();
	void StopScroll();
	void Reset();
	void SetScrollSpeed(int nTime);
	void SetWaitTime(DWORD dwTime);
	void Logic();
};

