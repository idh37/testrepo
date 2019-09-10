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
	// [������� �۾�] [2009-02-18]

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

		//�߰�~!
		if(g_SeatKeeper.IsUsingSeatKeeper() //���� �ڸ���Ŵ�� ������� ��
			|| g_SeatKeeper.GetReserveUser()==GAME()->GetPlayerNP(0)->ServPNum ) //���� �ڸ���Ŵ�� �������϶�?
		{
			AddGameChatViewEdit( "�ڸ���Ŵ�� ���� �Ǵ� ���ÿ��� ���������� ����� �� �����ϴ�.\n", GetChatColor(ECC_JEKI_CHARGE) );
			return;
		}

		//JekiChargeBtn		
		if( g_PremOption.HaveOption( PREMOPTION::CODE_OPTION_JEKI ))
		{
			if( g_PremOption.GetRemainCount( PREMOPTION::CODE_OPTION_JEKI ) > 0 )
			{
				// �Ŵ� 1�� 00 ~ 17�ñ����� ���´�.

				if( CRecoveryChargeDlg::IsJekiChargeReserved() )
				{
					AddGameChatViewEdit( "�̹� ���������� �����ϼ̽��ϴ�.\n", GetChatColor(ECC_JEKI_CHARGE) );
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
						AddGameChatViewEdit( "�������� '�� ���� �ޱ�'�� ����ϼ̽��ϴ�.\n", GetChatColor(ECC_JEKI_CHARGE) );
					}
					else if( CCL_ASK_RECOVERY_CHARGE::HALF == dlg.GetUseKind() )
					{
						AddGameChatViewEdit( "�������� '�� ���� �ޱ�'�� ����ϼ̽��ϴ�.\n", GetChatColor(ECC_JEKI_CHARGE) );
					}
				}

			}
			else
			{
				AddGameChatViewEdit( "�������� ��ǰ�� ��� ����ϼ̽��ϴ�.\n", GetChatColor(ECC_JEKI_CHARGE) );
			}
		}
		else
		{
			// ���� ����â
		}

		//	CRecoveryChargeDlg dlg;
		//	dlg.DoModal( 2, 100000, "������� 2��" );


	}
*/
}

void CJekiCharge::OnPacketNotify(int Sid, int Signal, int TotSize, char *lpData)
{
/*
	switch(Signal)
	{
		// [������� �۾�] [2009-02-12]
		case SV_PREM_OPTION_INFO:
			{
				CSV_PREM_OPTION_INFO msg;
				msg.Get( lpData, TotSize );
				
				memcpy( &g_PremOption, msg.OptionInfo, sizeof(g_PremOption) );

				// [�йи� ������ �۾�]
				// ��Ʈ �������� �ִ� ����� �Ʒ� �ּ��� �����ϰ� �Ʒ�ó�� ó���Ѵ�.
				g_FamilyIcon.SetJekiState( ICS_OFF, g_PremOption.HaveOption( PREMOPTION::CODE_OPTION_JEKI ), g_PremOption.GetRemainCount( PREMOPTION::CODE_OPTION_JEKI ) );
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
					AddGameChatViewEdit( "���� ���� �Ⱓ���϶��� �ɼǻ�ǰ�� ����Ͻ� �� �����ϴ�.", GetChatColor(ECC_JEKI_CHARGE) );
					//2009�� 12�� 22�� �������� ����
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
	strMsg.Format( "�� [%s��] '%s %s'��ǰ �����ݾ� %s���� ���޵Ǿ����ϴ�.\n", 
					msg.ID, 
					g_GetPremName(GAME()->GetPlayerNP(PNum)->UI.PremMenuNo,FALSE),						
					optioncode_name,
					NumberToOrientalString(addmoney) );

	AddGameChatViewEdit(strMsg, GetChatColor(ECC_JEKI_CHARGE));

	// �ִϸ��̼�...
	CRecoveryChargeDlg::OnReceiveResult();	
	

	//*msg.TotalLeftCnt = �ɼ� ��ǰ ���� 1 , 2
	CAllinChargeInfo *pObj = new CAllinChargeInfo;
	
	if(pObj != NULL){
		pObj->Init(addmoney,LeftCnt,PNum,&m_sprJeki,CAllinChargeInfo::en_Jeki, TotalLeftCnt);
		GAME()->m_cAniSprObjectMan.AddObject(pObj);
	}	

	// [�йи� ������ �۾�]
	g_FamilyIcon.SetJekiState( ICS_OFF, g_PremOption.HaveOption( PREMOPTION::CODE_OPTION_JEKI ), g_PremOption.GetRemainCount( PREMOPTION::CODE_OPTION_JEKI ) ); 
	*/
}
