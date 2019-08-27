// NMEdit.cpp : implementation file
//

#include "stdafx.h"
#include "./NMWidget/NMEdit.h"

//////////////////////////////////////////////////////////////////////////////
// CNMCommonEdit

namespace NMBASE
{
	namespace SKINGDI
	{

		IMPLEMENT_DYNAMIC(CNMCommonEdit, CEdit)

			CNMCommonEdit::CNMCommonEdit()
		{
			m_pParent = NULL;
			for( int i=0; i<HISTORY_COUNT; i++ )
				m_strHistory[i] = "";
			m_nNowHistory = 0;
		}

		CNMCommonEdit::~CNMCommonEdit()
		{
		}


		BEGIN_MESSAGE_MAP(CNMCommonEdit, CEdit)
			//{{AFX_MSG_MAP(CNMCommonEdit)
			ON_WM_KEYDOWN()
			ON_WM_CHAR()
			ON_WM_MOUSEWHEEL()
			ON_WM_SYSKEYDOWN()
			//}}AFX_MSG_MAP
		END_MESSAGE_MAP()


		BOOL CNMCommonEdit::Initialize( INMWidget* pParent )
		{
			m_pParent = pParent;
			SetLimitText(MAX_TEXT_LEN);

			return TRUE;
		}

		// CNMCommonEdit message handlers

		void CNMCommonEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
		{
			// TODO: Add your message handler code here and/or call default
			switch(nChar)
			{
			case VK_RETURN:
				{
					m_nNowHistory = 0;

					char szBuf[MAX_TEXT_LEN]={0,};
					GetWindowText( szBuf, MAX_TEXT_LEN-1 );


					// 			if( strlen(szBuf) == 0 )
					// 				break;

					// 			if( CheckEasterEgg( szBuf ) )
					// 				break;

					if( CheckPapering( szBuf ) )
						break;

					//- lorddan ( 2008.10.07 ) - Bad Filter
					char szTChat[MAX_TEXT_LEN] = {0,};
					NMBASE::UTIL::NMIsBadToRep( szBuf, szTChat, MAX_TEXT_LEN-1 );
					//- lorddan ( 2008.10.07 ) - Bad Filter

					if( m_pParent )
						m_pParent->OnCommandWidget( WIDGET_ID_EDIT_ENTER, 0, (LPARAM)szTChat );

					GraspData();
				} break;

			case VK_UP:
				{
					// 			if( m_nNowHistory < HISTORY_COUNT-1 )
					// 			{
					// 				m_nNowHistory++;
					// 				SetWindowText( m_strHistory[m_nNowHistory] );
					// 				SendMessage(WM_KEYDOWN, VK_END, NULL);
					// 			}
				} break;

			case VK_DOWN:
				{
					// 			if( m_nNowHistory > 0 )
					// 			{
					// 				m_nNowHistory--;
					// 				SetWindowText( m_strHistory[m_nNowHistory] );
					// 				SendMessage(WM_KEYDOWN, VK_END, NULL);
					// 			}
				} break;
			case VK_LEFT:
			case VK_RIGHT:
				{

				} return;
			case VK_CONTROL:
			case VK_HOME:
			case VK_END:
				{
					return;
				}
			}

			CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		}

		LRESULT CNMCommonEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
		{
			// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
			if (WM_PASTE==message)
			{
				return TRUE;
			}
			return CEdit::WindowProc(message, wParam, lParam);
		}



		void CNMCommonEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
		{
			// TODO: Add your message handler code here and/or call default
			// 엔터가 들어올때 생기는 띵~ 소리를 제거함
			if( nChar == 13 )
			{
				// 개행문자 대신 홈키 누르는것으로 대체
				SendMessage(WM_KEYDOWN, VK_END, NULL);
				return;
			}

			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}

		BOOL CNMCommonEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
		{
			// TODO: Add your message handler code here and/or call default
			return CEdit::OnMouseWheel(nFlags, zDelta, pt);
		}

		void CNMCommonEdit::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
		{
			// TODO: Add your message handler code here and/or call default
			CEdit::OnSysKeyDown(nChar, nRepCnt, nFlags);
		}

