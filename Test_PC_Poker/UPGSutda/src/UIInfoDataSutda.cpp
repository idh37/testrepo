// UIInfoDataSutda.cpp: implementation of the CSprMoneyDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UIInfoDataSutda.h"
#include "MyCard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CUIInfoDataSutda::CUIInfoDataSutda()
{
	Init();
}

CUIInfoDataSutda::~CUIInfoDataSutda()
{
}

void CUIInfoDataSutda::Init(void)
{
	m_clrBtnBetText = RGB(219, 198, 115);
	m_clrBtnBetTextShadow = RGB(76, 67, 28);
 
	//�� �÷��̾��� ī����� ��ġ
	m_listCardStartPos.clear();
	m_listCardStartPos.push_back(CPoint(470, 548));
	m_listCardStartPos.push_back(CPoint(151, 334));
	m_listCardStartPos.push_back(CPoint(151, 146));
	m_listCardStartPos.push_back(CPoint(673, 146));
	m_listCardStartPos.push_back(CPoint(673, 334));

	//�� �÷��̾��� �ƹ�Ÿ ��ġ
	m_listAvatarPos.clear();
	m_listAvatarPos.push_back(CPoint(385, 574));
	m_listAvatarPos.push_back(CPoint(72, 362));
	m_listAvatarPos.push_back(CPoint(72, 174));
	m_listAvatarPos.push_back(CPoint(868, 174));
	m_listAvatarPos.push_back(CPoint(868, 362));

	//�� �÷��̾��� ��׶��� ��ġ
	m_listPlayerBackPos.clear();
	m_listPlayerBackPos.push_back(CPoint(304,512));
	m_listPlayerBackPos.push_back(CPoint(55,309));
	m_listPlayerBackPos.push_back(CPoint(55,121));
	m_listPlayerBackPos.push_back(CPoint(668,121));
	m_listPlayerBackPos.push_back(CPoint(668,309));


	//�� ������ �ӴϿ���
	m_listUserMoneyRect.clear();
	m_listUserMoneyRect.push_back(CRect(469, 672, 469+206, 672+16));
	m_listUserMoneyRect.push_back(CRect(151, 443, 151+175, 443+16));
	m_listUserMoneyRect.push_back(CRect(151, 254, 151+175, 254+16));
	m_listUserMoneyRect.push_back(CRect(673, 254, 673+175, 254+16));
	m_listUserMoneyRect.push_back(CRect(673, 443, 673+175, 443+16));


	//�� �÷��̾��� ���� ���� �ݾ� ǥ�� ��ġ 
	m_rtBettingMoney			= CRect(0,0,176,13);	
	m_listBettingMoneyPos.clear();
 	m_listBettingMoneyPos.push_back(CPoint(716, 517));
	m_listBettingMoneyPos.push_back(CPoint(151, 315));
	m_listBettingMoneyPos.push_back(CPoint(151, 127));
	m_listBettingMoneyPos.push_back(CPoint(673, 127)); 
	m_listBettingMoneyPos.push_back(CPoint(673, 315));

	//�� �÷��̾��� ����� �����̸� ǥ�� ��ġ
	m_listJokboNameRect.clear();

	m_rtJokoName				= CRect(471, 525,471+204,525+13);
	m_listJokboNameRect.push_back(m_rtJokoName);
	m_listJokboNameRect.push_back(CRect(151, 315, 151+176, 315+13));
	m_listJokboNameRect.push_back(CRect(151, 127, 151+176, 127+13));
	m_listJokboNameRect.push_back(CRect(673, 127, 673+176, 127+13));
	m_listJokboNameRect.push_back(CRect(673, 315, 673+176, 315+13));

	
	//���� ǥ�� ����
	m_clrJokboNameShadowColor	= RGB(0,0,0);
	m_clrJokboNameColor			= RGB(240,222,98);

//	m_listBettingMoneyPos.push_back(CPoint(716, 519));
//  	m_listBettingMoneyPos.push_back(CPoint(47, 307));
//  	m_listBettingMoneyPos.push_back(CPoint(47, 112));
// 	m_listBettingMoneyPos.push_back(CPoint(665, 112));
// 	m_listBettingMoneyPos.push_back(CPoint(665, 307));


	m_ptUserNameGab = CPoint(367 - 385, 654- 574 );

	//������ũ
	m_ptlistBossMarkGab.clear();
	m_ptlistBossMarkGab.push_back(CPoint(-46, 10));	
	m_ptlistBossMarkGab.push_back(CPoint(111-151, 158-146));
	m_ptlistBossMarkGab.push_back(CPoint(907-673, 346-334));
	
	m_listUserMoneyRect[0] = CRect(471, 660, 471+132, 660+15);
	
	m_listButtonPos[EBT_SEATKEEPER] = CPoint(431, 710); // �ڸ���Ŵ�� ��ư ��ġ
	m_listButtonPos[EBT_ONESHOTCHARGE] = CPoint(492, 710); // �������� ��ư ��ġ
	m_listButtonPos[EBT_CARD_OPEN] = CPoint(538, 710); // OPEN ��ư ��ġ
	m_listButtonPos[EBT_SINOUT] = CPoint(576, 709); // ���� ��ư ��ġ
	m_listButtonPos[EBT_BOTTOM_EXIT] = CPoint(616, 709); // ������ ��ư ��ġ
	m_listButtonPos[EBT_WAITUSER] = CPoint(657, 710); // ��� ��ư ��ġ
	
	m_rtResultJokboYouSize			= CRect(0,0,194,20);
	m_rtResultMoneyYouSize			= CRect(0,0,187,19);

	m_rtResultJokboMySize			= CRect(0,0,200,20);
	m_rtResultMoneyMySize			= CRect(0,0,193,19);

	m_nMyCardGab = CARD_WIDTH + 1;		//��ī�� ����
	m_nCardGab = 39;


	
	m_listParkmoonsuPos.push_back(CPoint(632, 529));
	m_listParkmoonsuPos.push_back(CPoint(321, 334));
	m_listParkmoonsuPos.push_back(CPoint(321, 146));
	m_listParkmoonsuPos.push_back(CPoint(843, 146));
	m_listParkmoonsuPos.push_back(CPoint(843, 334));	
	


	m_listTangKillerPos.push_back(CPoint(322, 529));
	m_listTangKillerPos.push_back(CPoint(32, 334));	
	m_listTangKillerPos.push_back(CPoint(32, 148));
	m_listTangKillerPos.push_back(CPoint(822, 147));
	m_listTangKillerPos.push_back(CPoint(828, 334));
	

	m_listResultStampPos.push_back(CPoint(477, 526));	
	m_listResultStampPos.push_back(CPoint(178, 338));
	m_listResultStampPos.push_back(CPoint(178, 150));
	m_listResultStampPos.push_back(CPoint(683, 150));	
	m_listResultStampPos.push_back(CPoint(683, 338));


	//���� ������ ��ġ
	m_listPenaltyMoneyPos.push_back(CPoint(484, 595));
	m_listPenaltyMoneyPos.push_back(CPoint(148, 387));
	m_listPenaltyMoneyPos.push_back(CPoint(148, 199));
	m_listPenaltyMoneyPos.push_back(CPoint(671, 199));
	m_listPenaltyMoneyPos.push_back(CPoint(671, 387));


	//Ĩ�� ���� �� ������ġ
	m_listChipStartPos.push_back(CPoint(532, 576));
	m_listChipStartPos.push_back(CPoint(279, 392));
	m_listChipStartPos.push_back(CPoint(279, 196));
	m_listChipStartPos.push_back(CPoint(717, 196));
	m_listChipStartPos.push_back(CPoint(717, 392));


	m_ptRematchNotice = CPoint(396, 230);					//���� �˸�â �ִϸ��̼� ��ġ

	m_ptBetBoard = CPoint(332, 115);



	//�ǵ�, �ݺ�� ��Ʈ
	m_rtBackTotalMoney			= CRect(401, 425, 401+216, 425+17);
	m_rtBackCallMoney			= CRect(401, 449, 401+216, 449+17);

	m_ptGold						= CRect(487, 688, 487+116, 688+12);

	//ä�ÿ��� �̺�Ʈ ��ư ��ġ
	m_listButtonPos[EBT_EVENT] = CPoint(153, 514);
}