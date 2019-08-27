#include "StdAfx.h"
#include "PromotionSuperMoon.h"
#include "PromotionManager.h"

#include "../Sound.h"


#pragma 

CPromotionSuperMoon::CPromotionSuperMoon( CPromotionManager *pManager, int nType )
: CPromotionBase(pManager, nType)
{
	SetShowTab(true);
	string strDir = ".\\PokerCommon\\data\\Event\\SuperMoon\\";
	_LOADSPRITEDEFINE(&m_sprBack, strDir, "Evt_moon.spr");
	m_cEffect_Moon.SetSprite(&m_sprBack);
	m_bDrawTip=false;
	m_level = 2;
	m_count = 5;
	m_rewardCnt = 10;
	m_isEnd = false;
}

CPromotionSuperMoon::~CPromotionSuperMoon(void)
{
}

void CPromotionSuperMoon::SetEventInfo( CSV_PLAYINFO_SUPERMOON *pInfo )
{
	if (pInfo==NULL) 
		return;
	
	m_level = *pInfo->m_CurrentLevel;
	m_count = *pInfo->m_CurrentCount;
	m_rewardCnt = *pInfo->m_GoalCount;

	GM().GetPromotionData()->nStep = m_level;
	GM().GetPromotionData()->nCnt = m_count;

	GM().GetPromotionData()->nCnts[0] = m_rewardCnt;		// 보상 판수				
}

void CPromotionSuperMoon::Accept_OutRoom()
{
}

void CPromotionSuperMoon::Accept_CreateRoom()
{
}

void CPromotionSuperMoon::Accept_EnterRoom()
{
}

void CPromotionSuperMoon::OnEnterRoom()
{
}

void CPromotionSuperMoon::OnEndGame()
{
}

void CPromotionSuperMoon::DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
}

void CPromotionSuperMoon::DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
}

void CPromotionSuperMoon::DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	DWORD dwNow = timeGetTime();
	int countPer = 0;
	if (m_rewardCnt>0)
		countPer = (m_count * 9) / m_rewardCnt;;

	if(IDX_GAME_SD == GM().GetCurrentGameType()){
		if(!m_isEnd){
			m_sprBack.multiscene.ppMScene[3]->ppTrack[1]->GetDataScene()->SceneNo = m_level;
			m_sprBack.multiscene.ppMScene[3]->ppTrack[2]->GetDataScene()->SceneNo = 5+countPer;
			pPage->DrawMultiScene(0, 0, &m_sprBack, 3, dwNow, TRUE);
			TextSetting(pDC, 347, 94);
		}
		else{
			pPage->DrawMultiScene(0, 0, &m_sprBack, 5, dwNow, TRUE);
		}
		if(true==m_bDrawTip){
			pPage->DrawMultiScene(0, 0, &m_sprBack, 4, dwNow, TRUE);		
		}
	}else if(IDX_GAME_HA == GM().GetCurrentGameType()){
		if(!m_isEnd){
			m_sprBack.multiscene.ppMScene[6]->ppTrack[1]->GetDataScene()->SceneNo = m_level;
			m_sprBack.multiscene.ppMScene[6]->ppTrack[2]->GetDataScene()->SceneNo = 5+countPer;
			pPage->DrawMultiScene(0, 0, &m_sprBack, 6, dwNow, TRUE);
			TextSetting(pDC, 299, 80);
		}
		else{
			pPage->DrawMultiScene(0, 0, &m_sprBack, 8, dwNow, TRUE);
		}
		if(true==m_bDrawTip){
			pPage->DrawMultiScene(0, 0, &m_sprBack, 7, dwNow, TRUE);		
		}
	}else{
		if(!m_isEnd){
			m_sprBack.multiscene.ppMScene[0]->ppTrack[1]->GetDataScene()->SceneNo = m_level;
			m_sprBack.multiscene.ppMScene[0]->ppTrack[2]->GetDataScene()->SceneNo = 5+countPer;
			pPage->DrawMultiScene(0, 0, &m_sprBack, 0, dwNow, TRUE);
			TextSetting(pDC, 215, 491);
		}
		else{
			pPage->DrawMultiScene(0, 0, &m_sprBack, 2, dwNow, TRUE);
		}
		if(true==m_bDrawTip){
			pPage->DrawMultiScene(0, 0, &m_sprBack, 1, dwNow, TRUE);		
		}
	}
}

