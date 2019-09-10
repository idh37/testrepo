// SprMoneyDraw.cpp: implementation of the CSprMoneyDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UIInfoDataHighLow.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CUIInfoDataHighLow::CUIInfoDataHighLow()
{
	Init();
}

CUIInfoDataHighLow::~CUIInfoDataHighLow()
{
}

void CUIInfoDataHighLow::Init(void)
{

	CUIInfoData::Init();


	m_clrBtnBetText = RGB(108, 170, 255);
	m_clrBtnBetTextShadow = RGB(18, 57, 88);

	m_listCardStartPos.clear();
	m_listCardStartPos.push_back(CPoint(454, 548));
	m_listCardStartPos.push_back(CPoint(140, 331));
	m_listCardStartPos.push_back(CPoint(140, 136));
	m_listCardStartPos.push_back(CPoint(667, 136));
	m_listCardStartPos.push_back(CPoint(667, 331));

	m_listAvatarPos.clear();
	m_listAvatarPos.push_back(CPoint(354, 584));
	m_listAvatarPos.push_back(CPoint(36, 369));
	m_listAvatarPos.push_back(CPoint(36, 174));
	m_listAvatarPos.push_back(CPoint(902, 174));
	m_listAvatarPos.push_back(CPoint(902, 369));


	m_listUserMoneyRect[0] = CRect(454, 668, 454+115, 668+15);

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


	//������ 
	m_clrDisplayNormal=RGB(142,63,66);				//�������� �Ϲ� �ؽ�Ʈ ��
	m_clrDisplayJokbo=RGB(161,161,161);				//�������� ���� ��
	m_clrDisplayMoney=RGB(156, 157, 81);				//�������� �Ӵ� ��

	

	m_clrBtnBetText=RGB(231, 143, 143);				//��ư�� ���� �ݾ� ǥ�� ���ڻ�
	m_clrBtnBetTextShadow=RGB(62, 27, 23);			//��ư�� ���� �ݾ� ǥ�� ������ �׸��ڻ�


	m_clrBettingMoneyText=RGB(240, 189, 189);		//���� ���� �ݾ� ǥ�� ��
	m_clrBettingMoneyOutline=RGB(78, 33, 44);		//���� ���� �ݾ� ǥ�� �ܰ��� ��

	//�� ������ ǥ�� ����
	m_rtJokoName				= CRect(384,521,384+188,521+20);	//����������
	m_rtLowJokoName				= CRect(582,521,582+100,521+20);	//�ο�������
		
	
	
	m_clrJokboNameColor			= RGB(174, 105, 118);				//���� ���� �÷�
	m_clrLowJokboNameColor		= RGB(51, 123, 146);				//�ο� ���� �÷�
	m_clrSwingJokboNameColor	= RGB(72, 118, 51);					//���� ���� �÷�

	m_clrJokboNameShadowColor	= RGB(78,33,44);					//���� �ؽ�Ʈ �׸���

	//���������(��ī���̻�) ����Ʈ ǥ�ý� ���̵� ����
	m_clrHighJokboUserName				= RGB(180,48,48);




	////////////////////////////////////////////////////////////////////////////
	//ä��â���� ���� ǥ�� �κ�
	m_nMaxJokbo = 22;		//���̷ο� ���� ���� (���� 12�� , �ο� 10��)
	m_ptJokboHelpPercentGab =  CPoint(34,-2); 			//�������� �ۼ�Ʈ ǥ�� ������
	m_ptLowJokboHelpPercentGab	= CPoint(18,-2);		//�ο����� �ۼ�Ʈ ǥ�� ������
	m_ptSwingJokboHelpPercentGab = CPoint(18,-2);		//���� �����ۼ�Ʈ ǥ�� ������
		
	//���̷ο� ������ ����Ʈ ����
	m_listJokboName.clear();
	for(int i=0;  i<m_nMaxJokbo; i++)
	{	
		m_listJokboName.push_back("");	///�� �ؽ�Ʈ�� ���� �ִ´� .(�������� �̹� �̹����� �����ֱ� ����)
	}	


	//ä��â���� ���ӵ���� 
	m_listButtonPos[EBT_JOKBOHELP_ON] = CPoint( 133, 541);	
	m_listButtonPos[EBT_JOKBOHELP_OFF] = CPoint( 190, 541);	
	m_listButtonPos[EBT_JOKBOHELP_USE] = m_listButtonPos[EBT_JOKBOHELP_ON];



	//����/�ο�/���� ��ư ��ġ ����
	m_mapHIghLowBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HIGHLOW, CPoint>(EBT_HILO_HIGH, CPoint(715, 547)));
	m_mapHIghLowBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HIGHLOW, CPoint>(EBT_HILO_LOW, CPoint(805, 547)));
	m_mapHIghLowBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HIGHLOW, CPoint>(EBT_HILO_SWING, CPoint(895, 547)));

	m_ptGold = CRect(470, 689, 470+110, 689+12);

}


CPoint CUIInfoDataHighLow::GetHighLowBtnPos(ENUM_BUTTON_TYPE_HIGHLOW eHighlowPos)
{	
	if (m_mapHIghLowBtnPos.find(eHighlowPos) == m_mapHIghLowBtnPos.end())
	{
		ASSERT(0); //���� �������� ��ư
		return CPoint(-1000, 1000);
	}

	return m_mapHIghLowBtnPos[eHighlowPos];	
}