		void CNMCommonEdit::GraspData()
		{
			char szBuf[MAX_TEXT_LEN]={0,};
			GetWindowText( szBuf, MAX_TEXT_LEN-1 );

			for(int i=HISTORY_COUNT-1; i>1; i--)
			{
				m_strHistory[i] = m_strHistory[i-1];
			}
			m_strHistory[1].Format( "%s", szBuf );
			m_strHistory[0].Format( "" );

			SetWindowText( "" );
		}


		BOOL CNMCommonEdit::CheckEasterEgg( char* szBuf )
		{
			if( szBuf == NULL )	return FALSE;
			/*
			CString strCmd( szBuf );
			if( strCmd.Left(3).Find( "./" ) < 0 )
			return FALSE;

			////////////////////////////////////////////////////////
			// add cheat code 
			if( strCmd.Find( "./RELOAD" ) >= 0 )
			{
			AfxMessageBox( "준비중입니다." );
			}

			if( strCmd.Find( "./RS" ) >= 0 )
			{
			if( !g_bViewDebug )	return TRUE;

			int nAdd = atoi( szBuf+5 );
			if( strCmd.GetLength() > 0 )
			{
			NPZONE_DATA tNPZone_Data;
			memcpy( &tNPZone_Data, CNPZoneClient::GetInstance().GetNPZoneData(), sizeof(NPZONE_DATA) );

			tNPZone_Data.m_iQuestCurrent = 0;
			tNPZone_Data.m_iQuestGoal = nAdd;
			CNPZoneClient::GetInstance().SetNPZoneData( &tNPZone_Data );
			}
			}

			if( strCmd.Find( "./SG" ) >= 0 )
			{
			if( !g_bViewDebug )	return TRUE;

			int nAdd = atoi( szBuf+5 );
			if( strCmd.GetLength() > 0 )
			{
			NPZONE_DATA tNPZone_Data;
			memcpy( &tNPZone_Data, CNPZoneClient::GetInstance().GetNPZoneData(), sizeof(NPZONE_DATA) );

			tNPZone_Data.m_iQuestCurrent += nAdd;
			CNPZoneClient::GetInstance().SetNPZoneData( &tNPZone_Data );
			}
			}

			if( strCmd.Find( "./QS" ) >= 0 )
			{
			if( !g_bViewDebug )	return TRUE;

			int nAdd = atoi( szBuf+5 );
			if( strCmd.GetLength() > 0 )
			{
			NPZONE_DATA tNPZone_Data;
			memcpy( &tNPZone_Data, CNPZoneClient::GetInstance().GetNPZoneData(), sizeof(NPZONE_DATA) );

			tNPZone_Data.m_iSupplayNP += nAdd;
			tNPZone_Data.m_eQuestStatus = NPQUEST_COMPLETE;
			tNPZone_Data.m_iSuccessCount += 1;
			CNPZoneClient::GetInstance().SetNPZoneData( &tNPZone_Data );
			}
			}

			if( strCmd.Find( "./ANI" ) >= 0 )
			{
			if( !g_bViewDebug )	return TRUE;

			char* szKey = szBuf+6;
			if( strCmd.GetLength() > 0 )
			NMANIQUE.TestAnimation( szKey );
			}

			if( strCmd.Find( "./AUD" ) >= 0 )
			{
			if( !g_bViewDebug )	return TRUE;

			char* szKey = szBuf+6;
			if( strCmd.GetLength() > 0 )
			NMRESAM.PlayWav( szKey );
			}

			// 	if( strCmd.Find( "./ADDGURACHAN" ) >= 0 )
			// 	{
			// 		for( int i=0; i<10; i++ )
			// 		{
			// 			CHANNELINFO tTestInfo;
			// 			tTestInfo.GroupNo = g_nChanGroupIndex[i];
			// 			tTestInfo.GameKind = 0;		
			// 			tTestInfo.MaxMoney = 999999999999;
			// 			tTestInfo.MinMoney = 0;
			// 			tTestInfo.PrevMinMoney = 999999999999;
			// 			tTestInfo.WorkState = 0;
			// 
			// 			if( i == 0 || i == 1 )
			// 			{
			// 				tTestInfo.MaxMoney = 100000000;
			// 				tTestInfo.MinMoney = 0;
			// 			}
			// 			if( i == 8 || i == 7 )
			// 			{
			// 				tTestInfo.MinMoney = 1000000000;
			// 				tTestInfo.PrevMinMoney = 100000000;
			// 			}
			// 
			// 			for( int c=0; c<20; c++ )
			// 			{
			// 				tTestInfo.ChanNo = c+1;
			// 				tTestInfo.NowUser = 200-c;
			// 				tTestInfo.MaxUser = 200;
			// 
			// 				if( c == 18 )
			// 					tTestInfo.WorkState = 1;
			// 				else if( c == 19 )
			// 					tTestInfo.WorkState = 2;
			// 				g_pLobbyDlg->GetChanList()->InsertObjectList( &tTestInfo );
			// 			}
			// 		}
			// 
			// 		g_pLobbyDlg->GetChanList()->RenewalObject();
			// 	}

			if( strCmd.Find( "./ADDGURAROOM" ) >= 0 )
			{
			for( int i=1; i<100; i++ )
			{
			ROOMINFO tTestInfo;
			tTestInfo.Super = rand()%2;
			tTestInfo.bSpeed = 1;
			tTestInfo.bDaebak = 0;
			tTestInfo.GwangGame = 0;

			tTestInfo.RoomNum = i;
			tTestInfo.UNum = Play[0].UI.UNum;
			wsprintf( tTestInfo.ID[ID_INDEX], Play[0].UI.ID[ID_INDEX] );
			wsprintf( tTestInfo.ID[NICK_INDEX], Play[0].UI.ID[NICK_INDEX] );
			tTestInfo.NowUserNum = 1 + i%5;
			tTestInfo.MaxUserNum = 5;
			tTestInfo.State = 0;
			ZeroMemory( tTestInfo.Pass, sizeof(tTestInfo.Pass) );
			tTestInfo.BetMoney = 1000000;
			tTestInfo.UserMoney[0] = 498765432 + rand()%999999999;
			tTestInfo.UserMoney[1] = 498765432 + rand()%999999999;
			tTestInfo.UserMoney[2] = 999999999999;
			tTestInfo.UserMoney[3] = 999999999999;
			tTestInfo.Sex[0] = rand()%2;
			tTestInfo.Sex[1] = rand()%2;
			tTestInfo.Sex[2] = rand()%2;
			tTestInfo.Sex[3] = rand()%2;

			wsprintf( tTestInfo.Title, "가나다라마바사아자차카타파하거" );
			tTestInfo.Secret = rand()%2;
			if( i<67 )
			tTestInfo.MinMoney = g_llTestMoney[i];
			else 
			tTestInfo.MinMoney = 0;

			g_pLobbyDlg->GetRoomList()->InsertObjectList( &tTestInfo );
			}
			}

			if( strCmd.Find( "./ADDGURAUSER" ) >= 0 )
			{
			for( int i=1; i<100; i++ )
			{
			SMALLUSERINFO tTestInfo;
			tTestInfo.UNum = i;
			wsprintf( tTestInfo.ID[ID_INDEX], "NETTEST%003d", i );
			wsprintf( tTestInfo.ID[NICK_INDEX], "기소불욕물시어인%003d", i );
			tTestInfo.Sex = rand()%2;
			tTestInfo.WinNum = rand()%100000;
			tTestInfo.LooseNum = rand()%100000;
			tTestInfo.Point = 0;
			tTestInfo.GoStopMoney = 10000000 + 1000000*i;
			tTestInfo.nAIN_InsuranceKind = 0;
			tTestInfo.nAIN_Insurance_Money = 0;
			tTestInfo.level = 0;
			tTestInfo.Position = WH_LOBBY;
			tTestInfo.RoomNum = 0;
			g_pLobbyDlg->GetUserList()->InsertObjectList( &tTestInfo );
			}
			}

			if( strCmd.Find( "./DEBUG" ) >= 0 )
			{
			g_bViewDebug = !g_bViewDebug;
			}

			if( strCmd.Find( "./WEB" ) >= 0 )
			{
			g_bViewWebURL = !g_bViewWebURL;
			}
			////////////////////////////////////////////////////////
			*/
			GraspData();

			return TRUE;
		}

