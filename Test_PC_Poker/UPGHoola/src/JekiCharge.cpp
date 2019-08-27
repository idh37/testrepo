// g_OneShotCharge.cpp: implementation of the COneShotCharge class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "OneShotCharge.h"
//#include "RecoveryChargeDlg.h"// [재기충전 작업] [2009-02-17]
#include "GlobalGame.h"
#include "FamilyIcon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define AddInfoText(x) g_pChatWinMan->m_pChatting->AddText(x, RGB(255,10,10))	

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COneShotCharge& COneShotCharge::GetInstance()
{
	static COneShotCharge jeki;
	return jeki;
}

COneShotCharge& g_OneShotCharge()
{
	return COneShotCharge::GetInstance();
}


COneShotCharge::COneShotCharge()
{

}

COneShotCharge::~COneShotCharge()
{

}


void COneShotCharge::LoadSprites()
{
	// [재기충전 작업] [2009-02-18]

	m_JekiSpr.Load( (char*)JEKIFFECT_PATH.c_str(), 555 );
	m_JekiChargeSprBtn.Load( (char*)JEKICHARGEBTN_PATH.c_str(), 555 );

	return;
}

BOOL COneShotCharge::Init(NMBASE::GRAPHICGDI::CPage *pPage)
{
	LoadSprites();
	m_JekiChargeBtn.Init(g_p7PokerGameView, pPage, 498, 715, &m_JekiChargeSprBtn, 0, IDM_JEKI_CHARGE, 4 );
	return TRUE;
}


void COneShotCharge::OnDraw(CDC *pDC)
{
	m_JekiChargeBtn.Draw(pDC);
}

BOOL COneShotCharge::OnLButtonDown(int x, int y)
{
	m_JekiChargeBtn.OnLButtonDown(x,y);
	return 1;
}


BOOL COneShotCharge::OnLButtonUp(int x, int y)
{
	m_JekiChargeBtn.OnLButtonUp(x,y);
	return 1;
}

BOOL COneShotCharge::OnMouseMove(int x, int y)
{
	m_JekiChargeBtn.OnMouseMove(x,y);
	return 1;
}

BOOL COneShotCharge::OnCommand(UINT nCmd)
{
	if(nCmd == IDM_JEKI_CHARGE)
	{
		//추가~!
		if(g_SeatKeeper.IsUsingSeatKeeper()	//내가 자리지킴이 사용중일 때
			|| g_SeatKeeper.GetReserveUser()==g_PLAYER(0)->ServPNum ) //내가 자리지킴이 예약중일때?
		{
			AddInfoText( "자리지킴이 예약 또는 사용시에는 원샷충전을 사용할 수 없습니다.\n" );
			return FALSE;
		}

		//JekiChargeBtn		
		if( g_PremOption.HaveOption( PREMOPTION::CODE_OPTION_JEKI ))
		{
			if( g_PremOption.GetRemainCount( PREMOPTION::CODE_OPTION_JEKI ) > 0 )
			{
				// 매달 1일 00 ~ 17시까지는 막는다.
				if( CRecoveryChargeDlg::IsJekiChargeReserved() )
				{
					AddInfoText( "이미 원샷충전을 예약하셨습니다.\n" );
				}
				else
				{
					CRecoveryChargeDlg dlg;
					dlg.DoModal( g_PremOption.GetRemainCount(PREMOPTION::CODE_OPTION_JEKI),
								 g_PremOption.GetMoney(PREMOPTION::CODE_OPTION_JEKI),
								 g_PremOption.GetOptionGoodsCode(PREMOPTION::CODE_OPTION_JEKI) );

					if( dlg.GetUseKind() > 0 )
					{
						// [패밀리 아이콘 작업]
						g_FamilyIcon.SetJekiState( ICS_ON, g_PremOption.HaveOption( PREMOPTION::CODE_OPTION_JEKI ), g_PremOption.GetRemainCount( PREMOPTION::CODE_OPTION_JEKI ) ); 
					}

					if( CCL_ASK_RECOVERY_CHARGE::ALLIN == dlg.GetUseKind() )
					{
						AddInfoText( "원샷충전 '한 번에 받기'를 사용하셨습니다.\n" );
					}
					else if( CCL_ASK_RECOVERY_CHARGE::HALF == dlg.GetUseKind() )
					{
						AddInfoText( "원샷충전 '두 번에 받기'를 사용하셨습니다.\n" );
					}
				}
			}
			else
			{
				AddInfoText( "원샷충전 상품을 모두 사용하셨습니다.\n" );
			}
		}
		else
		{
			// 가입 유도창
		}

		//	CRecoveryChargeDlg dlg;
		//	dlg.DoModal( 2, 100000, "재기충전 2만" );

		return TRUE;
	}

	return FALSE;

}

