#include "StdAfx.h"
#include "UIInfoData_BD.h"

namespace ui
{
CUIInfoData_BD::CUIInfoData_BD(void)
{
	Initialize();
}

CUIInfoData_BD::~CUIInfoData_BD(void)
{
	Clear();
}

void CUIInfoData_BD::Clear()
{
	m_mapPos.clear();
	m_mapRect.clear();
	m_mapGab.clear();
	m_mapFloat.clear();
}

void CUIInfoData_BD::Initialize()
{
	Clear();

	//Pos
	m_mapPos.insert( std::make_pair( EPOS_DEALING_TITEL, CPoint( 310, 0 ) )); // ��� ī�� ������
	
	// ī����ǥ
	m_mapPos.insert( std::make_pair( EPOS_CARD_POS_0, CPoint( 486, 548 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CARD_POS_1, CPoint( 121, 331 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CARD_POS_2, CPoint( 121, 136 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CARD_POS_3, CPoint( 710, 136 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CARD_POS_4, CPoint( 710, 331 ) ));
	// ��ư
	m_mapPos.insert( std::make_pair( EPOS_CHANGE, CPoint( 714, 546 ) ));
	m_mapPos.insert( std::make_pair( EPOS_PASS, CPoint( 849, 546 ) ));

	// ��ħ, ����, ����
	m_mapPos.insert( std::make_pair( EPOS_CUTINFO, CPoint( 404, 157 ) ));

	// �ٲ� ī�带 �����ϼ���
	m_mapPos.insert( std::make_pair( EPOS_CUTSELECT, CPoint( 351, 276 ) ));

	// ���� ����Ʈ �ؽ�Ʈ
	m_mapPos.insert( std::make_pair( EPOS_RESULT_0, CPoint( 489, 623 ) ));
	m_mapPos.insert( std::make_pair( EPOS_RESULT_1, CPoint( 124, 405 ) ));
	m_mapPos.insert( std::make_pair( EPOS_RESULT_2, CPoint( 124, 210 ) ));
	m_mapPos.insert( std::make_pair( EPOS_RESULT_3, CPoint( 713, 210 ) ));
	m_mapPos.insert( std::make_pair( EPOS_RESULT_4, CPoint( 713, 405 ) ));
	
	// Ŀ�ü��� 
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_0, CPoint( 438, 548 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_1, CPoint( 296, 330 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_2, CPoint( 296, 135 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_3, CPoint( 667, 135 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_4, CPoint( 667, 330 ) ));

	// Ŀ�ü��� Scene Ani P, 1, 2, 3, 4 
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_SCENE_0, CPoint( 518, 545 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_SCENE_1, CPoint( 223, 345 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_SCENE_2, CPoint( 223, 167 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_SCENE_3, CPoint( 790, 167 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_SCENE_4, CPoint( 790, 345 ) ));

	// �������Ű
	m_mapPos.insert( std::make_pair( EPOS_HIDDEN_OPENKEY, CPoint( 487, 632 ) ));
	
	// ���۹�ư
	m_mapPos.insert( std::make_pair( EPOS_START_BTN, CPoint( 411, 286 ) ));

	// �ڵ� �ø�
	m_mapPos.insert( std::make_pair( EPOS_AUTOUP_BTN, CPoint( 305, 710 ) ));
		
	// �ڵ� ����:2012.09.27 YS.Jeong 
	m_mapPos.insert( std::make_pair( EPOS_AUTOSORT_BTN, CPoint( 375, 710 ) ));

	// ������ǥ
	m_mapPos.insert( std::make_pair( EPOS_DEALER_POS_0, CPoint( 535, 483 ) ));
	m_mapPos.insert( std::make_pair( EPOS_DEALER_POS_1, CPoint( 343, 436 ) ));
	m_mapPos.insert( std::make_pair( EPOS_DEALER_POS_2, CPoint( 343, 243 ) ));
	m_mapPos.insert( std::make_pair( EPOS_DEALER_POS_3, CPoint( 630, 243 ) ));
	m_mapPos.insert( std::make_pair( EPOS_DEALER_POS_4, CPoint( 630, 436 ) ));

	//
	//Rect
	m_mapRect.insert( std::make_pair( ERECT_WININFO, CRect( 0, 0, 164, 14 ) )); // ��,�� ����Ʈ ���� ��¿���

	//
	//Gab
	m_mapGab.insert( std::make_pair( EGAB_CARD_POS_X, 30 ));	    // ī�嵦 ù��°, �ι�° ����
	m_mapGab.insert( std::make_pair( EGAB_CARD_POS_OTHER_X, 30 ));  // ī�嵦 ù��°, �ι�° ����
	m_mapGab.insert( std::make_pair( EGAB_CUTINFO_X, 64 ));  		// ��ħ, ����, ���� 
	m_mapGab.insert( std::make_pair( EGAB_CUTNUM_Y, 38 ));   		// ������ ���� ���ΰ��� �Ƴѹ� �������� �����ǥ
	m_mapGab.insert( std::make_pair( EGAB_RESULT_X, 0 ));    		// ���Ӱ�� ���������� �����ǥ �Ӵ�
	m_mapGab.insert( std::make_pair( EGAB_RESULT_Y, 19 ));   		// ���Ӱ�� ���������� �����ǥ �Ӵ�

	//
	//Float
	m_mapFloat.insert( std::make_pair( EFLOAT_F4_SPEED, 1.0f ));			 // ó�� 4�� ī�� �̵��ӵ� �ִϼӵ� ����
	m_mapFloat.insert( std::make_pair( EFLOAT_F4_TOTAL_CARD_DELAY, 20.0f )); // ó�� 4�� ī�� �̵��� ���ð�
	m_mapFloat.insert( std::make_pair( EFLOAT_F4_CARD_DELAY, 5.0f ));   	 // ó�� 4�� ī�� �̵����( ī��� ī�� )
	m_mapFloat.insert( std::make_pair( EFLOAT_F4_USER_DELAY, 5.0f ));	     // ó�� 4�� ī�� �̵����( ������ ���� )
	m_mapFloat.insert( std::make_pair( EFLOAT_CUT_SPEED, 7.1f ));   		 // ���� ī�� �̵��ӵ�
	m_mapFloat.insert( std::make_pair( EFLOAT_CUT_TOTAL_DELAY, 1.0f ));   	 // ���� ī�� �̵� �� �� ���ð�
	m_mapFloat.insert( std::make_pair( EFLOAT_CUT_CARD_DELAY, 5.0f ));   	 // ���� ī�� �̵����( ī��� ī�� )
	m_mapFloat.insert( std::make_pair( EFLOAT_CUT_PASS_DELAY, 15.0f ));   	 // Ŀ�� �н� �̵� �� ���ð�
	
	//
	//RGB
	m_mapRGB.insert( std::make_pair( ERGB_BETBTNTEXT, RGB( 164, 201, 88 ) ));	   // ��ư�� ���� �ݾ� ǥ�� ���ڻ�
	m_mapRGB.insert( std::make_pair( ERGB_BETBTNTEXT_SHADOW, RGB( 58, 86, 25 ) )); // ��ư�� ���� �ݾ� ǥ�� ������ �׸��ڻ�

	m_nMaxJokbo = 7;
	m_listJokboName.clear();
	m_listJokboName.push_back("��ž");
	m_listJokboName.push_back("�����̽�");
	m_listJokboName.push_back("���̽�");
	m_listJokboName.push_back("���̵�");
	m_listJokboName.push_back("���");
	m_listJokboName.push_back("������");
	m_listJokboName.push_back("����");

	m_listAvatarPos.clear();
	m_listAvatarPos.push_back(CPoint(352, 584));
	m_listAvatarPos.push_back(CPoint(36, 369));
	m_listAvatarPos.push_back(CPoint(36, 174));
	m_listAvatarPos.push_back(CPoint(902, 174));
	m_listAvatarPos.push_back(CPoint(902, 369));

	m_ptJokboHelpPercent = CPoint(20,536);
	m_nJokboHelpHightGab = 28;
	m_rtJokboHelpPercent = CRect(0,0,62,28);

	m_listUserMoneyRect[0] = CRect(487, 668, 487+110, 668+15);

	m_listButtonPos[EBT_SEATKEEPER] = CPoint(444, 710); // �ڸ���Ŵ�� ��ư ��ġ
	m_listButtonPos[EBT_ONESHOTCHARGE] = CPoint(505, 710); // �������� ��ư ��ġ
	m_listButtonPos[EBT_CARD_OPEN] = CPoint(551, 710); // OPEN ��ư ��ġ
	m_listButtonPos[EBT_SINOUT] = CPoint(589, 709); // ���� ��ư ��ġ
	m_listButtonPos[EBT_BOTTOM_EXIT] = CPoint(628, 709); // ������ ��ư ��ġ
	m_listButtonPos[EBT_WAITUSER] = CPoint(668, 710); // ��� ��ư ��ġ

	m_rtResultJokboYouSize			= CRect(0,0,170,20);
	m_rtResultMoneyYouSize			= CRect(0,0,163,19);

	m_rtResultJokboMySize			= CRect(0,0,170,20);
	m_rtResultMoneyMySize			= CRect(0,0,163,19);

	m_ptGold						= CRect(503, 689, 503+94, 689+11);
}
}