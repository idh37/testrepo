#include "StdAfx.h"
#include "ClientStats.h"
#include <afxsock.h>

bool g_bUILogEnable=true;

void SendUILog_Enable(bool bEnable)		//�α׸� �Ѱ� ����.
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
	

//Ŭ���α� ó��
void SendUILog_Basic(int nUINo)
{
	if (g_bUILogEnable==false)			//�α� ����� ON�Ǿ��ִ��� ( ex) �ٵ��� ��ȸ���������� OFF)
		return;

	//make url
	
	
	//[����] http://218.153.10.70/logging/{�����ڵ�}/{�����ڵ�}/{ui_code}?cn={CN��}&status={�йи�����ڵ�}&channel={ä�ι�ȣ}&money={�Ӵ�}&date={��¥�ʴ���}
	//[����] http://218.153.10.70/logging/logging/0/01/002?cn=NM000000000&status=3&channel=23&money=10000&date=20110426235959

	CPlayer *pMyPlayer = GM().GetMyInfo();
	if (pMyPlayer==NULL)
		return;

	
	//QA �׽�Ʈ ��
// 	const char *szHostName = "clickstreamlog.cjinternet.co.kr/logging/logging";		//QA �׽�Ʈ ��
// 	const int nCodeVer = 9;							//QA�׽�Ʈ��

	//���� ��
	const char *szHostName = "clickstreamlog.netmarble.net/logging";		//���� ��
	const int nCodeVer = 0;							//���� ��

	
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
	case 100000000:				nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_1Y;	 break; //1��
	case 1000000000:			nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_10Y;	 break; //10��
	case 10000000000:			nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_100Y;	 break; //100��
	case 100000000000:			nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_1000Y;	 break; //1000��
	case 1000000000000:			nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_1J;	 break; //1��
	case 3000000000000:			nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_3J;	 break; //3��
	case 5000000000000:			nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_5J;	 break; //5��
	case 10000000000000:		nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_10J;	 break; //10��
	case 30000000000000:		nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_30J;	 break; //30��
	case 50000000000000:		nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_50J;	 break; //50��
	case 100000000000000:		nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_100J;	 break; //100��
	case 200000000000000:		nUINo= CLSTATS_UINO_POKER_LOBBY_GAUGE_BUTTON_200J;	 break; //200��						
	}

	if (nUINo==0)
		return;
	
	SendUILog_Basic(nUINo);
}
