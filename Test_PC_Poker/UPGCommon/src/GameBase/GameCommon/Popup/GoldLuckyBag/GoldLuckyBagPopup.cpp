#include "StdAfx.h"
#include "GoldLuckyBagPopup.h"
#include "goldluckybagpopupid.h"

CGoldLuckyBagPopup::CGoldLuckyBagPopup(CWnd* pParent/* = NULL*/) : LSY::CLSYLibDialogBase(pParent)
{
}

CGoldLuckyBagPopup::~CGoldLuckyBagPopup(void)
{
}

INT_PTR CGoldLuckyBagPopup::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_GOLDLUCKYBAGPOPUP);
}

bool CGoldLuckyBagPopup::OnCreatedProject(void)
{
	GM().SetRefreshPopup(true);
	// LSY Control 연결
	InitLSYControl();
	// 데이터 세팅
	InitData();
	
	return TRUE;
}

/// <summary>
/// 버튼 클릭 이벤트. 대부분 서버에 요청 보내는 기능.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CGoldLuckyBagPopup::OnClickButton(LSY::CMessage *pMsg)
{
	// 골드 150만개
	int nGiftIdx = 0;
	char* cGiftName = "";
	bool bEnter = false;
	
	TICKET_GIFT_INFO *ArrayTicketGiftList_gold = GM().GetTicketGiftList();
	
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_GOLDLUCKYBAGPOPUP_BTN_CLOSE: // 닫기 버튼
		{
			CloseDlg();
		}
		break;
	case ID_GOLDLUCKYBAGPOPUP_BTN_ENTER1: // 응모하기 버튼
		{
			// 골드 150만개
			nGiftIdx = ArrayTicketGiftList_gold[0].GiftIndex;
			cGiftName = ArrayTicketGiftList_gold[0].GiftName;
			bEnter = true;
		}
		break;
	case ID_GOLDLUCKYBAGPOPUP_BTN_ENTER2: // 응모하기 버튼
		{
			// 머니 100조
			nGiftIdx = ArrayTicketGiftList_gold[1].GiftIndex;
			cGiftName = ArrayTicketGiftList_gold[1].GiftName;
			bEnter = true;
		}
		break;
	case ID_GOLDLUCKYBAGPOPUP_BTN_ENTER3: // 응모하기 버튼
		{
			// 보너스 쿠폰 50%
			nGiftIdx = ArrayTicketGiftList_gold[2].GiftIndex;
			cGiftName = ArrayTicketGiftList_gold[2].GiftName;
			bEnter = true;
		}
		break;
	case ID_GOLDLUCKYBAGPOPUP_BTN_ENTER4: // 응모하기 버튼
		{
			// 보너스 쿠폰 30%
			nGiftIdx = ArrayTicketGiftList_gold[3].GiftIndex;
			cGiftName = ArrayTicketGiftList_gold[3].GiftName;
			bEnter = true;
		}
		break;
	}
	
	// 응모 버튼 눌렀다면
	if (bEnter)
	{
		// 응모권을 사용하여 상품에 응모한다.
		CCL_PROMOTION_COMMON_TICKET_REQUEST_SUBSCRIPTION msg2;
		msg2.Set(PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_GOLDEN_LUCKYBAG, nGiftIdx);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg2.pData, msg2.GetTotalSize());
	}
	
	return TRUE;
}