		BOOL CNMCommonEdit::CheckPapering( char* szBuf )
		{
#ifdef _DEBUG
			//return FALSE;
#endif

			if( szBuf == NULL )	return FALSE;

			if( m_PaperingChat.CheckNoChat( szBuf ) == TRUE )
			{
				char szWarning[MAX_TEXT_LEN] = {0,};
				wsprintf( szWarning, "[알림] 대화 도배로 인하여 2분간 채팅이 금지된 상태입니다.\n" );

				if( m_pParent )
					m_pParent->OnCommandWidget( WIDGET_ID_EDIT_WARNING, 0, (LPARAM)szWarning );

				GraspData();

				return TRUE;
			}

			return FALSE;
		}

		//////////////////////////////////////////////////////////////////////////////
		// CNMEdit
		CNMEdit::CNMEdit()
		{
			m_nControlID = WIDGET_ID_NULL;
			m_pParent = NULL;
			m_rectPosition.SetRect(0,0,0,0);
			m_crText = RGB(255,255,255);
			m_nLimitTextCnt=-1;
			m_strCurrentText = "";

			m_bNumber = FALSE;
		}
		CNMEdit::~CNMEdit()
		{

		}
		BOOL CNMEdit::Initialize( int nControlID, CWnd* pParent, CRect rectEdit )
		{
			m_nControlID = nControlID;
			m_pParent = pParent;
			m_rectPosition = rectEdit;

			if( m_CommonEdit.m_hWnd == NULL )
			{		
				// window edit 생성
				if( m_CommonEdit.Create( ES_WANTRETURN | ES_MULTILINE | ES_AUTOHSCROLL,
					m_rectPosition,
					m_pParent,
					nControlID ) == FALSE )
					return FALSE;
				m_CommonEdit.Initialize( this );

				CRect rectTemp;
				// window edit를 않보이는 곳으로 옮겨 놓는다.
				m_CommonEdit.GetClientRect( rectTemp );
				rectTemp.OffsetRect( -1000, 0 );
				m_CommonEdit.MoveWindow( rectTemp );
				m_CommonEdit.ShowWindow( SW_SHOW );

				if (m_nLimitTextCnt>=0)
				{
					SetLimitText(m_nLimitTextCnt);
				}
			}

			return TRUE;
		}

