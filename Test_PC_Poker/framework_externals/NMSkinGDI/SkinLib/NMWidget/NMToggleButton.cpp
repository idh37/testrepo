#include "StdAfx.h"
#include "./NMWidget/NMToggleButton.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		CNMToggleButton::CNMToggleButton(void)
		{

			m_nTotToggleState = 0;
			m_nCurToggleState = 0;
		}

		CNMToggleButton::~CNMToggleButton(void)
		{
			m_vec_btnsprinfo.clear();

		}



		BOOL CNMToggleButton::Initialize( int nControlID, CWnd* pParent, int nPosX, int nPosY)
		{
			m_nTotToggleState = 0;
			m_nCurToggleState = 0;	

			return CNMTextButton::Initialize(nControlID, pParent, nPosX, nPosY, 0,0);
		}



		BOOL CNMToggleButton::AddToggleStateSprBtn(char* szSprKey,		//토글 상태를 추가 (초기화관련)
			int nSprID,
			int nSprCount , const char *pText)
		{
			//스프라이트가 있는 경우

			BUTTONSPRITEINFO bi;
			bi.strSprKey = szSprKey;
			bi.nSprID = nSprID;
			bi.nSprCount = nSprCount;
			bi.strText = pText;
			bi.nWidth = 0;
			bi.nHeight = 0;

			m_vec_btnsprinfo.push_back(bi);

			m_nTotToggleState++;

			if (m_nTotToggleState==1)
			{
				SetToggleState(0);
			}

			return TRUE;
		}

		BOOL CNMToggleButton::AddToggleStateTextBtn(const char *pText, int nWidth, int nHeight)
		{
			//스프라이트가 없는 경우

			BUTTONSPRITEINFO bi;
			bi.strSprKey = "";
			bi.nSprID = -1;
			bi.nSprCount = 0;
			bi.strText = pText;
			bi.nWidth = nWidth;
			bi.nHeight = nHeight;

			m_vec_btnsprinfo.push_back(bi);

			m_nTotToggleState++;

			if (m_nTotToggleState==1)
			{
				SetToggleState(0);
			}

			return TRUE;
		}



		BOOL CNMToggleButton::SetToggleState(int nState)	//토글 상태를 바꾼다.
		{
			if (nState >= (int) m_vec_btnsprinfo.size() || nState <0) 
				return FALSE;

			m_nCurToggleState = nState;

			m_strText = m_vec_btnsprinfo[nState].strText.c_str();

			//스프라이트가 있는 경우
			if (m_vec_btnsprinfo[nState].strSprKey.size()>0)
			{
				return SetSprite(	m_vec_btnsprinfo[nState].strSprKey.c_str(),
					m_vec_btnsprinfo[nState].nSprID,
					m_vec_btnsprinfo[nState].nSprCount);
			}
			else
			{	
				//스프라이트가 없는경우
				m_nSprCount = 4;
				m_nWidth = m_vec_btnsprinfo[nState].nWidth;
				m_nHeight = m_vec_btnsprinfo[nState].nHeight;
			}

			SetSendParam(GetwParam(), m_nCurToggleState);

			return TRUE;
		}


		BOOL CNMToggleButton::OnLButtonUpWidget( UINT nFlags, CPoint point )
		{

			if (m_bShow==FALSE)
				return FALSE;

			if (IsInRect( point ))
			{
				if (m_bDown)
				{
					m_nCurToggleState ++;
					if (m_nCurToggleState >=m_nTotToggleState)
						m_nCurToggleState = 0;

					SetToggleState(m_nCurToggleState);			
				}
			}


			return CNMTextButton::OnLButtonUpWidget(nFlags, point);


			// 	if (CNMButton::OnLButtonUpWidget(nFlags, point))
			// 	{
			// 		m_nCurToggleState ++;
			// 		if (m_nCurToggleState >=m_nTotToggleState)
			// 			m_nCurToggleState = 0;
			// 
			// 		SetToggleState(m_nCurToggleState);
			// 		return TRUE;
			// 	}
			// 
			// 	return FALSE;
		}


		void CNMToggleButton::SetText(int nToggleState, const char *pText)
		{
			if (nToggleState <0 || nToggleState>=m_nTotToggleState)
				return;


			ASSERT(m_nTotToggleState == m_vec_btnsprinfo.size());

			m_vec_btnsprinfo[nToggleState].strText = pText;

			if (m_nCurToggleState==nToggleState)
			{
				m_strText = pText;
			}



		}


	} //namespace SKINGDI

}//namespace NMBASE