BOOL CPromotionSuperMoon::OnLButtonDownMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionSuperMoon::OnLButtonUpMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionSuperMoon::OnMouseMoveMiddle(int &x , int &y)
{
	CRect crArea=CRect(11, 468, 11+282, 468+44);
	if(IDX_GAME_HA == GM().GetCurrentGameType()){
		crArea=CRect(95, 57, 95+282, 57+44);
	}
	if(IDX_GAME_SD == GM().GetCurrentGameType()){
		crArea=CRect(143, 71, 143+282, 71+44);
	}
	m_bDrawTip=false;
	if(TRUE==crArea.PtInRect(CPoint(x, y))){
		m_bDrawTip=true;
	}
	return FALSE;
}

void CPromotionSuperMoon::DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart)
{
	int backIdx = 76;
	pPage->PutSprAuto(ptStart.x, ptStart.y, &m_sprBack, backIdx);	
	if(m_level>1)	pPage->PutSprAuto(ptStart.x+1, ptStart.y, &m_sprBack, GM().GetPromotionData()->nCnts[1]==0?82:86);
	if(m_level>2)	pPage->PutSprAuto(ptStart.x+71, ptStart.y, &m_sprBack, GM().GetPromotionData()->nCnts[2]==0?83:87);
	if(m_level>3)	pPage->PutSprAuto(ptStart.x+141, ptStart.y, &m_sprBack, GM().GetPromotionData()->nCnts[3]==0?84:88);
	if(m_level==4 && m_count==m_rewardCnt)
	{
		pPage->PutSprAuto(ptStart.x+211, ptStart.y, &m_sprBack, GM().GetPromotionData()->nCnts[4]==0?85:89);
		pPage->PutSprAuto(ptStart.x, ptStart.y+41, &m_sprBack, 81);
	}
	else
	{
		pPage->PutSprAuto(ptStart.x+1+70*(m_level-1), ptStart.y, &m_sprBack, backIdx+13+m_level);
		pPage->PutSprAuto(ptStart.x, ptStart.y+41, &m_sprBack, backIdx+m_level);
	}
}

BOOL CPromotionSuperMoon::OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData)
{
	switch(Signal)
	{
	case OPCODE_SV_SUPERMOON_ALARM:
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::SUPERMOON_PROMOTION_ALARM > Packet;
				Packet.Get(lpData, TotSize );
				promotion::SUPERMOON_PROMOTION_ALARM* pAlarm=static_cast< promotion::SUPERMOON_PROMOTION_ALARM* >(Packet.Various() );
				m_level = pAlarm->CurrentLevel;
				m_count = pAlarm->ThisLevel_CurrentCount;
				m_rewardCnt = pAlarm->ThisLevel_GoalCount;

				GM().GetPromotionData()->nStep = m_level;
				GM().GetPromotionData()->nCnt = m_count;

				GM().GetPromotionData()->nCnts[0] = m_rewardCnt;		// 보상 미션
				m_isEnd = true;
				for(int i = 0; i < 4; i++)
				{
					m_giftOpen[i] = pAlarm->GiftOpen[i];
					if(m_giftOpen[i]==false)
						m_isEnd = false;

					GM().GetPromotionData()->nCnts[i+1] = m_giftOpen[i] ? 1:0;	
				}

				if(m_isEnd){
					GM().GetPromotionData()->SetState(tPromotionData::PROMOTIONSTATE_END);
				}
				else{
					GM().GetPromotionData()->SetState(tPromotionData::PROMOTIONSTATE_ING);
				}

				GM().SetShowEndEventPopup(true);
			}
		
		}
		break;
	case SV_PLAYINFO_SUPERMOON:
		{	
			CSV_PLAYINFO_SUPERMOON msg;
			msg.Get( lpData, TotSize );
			SetEventInfo(&msg);
		}
		break;
	}

	return FALSE;
}

void CPromotionSuperMoon::TextSetting(CDC *pDC, int x, int y)
{
	CString strOutput;
	CFont *pOldFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_14_BOLD));
	COLORREF temp = pDC->GetTextColor();
	UINT oldFlags = pDC->SetTextAlign(TA_RIGHT);
	pDC->SetTextColor( RGB(255, 248, 207) );
	strOutput.Format("%d", m_count);
	pDC->TextOut(x, y, strOutput);
	pDC->SetTextColor( RGB(239, 220, 109) );
	strOutput.Format("/%d", m_rewardCnt);
	pDC->TextOut(x+28, y, strOutput);
	pDC->SetTextColor( temp );
	pDC->SelectObject(pOldFont);
	pDC->SetTextAlign(oldFlags);
}