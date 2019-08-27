// GraphButton.h: interface for the CGraphButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHBUTTON_H__F560D682_2133_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_GRAPHBUTTON_H__F560D682_2133_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		class NMSKINGDI_CLASS CGraphButton  
		{
		public:
			int		m_Xp, m_Yp;
			int		m_Width, m_Height;
			BOOL	m_bClick;
			BOOL	m_bOver;
			int		m_CmndID;
			BOOL    m_SceneObject;

			//	========= 마우스 오버됐을때 한번만 나오게 한다 =============
			//	사운드 번호가 지정되었다
			bool	m_bOverSound;
			CString m_strOverSoundID;
			CString m_strClickSoundID;

			//	===================== 툴팁 관련 ===========================================
			bool	m_bShowToolTip;
			CString m_strToolTip;
			CFont	*m_pFont;

			int		m_ButtonCnt;

		public:
			CGraphButton();
			virtual ~CGraphButton();

			void Init(CWnd *pWnd, NMBASE::GRAPHICGDI::CPage *ppage, int x, int y, NMBASE::GRAPHICGDI::xSprite *pspr, int iButtonNumber, int cmndid, int ButtonCnt = 4,bool scenebtn = false, bool bSound = false );
			void InitAdd(CWnd *pWnd, NMBASE::GRAPHICGDI::CPage *ppage, int x, int y, NMBASE::GRAPHICGDI::xSprite *pspr, int iAddNumber, int iButtonNumber, int cmndid, int ButtonCnt = 4,bool scenebtn = false, bool bSound = false );
			BOOL SetOverImage(NMBASE::GRAPHICGDI::xSprite *pspr, int nButtonNumber, int x = 0, int y = 0, int nButtonCnt = 4);	// 버튼 Over이미지
			void SetOverImageIndex( int nButtonNumer );
			void SetOverImageUseFlag(BOOL bFlag){m_bOverSpriteEnable = bFlag;}	// 버튼 Over이미지 사용여부 Flag
			void SetSprite(NMBASE::GRAPHICGDI::xSprite *pspr, int iButtonNumber);
			void SetSprite(NMBASE::GRAPHICGDI::xSprite *pspr, int iAddNumber, int iButtonNumber);
			void Enable(BOOL bVal);
			void Show(BOOL bVal);
			virtual void InitBtn(){};
			virtual BOOL OnMouseMove(int x, int y);
			virtual BOOL OnLButtonDown(int x, int y);
			virtual BOOL OnLButtonUp(int x, int y);
			virtual void Draw( CDC *pDC, int opt = 0, int grade = 0 );
			virtual int GetButtonState( void );
			virtual void SetButtonState( int state );
			CWnd * GetParent(){return m_pParent;}
			NMBASE::GRAPHICGDI::xSprite* GetSprite()
			{
				return m_pSpr;
			}

			int GetButtonNumber()
			{
				return m_iButtonNumber; 
			}

			BOOL PtInRect(int x, int y)
			{
				if(m_bShow == FALSE) return FALSE;				
				if(x >= m_Xp && x <= m_Xp+m_Width && y >= m_Yp && y <= m_Yp+m_Height)
				{
					return TRUE;
				}
				return FALSE;
			}
			
			inline  BOOL IsShow() const	  { return m_bShow; }
			inline  BOOL IsEnable() const { return m_bEnable; }

			//	========= 마우스 오버됐을때 한번만 나오게 한다 =============
			void	SetMouseOverSound( CString snd );
			void	MouseOverSoundNoUse( void );

			void	SetMouseClickSound( CString snd );
			void	MouseClickSoundNoUse( void );

			POINT	GetButtonPoint( void );
			//	============================================================

			//	===================== 윈도 이동 ===========================================
			void	MoveWindow( int x, int y )   { m_Xp = x; m_Yp = y; }
			void	MoveWindow( POINT point )    { MoveWindow( point.x, point.y ); }
			void	MoveWindowEx( int x, int y ) { m_Xp += x; m_Yp += y; }
			//	===========================================================================

			//	===================== 툴팁 관련 ===========================================
			void	SetToolTip( char *pTooltip = NULL, CFont *pFont = NULL );		//	툴팁넣기
			void	ShowToolTip( bool bShow = true );
			void	DrawToolTip( CDC *pDC, int Alpha = 20, COLORREF clrBoxFillBack = RGB( 0, 0, 0 ), COLORREF clrText = RGB( 255, 255, 255 ), bool bFixing = false );
			//	===========================================================================
			void SetButtonPos(int x, int y, NMBASE::GRAPHICGDI::xSprite *pspr);
			void SetButtonPos(int x, int y);

		private:
			
			void DrawScene(int nButSprPos, int nButOverSprPos);
			void DrawSprite(int nButSprPos, int nButOverSprPos, int opt, int grade);

		protected:
			CWnd*	m_pParent;			
			BOOL	m_bShow;
			BOOL	m_bEnable;
			int		m_iButtonNumber;
			int		m_iAddNumber;

			NMBASE::GRAPHICGDI::CPage*		m_pPage;
			NMBASE::GRAPHICGDI::xSprite*	m_pSpr;

			///< Button OverSprite
			NMBASE::GRAPHICGDI::xSprite*	m_pOverSpr;		// 버튼 Over스프라이트
			BOOL	m_bOverSpriteEnable;					// 버튼 Over스프라이트 보일지 여부
			int		m_nOverImgXp, m_nOverImgYp;				// 버튼 Over스프라이트 위치(기본버튼의 상대 좌표)
			int		m_nOverBtnNumber;						// 버튼 Over스프라이트 버튼 위치
			int		m_nOverBtnCount;						// 버튼 Over스프라이트 버튼 개수
		};


		/////////////////////////// 체크박스 버튼 클레스 
		class NMSKINGDI_CLASS CGraphCheckButton : public CGraphButton
		{	
		public:
			CGraphCheckButton();
			BOOL m_ClickState;
			virtual ~CGraphCheckButton();
			virtual BOOL OnLButtonDown(int x, int y);
			virtual BOOL OnLButtonUp(int x, int y);
			virtual BOOL OnMouseMove(int x, int y);
			virtual void Draw( CDC *pDC, int opt = 0, int grade = 0 );
			virtual int GetButtonState( void );
			virtual void SetButtonState( int state );
		};

		class NMSKINGDI_CLASS CGraphRadioButton : public CGraphButton
		{	
		public:
			CGraphRadioButton();
			virtual ~CGraphRadioButton();
			virtual BOOL OnLButtonDown(int x, int y);
			virtual BOOL OnLButtonUp(int x, int y);
			virtual BOOL OnMouseMove(int x, int y);
			virtual void Draw( CDC *pDC, int opt = 0, int grade = 0 );
			virtual void SetButtonState( int state );
			virtual int GetButtonState( void );
			virtual void InitBtn();
			void ClearRadioData();
			void LinkButton(CGraphButton* pBtn);

			int m_BtnNo;
			int m_RadioID; //라디오 버튼을 구별해주는 인덱스
			CGraphButton* m_pHeader;
			CGraphButton* m_pNext;
		};

		//////////////////////////// 슬라이더 버튼 클래스 //////////////////////////////

		class NMSKINGDI_CLASS CSliderButton  
		{
		public:
			CWnd *m_pParent;
			NMBASE::GRAPHICGDI::CPage *m_pPage;
			NMBASE::GRAPHICGDI::xSprite *m_pSpr;
			int CmndID;
			int MovCmndID;

			BOOL bShow;
			BOOL bEnable;
			BOOL bClick;
			BOOL bOver;
			int m_Xp, m_Yp, m_FirstXp, m_FirstYp;
			int m_Width, m_Height;

			int TotGrade;
			int NowGrade;
			int m_MStartPos;

			int	m_iButtonNum;
			int m_ButtonCnt;


			int		m_BodyLength;		//	바디 길이
			void	SetBodyLength( int BodyLength );
			int		m_NowPage;			//	현재 페이지
			int		m_TotPage;			//	총 페이지 갯수
			void	SetNowPage( int NowPage );
			void	SetTotPage( int TotPage );
			int		GetNowPageNum( void ) { return m_NowPage; }

			bool	m_bVertical;		//	세로냐?
			bool	IsStyleVertical( void ) { return m_bVertical; }

			//	===================== 윈도 이동 ===========================================
			void	MoveWindow( int x, int y ) { m_Xp = x; m_Yp = y; }
			void	MoveWindow( POINT point ) { MoveWindow( point.x, point.y ); }
			//	===========================================================================

			CSliderButton();
			virtual ~CSliderButton();
			void Init(CWnd *pWnd, NMBASE::GRAPHICGDI::CPage *pPage, int x, int y, NMBASE::GRAPHICGDI::xSprite *pSpr, int cmndid, int movcmndid, int iButtonNum, int ButtonCnt = 3, bool bVertical = false );
			void SetSprite(NMBASE::GRAPHICGDI::xSprite *pspr);
			void Enable(BOOL bVal);
			BOOL OnMouseMove(int x, int y);
			BOOL OnLButtonDown(int x, int y);
			BOOL OnLButtonUp(int x, int y);
			void Draw(CDC *pDC);
		};


		class NMSKINGDI_CLASS CAniGraphButton : public CGraphButton
		{

		public:

			CAniGraphButton();
			virtual ~CAniGraphButton();

			// Take me down to the paradise city where the grass is green and the girls are pretty~
			CFont	*m_pFont;
			BOOL m_bShowBtnText;
			CString m_BtnStr;
			int BtnXp, BtnYp;
			void Init(CWnd *pWnd, NMBASE::GRAPHICGDI::CPage *ppage, int x, int y, NMBASE::GRAPHICGDI::xSprite *pspr, int iButtonNumber, int cmndid, int ButtonCnt = 4, bool bSound = false);
			void Draw(CDC *pDC, int opt = 0, int grade = 0 );
			void DrawScene(CDC *pDC = NULL, int opt = 0, int grade = 0 );
			void SetBtnText(int xp, int yp, CString str, CFont *pFont = NULL);
			void ShowBtnText(BOOL bVal);
			void DrawBtnText(CDC *pDC, COLORREF TextColor);
		};

	} //namespace SKINGDI

}//namespace NMBASE

#endif // !defined(AFX_GRAPHBUTTON_H__F560D682_2133_11D4_97A5_0050BF0FBE67__INCLUDED_)
