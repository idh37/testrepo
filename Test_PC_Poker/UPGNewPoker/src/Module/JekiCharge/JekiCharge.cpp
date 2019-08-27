// JekiCharge.cpp: implementation of the CJekiCharge class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JekiCharge.h"
#include "../FamilyIcon/FamilyIcon.h"
#include "../../AniListMan/AniFileListMan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJekiCharge& CJekiCharge::GetInstance()
{
	static CJekiCharge jeki;
	return jeki;
}

CJekiCharge& JekiCharge()
{
	return CJekiCharge::GetInstance();
}


CJekiCharge::CJekiCharge()
{

}

CJekiCharge::~CJekiCharge()
{

}


void CJekiCharge::LoadSprites()
{
	// [재기충전 작업] [2009-02-18]

	m_sprJeki.Load( ".\\newpoker\\data\\JekiEffect.spr", 555 );
	return;
}

BOOL CJekiCharge::Init(NMBASE::GRAPHICGDI::CPage *pPage)
{
	LoadSprites();
	return TRUE;
}


void CJekiCharge::OnDraw(CDC *pDC)
{
}

BOOL CJekiCharge::OnLButtonDown(int x, int y)
{
	return 1;
}


BOOL CJekiCharge::OnLButtonUp(int x, int y)
{
//	return m_JekiChargeBtn.OnLButtonUp(x,y);
	return 1;
}

BOOL CJekiCharge::OnMouseMove(int x, int y)
{
//	return m_JekiChargeBtn.OnMouseMove(x,y);
	return 1;
}

void CJekiCharge::OnCommand(UINT nCmd)
{
/*
	if (nCmd==IDM_JEKI_CHARGE)
	{

		//추가~!
		if(g_SeatKeeper.IsUsingSeatKeeper() //내가 자리지킴이 사용중일 때
			|| g_SeatKeeper.GetReserveUser()==GAME()->GetPlayerNP(0)->ServPNum ) //내가 자리지킴이 예약중일때?
		{
			AddGameChatViewEdit( "자리지킴이 예약 또는 사용시에는 원샷충전을 사용할 수 없습니다.\n", GetChatColor(ECC_JEKI_CHARGE) );
			return;
		}

		//JekiChargeBtn		
		if( g_PremOption.HaveOption( PREMOPTION::CODE_OPTION_JEKI ))
		{
			if( g_PremOption.GetRemainCount( PREMOPTION::CODE_OPTION_JEKI ) > 0 )
			{
				// 매달 1일 00 ~ 17시까지는 막는다.

				if( CRecoveryChargeDlg::IsJekiChargeReserved() )
				{
					AddGameChatViewEdit( "이미 원샷충전을 예약하셨습니다.\n", GetChatColor(ECC_JEKI_CHARGE) );
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
						AddGameChatViewEdit( "원샷충전 '한 번에 받기'를 사용하셨습니다.\n", GetChatColor(ECC_JEKI_CHARGE) );
					}
					else if( CCL_ASK_RECOVERY_CHARGE::HALF == dlg.GetUseKind() )
					{
						AddGameChatViewEdit( "원샷충전 '두 번에 받기'를 사용하셨습니다.\n", GetChatColor(ECC_JEKI_CHARGE) );
					}
				}

			}
			else
			{
				AddGameChatViewEdit( "원샷충전 상품을 모두 사용하셨습니다.\n", GetChatColor(ECC_JEKI_CHARGE) );
			}
		}
		else
		{
			// 가입 유도창
		}

		//	CRecoveryChargeDlg dlg;
		//	dlg.DoModal( 2, 100000, "재기충전 2만" );


	}
*/
}

void CJekiCharge::OnPacketNotify(int Sid, int Signal, int TotSize, char *lpData)
{
/*
	switch(Signal)
	{
		// [재기충전 작업] [2009-02-12]
		case SV_PREM_OPTION_INFO:
			{
				CSV_PREM_OPTION_INFO msg;
				msg.Get( lpData, TotSize );
				
				memcpy( &g_PremOption, msg.OptionInfo, sizeof(g_PremOption) );

				// [패밀리 아이콘 작업]
				// 힌트 아이템이 있는 사람은 아래 주석을 삭제하고 아래처럼 처리한다.
				g_FamilyIcon.SetJekiState( ICS_OFF, g_PremOption.HaveOption( PREMOPTION::CODE_OPTION_JEKI ), g_PremOption.GetRemainCount( PREMOPTION::CODE_OPTION_JEKI ) );
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
					AddGameChatViewEdit( "결제 진행 기간중일때는 옵션상품을 사용하실 수 없습니다.", GetChatColor(ECC_JEKI_CHARGE) );
					//2009년 12월 22일 점검이후 적용
					g_FamilyIcon.SetJekiState( ICS_OFF, g_PremOption.HaveOption( PREMOPTION::CODE_OPTION_JEKI ), g_PremOption.GetRemainCount( PREMOPTION::CODE_OPTION_JEKI ) );		
				}
			}
			break;
	}*/
}

//G_TFT
void CJekiCharge::OnChargeMoney(CSV_CHANGE_MONEY& msg)
{
/*	int PNum = GAME()->GetPlayerPNum(msg.ID);
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
	strMsg.Format( "★ [%s님] '%s %s'상품 충전금액 %s원이 지급되었습니다.\n", 
					msg.ID, 
					g_GetPremName(GAME()->GetPlayerNP(PNum)->UI.PremMenuNo,FALSE),						
					optioncode_name,
					NumberToOrientalString(addmoney) );

	AddGameChatViewEdit(strMsg, GetChatColor(ECC_JEKI_CHARGE));

	// 애니메이션...
	CRecoveryChargeDlg::OnReceiveResult();	
	

	//*msg.TotalLeftCnt = 옵션 상품 종류 1 , 2
	CAllinChargeInfo *pObj = new CAllinChargeInfo;
	
	if(pObj != NULL){
		pObj->Init(addmoney,LeftCnt,PNum,&m_sprJeki,CAllinChargeInfo::en_Jeki, TotalLeftCnt);
		GAME()->m_cAniSprObjectMan.AddObject(pObj);
	}	

	// [패밀리 아이콘 작업]
	g_FamilyIcon.SetJekiState( ICS_OFF, g_PremOption.HaveOption( PREMOPTION::CODE_OPTION_JEKI ), g_PremOption.GetRemainCount( PREMOPTION::CODE_OPTION_JEKI ) ); 
	*/
}