/// <summary>
/// LSY Control 연결
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldLuckyBagPopup::InitLSYControl()
{
	// 닫기 버튼
	m_pBtnClose = (LSY::CButton *)GetObject(ID_GOLDLUCKYBAGPOPUP_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldLuckyBagPopup::OnClickButton));
	
	// 응모하기 버튼
	m_pBtnEnter1 = (LSY::CButton *)GetObject(ID_GOLDLUCKYBAGPOPUP_BTN_ENTER1);
	m_pBtnEnter1->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldLuckyBagPopup::OnClickButton));
	m_pBtnEnter2 = (LSY::CButton *)GetObject(ID_GOLDLUCKYBAGPOPUP_BTN_ENTER2);
	m_pBtnEnter2->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldLuckyBagPopup::OnClickButton));
	m_pBtnEnter3 = (LSY::CButton *)GetObject(ID_GOLDLUCKYBAGPOPUP_BTN_ENTER3);
	m_pBtnEnter3->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldLuckyBagPopup::OnClickButton));
	m_pBtnEnter4 = (LSY::CButton *)GetObject(ID_GOLDLUCKYBAGPOPUP_BTN_ENTER4);
	m_pBtnEnter4->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldLuckyBagPopup::OnClickButton));
	
	// 나의 황금 복주머니 갯수
	m_pImgMyBagNum1 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_MYBAGNUM1);
	m_pImgMyBagNum2 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_MYBAGNUM2);
	m_pImgMyBagNum3 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_MYBAGNUM3);
	m_pImgMyBagNum4 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_MYBAGNUM4);
	m_pImgMyBagNum5 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_MYBAGNUM5);
	
	// 골드 응모 횟수
	m_pImgEnterNum_gold1 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_GOLD1);
	m_pImgEnterNum_gold2 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_GOLD2);
	m_pImgEnterNum_gold3 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_GOLD3);
	m_pImgEnterNum_gold4 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_GOLD4);
	m_pImgEnterNum_gold5 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_GOLD5);
	m_pImgEnterNum_gold6 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_GOLD6);
	
	// 머니 응모 횟수
	m_pImgEnterNum_money1 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_MONEY1);
	m_pImgEnterNum_money2 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_MONEY2);
	m_pImgEnterNum_money3 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_MONEY3);
	m_pImgEnterNum_money4 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_MONEY4);
	m_pImgEnterNum_money5 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_MONEY5);
	m_pImgEnterNum_money6 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_MONEY6);
	
	// 보너스 쿠폰 30% 응모 횟수
	m_pImgEnterNum_301 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_301);
	m_pImgEnterNum_302 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_302);
	m_pImgEnterNum_303 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_303);
	m_pImgEnterNum_304 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_304);
	m_pImgEnterNum_305 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_305);
	m_pImgEnterNum_306 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_306);
	
	// 보너스 쿠폰 50% 응모 횟수
	m_pImgEnterNum_501 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_501);
	m_pImgEnterNum_502 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_502);
	m_pImgEnterNum_503 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_503);
	m_pImgEnterNum_504 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_504);
	m_pImgEnterNum_505 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_505);
	m_pImgEnterNum_506 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_ENTERNUM_506);
	
	// 필요한 복주머니 갯수
	m_pImgBagGold1 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_BAGGOLD);
	m_pImgBagGold2 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_BAGMONEY);
	m_pImgBagGold3 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_BAG30);
	m_pImgBagGold4 = (LSY::CImage *)GetObject(ID_GOLDLUCKYBAGPOPUP_IMG_BAG50);
}

