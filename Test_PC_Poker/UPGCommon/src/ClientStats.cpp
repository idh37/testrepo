#include "StdAfx.h"
#include "ClientStats.h"
#include <afxsock.h>

bool g_bUILogEnable=true;

void SendUILog_Enable(bool bEnable)		//로그를 켜고 끈다.
{
	g_bUILogEnable=bEnable;
}

int GetCLStatsPremCode(USERINFO *pUI)
{
	if (pUI==NULL) return CLSTATS_MEMBERSCODE_POKER_NONE;



	switch(UPGCheckPremiumState(pUI))
	{
	case CODE_NEW_FAMILY_MINI: return CLSTATS_MEMBERSCODE_POKER_MINI;
	case CODE_NEW_FAMILY_SILVER: return CLSTATS_MEMBERSCODE_POKER_SILVER;
	case CODE_NEW_FAMILY_GOLD: return CLSTATS_MEMBERSCODE_POKER_GOLD;
	case CODE_NEW_FAMILY_DIAMOND: return CLSTATS_MEMBERSCODE_POKER_DIAMOND;
	default: return CLSTATS_MEMBERSCODE_POKER_NONE;
	}

	return CLSTATS_MEMBERSCODE_POKER_NONE;

}
	

//클릭로그 처리
void SendUILog_Basic(int nUINo)
{
	if (g_bUILogEnable==false)			//로그 기록이 ON되어있는지 ( ex) 바둑이 대회서버에서는 OFF)
		return;

	//make url
	
	
	//[형식] http://218.153.10.70/logging/{버전코드}/{게임코드}/{ui_code}?cn={CN값}&status={패밀리멤버코드}&channel={채널번호}&money={머니}&date={날짜초단위}
	//[예시] http://218.153.10.70/logging/logging/0/01/002?cn=NM000000000&status=3&channel=23&money=10000&date=20110426235959

	CPlayer *pMyPlayer = GM().GetMyInfo();
	if (pMyPlayer==NULL)
		return;

	
	//QA 테스트 용
// 	const char *szHostName = "clickstreamlog.cjinternet.co.kr/logging/logging";		//QA 테스트 용
// 	const int nCodeVer = 9;							//QA테스트용

	//리얼 용
	const char *szHostName = "clickstreamlog.netmarble.net/logging";		//리얼 용
	const int nCodeVer = 0;							//리얼 용

	
	int nGameCode = GM().GetCurrentGameCode();
	int nPremCode = GetCLStatsPremCode(&pMyPlayer->UI);
	int nChannelNo = 0;
	
	CString strNowTime = COleDateTime::GetCurrentTime().Format("%Y%m%d%H%M%S");
	
	WORD wVersionRequested;
	WSADATA wsaData;
	char name[355] = {0,};
	PHOSTENT hostinfo;
	CString strIPAddress = _T("");
	wVersionRequested = MAKEWORD(2, 0);

	if (WSAStartup(wVersionRequested, &wsaData) == 0)
	{
		if(gethostname(name, sizeof(name)) == 0)
		{
			if ((hostinfo = gethostbyname(name)) != NULL)
				strIPAddress = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
		}
		WSACleanup();
	}

	CString strURL;
	strURL.Format("http://%s/%d/%d/%d?cn=%s&status=%d&channel=%d&money=%I64d&date=%s&ip=%s",
					szHostName,
					nCodeVer,
					nGameCode,
					nUINo, 
					pMyPlayer->UI.UniqNo,
					nPremCode,
					nChannelNo,
					pMyPlayer->UI.GetMoney(),
					strNowTime,
					strIPAddress);

	NMBASE::UTIL::gStatisticsMan().AddStatisticsURL(strURL.GetString());

}
void SendUILog_GameOffset(int nUINo, IDX_GAME nGame)
{
	if (nGame <0 || nGame>=MAX_GAME)
		return;




	SendUILog_Basic(nUINo + nGame);
}
void SendUILog_GaugeButton(INT64 llEnterLimitMoney)
{
	int nUINo = 0;
	switch(llEnterLimitMoney)
	{
	case 100000000:				nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_1Y;	 break; //1억
	case 1000000000:			nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_10Y;	 break; //10억
	case 10000000000:			nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_100Y;	 break; //100억
	case 100000000000:			nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_1000Y;	 break; //1000억
	case 1000000000000:			nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_1J;	 break; //1조
	case 3000000000000:			nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_3J;	 break; //3조
	case 5000000000000:			nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_5J;	 break; //5조
	case 10000000000000:		nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_10J;	 break; //10조
	case 30000000000000:		nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_30J;	 break; //30조
	case 50000000000000:		nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_50J;	 break; //50조
	case 100000000000000:		nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_100J;	 break; //100조
	case 200000000000000:		nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_200J;	 break; //200조						
	}

	if (nUINo==0)
		return;
	
	SendUILog_Basic(nUINo);
}
