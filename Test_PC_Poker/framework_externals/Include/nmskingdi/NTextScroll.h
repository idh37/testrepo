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
			BOOL		m_bScroll;				// ��ũ�� �ϳ� ���ϳ�(�̰� ���� �ݺ�������� ���̴°� ����)
			float		m_nScrollSpeed;			// ��ũ�� �ӵ�
			BOOL		m_bScrollWidth;			// ��ũ���� ���η� �ϳ� FALSE �̸� ����
			int			m_nScrollNum;
			int			m_nCurScrollNum;
			BOOL		m_bRollingScrool;		// ��ũ�� ���(TRUE) �Ұų� �ƴ� �־��� �� ��ŭ(FALSE) ��ũ�� �ɲ���
			BOOL		m_bContinueScrool;		// ��ũ�� ���̳� �ƴ� �� ��ũ�� ��Ű�� ���� �ִ� ���³�

			CDC			*m_pViewDC;
			DWORD		nReFlashTime;			// �ٽ� ��ũ�ѵ� �ֱ� 
			DWORD		nReFlashStartTime;		// �ٽ� ��ũ�ѵ� ���� �ð���

			// ��ũ�ѵ� ��Ʈ�� ��Ʈ��
			std::deque<SCROLL_INFO>	m_lstScrollInfo;

		public:

			void Init(CRect rt);
			void StartScroll(int nNum , BOOL bRollingScrool = TRUE/*TRUE ���� �ݺ�*/);
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
