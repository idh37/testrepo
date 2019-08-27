#include "StdAfx.h"
#include "GameHelperManHighLow.h"
#include "GameHelper/JokboProbabilityHighLow.h"
#include "UIInfoDataHighLow.h"


#define  INDEX_LOW_START (en_Max) //하이로우 

CGameHelperManHighLow::CGameHelperManHighLow(void)
{
	m_nMyCurJokboIndex_low=-1;
	m_bOnlySwing=FALSE;
}

CGameHelperManHighLow::~CGameHelperManHighLow(void)
{
}

void CGameHelperManHighLow::Init(CUIInfoData *pUIInfoData, CGame *pGame, int nMaxRule, COtherUserJokboCheckMan *pOtherUserJokboCheckMan, CJokboProbability *pJokboProbability)
{
	CGameHelperMan::Init(pUIInfoData, pGame, nMaxRule, pOtherUserJokboCheckMan, pJokboProbability);

	
	//하이로우 족보 갯수 (하이 12개(하위인덱스), 로우 10개(상위인덱스))	
	const int WIDTH_PERCENT_RECT=56;
	const int HEIGHT_PERCENT_RECT=15;

	//탑 ~ 풀하우스 확률 렉트
	for(int i = 0 ; i < 9 ; i++)
	{		
		m_plistProbabilityRect[i].SetRect(0,0, WIDTH_PERCENT_RECT,HEIGHT_PERCENT_RECT) ;
		m_plistProbabilityRect[i].OffsetRect(111, 710 - (i*HEIGHT_PERCENT_RECT) );
	}

	//포카드 확률 렉트
	m_plistProbabilityRect[9].SetRect(0,0, WIDTH_PERCENT_RECT, 45);
	m_plistProbabilityRect[9].OffsetRect(17, 704-16);

	//스티플 확률 렉트
	m_plistProbabilityRect[10].SetRect(0,0, WIDTH_PERCENT_RECT, 45);
	m_plistProbabilityRect[10].OffsetRect(17, 655-16);

	//로티플 확률 렉트
	m_plistProbabilityRect[11].SetRect(0,0, WIDTH_PERCENT_RECT, 45);
	m_plistProbabilityRect[11].OffsetRect(17, 608-16);


	//로우 무탑 ~ 5탑
	int cnt =0;
	for(int i = 12 ; i < nMaxRule ; i++, cnt++)
	{		
		m_plistProbabilityRect[i].SetRect(0,0, WIDTH_PERCENT_RECT, HEIGHT_PERCENT_RECT);
		m_plistProbabilityRect[i].OffsetRect(210, 725 - (cnt*HEIGHT_PERCENT_RECT) );
	}


}