		void CNMEdit::SetPosition( int nPosX, int nPosY )
		{
			int nWidth = m_rectPosition.Width();
			int nHeight = m_rectPosition.Height();
			m_rectPosition.SetRect( nPosX, nPosY, nPosX + nWidth, nPosY + nHeight );
		}

		void CNMEdit::SetLimitText(int nLimitTextCnt)
		{
			m_nLimitTextCnt = nLimitTextCnt;

			if( m_CommonEdit.m_hWnd )
				m_CommonEdit.SendMessage(EM_LIMITTEXT, m_nLimitTextCnt, 0);
		}

		void CNMEdit::SetStyleNumber(BOOL bNumber)
		{
			m_bNumber = bNumber;

			if( m_CommonEdit.m_hWnd )
			{
				if (m_bNumber)
					m_CommonEdit.ModifyStyle( NULL, ES_NUMBER, NULL);
				else
					m_CommonEdit.ModifyStyle( ES_NUMBER, NULL, NULL);

			}

		}

		void CNMEdit::SetTextFont( CFont* pFont )
		{
			m_CommonEdit.SetFont( pFont );
		}
		void CNMEdit::SetTextColor( COLORREF crText )
		{
			m_crText = crText;
		}

		void CNMEdit::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{
			char szBuf[MAX_TEXT_LEN]={0,};
			m_CommonEdit.GetWindowText( szBuf, MAX_TEXT_LEN-1 );

			CString strText;
			strText.Format( "%s", szBuf );

			BOOL bShowCaret = (GetTickCount()%1000)/500;
			BOOL bDrawCaret = TRUE;
			if( strText.GetLength() > 0 && strText.GetAt(0) != -1 )
			{
				CRect rectTmp = m_rectPosition;

				DC.IntersectClipRect(rectTmp);

				CPoint ptCursor = m_CommonEdit.PosFromChar(0);
				rectTmp.left += ptCursor.x;
				DC.SetTextColor( m_crText );
				DC.SelectObject( m_CommonEdit.GetFont() );
				DC.DrawText( strText, &rectTmp, DT_LEFT );

				DC.SelectClipRgn(NULL);

				if( bShowCaret )
				{
					if( m_CommonEdit.GetFocus() != NULL && ::GetFocus() == m_CommonEdit )
					{
						SIZE sz = DC.GetTextExtent( strText );
						DC.TextOut( rectTmp.left+sz.cx, rectTmp.top, "|" );
						bDrawCaret = FALSE;

						//CPoint ptCaret = m_CommonEdit.GetCaretPos();
						//DC.TextOut( m_rectPosition.left+ptCaret.x, m_rectPosition.top, "|" );
					}
				}
			}

			if( bShowCaret && bDrawCaret )
			{
				if( m_CommonEdit.GetFocus() != NULL && ::GetFocus() == m_CommonEdit )
				{
					DC.SetTextColor( m_crText );
					DC.SelectObject( m_CommonEdit.GetFont() );
					DC.TextOut( m_rectPosition.left, m_rectPosition.top, "|" );

					//CPoint ptCaret = m_CommonEdit.GetCaretPos();
					//DC.TextOut( m_rectPosition.left+ptCaret.x, m_rectPosition.top, "|" );
				}
			}

#ifdef _DEBUG
			//	Page.DrawEmptyBox( m_rectPosition.left, m_rectPosition.top, m_rectPosition.right, m_rectPosition.bottom, RGB555mix(0,0,31) );
#endif
		}