/// <summary>
/// 데이터 세팅
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldLuckyBagPopup::InitData()
{
	// 나의 황금 복주머니 갯수
	int nMyBagNum = GM().GetMyTicketCount();
	
	if (nMyBagNum >= 0)
		m_pImgMyBagNum1->SetIndex(63 + ((nMyBagNum / 1) % 10));
	if (nMyBagNum >= 10)
	{
		m_pImgMyBagNum2->SetShow(true);
		m_pImgMyBagNum2->SetIndex(63 + ((nMyBagNum / 10) % 10));
	}
	else
		m_pImgMyBagNum2->SetShow(false);
	if (nMyBagNum >= 100)
	{
		m_pImgMyBagNum3->SetShow(true);
		m_pImgMyBagNum3->SetIndex(63 + ((nMyBagNum / 100) % 10));
	}
	else
		m_pImgMyBagNum3->SetShow(false);
	if (nMyBagNum >= 1000)
	{
		m_pImgMyBagNum4->SetShow(true);
		m_pImgMyBagNum4->SetIndex(63 + ((nMyBagNum / 1000) % 10));
	}
	else
		m_pImgMyBagNum4->SetShow(false);
	if (nMyBagNum >= 10000)
	{
		m_pImgMyBagNum5->SetShow(true);
		m_pImgMyBagNum5->SetIndex(63 + ((nMyBagNum / 10000) % 10));
	}
	else
		m_pImgMyBagNum5->SetShow(false);
	
	//// 응모 횟수
	TICKET_SUBSCRIPTION_INFO *ArraySubscriptionList_gold = GM().GetSubscriptionList();
	
	// 골드 응모 횟수
	int n = ArraySubscriptionList_gold[0].SB_Count;
	if (n >= 0)
		m_pImgEnterNum_gold1->SetIndex(78 + ((n / 1) % 10));
	if (n >= 10)
	{
		m_pImgEnterNum_gold2->SetShow(true);
		m_pImgEnterNum_gold2->SetIndex(78 + ((n / 10) % 10));
	}
	else
		m_pImgEnterNum_gold2->SetShow(false);
	if (n >= 100)
	{
		m_pImgEnterNum_gold3->SetShow(true);
		m_pImgEnterNum_gold3->SetIndex(78 + ((n / 100) % 10));
	}
	else
		m_pImgEnterNum_gold3->SetShow(false);
	if (n >= 1000)
	{
		m_pImgEnterNum_gold4->SetShow(true);
		m_pImgEnterNum_gold4->SetIndex(78 + ((n / 1000) % 10));
	}
	else
		m_pImgEnterNum_gold4->SetShow(false);
	if (n >= 10000)
	{
		m_pImgEnterNum_gold5->SetShow(true);
		m_pImgEnterNum_gold5->SetIndex(78 + ((n / 10000) % 10));
	}
	else
		m_pImgEnterNum_gold5->SetShow(false);
	if (n >= 100000)
	{
		m_pImgEnterNum_gold6->SetShow(true);
		m_pImgEnterNum_gold6->SetIndex(78 + ((n / 100000) % 10));
	}
	else
		m_pImgEnterNum_gold6->SetShow(false);
	
	// 머니 응모 횟수
	n = ArraySubscriptionList_gold[1].SB_Count;
	if (n >= 0)
		m_pImgEnterNum_money1->SetIndex(78 + ((n / 1) % 10));
	if (n >= 10)
	{
		m_pImgEnterNum_money2->SetShow(true);
		m_pImgEnterNum_money2->SetIndex(78 + ((n / 10) % 10));
	}
	else
		m_pImgEnterNum_money2->SetShow(false);
	if (n >= 100)
	{
		m_pImgEnterNum_money3->SetShow(true);
		m_pImgEnterNum_money3->SetIndex(78 + ((n / 100) % 10));
	}
	else
		m_pImgEnterNum_money3->SetShow(false);
	if (n >= 1000)
	{
		m_pImgEnterNum_money4->SetShow(true);
		m_pImgEnterNum_money4->SetIndex(78 + ((n / 1000) % 10));
	}
	else
		m_pImgEnterNum_money4->SetShow(false);
	if (n >= 10000)
	{
		m_pImgEnterNum_money5->SetShow(true);
		m_pImgEnterNum_money5->SetIndex(78 + ((n / 10000) % 10));
	}
	else
		m_pImgEnterNum_money5->SetShow(false);
	if (n >= 100000)
	{
		m_pImgEnterNum_money6->SetShow(true);
		m_pImgEnterNum_money6->SetIndex(78 + ((n / 100000) % 10));
	}
	else
		m_pImgEnterNum_money6->SetShow(false);
	
	// 보너스 쿠폰 30% 응모 횟수
	n = ArraySubscriptionList_gold[2].SB_Count;
	if (n >= 0)
		m_pImgEnterNum_301->SetIndex(78 + ((n / 1) % 10));
	if (n >= 10)
	{
		m_pImgEnterNum_302->SetShow(true);
		m_pImgEnterNum_302->SetIndex(78 + ((n / 10) % 10));
	}
	else
		m_pImgEnterNum_302->SetShow(false);
	if (n >= 100)
	{
		m_pImgEnterNum_303->SetShow(true);
		m_pImgEnterNum_303->SetIndex(78 + ((n / 100) % 10));
	}
	else
		m_pImgEnterNum_303->SetShow(false);
	if (n >= 1000)
	{
		m_pImgEnterNum_304->SetShow(true);
		m_pImgEnterNum_304->SetIndex(78 + ((n / 1000) % 10));
	}
	else
		m_pImgEnterNum_304->SetShow(false);
	if (n >= 10000)
	{
		m_pImgEnterNum_305->SetShow(true);
		m_pImgEnterNum_305->SetIndex(78 + ((n / 10000) % 10));
	}
	else
		m_pImgEnterNum_305->SetShow(false);
	if (n >= 100000)
	{
		m_pImgEnterNum_306->SetShow(true);
		m_pImgEnterNum_306->SetIndex(78 + ((n / 100000) % 10));
	}
	else
		m_pImgEnterNum_306->SetShow(false);
	
	// 보너스 쿠폰 50% 응모 횟수
	n = ArraySubscriptionList_gold[3].SB_Count;
	if (n >= 0)
		m_pImgEnterNum_501->SetIndex(78 + ((n / 1) % 10));
	if (n >= 10)
	{
		m_pImgEnterNum_502->SetShow(true);
		m_pImgEnterNum_502->SetIndex(78 + ((n / 10) % 10));
	}
	else
		m_pImgEnterNum_502->SetShow(false);
	if (n >= 100)
	{
		m_pImgEnterNum_503->SetShow(true);
		m_pImgEnterNum_503->SetIndex(78 + ((n / 100) % 10));
	}
	else
		m_pImgEnterNum_503->SetShow(false);
	if (n >= 1000)
	{
		m_pImgEnterNum_504->SetShow(true);
		m_pImgEnterNum_504->SetIndex(78 + ((n / 1000) % 10));
	}
	else
		m_pImgEnterNum_504->SetShow(false);
	if (n >= 10000)
	{
		m_pImgEnterNum_505->SetShow(true);
		m_pImgEnterNum_505->SetIndex(78 + ((n / 10000) % 10));
	}
	else
		m_pImgEnterNum_505->SetShow(false);
	if (n >= 100000)
	{
		m_pImgEnterNum_506->SetShow(true);
		m_pImgEnterNum_506->SetIndex(78 + ((n / 100000) % 10));
	}
	else
		m_pImgEnterNum_506->SetShow(false);
	
	// 필요한 복주머니 갯수
	TICKET_GIFT_INFO *ArrayTicketGiftList_gold = GM().GetTicketGiftList();
	
	m_pImgBagGold1->SetIndex(72 + ArrayTicketGiftList_gold[0].NeedTicketCount);
	m_pImgBagGold2->SetIndex(72 + ArrayTicketGiftList_gold[1].NeedTicketCount);
	m_pImgBagGold3->SetIndex(72 + ArrayTicketGiftList_gold[2].NeedTicketCount);
	m_pImgBagGold4->SetIndex(72 + ArrayTicketGiftList_gold[3].NeedTicketCount);
}

void CGoldLuckyBagPopup::CloseDlg(void)
{
	GM().SetRefreshPopup(false);
	OnCancel();
}

/// <summary>
/// 서버에서 보내는 패킷(응답) 처리
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldLuckyBagPopup::HandleResponse(int Signal, int TotSize, char *lpData) // 서버 응답 핸들링
{
	switch(Signal)
	{
	case SV_PROMOTION_COMMON_TICKET_RESPONSE_SUBSCRIPTION_LIST:
		{
			//해당 유저의 티켓 응모 현황
			CSV_PROMOTION_COMMON_TICKET_RESPONSE_SUBSCRIPTION_LIST msg;
			msg.Get(lpData, TotSize);
			
			if (GM().GetRefreshPopup() && (GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME))
				InitData();
		}
		break;
	}
}