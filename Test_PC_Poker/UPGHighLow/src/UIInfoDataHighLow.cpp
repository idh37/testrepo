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

	m_listButtonPos[EBT_SEATKEEPER] = CPoint(431, 710); // 자리지킴이 버튼 위치
	m_listButtonPos[EBT_ONESHOTCHARGE] = CPoint(492, 710); // 원샷충전 버튼 위치
	m_listButtonPos[EBT_CARD_OPEN] = CPoint(538, 710); // OPEN 버튼 위치
	m_listButtonPos[EBT_SINOUT] = CPoint(576, 709); // 관전 버튼 위치
	m_listButtonPos[EBT_BOTTOM_EXIT] = CPoint(616, 709); // 나가기 버튼 위치
	m_listButtonPos[EBT_WAITUSER] = CPoint(657, 710); // 대기 버튼 위치

	m_rtResultJokboYouSize			= CRect(0,0,194,20);
	m_rtResultMoneyYouSize			= CRect(0,0,187,19);

	m_rtResultJokboMySize			= CRect(0,0,200,20);
	m_rtResultMoneyMySize			= CRect(0,0,193,19);


	//전광판 
	m_clrDisplayNormal=RGB(142,63,66);				//전광판의 일반 텍스트 색
	m_clrDisplayJokbo=RGB(161,161,161);				//전광판의 족보 색
	m_clrDisplayMoney=RGB(156, 157, 81);				//전광판의 머니 색

	

	m_clrBtnBetText=RGB(231, 143, 143);				//버튼의 베팅 금액 표시 글자색
	m_clrBtnBetTextShadow=RGB(62, 27, 23);			//버튼의 베팅 금액 표시 글자의 그림자색


	m_clrBettingMoneyText=RGB(240, 189, 189);		//이전 베팅 금액 표시 색
	m_clrBettingMoneyOutline=RGB(78, 33, 44);		//이전 베팅 금액 표시 외곽선 색

	//내 족보명 표시 영역
	m_rtJokoName				= CRect(384,521,384+188,521+20);	//하이족보명
	m_rtLowJokoName				= CRect(582,521,582+100,521+20);	//로우족보명
		
	
	
	m_clrJokboNameColor			= RGB(174, 105, 118);				//하이 족보 컬러
	m_clrLowJokboNameColor		= RGB(51, 123, 146);				//로우 족보 컬러
	m_clrSwingJokboNameColor	= RGB(72, 118, 51);					//스윙 족보 컬러

	m_clrJokboNameShadowColor	= RGB(78,33,44);					//족보 텍스트 그림자

	//스페셜족보(포카드이상) 이펙트 표시시 아이디 색깔
	m_clrHighJokboUserName				= RGB(180,48,48);




	////////////////////////////////////////////////////////////////////////////
	//채팅창역역 족보 표시 부분
	m_nMaxJokbo = 22;		//하이로우 족보 갯수 (하이 12개 , 로우 10개)
	m_ptJokboHelpPercentGab =  CPoint(34,-2); 			//하이족보 퍼센트 표시 오프셋
	m_ptLowJokboHelpPercentGab	= CPoint(18,-2);		//로우족보 퍼센트 표시 오프셋
	m_ptSwingJokboHelpPercentGab = CPoint(18,-2);		//스윙 족보퍼센트 표시 오프셋
		
	//하이로우 족보명 리스트 세팅
	m_listJokboName.clear();
	for(int i=0;  i<m_nMaxJokbo; i++)
	{	
		m_listJokboName.push_back("");	///빈 텍스트를 집어 넣는다 .(족보명은 이미 이미지에 박혀있기 때문)
	}	


	//채팅창영역 게임도우미 
	m_listButtonPos[EBT_JOKBOHELP_ON] = CPoint( 133, 541);	
	m_listButtonPos[EBT_JOKBOHELP_OFF] = CPoint( 190, 541);	
	m_listButtonPos[EBT_JOKBOHELP_USE] = m_listButtonPos[EBT_JOKBOHELP_ON];



	//하이/로우/스윙 버튼 위치 설정
	m_mapHIghLowBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HIGHLOW, CPoint>(EBT_HILO_HIGH, CPoint(715, 547)));
	m_mapHIghLowBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HIGHLOW, CPoint>(EBT_HILO_LOW, CPoint(805, 547)));
	m_mapHIghLowBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HIGHLOW, CPoint>(EBT_HILO_SWING, CPoint(895, 547)));

	m_ptGold = CRect(470, 689, 470+110, 689+12);

}


CPoint CUIInfoDataHighLow::GetHighLowBtnPos(ENUM_BUTTON_TYPE_HIGHLOW eHighlowPos)
{	
	if (m_mapHIghLowBtnPos.find(eHighlowPos) == m_mapHIghLowBtnPos.end())
	{
		ASSERT(0); //정의 되지않은 버튼
		return CPoint(-1000, 1000);
	}

	return m_mapHIghLowBtnPos[eHighlowPos];	
}

