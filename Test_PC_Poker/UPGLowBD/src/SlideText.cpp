#include "stdafx.h"
#include "SlideText.h"

#define SCROLL_SPEED 50

CSlideText::CSlideText()
: m_pPage(NULL),
m_pDC(NULL),
m_dwWaitTime(0),
m_nScrollSpeed(SCROLL_SPEED),
m_nShowTextIndex(0),
m_strShowingText(""),
m_dwPlayStartTime(0),
m_bIsScroll(false),
m_nScrollGab(0),
m_dwCurrentTime(0),
m_nState(State_Stop),
m_dwWaitStartTime(0),
m_strDefaultText("")
{

}

CSlideText::~CSlideText()
{
	if (m_pDC != NULL)
	{
		m_pDC->DeleteDC();
		delete m_pDC;
		m_pDC = NULL;
	}
}

bool CSlideText::Create(int nPosx, int nPosy, int nWidth, int nHeight)
{
	if(m_rtPageRect.Width() == nWidth && m_rtPageRect.Height() == nHeight) return false;

	if(m_pPage) delete m_pPage;
	m_pPage = NULL;

	if(m_pDC)
	{
		m_pDC->DeleteDC();
		delete m_pDC;
	}
	m_pDC = NULL;
	
	if(nWidth == 0 || nHeight == 0)
	{
		m_rtPageRect = CRect(0, 0, 0, 0);
	}
	else
	{
		m_pPage = new NMBASE::GRAPHICGDI::CPage;
		m_pPage->Init(nWidth + nWidth%2, nHeight, 16);
		m_rtPageRect = CRect(nPosx, nPosy, nPosx + nWidth, nPosy + nHeight);

		CDC TempDC;
		HDC dc = ::GetDC(NULL);
		TempDC.Attach(dc);

		m_pDC = new CDC;
		m_pDC->CreateCompatibleDC(&TempDC);
		m_pDC->SelectObject(m_pPage->hBmp);
		m_pDC->SetBkMode(TRANSPARENT);
		m_pDC->SelectObject(NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_BOLD));
		::ReleaseDC(NULL, dc);
	}

	return true;
}

void CSlideText::Draw(NMBASE::GRAPHICGDI::CPage *pPage)
{
	if (m_pDC == NULL)
		return;
	CDC *pDC = m_pDC;
	m_dwCurrentTime = timeGetTime();

	Logic();
	
	m_pDC->FillSolidRect(0, 0, m_rtPageRect.Width(), m_rtPageRect.Height(), RGB(0,0,255));
	pDC->SetTextColor(RGB(57, 99, 107));

	if (m_bIsScroll == false)
		::TextOut(pDC->m_hDC, 0, 0, m_vecStringList[m_nShowTextIndex], m_vecStringList[m_nShowTextIndex].GetLength());
	else
	{
		if (m_nState == State_Stop || m_nState == State_Wait)
		{
			CSize szSize;
			::GetTextExtentPoint( pDC->m_hDC, m_strDefaultText, m_strDefaultText.GetLength(), &szSize);

			int nTextOutPosX = 0;
			int nTextOutPosY = 0;

			if (m_rtPageRect.Width() > szSize.cx)
				nTextOutPosX = (m_rtPageRect.Width() - szSize.cx) / 2;
			if (m_rtPageRect.Height() > szSize.cy)
				nTextOutPosY = (m_rtPageRect.Height() - szSize.cy) / 2;

			::TextOut(pDC->m_hDC, nTextOutPosX, nTextOutPosY, m_strDefaultText, m_strDefaultText.GetLength());
		}
		else if (m_nState == State_Scroll)
		{
			int nGapX = 0;
			int nStringSize = 0;

			CString strShowingText = m_vecStringList[m_nShowTextIndex];

			CSize szSize;
			::GetTextExtentPoint( pDC->m_hDC, strShowingText, strShowingText.GetLength(), &szSize);

			if (m_dwCurrentTime >= m_dwPlayStartTime)
			{
				nGapX = (int)((float)(m_dwCurrentTime - m_dwPlayStartTime) * m_nScrollSpeed / 1000);
			}

			int nTextOutPosX = m_rtPageRect.Width() - nGapX;
			int nTextOutPosY = (m_rtPageRect.Height() - szSize.cy) / 2;

			::TextOut(pDC->m_hDC, nTextOutPosX, nTextOutPosY, strShowingText, strShowingText.GetLength());
		}
	}

	pPage->PutSprAuto(m_rtPageRect.left, m_rtPageRect.top, m_pPage);
}

void CSlideText::SetDefaultString(CString strText)
{
	m_strDefaultText = strText;
}

void CSlideText::AddScrollText(CString strText)
{
	m_vecStringList.push_back(strText);
}

void CSlideText::StartScroll()
{
	m_bIsScroll = true;
	m_dwPlayStartTime = timeGetTime();
	m_nState = State_Scroll;
}

void CSlideText::StopScroll()
{
	m_bIsScroll = false;
}

void CSlideText::Reset()
{
	m_dwWaitTime = 0;
	m_nScrollSpeed = SCROLL_SPEED;

	m_nShowTextIndex = 0;
}

void CSlideText::SetScrollSpeed(int nTime)
{
	m_nScrollSpeed = nTime;
}

void CSlideText::SetWaitTime(DWORD dwTime)
{
	m_dwWaitTime = dwTime;
}

void CSlideText::Logic()
{
	if (m_nState == State_Stop)
		return;
	else if (m_nState == State_Scroll)
	{
		/*
		if (m_dwCurrentTime - m_dwPlayStartTime > m_dwScrollTime)
		{
			m_nState = State_Wait;
			m_nShowTextIndex = (m_nShowTextIndex + 1) % m_vecStringList.size();
			m_dwWaitStartTime = m_dwCurrentTime;
		}
		*/

		if (m_pDC == NULL)
			return;

		CDC *pDC = m_pDC;
		CString strShowingText = m_vecStringList[m_nShowTextIndex];

		CSize szSize;
		::GetTextExtentPoint( pDC->m_hDC, strShowingText, strShowingText.GetLength(), &szSize);

		if ((float)(m_dwCurrentTime - m_dwPlayStartTime) * m_nScrollSpeed / 1000 > m_rtPageRect.Width() + szSize.cx)
		{
			m_nState = State_Wait;
			m_nShowTextIndex = (m_nShowTextIndex + 1) % m_vecStringList.size();
			m_dwWaitStartTime = m_dwCurrentTime;
		}
	}
	else if (m_nState == State_Wait)
	{
		if (m_dwCurrentTime - m_dwWaitStartTime > m_dwWaitTime)
		{
			m_nState = State_Scroll;
			m_dwPlayStartTime = m_dwCurrentTime;
		}
	}

}


