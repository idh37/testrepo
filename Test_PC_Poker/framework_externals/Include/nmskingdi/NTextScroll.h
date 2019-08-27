// NTextScroll.h: interface for the CNTextScroll class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NTEXTSCROLL_H__AC37E319_BD1F_422E_AA30_76E7E4B6DB80__INCLUDED_)
#define AFX_NTEXTSCROLL_H__AC37E319_BD1F_422E_AA30_76E7E4B6DB80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NText.h"
#include <deque>

#include "skingdidefine.h"
#include "MyBitmap.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		typedef struct 
		{
			CMyBitmap*	pBitmap;
			int width;
			int height;
			float offsetX;
			float offsetY;	
		}SCROLL_INFO ;

		class NMSKINGDI_CLASS CNTextScroll : public CNText  
		{
		public:
			CNTextScroll();
			CNTextScroll(CRect rt, char* szText, int nID=-1, NMBASE::GRAPHICGDI::CPage* pPage=NULL,CDC *pDC=NULL);
			//	CNTextScroll(int x, int y, char* szText, int nID=-1, CNObject* pParent=NULL, NMBASE::GRAPHICGDI::CPage* pPage=NULL, NMBASE::GRAPHICGDI::xSprite* pSpr=NULL, int nSprNum=0);
			virtual ~CNTextScroll();

		public:
			BOOL		m_bScroll;				// 스크롤 하나 안하나(이곤 무한 반복기능으로 쓰이는거 같음)
			float		m_nScrollSpeed;			// 스크롤 속도
			BOOL		m_bScrollWidth;			// 스크롤을 가로로 하나 FALSE 이면 세로
			int			m_nScrollNum;
			int			m_nCurScrollNum;
			BOOL		m_bRollingScrool;		// 스크롤 계속(TRUE) 할거냐 아님 주어진 수 만큼(FALSE) 스크롤 될끄냐
			BOOL		m_bContinueScrool;		// 스크롤 중이냐 아님 다 스크롤 시키고 멈춰 있는 상태냐

			CDC			*m_pViewDC;
			DWORD		nReFlashTime;			// 다시 스크롤될 주기 
			DWORD		nReFlashStartTime;		// 다시 스크롤될 시작 시간값

			// 스크롤될 스트링 비트맵
			std::deque<SCROLL_INFO>	m_lstScrollInfo;

		public:

			void Init(CRect rt);
			void StartScroll(int nNum , BOOL bRollingScrool = TRUE/*TRUE 무한 반복*/);
			void ResumeScroll();
			void StopScroll();
			void ADDSetText(char* szText);
			void Clear();	
			BOOL IsContinueScroll()
			{
				return m_bContinueScrool;
			}

			void set_TextInfo( CFont *pFont , COLORREF color);

		public:
			virtual void SetText(char* szText);	
			virtual BOOL DrawObject(CDC* pDC);
			virtual BOOL ProcessObject(DWORD time);


		};

	} //namespace SKINGDI

}//namespace NMBASE

#endif // !defined(AFX_NTEXTSCROLL_H__AC37E319_BD1F_422E_AA30_76E7E4B6DB80__INCLUDED_)