		BOOL CNMEdit::OnLButtonDownWidget( UINT nFlags, CPoint point )
		{
			if( m_rectPosition.PtInRect( point ) )
			{
				SetFocus();
				return TRUE;
			}
			return FALSE;
		}

		BOOL CNMEdit::OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam )
		{
			switch( nCommand )
			{
			case WIDGET_ID_EDIT_ENTER:	// 추후 GraspData()와 합성하자..
				{
					m_strCurrentText = (char*)(lParam);
					NMSendMessage( m_pParent, m_nControlID, WIDGET_ID_EDIT_NORMAL, (LPARAM)(&m_strCurrentText) );
				} return TRUE;
			case WIDGET_ID_EDIT_WARNING:
				{
					m_strCurrentText = (char*)(lParam);
					NMSendMessage( m_pParent, m_nControlID, WIDGET_ID_EDIT_WARNING, (LPARAM)(&m_strCurrentText) );
				} return TRUE;
			}

			return FALSE;
		}

		void CNMEdit::GraspData()
		{
			m_strCurrentText = "";
		}

		void CNMEdit::EnterMessage()
		{
			m_CommonEdit.SendMessage( WM_KEYDOWN, VK_RETURN, NULL );
		}

		BOOL CNMEdit::GetFocus()
		{
			if( m_CommonEdit.GetFocus()->GetSafeHwnd() == m_CommonEdit.GetSafeHwnd() )
				return TRUE;

			return FALSE;
		}

		void CNMEdit::SetFocus()
		{
			CString strText;
			m_CommonEdit.GetWindowText(strText);
			if (m_strDefalutText == strText)
			{
				m_CommonEdit.SetWindowText("");
			}

			m_CommonEdit.SetFocus();
		}

		BOOL CNMEdit::EnableWindow(BOOL bEnable) 
		{
			return m_CommonEdit.EnableWindow(bEnable);
		}


		void CNMEdit::SetWindowText(LPCTSTR lpszString)
		{
			m_CommonEdit.SetWindowText(lpszString);
		}


		BOOL CNMEdit::IsWindowEnabled()
		{
			return m_CommonEdit.IsWindowEnabled();
		}

		void CNMEdit::SetDefaultText(LPCTSTR lpszString)
		{	
			m_strDefalutText = lpszString;
			SetWindowText(lpszString);
		}

		void CNMEdit::ClearText()
		{
			CString strText;
			m_CommonEdit.GetWindowText(strText);
			if (m_strDefalutText == strText)
			{
				return;
			}

			m_CommonEdit.SetWindowText("");	

		}

	} //namespace SKINGDI

}//namespace NMBASE