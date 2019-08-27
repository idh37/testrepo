// GameURL.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameURL.h"

CString getGameURL(IDX_GAMEURL nGameURL, LPARAM lParam /* = NULL */)
{
	CString strGameCode = GM().GetCurrentGameString();
	int nGameCode = GM().GetCurrentGameCode();
	int nGameIndex = GM().GetCurrentGameType();
	CString strRet = "";

	switch(nGameURL)
	{
	case IDX_GAMEURL_CHANCEAVATAR:
		{
			int nGroupCode = (int)lParam;
			strRet.Format("http://game1.netmarble.net/gameshop/client/chanceavatar/chanceavatar.asp?game=%s&groupcode=%d&client=Y", strGameCode.GetString(), nGroupCode);
		}
		break;
	case IDX_GAMEURL_ROULETTE:
		strRet.Format("http://game1.netmarble.net/sub/Roulette/?game=%s",strGameCode.GetString());
		break;
	case IDX_GAMEURL_KISA:
		strRet.Format("http://game1.netmarble.net/family/client/option/index.asp?gamecode=%d", nGameCode);
		break;
	case IDX_GAMEURL_FAMILYAVATARLIST:
		#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
			if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
				strRet.Format( "http://poker.toctocgame.com/gameShop/client/gameavatar/familyavatarlist.asp?game=%s&client=Y", strGameCode.GetString());
			else
				strRet.Format( "http://game1.netmarble.net/gameshop/client/gameavatar/familyavatarlist.asp?game=%s&client=Y", strGameCode.GetString());
		#else //UPGRADE_20120308_TOCTOC_CHANNELING
			strRet.Format( "http://game1.netmarble.net/gameshop/client/gameavatar/familyavatarlist.asp?game=%s&client=Y", strGameCode.GetString());
		#endif //UPGRADE_20120308_TOCTOC_CHANNELING
		
		break;
	case IDX_GAMEURL_GAMEAVATARLIST:
		#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
			if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
				strRet.Format( "http://poker.toctocgame.com/gameshop/client/gameavatar/gameavatarlist.asp?game=%s&client=Y", strGameCode.GetString());
			else
				strRet.Format( "http://game1.netmarble.net/gameshop/client/gameavatar/gameavatarlist.asp?game=%s&client=Y", strGameCode.GetString());
		#else //UPGRADE_20120308_TOCTOC_CHANNELING
			strRet.Format( "http://game1.netmarble.net/gameshop/client/gameavatar/gameavatarlist.asp?game=%s&client=Y", strGameCode.GetString());
		#endif //UPGRADE_20120308_TOCTOC_CHANNELING

		break;
	case IDX_GAMEURL_LOTTOAVATARLIST:
		#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
			if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
				strRet.Format( "http://poker.toctocgame.com/gameshop/client/gameitem/lottoavatarlist.asp?game=%s&client=Y", strGameCode.GetString());
			else
				strRet.Format( "http://game1.netmarble.net/gameshop/client/gameitem/lottoavatarlist.asp?game=%s&client=Y", strGameCode.GetString());
		#else //UPGRADE_20120308_TOCTOC_CHANNELING
			strRet.Format( "http://game1.netmarble.net/gameshop/client/gameitem/lottoavatarlist.asp?game=%s&client=Y", strGameCode.GetString());
		#endif //UPGRADE_20120308_TOCTOC_CHANNELING

		break;
	case IDX_GAMEURL_GAMEITEMLIST:
		#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
			if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
				strRet.Format( "http://poker.toctocgame.com/gameshop/client/gameitem/gameitemlist.asp?game=%s&client=Y", strGameCode.GetString());
			else
				strRet.Format( "http://game1.netmarble.net/gameshop/client/gameitem/gameitemlist.asp?game=%s&client=Y", strGameCode.GetString());
		#else //UPGRADE_20120308_TOCTOC_CHANNELING
			strRet.Format( "http://game1.netmarble.net/gameshop/client/gameitem/gameitemlist.asp?game=%s&client=Y", strGameCode.GetString());
		#endif //UPGRADE_20120308_TOCTOC_CHANNELING

		break;
	case IDX_GAMEURL_MYITEMLIST:
		#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
			if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
				strRet.Format( "http://poker.toctocgame.com/gameshop/client/myinfo/myitemlist.asp?game=%s&client=Y", strGameCode.GetString());
			else
				strRet.Format( "http://game1.netmarble.net/gameshop/client/myinfo/myitemlist.asp?game=%s&client=Y", strGameCode.GetString());
		#else //UPGRADE_20120308_TOCTOC_CHANNELING
			strRet.Format( "http://game1.netmarble.net/gameshop/client/myinfo/myitemlist.asp?game=%s&client=Y", strGameCode.GetString());
		#endif //UPGRADE_20120308_TOCTOC_CHANNELING

		break;
	case IDX_GAMEURL_REFILL:

		#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
			if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
			{
				strRet.Format( "http://www.toctocgame.com/M_Shop/Cash_Charge_PopUp.asp");
			}
			else
			{
				//strRet.Format( "https://nbill.netmarble.net/refill/cashFrameWin.asp?calltype=web&OPEN_CODE=poker");
				strRet.Format( "https://nbill.netmarble.net/Cash/Payment/Main.aspx?calltype=web&OPEN_CODE=poker");
			}
		#else //UPGRADE_20120308_TOCTOC_CHANNELING
			//strRet.Format( "https://nbill.netmarble.net/refill/cashFrameWin.asp?calltype=web&OPEN_CODE=poker");
			strRet.Format( "https://nbill.netmarble.net/Cash/Payment/Main.aspx?calltype=web&OPEN_CODE=poker");
		#endif //UPGRADE_20120308_TOCTOC_CHANNELING
		break;
	case IDX_GAMEURL_JACKPOT_RANK:
		strRet.Format( "http://game1.netmarble.net/sub/jackpot/Rank/maxRank.asp?gamecode=%s&chk=game", strGameCode.GetString());
		break;
	case IDX_GAMEURL_MYFAMILY:
		#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
			if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
				strRet.Format( "http://poker.toctocgame.com/family/client/myfamily/index.asp?gamecode=%d", nGameCode);
			else
				strRet.Format( "http://game1.netmarble.net/family/client/myfamily/index.asp?gamecode=%d", nGameCode);
		#else //UPGRADE_20120308_TOCTOC_CHANNELING
			strRet.Format( "http://game1.netmarble.net/family/client/myfamily/index.asp?gamecode=%d", nGameCode);
		#endif //UPGRADE_20120308_TOCTOC_CHANNELING

		break;
	case IDX_GAMEURL_FREECHARGEINSURANCE:
		#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
			if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
				strRet.Format( "http://poker.toctocgame.com/Service/Charge/FreeChargeInsurance.asp?game=%s&client=Y", strGameCode.GetString());
			else
				strRet.Format( "http://game1.netmarble.net/service/freechargeinsurance.asp?game=%s&client=Y", strGameCode.GetString());
		#else //UPGRADE_20120308_TOCTOC_CHANNELING
			strRet.Format( "http://game1.netmarble.net/service/freechargeinsurance.asp?game=%s&client=Y", strGameCode.GetString());
		#endif //UPGRADE_20120308_TOCTOC_CHANNELING
		break;
	case IDX_GAMEURL_INSURANCE:
		#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
			if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
				strRet.Format( "http://poker.toctocgame.com/Service/Insurance/InsurancePopup.asp?game=%s&client=Y", strGameCode.GetString());
			else
				strRet.Format( "http://game1.netmarble.net/Service/Insurance/InsurancePopup.asp?game=%s&client=Y", strGameCode.GetString());
		#else //UPGRADE_20120308_TOCTOC_CHANNELING
			strRet.Format( "http://game1.netmarble.net/Service/Insurance/InsurancePopup.asp?game=%s&client=Y", strGameCode.GetString());
		#endif //UPGRADE_20120308_TOCTOC_CHANNELING
		break;
	case IDX_GAMEURL_INSURANCE_GOLD:
		{
			// 기본 보험금 안내 팝업 
			// 보험을 못받는 상태에서 지급받기 버튼을 클릭시 노출 (보유골드가 보험금 보다 많은 경우)
			// 현재 보유 골드 및 보험 내용을 안내
			strRet.Format("http://game1.netmarble.net/Service/GoldInsurance/index.asp");
		}
		break;
	case IDX_GAMEURL_INSURANCE_GOLD2:
		{
			// 보험금 가입/지급 팝업 
			strRet.Format("http://game1.netmarble.net/Service/GoldInsurance/index.asp?gold=%I64d", GM().GetGoldInsuChangeChip());
		}
		break;
	case IDX_GAMEURL_INSURANCE_RESULT:	//보험가입결과 URL
		{	
			int nInsuKind = (int)lParam;

		#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
			if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
				strRet.Format( "http://poker.toctocgame.com/Service/Insurance/InsuranceEndPopup.asp?game=%s&client=Y&process=REGIST&insurancekind=%d", strGameCode.GetString(), nInsuKind);
			else
				strRet.Format( "http://game1.netmarble.net/Service/Insurance/InsuranceEndPopup.asp?game=%s&client=Y&process=REGIST&insurancekind=%d", strGameCode.GetString(), nInsuKind);
		#else //UPGRADE_20120308_TOCTOC_CHANNELING
			strRet.Format( "http://game1.netmarble.net/Service/Insurance/InsuranceEndPopup.asp?game=%s&client=Y&process=REGIST&insurancekind=%d", strGameCode.GetString(), nInsuKind);
		#endif //UPGRADE_20120308_TOCTOC_CHANNELING
		}
		break;
	case IDX_GAMEURL_FREECHARGE:
		#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
			if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
				strRet.Format( "http://poker.toctocgame.com/Service/Charge/FreeChargePopup.asp?game=%s&client=Y", strGameCode.GetString());
			else
				strRet.Format( "http://game1.netmarble.net/Service/Charge/FreeChargePopup.asp?game=%s&client=Y", strGameCode.GetString());
		#else //UPGRADE_20120308_TOCTOC_CHANNELING
			strRet.Format( "http://game1.netmarble.net/Service/Charge/FreeChargePopup.asp?game=%s&client=Y", strGameCode.GetString());
		#endif //UPGRADE_20120308_TOCTOC_CHANNELING

		break;
	case IDX_GAMEURL_COLLECTOVERMONEY:
		#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
			if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
				strRet.Format( "http://poker.toctocgame.com/family/LimitMoney/popCollectOverMoney.asp?game=%s", strGameCode.GetString());
			else
				strRet.Format( "http://game1.netmarble.net/family/LimitMoney/popCollectOverMoney.asp?game=%s", strGameCode.GetString());
		#else //UPGRADE_20120308_TOCTOC_CHANNELING
			strRet.Format( "http://game1.netmarble.net/family/LimitMoney/popCollectOverMoney.asp?game=%s", strGameCode.GetString());
		#endif //UPGRADE_20120308_TOCTOC_CHANNELING
		break;
	case IDX_GAMEURL_ACHIEVELIMITMONEY:
		#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
			if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
				strRet.Format( "http://poker.toctocgame.com/family/LimitMoney/popAchieveLimitMoney.asp?game=%s", strGameCode.GetString());
			else
				strRet.Format( "http://game1.netmarble.net/family/LimitMoney/popWebAchieveLimitMoney.asp?game=%s", strGameCode.GetString());
		#else //UPGRADE_20120308_TOCTOC_CHANNELING
			strRet.Format( "http://game1.netmarble.net/family/LimitMoney/popWebAchieveLimitMoney.asp?game=%s", strGameCode.GetString());
		#endif //UPGRADE_20120308_TOCTOC_CHANNELING
		break;
	case IDX_GAMEURL_COLLECTREVISIONMONEY:
		#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
			if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
				strRet.Format( "http://poker.toctocgame.com/family/LimitMoney/popCollectRevisionMoney.asp?game=%s", strGameCode.GetString());
			else
				strRet.Format( "http://game1.netmarble.net/family/LimitMoney/popCollectRevisionMoney.asp?game=%s", strGameCode.GetString());
		#else //UPGRADE_20120308_TOCTOC_CHANNELING
			strRet.Format( "http://game1.netmarble.net/family/LimitMoney/popCollectRevisionMoney.asp?game=%s", strGameCode.GetString());
		#endif //UPGRADE_20120308_TOCTOC_CHANNELING
		break;
	case IDX_GAMEURL_COLLECTBOTHMONEY:
		#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
			if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
				strRet.Format( "http://poker.toctocgame.com/family/LimitMoney/popCollectBothMoney.asp?game=%s", strGameCode.GetString());
			else
				strRet.Format( "http://game1.netmarble.net/family/LimitMoney/popCollectBothMoney.asp?game=%s", strGameCode.GetString());
		#else //UPGRADE_20120308_TOCTOC_CHANNELING
			strRet.Format( "http://game1.netmarble.net/family/LimitMoney/popCollectRevisionMoney.asp?game=%s", strGameCode.GetString());
		#endif //UPGRADE_20120308_TOCTOC_CHANNELING
		break;
	case IDX_GAMEURL_COLLECTOVERGOLDBANK:
		strRet.Format("http://game1.netmarble.net/family/limitgold/popCollectOverGold.asp");
		break;
	case IDX_GAMEURL_LOBBYITEMSHOP:
		strRet.Format( "http://game1.netmarble.net/agame/gameshop/shop/?gamecode=%s", strGameCode.GetString());
		break;
	case IDX_GAMEURL_SEATKEEPER:	//자리지킴이 구매페이지
		//strRet.Format("http://game1.netmarble.net/gameshop/client/buy/GamePlaceDefend.asp?game=%s", strGameCode.GetString());					   
		strRet.Format("http://game1.netmarble.net/gameshop/client/buy/GamePlaceDefend_Totalclient.asp?game=%s", strGameCode.GetString());
		break;
	case IDX_GAMEURL_GOLDENCHIP_CHANGE:
		strRet.Format("http://game1.netmarble.net/GoldenChip/Client/GoldenChip_Change.asp?game=%s", strGameCode.GetString());
		break;
	case IDX_GAMEURL_GOLDENCHIP_LIST:
		strRet.Format("http://game1.netmarble.net/GoldenChip/Client/GoldenChip_List.asp");
		break;
	case IDX_GAMEURL_ENDING_BANNER:
		strRet.Format("http://service.netmarble.net/banner/ending/focus.asp?game=pokergroup");
		break;
	case IDX_GAMEURL_ENDING_BANNER_NATE:
		strRet.Format("http://service.netmarble.net/banner/ending/focus.asp?game=nate");
		break;
	case IDX_GAMEURL_GAMEMANUAL:			//게임방법창 		
		{
			//strRet.Format("http://c2.img.netmarble.kr/web/2007/html/gopo/po_guide/%s/main.html", strGameCode.GetString());
			//strRet.Format("http://game1.netmarble.net/Poker/info/?game=%s", strGameCode.GetString());
			strRet.Format("http://game1.netmarble.net/Poker/info/?game=%s", strGameCode.GetString());
// 
// 			if (strGameCode == "spoker2")			
// 				strRet="http://nrd.netmarble.net/201012010057.RD";
// 			else if (strGameCode == "low")
// 				strRet="http://nrd.netmarble.net/201012010058.RD";
// 			else if (strGameCode == "newpoker")
// 				strRet="http://nrd.netmarble.net/201012010056.RD";
// 			else if (strGameCode == "sutda")
// 				strRet="http://nrd.netmarble.net/201012010062.RD";
// 			else if (strGameCode == "hoola")
// 				strRet="http://nrd.netmarble.net/201012010063.RD";
// 			else if (strGameCode == "7cardhighlow")
// 				strRet="http://nrd.netmarble.net/201012010061.RD";			
		}
		
		break;
	case	IDX_GAMEURL_LOBBYEVENTDIALOG:	//로비이벤트 버튼 클릭하며 나오는 다이얼로그
		#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
			if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
			{
				strRet.Format("http://poker.toctocgame.com/gameshop/client/event/index.asp");
			}
			else
			{
				strRet.Format("http://game1.netmarble.net/pokerclient/index.asp");
			}
		#else //UPGRADE_20120308_TOCTOC_CHANNELING
			strRet.Format("http://game1.netmarble.net/pokerclient/index.asp");
		#endif //UPGRADE_20120308_TOCTOC_CHANNELING
		break;
	case	IDX_GAMEURL_LOBBY_EVENT:		//우측하단 이벤트
		#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
			if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
			{
				strRet.Format("http://poker.toctocgame.com/gameshop/client/event/eventbanner.asp");//http://game1.netmarble.net/event/2010/_12/20101207_pokerbeta/");
			}
			else
			{
				strRet.Format("http://game1.netmarble.net/pokerclient/eventbanner.asp");//http://game1.netmarble.net/event/2010/_12/20101207_pokerbeta/");
			}
			break;
		#else //UPGRADE_20120308_TOCTOC_CHANNELING
			strRet.Format("http://game1.netmarble.net/pokerclient/eventbanner.asp");//http://game1.netmarble.net/event/2010/_12/20101207_pokerbeta/");
			break;
		#endif //UPGRADE_20120308_TOCTOC_CHANNELING
	case	IDX_GAMEURL_LOBBY_SHOP:			//우측하단 추천상품
		#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
			if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
			{
				strRet.Format("http://poker.toctocgame.com/gameshop/client/waitroom/index.asp?gamecode=%s", strGameCode.GetString());
			}
			else
			{
				strRet.Format("http://game1.netmarble.net/gameshop/client/waitroom/?gamecode=%s", strGameCode.GetString());
			}
			break;
		#else //UPGRADE_20120308_TOCTOC_CHANNELING
			strRet.Format("http://game1.netmarble.net/gameshop/client/waitroom/?gamecode=%s", strGameCode.GetString());
			break;
		#endif //UPGRADE_20120308_TOCTOC_CHANNELING

	case IDX_GAMEURL_BADUSER:	//불량유저 신고하기 페이지
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
		if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
			strRet.Format("http://www.toctocgame.com/M_Customer/Customer_Main.asp");
		else
			strRet.Format("http://helpdesk.netmarble.net/HelpMyPage.asp?tab=4");
#else //UPGRADE_20120308_TOCTOC_CHANNELING
		strRet.Format("http://helpdesk.netmarble.net/HelpMyPage.asp?tab=4");
#endif //UPGRADE_20120308_TOCTOC_CHANNELING
		break;
	case IDX_GAMEURL_PCROOM_ADVANTAGE:
		{			
			if (strGameCode == "spoker2")
				//strRet="http://pcbang.netmarble.net/PcBangPop/Benefit/main.asp?gtype=PG&gCode=5.02";
				strRet="http://pcbang.netmarble.net/Popup/?gtype=PG&gCode=spoker2";
			else if (strGameCode == "low")
				//strRet="http://pcbang.netmarble.net/PcBangPop/Benefit/main.asp?gtype=PG&gCode=5.04";
				strRet="http://pcbang.netmarble.net/Popup/?gtype=PG&gCode=low";
			else if (strGameCode == "newpoker")
				//strRet="http://pcbang.netmarble.net/PcBangPop/Benefit/main.asp?gtype=PG&gCode=5.01";
				strRet="http://pcbang.netmarble.net/Popup/?gtype=PG&gCode=newpoker";
			else if (strGameCode == "sutda")
				//strRet="http://pcbang.netmarble.net/PcBangPop/Benefit/main.asp?gtype=PG&gCode=5.08";
				strRet="http://pcbang.netmarble.net/Popup/?gtype=PG&gCode=sutda";
			else if (strGameCode == "hoola")
				//strRet="http://pcbang.netmarble.net/PcBangPop/Benefit/main.asp?gtype=PG&gCode=5.06";
				strRet="http://pcbang.netmarble.net/Popup/?gtype=PG&gCode=hoola";
			else if (strGameCode == "7cardhighlow")
				//strRet="http://pcbang.netmarble.net/PcBangPop/Benefit/main.asp?gtype=PG&gCode=5.03";			
				strRet="http://pcbang.netmarble.net/Popup/?gtype=PG&gCode=7cardhighlow";
		}break;

	case IDX_GAMEURL_GAMEMAINPAGE:	// 바로가기 주소
		{
			IDX_GAME idxGame = (IDX_GAME) lParam;
			CGame *pGame = GM().GetGame(idxGame);
			if (pGame)
			{				
				strRet.Format("http://game1.netmarble.net/%s", pGame->GetGameString());
			}
			else
			{
				strRet.Format("http://www.netmarble.net");
			}
		}break;
	
	case IDX_GAMEURL_GAMEMAINPAGE_NATE:  //네이트 바로가기 주소
		{
			IDX_GAME idxGame = (IDX_GAME) lParam;
			CGame *pGame = GM().GetGame(idxGame);
			if (pGame)
			{				
				strRet.Format("http://game.nate.com/NateFilter.asp?targetURL=http://game1.netmarble.net/%s/", pGame->GetGameString());
			}
			else
			{
				strRet.Format("http://game.nate.com/");
			}
		}break;

	case IDX_GAMEURL_GAMEMAINPAGE_TOCTOC:	// 톡톡 바로가기 주소
		{
			IDX_GAME idxGame = (IDX_GAME) lParam;
			CGame *pGame = GM().GetGame(idxGame);
			if (pGame)
			{				
				strRet.Format("http://poker.toctocgame.com/?game=%s", pGame->GetGameString());
			}
			else
			{
				strRet.Format("http://poker.toctocgame.com/");
			}
		}break;

	case IDX_GAMEURL_ENDPOPUP: //종료팝업		
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
		if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		{
			strRet.Format("http://poker.toctocgame.com/gameshop/client/event/EndingPop.asp?game=%s", strGameCode.GetString());
		}
		else
			strRet.Format("http://game1.netmarble.net/gameshop/client/shop/EndingEventPop.asp?game=%s", strGameCode.GetString());
#else //UPGRADE_20120308_TOCTOC_CHANNELING
		strRet.Format("http://game1.netmarble.net/gameshop/client/shop/EndingEventPop.asp?game=%s", strGameCode.GetString());
#endif //UPGRADE_20120308_TOCTOC_CHANNELING
		break;
	case IDX_GAMEURL_EVENT_PCPROMOTION_LOBBY_TOP:
		{
			strRet.Format("http://game1.netmarble.net/event/2011/_03/20110315_PCRoomPromotion/ClientPrizewinnerLstSlidebarType1.asp");

		} break;

	case IDX_GAMEURL_EVENTPAGE:
		{
			/*if (GM().GetMyInfo()->UI.SiteCode == NMBASE::UTIL::SNO_NATE)
				strRet.Format("http://game1.netmarble.net/event/2014/poker/06/?siteinfo=nate");
			else
				strRet.Format("http://game1.netmarble.net/event/2014/poker/06/");*/

			//if (GM().GetMyInfo()->UI.SiteCode == NMBASE::UTIL::SNO_NATE)
				//strRet.Format("http://nrd.netmarble.net/201406170103.TR");
			//else
				//strRet.Format("http://nrd.netmarble.net/201406170102.TR");		
			/*strRet.Format("http://game1.netmarble.net/event/2014/poker/09/");*/
			//strRet.Format("http://game1.netmarble.net/event/2014/poker/12/index.asp");
			//if(true==GM().IsOverDay(2015, 2, 27)){
			//	strRet.Format("http://promotion.netmarble.net/m/event/mpoker/26");
			//}
			//else{
			//	if (GM().GetMyInfo()->UI.SiteCode == NMBASE::UTIL::SNO_NATE){
			//		strRet.Format("http://game1.netmarble.net/event/2015/poker/01/index.asp?siteinfo=nate");
			//	}
			//	else{
			//		strRet.Format("http://game1.netmarble.net/event/2015/poker/01/index.asp");
			//	}
			//}
			strRet.Format("http://promotion.netmarble.net/m/event/mpoker/26");
		} break;

	case IDX_GAMEURL_TIMELIMITAVATAR:
		{
			strRet.Format( "http://game1.netmarble.net/Item/LimitedGoods/?game=%s&client=Y", strGameCode.GetString());
		} break;
	case IDX_GAMEURL_POSTBOX:
		{
#if defined(_DEBUG)
			strRet.Format( "http://10.103.92.128:12002/pcgostop/message/repo/1/view" );	

#else 
			if(g_strConnectecMasterServerName == "183.110.61.192:12000")
			{
				strRet.Format( "http://10.103.92.128:12002/pcgostop/message/repo/1/view" );
			}
			else
			{
				strRet.Format( "https://matgo-message.netmarble.net:12002/pcgostop/message/repo/1/view" );
			}
#endif
		} break;	
	case IDX_GAMEURL_POSTBOX_SEND_MESSAGE:
		{
#if defined(_DEBUG)
			strRet.Format( "http://10.103.92.128:12002/pcgostop/message/send/view" );	
#else
			if(g_strConnectecMasterServerName == "183.110.61.192:12000")
			{
				strRet.Format( "http://10.103.92.128:12002/pcgostop/message/send/view" );
			}
			else
			{
				strRet.Format( "https://matgo-message.netmarble.net:12002/pcgostop/message/send/view" );
			}
#endif
		} break;
	case IDX_GAMEURL_POSTBOX_URL:
		{
			#if defined(_DEBUG)
				strRet.Format( "10.103.92.128" );	
#else
			if(g_strConnectecMasterServerName == "183.110.61.192:12000")
			{
				strRet.Format( "10.103.92.128" );
			}
			else
			{
				strRet.Format( "matgo-message.netmarble.net" );
			}
#endif
		} break;
	case IDX_GAMEURL_CHICKEN_URL:
		{
			strRet.Format( "http://game1.netmarble.net/event/2018/nychk/p_list.asp?gc=%d", nGameCode);
		} break;
	case IDX_GAMEURL_CHICKEN_REWARD:
		{
			strRet.Format( "http://game1.netmarble.net/event/2018/nychk/p_info.asp?gc=%d", nGameCode);
		} break;
	case IDX_GAMEURL_GOLD_BIGWHEEL:
		{
			//strRet.Format( "http://game1.netmarble.net/game/bigwheel/");
			char buffer[MAX_PATH];
			GetModuleFileName( NULL, buffer, MAX_PATH );
			CString path = buffer;
			path.Replace("\\", "/");
			int pathLength = path.ReverseFind('/');
			path.Delete(pathLength, path.GetLength());
			strRet.Format( "file:///%s/Lobby/data/bigwheel/index.html", path.GetString());
		} break;
	case IDX_GAMEURL_CAPTCHA:
		{			
			strRet.Format("http://game1.netmarble.net/poker/captcha/main.asp");			
		} 
		break;
	case IDX_GAMEURL_CAFE:
		{			
			strRet.Format("http://game1.netmarble.net/poker/reward/_html/cafeComm/cafe.asp");			
		} 
		break;
	case IDX_GAMEURL_FREECHARGE_SHOP:
		{
			strRet.Format("http://game1.netmarble.net/poker/shop/freecharge.asp");
		}
		break;

	}
	return strRet;
}

