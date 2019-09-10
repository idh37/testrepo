// SprMoneyDraw.cpp: implementation of the CSprMoneyDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalPokerBase.h"
#include "sound.h"
#include <../src/MessageManager.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

COwnLimit					g_cOwnLimit;
CBanishManager				g_cBanishMan;
CPromotionManager			g_cPromotionMan;
CVipJackpot					g_cVipJackpot;
CWhisperlistComboPopup		g_cWhisperlistComboPopup;


const INT64 g_RoomMoneyRanges[13] = 
{
	0,					//1�� �̸�				
	100000000,			//1��
	1000000000,			//10��
	10000000000,		//100��
	100000000000,		//1000��
	1000000000000,		//1��
	3000000000000,		//3��
	10000000000000,		//10��
	30000000000000,		//30��
	50000000000000,		//50��
	100000000000000,	//100��
	100000000000000,	//200��
	100000000000000		//300��
};

class CPokerBaseFunctorRegister
{
public:
	CPokerBaseFunctorRegister();
	virtual ~CPokerBaseFunctorRegister(){}
};

CPokerBaseFunctorRegister s_cPokerBaseFunctorRegiste;

LRESULT PBMakeRoomName(WPARAM& wParam, LPARAM& lParam)
{
	CString title = "";

	int nRoomNameNum = (int)rand()%18;

	IDX_GAME eGameType = GM().GetCurrentGameType();
	if( eGameType == IDX_GAME_BD)
	{
		switch(nRoomNameNum){ 
		case 0: title = "����! �޷�! �������~"; break;
		case 1: title = "���� ȣ���� ã�Ƽ�~"; break;
		case 2: title = "������ ���� �̾���~"; break;
		case 3: title = "������ ���� ���߰� �Ѵ�"; break;
		case 4: title = "��Ű�� �ų� �Ҿ�� ��Ŀ�Ӵ�"; break;
		case 5: title = "����! ����! �� �۴�� ��~"; break;
		case 6: title = "�ٵ��̿� ������ �ð�"; break;
		case 7: title = "������ �Բ� �������"; break;
		case 8: title = "�ǵ����� �Ҹ��� ��������~"; break;
		case 9: title = "�ο�ٵ��� ���� �ϰ� ����?"; break;
		case 10: title = "�ϵ��� �ο�ٵ��̸� �˾�?"; break;
		case 11: title = "�ո��� ������ ������"; break;
		case 12: title = "�������̿�! �𿩶�"; break;
		case 13: title = "���� �Ҿ �ųʴ� ���� ����"; break;
		case 14: title = "�ѹ� �غ���? ��ٸ���~ ����!"; break;
		case 15: title = "�Ϸ�ٵ��� �� �������� �𸥴�"; break;
		case 16: title = "���̳� ���̳� �װ��� �����δ�"; break;	
		case 17: title = "������ �״� ǰ�ȿ�~"; break;
		}
	}
	else if( eGameType == IDX_GAME_SD)
	{
		switch(nRoomNameNum){ 
			case 0 : title = "������ ���� ���� ����"; break;
			case 1 : title = "������ ���� �̾���~"; break;
			case 2 : title = "���ȱ����� ���� ���߰� �Ѵ�"; break;
			case 3 : title = "���ȱ����� �״� ǰ�ȿ�~"; break;
			case 4 : title = "����! ����! �� �۴�� ��~"; break;
			case 5 : title = "���ε��� �߰ſ� �º�"; break;
			case 6 : title = "���ȱ����� �Բ� �������."; break;
			case 7 : title = "�ǵ����� �Ҹ��� �������ڱ�~"; break;
			case 8 : title = "���ٿ� ��籸!"; break;
			case 9 : title = "���� ���� �ϰ� ����?"; break;
			case 10 : title = "�ϵ��� ���ٸ� �˾�?"; break;
			case 11 : title = "�� ���Ŷ�~ �׷� ����!"; break;
			case 12 : title = "������ ��ξ�~"; break;
			case 13 : title = "�������� ���� ����!"; break;
			case 14 : title = "�ո��� ������ ������"; break;
			case 15 : title = "�������̿�! �𿩶�"; break;
			case 16 : title = "���� �Ⱦ�? �׷� �¸ų�~"; break;
			case 17 : title = "�� 1940 ���ٽô� ��"; break;
		}
	}
	else if( eGameType == IDX_GAME_HA)
	{
		switch(nRoomNameNum)
		{
			case 0 : title = "���� ȣ���� ã�Ƽ�~"; break;
			case 1 : title = "�������� ���� �̾���~"; break;
			case 2 : title = "�Ƕ�� ���� ���߰� �Ѵ�"; break;
			case 3 : title = "��Ű�� �ų� �Ҿ�� �Ӵ�"; break;
			case 4 : title = "����! ����! �� �۴�� ��~"; break;
			case 5 : title = "�Ƕ� ���� �ϰ� ����?"; break;
			case 6 : title = "�ϵ��� �Ƕ� �˾�?"; break;
			case 7 : title = "�������̿�! �𿩶�"; break;
			case 8 : title = "���� �Ⱦ�? �׷� �¸ų�~"; break;
			case 9 : title = "���� �Ҿ �ųʴ� ���� ����"; break;
			case 10 : title = "�ѹ� �غ���? ��ٸ���~ ����!"; break;
			case 11 : title = "�Ƕ��ܼ�Ʈ~"; break;
			case 12 : title = "�����~ �Ƕ��Ƕ�~"; break;
			case 13 : title = "������ �Ƕ� ����!"; break;
			case 14 : title = "�������� �Ƕ��ؿ�"; break;
			case 15 : title = "�Ƕ� ¯~��"; break;
			case 16 : title = "�Ƕ��� ����~��"; break;
			case 17 : title = "��ť��ť �����մϴ�!"; break;
			case 18 : title = "����! ����! ����!"; break;
			case 19 : title = "�������� �ƴ� �Ƕ�! ��ť!"; break;
			default : title = "�������� �ƴ� �Ƕ�! ��ť!"; break;
		}
	}
	else
	{
		switch(nRoomNameNum)
		{
		case 0: title = "����! �޷�! �������~";break;
		case 1: title = "���� ȣ���� ã�Ƽ�~";break;
		case 2: title = "������ ���� �̾���~";break;
		case 3: title = "��Ƽ�� ���� ���߰� �Ѵ�";break;
		case 4: title = "��Ű�� �ų� �Ҿ�� �Ӵ�";break;
		case 5: title = "���� ����! �� �۴�� ��";break;
		case 6: title = "�󽺺��������� �� ��!";break;
		case 7: title = "��ī������ �� ��!";break;
		case 8: title = "����ī���ο��� �� ��!";break;
		case 9: title = "��Ƽ�ð� �Բ� �������";break;
		case 10: title = "�ǵ����� �Ҹ��� ��������~";break;
		case 11: title = "��Ŀ ���� �ϰ� ����?";break;
		case 12: title = "�ϵ��� ��Ŀ�� �˾�?";break;
		case 13: title = "�ո��� ������ ������";break;
		case 14: title = "���Ծ ���̽�!!";break;
		case 15: title = "�������̿�! �𿩶�";break;
		case 16: title = "�� ���� �غ��߾�~ ��Ƽ��";break;
		case 17: title = "�������� ��Ŀ�ؿ�~!";break;
		}
	}

	*((CString *)lParam) = title;
	return TRUE;
}