//G_TFT
void COneShotCharge::OnChargeMoney(CSV_CHANGE_MONEY& msg)
{
	int PNum = g_GAME()->GetPlayerPNum(msg.ID);
	int LeftCnt = *msg.SmallData_1;
	int TotalLeftCnt = *msg.SmallData_2;
	INT64 addmoney = (INT64)*msg.ChangeMoney;

	if( 0 == PNum )
	{			
		g_PremOption.SetCount( PREMOPTION::CODE_OPTION_JEKI, LeftCnt );
	}

	CString optioncode_name;
	CString optioncode_recv;
	optioncode_recv.Format( "%3.d", TotalLeftCnt );
	CRecoveryChargeDlg::GetJekiOptionName( optioncode_recv,  optioncode_name );

	CString strMsg;
	strMsg.Format( "★ [%s님] '%s %s'상품 충전금액 %s원이 지급되었습니다.\n", msg.ID, g_GetPremName(g_PLAYER(PNum)->UI.PremMenuNo,FALSE),						
																			  optioncode_name.GetBuffer(), NumberToOrientalString(addmoney).GetBuffer() );

	AddInfoText(&strMsg);

	// 애니메이션...
	CRecoveryChargeDlg::OnReceiveResult();	

	CJekiChargeEffect *pObj = new CJekiChargeEffect;
	if(pObj != NULL)
	{
		pObj->Init(addmoney,LeftCnt,PNum,&m_JekiSpr,TotalLeftCnt);
		g_PLAYER(PNum)->m_EffectSprDrawListMan.AddObject(pObj);				
	}			

	// [패밀리 아이콘 작업]
	g_FamilyIcon.SetJekiState( ICS_OFF, g_PremOption.HaveOption( PREMOPTION::CODE_OPTION_JEKI ), g_PremOption.GetRemainCount( PREMOPTION::CODE_OPTION_JEKI ) ); 
}

void COneShotCharge::OnPacketNotify(int Sid, int Signal, int TotSize, char *lpData)
{
	switch(Signal)
	{
		// [재기충전 작업] [2009-02-12]
	case SV_PREM_OPTION_INFO:
		{
			CSV_PREM_OPTION_INFO msg;
			msg.Get( lpData, TotSize );

			memcpy( &g_PremOption, msg.OptionInfo, sizeof(g_PremOption) );
			// [패밀리 아이콘 작업]
			g_p7PokerGameView->SetupByConfig();
		}
		break;

		// [재기충전 작업] [2009-02-19]
	case CL_RECOVERY_CHARGE_INFO:
		{
			CCL_RECOVERY_CHARGE_INFO msg;
			msg.Get( lpData, TotSize );

			CRecoveryChargeDlg::OnReceiveResult();

			if( CCL_RECOVERY_CHARGE_INFO::PROHIBITED_TERM == *msg.Result )
			{
				AddInfoText( "결제 진행 기간 중일 때는 옵션상품을 사용하실 수 없습니다.\n" );

				g_FamilyIcon.SetJekiState( ICS_OFF, g_PremOption.HaveOption( PREMOPTION::CODE_OPTION_JEKI ), g_PremOption.GetRemainCount( PREMOPTION::CODE_OPTION_JEKI ) );		
			}
		}
		break;

	default:
		break;	
	}
}