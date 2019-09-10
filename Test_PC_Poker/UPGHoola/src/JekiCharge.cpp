// g_OneShotCharge.cpp: implementation of the COneShotCharge class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "OneShotCharge.h"
//#include "RecoveryChargeDlg.h"// [������� �۾�] [2009-02-17]
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
	// [������� �۾�] [2009-02-18]

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
		//�߰�~!
		if(g_SeatKeeper.IsUsingSeatKeeper()	//���� �ڸ���Ŵ�� ������� ��
			|| g_SeatKeeper.GetReserveUser()==g_PLAYER(0)->ServPNum ) //���� �ڸ���Ŵ�� �������϶�?
		{
			AddInfoText( "�ڸ���Ŵ�� ���� �Ǵ� ���ÿ��� ���������� ����� �� �����ϴ�.\n" );
			return FALSE;
		}

		//JekiChargeBtn		
		if( g_PremOption.HaveOption( PREMOPTION::CODE_OPTION_JEKI ))
		{
			if( g_PremOption.GetRemainCount( PREMOPTION::CODE_OPTION_JEKI ) > 0 )
			{
				// �Ŵ� 1�� 00 ~ 17�ñ����� ���´�.
				if( CRecoveryChargeDlg::IsJekiChargeReserved() )
				{
					AddInfoText( "�̹� ���������� �����ϼ̽��ϴ�.\n" );
				}
				else
				{
					CRecoveryChargeDlg dlg;
					dlg.DoModal( g_PremOption.GetRemainCount(PREMOPTION::CODE_OPTION_JEKI),
								 g_PremOption.GetMoney(PREMOPTION::CODE_OPTION_JEKI),
								 g_PremOption.GetOptionGoodsCode(PREMOPTION::CODE_OPTION_JEKI) );

					if( dlg.GetUseKind() > 0 )
					{
						// [�йи� ������ �۾�]
						g_FamilyIcon.SetJekiState( ICS_ON, g_PremOption.HaveOption( PREMOPTION::CODE_OPTION_JEKI ), g_PremOption.GetRemainCount( PREMOPTION::CODE_OPTION_JEKI ) ); 
					}

					if( CCL_ASK_RECOVERY_CHARGE::ALLIN == dlg.GetUseKind() )
					{
						AddInfoText( "�������� '�� ���� �ޱ�'�� ����ϼ̽��ϴ�.\n" );
					}
					else if( CCL_ASK_RECOVERY_CHARGE::HALF == dlg.GetUseKind() )
					{
						AddInfoText( "�������� '�� ���� �ޱ�'�� ����ϼ̽��ϴ�.\n" );
					}
				}
			}
			else
			{
				AddInfoText( "�������� ��ǰ�� ��� ����ϼ̽��ϴ�.\n" );
			}
		}
		else
		{
			// ���� ����â
		}

		//	CRecoveryChargeDlg dlg;
		//	dlg.DoModal( 2, 100000, "������� 2��" );

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
	strMsg.Format( "�� [%s��] '%s %s'��ǰ �����ݾ� %s���� ���޵Ǿ����ϴ�.\n", msg.ID, g_GetPremName(g_PLAYER(PNum)->UI.PremMenuNo,FALSE),						
																			  optioncode_name.GetBuffer(), NumberToOrientalString(addmoney).GetBuffer() );

	AddInfoText(&strMsg);

	// �ִϸ��̼�...
	CRecoveryChargeDlg::OnReceiveResult();	

	CJekiChargeEffect *pObj = new CJekiChargeEffect;
	if(pObj != NULL)
	{
		pObj->Init(addmoney,LeftCnt,PNum,&m_JekiSpr,TotalLeftCnt);
		g_PLAYER(PNum)->m_EffectSprDrawListMan.AddObject(pObj);				
	}			

	// [�йи� ������ �۾�]
	g_FamilyIcon.SetJekiState( ICS_OFF, g_PremOption.HaveOption( PREMOPTION::CODE_OPTION_JEKI ), g_PremOption.GetRemainCount( PREMOPTION::CODE_OPTION_JEKI ) ); 
}

void COneShotCharge::OnPacketNotify(int Sid, int Signal, int TotSize, char *lpData)
{
	switch(Signal)
	{
		// [������� �۾�] [2009-02-12]
	case SV_PREM_OPTION_INFO:
		{
			CSV_PREM_OPTION_INFO msg;
			msg.Get( lpData, TotSize );

			memcpy( &g_PremOption, msg.OptionInfo, sizeof(g_PremOption) );
			// [�йи� ������ �۾�]
			g_p7PokerGameView->SetupByConfig();
		}
		break;

		// [������� �۾�] [2009-02-19]
	case CL_RECOVERY_CHARGE_INFO:
		{
			CCL_RECOVERY_CHARGE_INFO msg;
			msg.Get( lpData, TotSize );

			CRecoveryChargeDlg::OnReceiveResult();

			if( CCL_RECOVERY_CHARGE_INFO::PROHIBITED_TERM == *msg.Result )
			{
				AddInfoText( "���� ���� �Ⱓ ���� ���� �ɼǻ�ǰ�� ����Ͻ� �� �����ϴ�.\n" );

				g_FamilyIcon.SetJekiState( ICS_OFF, g_PremOption.HaveOption( PREMOPTION::CODE_OPTION_JEKI ), g_PremOption.GetRemainCount( PREMOPTION::CODE_OPTION_JEKI ) );		
			}
		}
		break;

	default:
		break;	
	}
}