void CGameHelperManHighLow::DrawProbability( CDC *pDC , NMBASE::GRAPHICGDI::CPage *pPage )
{
	if( pDC == NULL || pPage == NULL )return;	

	bool bDrawProbability = (m_pGameProcess->GetPlayer(0)->PlayState == 1)?true:false;
	int index = m_nMyCurJokboIndex;
	if(bDrawProbability == false) index = -1;

	//족보 확률 표시
	CString str;
	CRect rt;

	int nCompleteIndex = 12;
	if (m_bOnlySwing) //포카드 이상
		nCompleteIndex = 14;	//포카드 이상 족보 박스 (좀더 큰박스)
	else
		nCompleteIndex = 15; //하이 족보 박스 



	for( int i= 0 ; i < m_nMaxRule ; i ++ )
	{
		TYPE_HL_PROB_BOX eTypeHLProbBox = GetProbBoxType(i);
		if (eTypeHLProbBox==TYPE_HL_PROB_BOX_NULL)
			continue;

		str.Empty();

		pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
		SetTextCharacterExtra(*pDC, -1);

		rt = m_plistProbabilityRect[i];

// 		if( i <= index )
// 		{
// 			if(i == index) 
// 			{
// 				pPage->PutSprAuto(m_plistProbabilityRect[i].left + m_pUIInfoData->m_ptJokboHelpCompleteGab.x,m_plistProbabilityRect[i].top + m_pUIInfoData->m_ptJokboHelpCompleteGab.y, &m_sprOnOff, nCompleteIndex, AB);
// 				pDC->SetTextColor(m_pUIInfoData->m_clrJokboHelpMyJokboName);
// 			}
// 			else
// 			{
// 				pDC->SetTextColor(m_pUIInfoData->m_clrJokboHelpNoPossibilityName);
// 			}
// 			pDC->DrawText(m_pUIInfoData->GetJokboName(i),&rt, DT_VCENTER | DT_CENTER | DT_WORDBREAK | DT_SINGLELINE);
// 
// 			SetTextCharacterExtra(*pDC, 0);
// 
// 			pDC->SetTextColor(m_pUIInfoData->m_clrJokboHelpPercent);
// 			pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
// 
// 
// 			continue;
// 		}

		if(i == index) 
		{
			pPage->PutSprAuto(m_plistProbabilityRect[i].left + m_pUIInfoData->m_ptJokboHelpCompleteGab.x,m_plistProbabilityRect[i].top + m_pUIInfoData->m_ptJokboHelpCompleteGab.y, &m_sprOnOff, nCompleteIndex, AB);
			//pDC->SetTextColor(m_pUIInfoData->m_clrJokboHelpMyJokboName);
		}
// 		else
// 		{
// 			//pDC->SetTextColor(m_pUIInfoData->m_clrJokboHelpNoPossibilityName);
// 		}

		if (m_bOnlySwing)//포카드 이상일때 로우 확률을 보여줄 필요가없다.
		{
			if ( eTypeHLProbBox==TYPE_HL_PROB_BOX_LOW )
				continue;
		}

		if(bDrawProbability && m_nStatus == EGHS_USEITEM_ON)
		{
			float pro = (float)m_pJokboProbability->GetProbability(i);
			if(pro < 0.1f && pro>0.0f) pro = 0.1f;

// 			if( pro <= 0.00f )
// 			{
// 				pDC->SetTextColor(m_pUIInfoData->m_clrJokboHelpNoPossibilityName);
// 				pDC->DrawText(m_pUIInfoData->GetJokboName(i),&rt, DT_VCENTER | DT_CENTER | DT_WORDBREAK | DT_SINGLELINE);
// 				continue;
// 			}
// 			else
// 			{
// 				if(pro < 0.1) pro = 0.1f;
// 				pDC->SetTextColor(m_pUIInfoData->m_clrJokboHelpPossibilityName);
// 				pDC->DrawText(m_pUIInfoData->GetJokboName(i),&rt, DT_VCENTER | DT_CENTER | DT_WORDBREAK | DT_SINGLELINE);
// 			}

			SetTextCharacterExtra(*pDC, 0);

			pDC->SetTextColor(m_pUIInfoData->m_clrJokboHelpPercent);
			pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));

			
			CPoint ptOffset(0,0);

			//로우족보
			if ( eTypeHLProbBox==TYPE_HL_PROB_BOX_LOW )
			{
				//로우족보
				rt.OffsetRect(((CUIInfoDataHighLow*)m_pUIInfoData)->m_ptLowJokboHelpPercentGab);

				int nLowJokboIndex= m_nMyCurJokboIndex_low + en_Max;
				if (i > nLowJokboIndex)
				{
					if (pro>0.0f)
						str.Format("%0.1f%%",pro);
				}
				else
					str.Format("-");

			}
			else if ( eTypeHLProbBox==TYPE_HL_PROB_BOX_SWING )
			{
				//스윙족보
				rt.OffsetRect(((CUIInfoDataHighLow*)m_pUIInfoData)->m_ptSwingJokboHelpPercentGab);

				if (i > index)
				{
					if (pro>0.0f)
						str.Format("%0.1f%%",pro);
				}
				else
					str.Format("-");

			}
			else
			{
				//하이족보
				rt.OffsetRect(m_pUIInfoData->m_ptJokboHelpPercentGab);

				if (i > m_nMyCurJokboIndex)
				{
					if (pro>0.0f)
						str.Format("%0.1f%%",pro);
				}
				else
					str.Format("-");
			}		

			
			pDC->DrawText(str,&rt, DT_VCENTER | DT_RIGHT | DT_WORDBREAK | DT_SINGLELINE);
		}
		else
		{
			pDC->SetTextColor(m_pUIInfoData->m_clrJokboHelpNoPossibilityName);
			pDC->DrawText(m_pUIInfoData->GetJokboName(i),&rt, DT_VCENTER | DT_CENTER | DT_WORDBREAK | DT_SINGLELINE);
		}

	}

	pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
	SetTextCharacterExtra(*pDC, 0);
	pDC->SetTextColor(RGB(150,150,150));

	

	//로우 족보릿트에서 내로우족보 박스 쳐주기
	if (m_bOnlySwing==FALSE && m_nMyCurJokboIndex_low>=en_Low_None && m_nMyCurJokboIndex_low<en_Low_Max)
	{	
		int nJokboIndex= m_nMyCurJokboIndex_low + en_Max;
		pPage->PutSprAuto(m_plistProbabilityRect[nJokboIndex].left + m_pUIInfoData->m_ptJokboHelpCompleteGab.x ,m_plistProbabilityRect[nJokboIndex].top + m_pUIInfoData->m_ptJokboHelpCompleteGab.y, &m_sprOnOff, 16, AB);
	}


	if(m_nInfo > -1)
	{
		pPage->PutSprAuto(m_pUIInfoData->m_ptJokboInfo.x, m_pUIInfoData->m_ptJokboInfo.y, &m_sprInfo, m_nInfo);
	}


	
}
void CGameHelperManHighLow::CalculationMyJokbo()
{
	CGameHelperMan::CalculationMyJokbo();

	if(g_bMyObserver) 
		return;
	if (m_pJokboProbability==NULL) 
		return;

	//포카드 이상인경우 로우 족보는 없다.
	m_bOnlySwing= (m_nMyCurJokboIndex >= en_FourCard);

	m_nMyCurJokboIndex_low = ((CJokboProbabilityHighLow*)m_pJokboProbability)->GetMyCurJokboIndex_low();	
	 
}

void CGameHelperManHighLow::Reset()
{
	CGameHelperMan::Reset();
	
	m_nMyCurJokboIndex_low = -1;
	m_bOnlySwing=FALSE;
	
}


TYPE_HL_PROB_BOX CGameHelperManHighLow::GetProbBoxType(int nIndexRule)
{
	//인덱스로 어떤 종료의 확률 박스인지 알아온다.

	if (nIndexRule >= INDEX_LOW_START )
		return TYPE_HL_PROB_BOX_LOW;
	else if (nIndexRule >= en_FourCard)
		return TYPE_HL_PROB_BOX_SWING;
	else  if (nIndexRule >=0)
		return TYPE_HL_PROB_BOX_HIGH;

	return TYPE_HL_PROB_BOX_NULL;
}