bool PBIsSuperRoom(void)
{
	//���۹� ���� ����
	return false;

	// ���۹��� �������� ������̸� ���۹� ����� �ɼ� �ڵ� üũ
	BOOL bExist = FALSE;
	CPlayer *pPlayer = GM().GetCurrentProcess()->GetPlayer(0);
	
	//���ӹ��̳� ȸ������ ������ �ִٸ�
	if( UPGCheckPremiumState(&GM().GetMyInfo()->UI) >= CODE_NEW_FAMILY_SILVER || 0 != pPlayer->UI.nPCRoomIPType )
	{
		return true;
	}

	//���۹��� �������� ������϶�
	if(UPGIsGameItemUsing(ITEM_SCODE_SUPERMASTER, &bExist)) 
	{
		return true;
	}

	return false;
}

int PBTurnTimeOut(ENUM_TURN_TIMEOUT_TYPE nType)
{
	int nRet = 5;
	switch(nType)
	{
	case ETTT_BETTING:
		nRet = 5;
#if defined(QA_TEST)
		nRet += 30;
#endif
#if defined(_DEBUG)
		nRet += 30;
#endif
		break;
	case ETTT_CARDCHOICE:
		nRet = 5;
		break;
	case ETTT_CUTTING:
		nRet = 5;
		break;
	}

	nRet += TURN_TIMER_WAIT_TIME/1000;

	return nRet;
}

static bool s_bCreateComboPopup = false;

void PBCreateWhisperlistComboPopup(void)
{
	if(!s_bCreateComboPopup)
	{
		s_bCreateComboPopup = true;
		g_cWhisperlistComboPopup.Create(12, 588);
	}
}

CPokerBaseFunctorRegister::CPokerBaseFunctorRegister()
{
	PBRegistSoundFiles();
	MM().AddMsgH(UPG_CMK_GENERATE_RANDOM_ROOM_NAME, Fnt(PBMakeRoomName));
}

bool PBIsComboPopupClose(const CPoint &ptPos)
{
	return MM().Call(PB_IS_COMBO_POPUP_CLOSE, 0, (LPARAM)(&ptPos))?true:false;
}