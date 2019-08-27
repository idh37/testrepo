#pragma once
#include "nmtextbutton.h"
#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		class NMSKINGDI_CLASS CNMToggleButton :	public CNMTextButton
		{
			struct BUTTONSPRITEINFO
			{
				string  strSprKey;
				int		nSprID;
				int		nSprCount;
				string strText;
				int		nWidth, nHeight;
			};

			int m_nTotToggleState;
			int m_nCurToggleState;

			vector<BUTTONSPRITEINFO>	m_vec_btnsprinfo;


		public:
			CNMToggleButton(void);
			virtual ~CNMToggleButton(void);



			virtual BOOL Initialize( int nControlID, CWnd* pParent, int nPosX, int nPosY);


			virtual BOOL OnLButtonUpWidget( UINT nFlags, CPoint point );

			virtual BOOL AddToggleStateSprBtn(char* szSprKey,		//토글 상태를 추가 (초기화관련)
				int nSprID,
				int nSprCount,
				const char *pText="");

			virtual BOOL AddToggleStateTextBtn(const char *pText, int nWidth, int nHeight);




			virtual BOOL SetToggleState(int nState);	//토글 상태를 바꾼다.

			virtual int GetTotToggleState() { return m_nTotToggleState; }
			virtual int GetToggleState() { return m_nCurToggleState; }
			virtual void SetText(int nToggleState, const char *pText);


		};


	} //namespace SKINGDI

}//namespace NMBASE