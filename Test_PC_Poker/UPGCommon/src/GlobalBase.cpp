#include "StdAfx.h"
#include "GlobalBase.h"
#include "MsgBoxDlg.h"
//#include "MsgBoxChkDlg.h"
#include "MsgBoxModelessDlg.h"
#include <math.h>
#include <CommMsgDef.h>

//NMBASE::SOCK::CSockMan			NMBASE::SOCK::GSOCKMAN();			// 소켓 메니져 클래스
NMBASE::UTIL::CNoChat				NoChat;
CAdminMessageDlg	*g_pAMes	= NULL; // [관리자 모드 작업]

//CTodayTipDlg		g_TipManDlg;			// 팁

CString				g_ServIP;	//마스터 서버의 IP
UINT				g_ServPort=0;		//디버그용 마스터 서버의 포트번호
CString				g_AuthCookie;				// 인증 쿠키						[SSO 작업]
CString				g_DataCookie;				// 데이터 쿠키						[SSO 작업]
CString				g_CpCookie;					// CP쿠키							[SSO 작업]
CString				g_SpareParam;				// 여분의 인자(CP게임등에서 사용)	[SSO 작업]
CString				g_LoginID;	// 자동 로그인 ID/
CString				g_LoginPass;		// 자동 로그인 패스워드
int					g_LoginGroup = 0;			// 로그인시 요청한 그룹
int					g_LoginGameCode = 0;		// 로그인시 요청한 게임코드
CString				g_MyUniqNo;					// 엑스트라 인자

Ms_DestServerInfo	g_DestSvInfo	= {0,};		// 접속할 목적지 서버 정보
int 				g_NowGroupNo;				// 접속한 그룹 번호
IDX_GAMERECORD		g_NowGameRecord;			// 접속한 채널 등급
IDX_GROUPCLASS		g_NowGroupClass;			// 접속한 그룹군(성격으로 군을 나눔)

int 				g_NowChanNo;				// 접속한 통합 채널 번호
CString				g_NowChanName;				// 접속한 채널 이름
CString				g_strSrvInfo;				// 채널 공지

int					g_MSCode		= 0;		// 0: Korean 1: English 2:Japanese 3:Chinese
NMBASE::UTIL::CStringManager			g_StrMan;

int					g_MasterSID = -1;			// 마스터 서버의 소켓 번호
int					g_MainSrvSID = 0;			// 메인 서버의 소켓 번호

CString				g_strMasterServerName;		// 마스터 서버의 소켓 번호
CString				g_strConnectecMasterServerName;
CString				g_strMainServerName;		// 메인 서버의 소켓 번호



CRect				g_OrgMainDlgRect;			// 메인 다이얼로그의 원래 크기
BOOL				g_bNoGo = FALSE;
BOOL				g_bChannel = FALSE;
int					g_MyPN = 0;					// 나의 플레이어 번호(0-2)
int					g_MastPN = 0;				// 방장의 플레이어 번호(0-2)
ROOMINFO			g_RI={0,};					// 방 정보 구조체
NMBASE::GRAPHICGDI::CDisplay			Display;					// 디스플레이 모드 변경 클래스

BOOL				g_bMyObserver	= FALSE;	//관전중인가
BOOL				g_bAdminUser	= FALSE;	// [관리자 모드 작업]
BOOL				g_bConfirmation = FALSE;	//[관리자 여부 확인]

GAMEITEM_LIST		g_MyGameItem;				// 내 게임 아이템
COleDateTimeSpan	g_CSTimeGap;				// 서버와 클라이언트의 시간차

DWORD				g_nStartTick		= 0;	// 서버와의 시간 설정 값
DWORD				g_DebugStartTick	= 0;	// 서버와의 시간 설정 값
bool				g_bLobyItemFirstNavigation = false;	// [아이템 유료화 영역]

POKERGAME			g_Poker;
CString				g_TargetID;					// [넷마블 보안관]

INT64				g_MaxServerMoney=0;	//내가 가질 수 있는 최대 머니 (로그인시 서버에서 받음)
INT64				g_MaxServerGold=0;	//내가 가질 수 있는 최대 골드 (로그인시 서버에서 받음)
INT64				g_MaxServerGoldBank=0;	//내가 가질 수 있는 최대 골드 뱅크 적립금 (로그인시 서버에서 받음)
INT64				g_DisplayMaxMoney[MAX_CODE_NEW_FAMILY]={0,};//패밀리 회원별 최대 보유머니 (로그인시 서버에서 받음)
INT64				g_DisplayMaxGold[MAX_CODE_NEW_FAMILY]={0,};//패밀리 회원별 최대 보유골드 (로그인시 서버에서 받음)
BOOL				g_bNewlySignedUp=FALSE;

CHAT_MACRO	g_ChatMacro;				// 메크로(채팅 관련)
float		g_fRaceBetRatio	= 0.99f;
GAMESPEED	g_GameSpeed;

BOOL		g_bJumpItemBuying		= FALSE;
BOOL		g_bRealAdmissionBuying	= FALSE; //실전 채널 입장권

BOOL		g_bAskContinue			= FALSE; 

MYGAMEINOF	g_MyGameInfo;	//게임 모드 옵션 을 저장 한다.
PREMOPTION	g_PremOption;//프리미엄 옵션
jackpot::JP_MONEYINFO g_JackPotMoneyInfo;	// 잭팟금액 정보

bool		s_bAccessRoomState = false;
bool		s_bFullScreenState = false;

const int	REFRESH_FPS = 50;
const int   HEIGHT_CAPTION_LOBBY = 28;

CONFIG g_Config={0,};

LSY::CObjectMan *g_pObjectMan = NULL;

LSY::CObjectMan *CreateObjectMan(CWnd *pWnd, CWnd *pMainWnd)
{
	ASSERT(g_pObjectMan == NULL);
	g_pObjectMan = new LSY::CObjectMan(pWnd, pMainWnd);
	g_pObjectMan->SetFPS(40);
	g_pObjectMan->SetCaptionHeight(HEIGHT_CAPTION_LOBBY);

	LSY::CObjectMan::SetMainObjectMan(g_pObjectMan);

	SetProp(pMainWnd->GetSafeHwnd(), "ObjectMan", (HANDLE)g_pObjectMan);

	return g_pObjectMan;
}

void ReleaseObjectMan(void)
{
	if(g_pObjectMan)
	{
		SetProp(GM().GetMainWnd()->GetSafeHwnd(), "ObjectMan", NULL);

		delete g_pObjectMan;
		g_pObjectMan = NULL;
	}
}

LSY::CObjectMan *GetObjectMan(void)
{
	return g_pObjectMan;
}

void AllDrawObjectMan(void)
{
	if(g_pObjectMan) g_pObjectMan->AllDraw();
}

// 진행 방향 각도 구하기
double GetRadian(int x1, int y1, int x2, int y2)
{
	// 픽셀 거리
	int w = x2-x1;
	int h = y1-y2;

	if(w==0 && h==0) return 0;
	else if(w==0) {
		if(h>0) return 90.0f * RADIAN;
		else return 270.0f * RADIAN;
	}
	else if(h==0) {
		if(w>0) return 0.0f;
		else return 180.0f * RADIAN;
	}

	// 기울기 구하기
	double al = (double)abs(h)/(double)abs(w);

	double rad = atan(al);

	if(w==0 && h==0) return 0;
	else if(w>=0 && h>=0) {		// 1사분면
	}
	else if(w<0 && h>=0) {	// 2사분면
		rad = (180.0f * RADIAN) - rad;
	}
	else if(w<0 && h<0) {	// 3사분면
		rad += 180.0f * RADIAN;
	}	
	else if(w>=0 && h<0) {	// 4사분면
		rad = (360.0f * RADIAN) - rad;
	}

	return rad;
}


float GetAngle(int x1, int y1, int x2, int y2)
{
	// 픽셀 거리
	int w = x2-x1;
	int h = y1-y2;

	if(w==0 && h==0) return 0;
	else if(w==0) {
		if(h>0) return 90.0f;
		else return 270.0f;
	}
	else if(h==0) {
		if(w>0) return 0.0f;
		else return 180.0f;
	}

	// 기울기 구하기
	double al = (double)abs(h)/(double)abs(w);
	float angle = (float)(atan(al) / RADIAN);

	if(w==0 && h==0) return 0;
	else if(w>=0 && h>=0) {		// 1사분면
	}
	else if(w<0 && h>=0) {	// 2사분면
		angle = 180.0f - angle;
	}
	else if(w<0 && h<0) {	// 3사분면
		angle += 180.0f;
	}	
	else if(w>=0 && h<0) {	// 4사분면
		angle = 360.0f - angle;
	}

	return angle;
}

// 목적지 거릴 리턴
float GetDistance(int x1, int y1, int x2, int y2)
{
	float len = 0;

	// 픽셀 거리
	int w = abs(x2-x1);
	int h = abs(y2-y1);

	if(w==0 && h==0) return 0;
	else if(w==0) len = (float)h;
	else if(h==0) len = (float)w;
	else len = (float)sqrt((float)(w*w + h*h));

	return len;
}

// 이동할 픽셀 좌표리턴 한다.
POINT GetMovePoint(int sx, int sy, float angle, float distance)
{
	POINT pnt;
	pnt.x = sx;
	pnt.y = sy;
	double rad = angle * RADIAN;
	int x = (int)(cos(rad) * distance);
	int y = -(int)(sin(rad) * distance);
	pnt.x += x;
	pnt.y += y;
	return pnt;
}

// 기준각에 대한 상대각의 각도구하기
// sangle : 진행 방향의 각도 , rangle :  휠 방향의 각도 (진행 방향을 기준으로)
float GetCrossAngle(float sangle, float rangle) 
{
	if(sangle == rangle) return 0;

	if(sangle < rangle) {
		float ang = rangle - sangle;
		if(ang > 180.0f) {
			ang = 360.0f - ang;
			return -ang;
		}
		else return ang;
	}
	else {
		float ang = sangle - rangle;
		if(ang > 180.0f) {
			ang = 360.0f - ang;
			return ang;
		}
		else return -ang;
	}
}

void _LOADSPRITEDEFINE( NMBASE::GRAPHICGDI::xSprite &spr,string &dir,char *pname, bool bForceLoad/* = true*/ )
{
	string stri;
	stri = dir + pname;
	if(bForceLoad)
	{
		if(spr.Load( (char*)stri.c_str(), 555)==FALSE) 
		{
			CMsgBoxDlg dlg(GM().GetMainWnd());
			dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, pname);
		}
	}
	else
	{
		AddLoadImageList(&spr, (char *)stri.c_str(),true);
	}
}

//< CYS 100525
bool _LOADSPRITEDEFINE( NMBASE::GRAPHICGDI::xSprite* pSpr, string &str, char* pname, bool bForceLoad/* = true*/ )
{
	std::string strFull;
	strFull = str + pname; 
	if(bForceLoad)
	{
		if ( pSpr->Load( ( char* )strFull.c_str(), 555 ) == false )
		{
			CMsgBoxDlg dlg(GM().GetMainWnd());
			dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE,( char* )strFull.c_str() );
			return false;
		}
	}
	else
	{
		AddLoadImageList(pSpr, (char *)strFull.c_str(),true);
	}

	return true;
}
bool _LOADSPRITEDEFINE( NMBASE::GRAPHICGDI::xSprite &pSpr,	string &pFilePath, bool bForceLoad )
{
	if(bForceLoad)
	{
		if ( pSpr.Load( ( char* )pFilePath.c_str(), 555 ) == false )
		{
			CMsgBoxDlg dlg(GM().GetMainWnd());
			dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, ( char* )pFilePath.c_str() );
			return false;
		}
	}
	else
	{
		AddLoadImageList(&pSpr, (char *)pFilePath.c_str(),true);
	}

	return true;
}
BOOL OnMacroCheck(BOOL bclose)
{
	return FALSE;
/*
	CString lpWindowTitleName;

#define  MAX_MACRO_CNT  11
	char MacroName[MAX_MACRO_CNT][64] = {
		{"Macro Express"},{"AutoPoker"},{"Macro"},{"Auto Mouse"},{"Speed Hack"},
		{"hack"},{"Toggle Mouse"},{"Ghost Mouse"},{"Auto Poker"},{"artmoney"},{"GMacro"}};		


		// 처음 최상위 윈도우를 목록에서 얻어온다.	
		CWnd *pWnd = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);

		CString strCaption;

		while(pWnd){

			if(!pWnd->GetOwner() && pWnd->GetWindowTextLength()){

				pWnd->GetWindowText(strCaption); // 윈도우의 캡션을 얻음			

				if( strCaption.GetLength() > 0 )
				{
					for( int i = 0 ; i < MAX_MACRO_CNT ; i ++ )
					{
						lpWindowTitleName = MacroName[i];

						if( lpWindowTitleName.GetLength() >0 )
						{					
							lpWindowTitleName.MakeLower();
							strCaption.MakeLower();
							lpWindowTitleName.TrimLeft();
							lpWindowTitleName.TrimRight();
							strCaption.TrimLeft();
							strCaption.TrimRight();
							lpWindowTitleName.Replace(" ","");
							strCaption.Replace(" ","");

							BOOL bresult = CheckCompareString(&strCaption , &lpWindowTitleName);
							if(bresult == 1)
							{				
								//경고만 보낸다^
								if(bclose == FALSE)
								{
									//	AfxMessageBox(strCaption);
									ShowMainMessageDlg("비정상적인 프로그램이 감지 되었습니다.\n비정상적인 프로그램 사용시 불이익을 받으실 수 있습니다.");
									return TRUE;
								}
								else
								{
									ShowMainMessageDlg("비정상적인 프로그램 사용시 게임에 참여 하실 수 없습니다. \n비정상적인 프로그램을 종료하여 주시기 바랍니다.");

									if( g_MainSrvSID == -1 ){
										CloseClient();
										return TRUE;
									}

									CPlayer *pMyInfo = GM().GetMyInfo();

									CSV_ASK_LOGOUT alomsg;
									// ### [ 관전기능 ] ###
									alomsg.Set(pMyInfo->UI.UNum, pMyInfo->UI.ID);
									NMBASE::SOCK::GSOCKMAN().SendData(g_MainSrvSID, alomsg.pData, alomsg.GetTotalSize());							

									return TRUE;
								}		
							}
						}
					}				
				}
			}		
			// 윈도우 목록에서 다음 윈도우를 얻어온다.		
			pWnd = pWnd->GetWindow(GW_HWNDNEXT);		
		}	

		return FALSE;
	*/
}

//	dest_str 에 compare_str의 문자가 포함 되어 있는지를 검사한다
BOOL CheckCompareString(CString *dest_str, CString *compare_str)
{
	if( dest_str == NULL || compare_str == NULL )return FALSE;


	if( dest_str->Compare( "devstudiomacroterminator") == 0 ) return FALSE;

	int len_dest = dest_str->GetLength();
	int len_compare = compare_str->GetLength();	

	if( len_dest == 0 || len_compare == 0)return FALSE;
	if( len_dest < len_compare )return FALSE;

	int result = dest_str->Find((char*)compare_str->operator LPCTSTR());

	if( result != -1 ){
		CString compare = dest_str->Mid( result ,  len_compare);
		if( compare.CompareNoCase(compare_str->operator LPCTSTR()) == 0 ){
			return TRUE;
		}
	}	
	else{
		return FALSE;
	}

	return FALSE;	
}

void SSO_OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, LPCTSTR lpszPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel)
{

	CString MacroString = lpszURL;

	if((MacroString.Find("Captcha=", 0) >= 0))
	{		
		CString authentiCode = GetPostedData("Captcha=", lpszURL);
		MM().Call(UPG_CMK_SET_AUTHENTICODE_CAPTCHA, (WPARAM)(&authentiCode));

		MM().Call(UPG_CMK_CLOSE_CAPTCHA_DLG);
	}
	else if ( MacroString.Find( "_WheelLoadCompleted_", 0 ) >= 0 ) 
	{
		CGoldBigWheelDlg *pGoldBigWheelDlg = NULL;
		MM().Call(UPG_CMK_GET_GOLDBIGWHEEL_DLG,(WPARAM)0,(LPARAM)&pGoldBigWheelDlg);
		pGoldBigWheelDlg->WheelLoadCompleted();
	}
	else if ( MacroString.Find( "_WheelComplete_", 0 ) >= 0 ) 
	{
		CGoldBigWheelDlg *pGoldBigWheelDlg = NULL;
		MM().Call(UPG_CMK_GET_GOLDBIGWHEEL_DLG,(WPARAM)0,(LPARAM)&pGoldBigWheelDlg);
		pGoldBigWheelDlg->WheelCompleted();
	}
	else if ( MacroString.Find( "_WheelNext_", 0 ) >= 0 ) 
	{
		CGoldBigWheelDlg *pGoldBigWheelDlg = NULL;
		MM().Call(UPG_CMK_GET_GOLDBIGWHEEL_DLG,(WPARAM)0,(LPARAM)&pGoldBigWheelDlg);
		pGoldBigWheelDlg->WheelNext();
	}
	//대기실에서 구매 의사 (클릭후)
	else if ( MacroString.Find( "_AppPopup_", 0 ) >= 0 ) 
	{
		CString strURL = GetPostedData( "URL=", lpszURL );
		GDM().SetString(GDK_SSOWEB_URL, (LPCTSTR)strURL);
		int nType = atoi( (LPCTSTR)GetPostedData( "Type=", lpszURL ));
		GDM().SetINT64(GDK_SSOWEB_TYPE, nType);
		GDM().SetINT64(GDK_SSOWEB_ITEMCODE, atoi( (LPCTSTR)GetPostedData( "Itemcode=", lpszURL ) ));

		// [아이템샵 Tab 추가 수정]
		int Tab				 = 0;
		Tab = atoi( (LPCTSTR)GetPostedData( "tab=", lpszURL ) );

		*pbCancel = TRUE;

		if ( nType == 1 ) // 아이템 상점행
		{
			MM().Call(UPG_CMK_AVATAR_SHOP, Tab, (LPARAM) strURL.operator LPCTSTR());
		}
		else if ( nType == 2 )
		{
			MM().Call(UPG_CMK_CHANCEITEMSHOP);
		}
		else if ( nType == 3 )
		{
			MM().Call(UPG_CMK_ITEMSHOP, 1);	///1번탭의 포커상점을 보여준다.
		}
	}
	// [게임아이템창 리뉴얼] 2006.04.28
	else if ( MacroString.Find( "Status=", 0 ) >= 0 )
	{
		CString status = GetPostedData1( "Status=", lpszURL );
		int nStatus = 0;

		if ( status == "_buyend_" )//구매가 끝났다
		{			
			nStatus = 1; 
		}
		else if ( status == "_useitem_" )
		{
			nStatus = 2; 
		}
		else if ( status == "_reload_" )
		{
			nStatus = 3;
		}
		else if ( status == "_close_" )
		{
			*pbCancel = TRUE;
			GM().PostMainWndMessage(UM_CLOSEALLPOPUP);	//팝업(상점, 찬스구매창)을 닫으라고 메인다이얼로그에 알린다.
		}

		if(nStatus != 0)
		{
			GDM().SetINT64(GDK_SSOWEB_STATUS, nStatus);
		}

		int nType = atoi( (LPCTSTR)GetPostedData1( "type=", lpszURL ) );
		int nItemCode = atoi( (LPCTSTR)GetPostedData1( "Itemcode=", lpszURL ));
		int nItemNo = atoi( (LPCTSTR)GetPostedData1( "Itemno=", lpszURL ));

		GDM().SetINT64(GDK_SSOWEB_TYPE, nType);
		GDM().SetINT64(GDK_SSOWEB_ITEMCODE, nItemCode);
		GDM().SetINT64(GDK_SSOWEB_ITEMNO, nItemNo);

		if ( nStatus == 1 ) // 아이템 구매 관련
		{
			if ( nType == 1 ) // 기능 아이템 
			{
				CSV_ASK_GAMEITEMLIST agmsg;
				agmsg.Set();
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), agmsg.pData, agmsg.GetTotalSize());

				
				GM().PostMainWndMessage(UM_CLOSEALLPOPUP);	//팝업(상점, 찬스구매창)을 닫으라고 메인다이얼로그에 알린다.
			}
			else if ( nType == 2 ) // 아바타 아이템
			{
				CSV_ASK_GAME_CONTINUE msg;
				msg.Set(GM().GetMyInfo()->UI.ID, 0);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());				
			}			
		}
		else if ( nStatus == 2 ) // 아이템 사용 관련
		{
			if ( nType == 1 )
			{		
				//실전 채널 입장권								
				CSV_ASK_GAMEITEMLIST agmsg;
				agmsg.Set( 0, nItemNo );
	
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), agmsg.pData, agmsg.GetTotalSize());
			}
		}
		else if ( nStatus == 3 ) // 내아이템 탭 이동
		{
			//%%%추후 수정
// 			if ( g_pMainDlg->m_NewGameItemDlg.m_hWnd != NULL )
// 			{
// 				g_pMainDlg->m_NewGameItemDlg.ShowTab( 2 );
// 			}	
		}
	}	
	else if ( MacroString.Find( "_닫기_",0 ) >= 0 )
	{		
		GM().PostMainWndMessage(UM_CLOSEALLPOPUP);	//팝업(상점, 찬스구매창)을 닫으라고 메인다이얼로그에 알린다.
	}
	else if( MacroString.Find("_룰렛구매_") >= 0 ){
		if( IDYES == AfxMessageBox( "룰렛 찬스 아바타는 게임 종료 후 구매가 가능합니다.\n지금 게임을 종료하시겠습니까?", MB_YESNO ) ){
			g_bNoGo = FALSE;			
			GM().PostMainWndMessage(WM_COMMAND, IDCANCEL, 0);	

			ShellExecute(NULL,  "open", (char*)(LPCTSTR)getGameURL(IDX_GAMEURL_ROULETTE), NULL, NULL, SW_SHOWNORMAL);
		}		
		else{
			//g_pLobyDlg->LobyItemNavigate(TRUE);
		}

		*pbCancel =TRUE;
	}	
	else if( MacroString.Find( "_안심닫기_", 0 ) >= 0 )
	{
		// 머니 업데이트 과정. // 실제 아이템을 샀는지 아닌지 체크하고 샀다면 머니를 업데이트 한다.
		CSV_ASK_GAME_CONTINUE msg;
		msg.Set(GM().GetMyInfo()->UI.ID, 0);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	}

	//포커상점창 캐쉬충전탭닫기
	else if ( MacroString.Find("javascript:f_charge_complete()") >=0)
	{
		*pbCancel = TRUE;
		//포커상점창을 닫는다. 
		//MM().Call(UPG_CMK_GAMEITEM_DLG_CLOSE);
		GM().PostMainWndMessage(UM_CLOSEALLPOPUP);	//팝업(상점, 찬스구매창)을 닫으라고 메인다이얼로그에 알린다.
	}
}

CString GetPostedData( CString sMacroString, LPCTSTR lpszPostedData )
{
	int nLength = 0;
	CString sPostedData = lpszPostedData;
	nLength = sPostedData.GetLength();

	if ( sMacroString.GetLength() <= 0 || nLength <= 0 )
		return "";

	int nPos = sPostedData.Find( sMacroString );
	if ( nPos >= 0 )
	{
		int nPos1 = sPostedData.Find( "|", nPos );
		if ( nPos1 == -1 )
		{
			return sPostedData.Mid( nPos+sMacroString.GetLength(), sPostedData.GetLength()-(nPos+sMacroString.GetLength()) );
		}
		else
		{
			if ( nPos < nPos1 && nPos1 < nLength && nPos+sMacroString.GetLength() < nPos1 )
			{
				return sPostedData.Mid( nPos+sMacroString.GetLength(), nPos1-(nPos+sMacroString.GetLength()) );
			}
			else
			{
				return "";
			}
		}
	}

	return "";
}

CString GetPostedData1( CString sMacroString, LPCTSTR lpszPostedData )
{
	int nLength = 0;
	CString sPostedData = lpszPostedData;
	nLength = sPostedData.GetLength();

	if ( sMacroString.GetLength() <= 0 || nLength <= 0 )
		return "";

	int nPos = sPostedData.Find( sMacroString );
	if ( nPos >= 0 )
	{
		int nPos1 = sPostedData.Find( "&", nPos );
		if ( nPos1 == -1 )
		{
			return sPostedData.Mid( nPos+sMacroString.GetLength(), sPostedData.GetLength()-(nPos+sMacroString.GetLength()) );
		}
		else
		{
			if ( nPos < nPos1 && nPos1 < nLength && nPos+sMacroString.GetLength() < nPos1 )
			{
				return sPostedData.Mid( nPos+sMacroString.GetLength(), nPos1-(nPos+sMacroString.GetLength()) );
			}
			else
			{
				return "";
			}
		}
	}

	return "";
}

CString g_GetLevelName(int nLevel)
{
	if(nLevel== 0) return g_StrMan.Get(_T("LEVEL_NAME1"));	  
	if(nLevel== 1) return g_StrMan.Get(_T("LEVEL_NAME2"));
	if(nLevel== 2) return g_StrMan.Get(_T("LEVEL_NAME3"));
	if(nLevel== 3) return g_StrMan.Get(_T("LEVEL_NAME4"));
	if(nLevel== 4) return g_StrMan.Get(_T("LEVEL_NAME5"));
	if(nLevel== 5) return g_StrMan.Get(_T("LEVEL_NAME6"));
	if(nLevel== 6) return g_StrMan.Get(_T("LEVEL_NAME7"));
	if(nLevel== 7) return g_StrMan.Get(_T("LEVEL_NAME8"));
	if(nLevel== 8) return g_StrMan.Get(_T("LEVEL_NAME9"));
	if(nLevel== 9) return g_StrMan.Get(_T("LEVEL_NAME10"));
	if(nLevel==10) return g_StrMan.Get(_T("LEVEL_NAME11"));
	if(nLevel==11) return g_StrMan.Get(_T("LEVEL_NAME12"));
	if(nLevel==12) return g_StrMan.Get(_T("LEVEL_NAME13"));
	if(nLevel==13) return g_StrMan.Get(_T("LEVEL_NAME14"));
	if(nLevel==14) return g_StrMan.Get(_T("LEVEL_NAME15"));
	if(nLevel==15) return g_StrMan.Get(_T("LEVEL_NAME15"));
	if(nLevel==16) return g_StrMan.Get(_T("LEVEL_NAME15"));
	if(nLevel==17) return g_StrMan.Get(_T("LEVEL_NAME15"));
	if(nLevel==18) return g_StrMan.Get(_T("LEVEL_NAME15"));
	if(nLevel==19) return g_StrMan.Get(_T("LEVEL_NAME15"));

	return  g_StrMan.Get(_T("LEVEL_NAME15"));
}



IDX_GAME get_GameMode( int groupcode)
{
	// 	if( groupcode < GROUP_WINNER_SP ){
	// 		IDX_GAME game = g_GetGameFromGroup((GROUP_NO)groupcode);
	// 		if( IDX_GAME_NULL != game ){
	// 			return game;
	// 		}
	// 	}

	return IDX_GAME_SP;
}

INT64 get_BetMoney( int index , int GroupNo )
{
	INT64 BetMoney = 0;

	switch(index) {
	case 0:
		BetMoney = 10000000000;
		break;
	case 1:
		BetMoney = 100000000000;
		break;
	case 2:
		BetMoney = 300000000000;
		break;
	default:
		BetMoney = 10000000000;
		break;
	}

	return BetMoney;
}

CString	get_Numberstring(INT64 number)
{
	CString str; // 한글표시
	CString str1;
	INT64 t = number; 
	INT64 Money = number;		

	int num[4] = {0,};	
	NMBASE::UTIL::g_NumberToParse(Money, num[3], num[2], num[1], num[0]);

	if (num[3] > 0){
		str.Format("%dJ",num[3]);				
	}
	else if (num[2] > 0){
		str.Format("%dY",num[2]);				
	}else if (num[1] > 0){
		str.Format("%dM",num[1]);				
	}else if (num[0] > 0){
		str.Format("%dW",num[0]);				
	}

	return str;	
}

CString get_str_AutoBetRule( int autokind ,int game, int groupclass, BOOL bName )
{
	CString rstr,temp_str;
	rstr = temp_str = _T("");

	if( autokind == 0 )
	{
		if( bName == TRUE )rstr.Format("수동");
	}
	else{
		temp_str.Format("%d",autokind);

		char num[4] = {0,};		
		num[0] = temp_str.GetAt(2);
		int number_1 = atoi(num);
		if( number_1 == 0 )
		{			
			rstr = temp_str.GetAt(0);
			rstr +="9";
			rstr+="F";			
		}
		else{

			rstr = temp_str.GetAt(0);
			rstr +="9";
			rstr +=temp_str.GetAt(2);
		}
	}

	return rstr;
}

//챔피언쉽 채널에서 입장시 제한 머니 체크 후 입장  처리
BOOL  g_Ask_RealChannel_JoinRoomMsg()
{
	return TRUE;
}

int g_GetURLChannelNo()
{
	//웹 URL 에서 사용할  채널번호 구하기
// 	GROUP_GRADE GroupGrade =g_GetGroupGrade(g_NowGroupNo);
// 
// 	if (GroupGrade>=GROUP_GRADE_FREE && GroupGrade<=GROUP_GRADE_ROOKIE)
// 		return 1;
// 	else if (GroupGrade>=GROUP_GRADE_AMA && GroupGrade<=GROUP_GRADE_PRO)	
// 		return 2;
// 	else if (GroupGrade >= GROUP_GRADE_PLATINUM1 )
// 		return 3;

	return 1;
}

CString get_GroupName(int group)
{
	return CString("그룹이름 세팅");	
}

#include <nb30.h>
#pragma comment(lib, "netapi32")

CString GetMACAddress(const char *strIP) 
{
	NCB Ncb;
	UCHAR uRetCode;
	LANA_ENUM lenum;
	int i;
	CString strOutput =_T("");
	CString string;
	ADAPTER_STATUS Adapter;

	try
	{
		memset( &Ncb, 0, sizeof(Ncb) );
		Ncb.ncb_command = NCBENUM;
		Ncb.ncb_buffer = (UCHAR *)&lenum;
		Ncb.ncb_length = sizeof(lenum);
		uRetCode = Netbios( &Ncb );

		for(i=0; i < lenum.length ;i++)
		{
			memset( &Ncb, 0, sizeof(Ncb) );
			Ncb.ncb_command = NCBRESET;
			Ncb.ncb_lana_num = lenum.lana[i];

			uRetCode = Netbios( &Ncb );


			memset( &Ncb, 0, sizeof (Ncb) );
			Ncb.ncb_command = NCBASTAT;
			Ncb.ncb_lana_num = lenum.lana[i];

			//strcpy( (char*)Ncb.ncb_callname, strIP ); 
			strncpy((char*)Ncb.ncb_callname, strIP, NCBNAMSZ);
			Ncb.ncb_buffer = (unsigned char *) &Adapter;
			Ncb.ncb_length = sizeof(Adapter);

			uRetCode = Netbios( &Ncb );
			if ( uRetCode == 0 )
			{
				string.Format("%02X%02X%02X%02X%02X%02X",
					Adapter.adapter_address[0],
					Adapter.adapter_address[1],
					Adapter.adapter_address[2],
					Adapter.adapter_address[3],
					Adapter.adapter_address[4],
					Adapter.adapter_address[5] );
				strOutput += string ;
			}
		}
	}
	catch(...)
	{
		return strOutput;
	}

	return strOutput;
}

//CString getGameString(IDX_GAME nGameType/* = IDX_GAME_NULL*/)
//{
//	CString strRet = "";
//	if(nGameType == IDX_GAME_NULL) nGameType = GM().GetCurrentGameType();
//
//	switch(nGameType)
//	{
//	case IDX_GAME_SP:
//		strRet = "spoker2";
//		break;
//	case IDX_GAME_BD:
//		strRet = "low";
//		break;
//	case IDX_GAME_NP:
//		strRet = "newpoker";
//		break;
//	default:
//		ASSERT(FALSE);
//		break;
//	}
//
//	return strRet;
//}
//
//int getGameCode(IDX_GAME nGameType/* = IDX_GAME_NULL*/)
//{
//	int nRet = -1;
//	if(nGameType == IDX_GAME_NULL) nGameType = GM().GetCurrentGameType();
//
//	switch(nGameType)
//	{
//	case IDX_GAME_SP:
//		nRet = 42;
//		break;
//	case IDX_GAME_BD:
//		nRet = 54;
//		break;
//	case IDX_GAME_NP:
//		nRet = 61;
//		break;
//	default:
//		ASSERT(FALSE);
//		break;
//	}
//
//	return nRet;
//}

//int	getJokboQuestCount(IDX_GAME nGameType/* = IDX_GAME_NULL*/)
//{
//	int nRet = 6;
//	if(nGameType == IDX_GAME_NULL) nGameType = GM().GetCurrentGameType();
//
//	switch(nGameType)
//	{
//	case IDX_GAME_SP:
//		nRet = 6;
//		break;
//	}
//	return nRet;
//}

BOOL IsValidObserverChat()
{	
	if( g_bMyObserver ){
		return FALSE;
	}		
	return TRUE;		 
}

void ClearLoadImageList(void)
{
	MM().Call(UPG_CMK_LOAD_IMAGE_LIST_CLEAER);
}

LRESULT AddLoadImageList(NMBASE::GRAPHICGDI::xSprite *pSprite, const char *pPath, bool bRegisterGameSprite/* = false*/)
{
	ASSERT(pSprite != NULL && pPath != NULL);
	if(bRegisterGameSprite) GM().AppendLoadSprite(pSprite);
	return MM().Call(UPG_CMK_LOAD_IMAGE_LIST_ADD, (WPARAM)pSprite, (LPARAM)pPath);
}

void LoadingImageList(DWORD dwMinShowTime, BOOL bBasedOnTime)
{
	MM().Call(UPG_CMK_LOADING_IMAGE_LIST, (WPARAM)dwMinShowTime, (LPARAM)bBasedOnTime);
}

LRESULT SetChangeWhere(IDX_GAMEWHERE nView, bool bRefresh)
{
	LRESULT nRet = MM().Call(UPG_CMK_CHANGE_VIEW, (WPARAM)nView,(LPARAM)bRefresh?1:0); 
	return nRet;
}

IDX_GAMEWHERE GetCurrentWhere()
{
	return GM().GetCurrentWhere();
}

LRESULT ShowMainMessageDlg(const char *pMsg)
{
	ASSERT(pMsg != NULL);
	return MM().Call(UPG_CMK_MAIN_MESSAGE_SHOW, NULL, (LPARAM)pMsg);
}

LRESULT ShowMsgChkDlg(const char *pMsg, int nType)
{
	ASSERT(pMsg != NULL);
	return MM().Call(UPG_CMK_MSG_CHK_SHOW, (WPARAM)pMsg, (LPARAM)nType);
}


LRESULT ShowMsgModelessDlg(HWND parentHWnd, LPCTSTR msg)
{
	return MM().Call(UPG_CMK_SHOW_MESSAGE_MODELESS_DLG, (WPARAM)parentHWnd, (LPARAM)msg);
}

void CloseMainMessageDlg(void)
{
	MM().Call(UPG_CMK_MAIN_MESSAGE_CLOSE,NULL,NULL);
	MM().Call(UPG_CMK_HIDE_RESTRICTION_DIALOG,NULL,NULL);
}

void ClearMainMessageDlg(void)
{
	MM().Call(UPG_CMK_MAIN_MESSAGE_CLEAR,NULL,NULL);
}

bool IsVisibleMainMessageDlg(void)
{
	return MM().Call(UPG_CMK_MAIN_MESSAGE_ISVISIBLE,(WPARAM)0,(LPARAM)0)?true:false;
}

void ShowTipManDlg(void)
{
	MM().Call(UPG_CMK_SHOW_TIPMAN_DLG);
}

void ShowDirectGoDlg(void)	///내맘대로 바로가기 호출
{
	MM().Call(UPG_CMK_SHOW_DIRECTGO_DLG);
}

void ShowSelectStartDlg(void)	///선택시작
{
	MM().Call(UPG_CMK_SHOW_SELECTSTART_DLG);
}

void ShowResetSelectStartDlg(void)	///선택시작
{
	MM().Call(UPG_CMK_SHOW_RESETSELECTSTART_DLG);
}

//내맘대로 바로가기 패킷을 보낸다.
bool SendSelectStart(void)
{
	return GM().SendQuickJoiner()?true:false;
}

LRESULT ShowQuickJoinerDlg(void)
{
	return GM().ShowQuickJoinerDlg();
}

//내맘대로 바로가기 패킷을 보낸다.
//내맘대로 바로가기가 설정되어 있지 않다면 false를 리턴한다.
bool SendQuickJoiner(void)
{
	return GM().SendQuickJoiner()?true:false;
}

void CloseLoadingDlg(bool bCancle/* = true*/)
{
	MM().Call(UPG_CMK_CLOSE_LOADING_DLG, (WPARAM)bCancle?1:0);
}

LRESULT SaveGameConfig(LPCTSTR lpID)
{
	ASSERT(lpID != NULL);
	return MM().Call(UPG_CMK_SAVE_CONFIG,(WPARAM)0,(LPARAM)lpID);
}

LRESULT LoadGameConfig(LPCTSTR lpID)
{
	ASSERT(lpID != NULL);
	return MM().Call(UPG_CMK_LOAD_CONFIG,(WPARAM)0,(LPARAM)lpID);
}


void RefreshGameItemDlg_MyItem(void)
{
	MM().Call(UPG_CMK_GAMEITEM_DLG_REFRESH_MYITEM);
}

void CloseGameItemDlg(void)
{
	MM().Call(UPG_CMK_GAMEITEM_DLG_CLOSE);
}

void ShowGameItemDlg(bool bForceClose)
{
	MM().Call(UPG_CMK_GAMEITEM_DLG_SHOW,(WPARAM)bForceClose?1:0);
}

void ExitGameClient(bool bShowEndPopup)
{
	SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_TITLEBAR_CLOSE);

	MM().Call(UPG_CMK_CLICK_LOBBY_EXIT_BUTTON,(WPARAM)bShowEndPopup?1:0,(LPARAM)0);
}

void SetMainWindowText(LPCTSTR lpText)
{
	MM().Call(UPG_CMK_SET_MAINWINDOW_TEXT,(WPARAM)0,(LPARAM)lpText);
}

CWnd *GetLobbyWindow(void)
{
	CWnd *pLobby = NULL;
	MM().Call(UPG_CMK_GET_LOBBY_WINDOW, 0, (LPARAM)&pLobby);
	return pLobby;
}

CObserverDlg *GetObserverDlg(void)
{
	CObserverDlg *pObserverDlg = NULL;
	MM().Call(UPG_CMK_GET_OBSERVER_DLG,(WPARAM)0,(LPARAM)&pObserverDlg);
	return pObserverDlg;
}

CInviteDlg *GetInviteDlg(void)
{
	CInviteDlg *pInviteDlg = NULL;
	MM().Call(UPG_CMK_GET_INVITE_DLG,(WPARAM)0,(LPARAM)&pInviteDlg);
	return pInviteDlg;
}

CEndEventPopupDlg *GetEndEventDlg(void)
{
	CEndEventPopupDlg *pEndEventDlg = NULL;
	MM().Call(UPG_CMK_GET_ENDEVENT_DLG,(WPARAM)0,(LPARAM)&pEndEventDlg);
	return pEndEventDlg;
}

CProfileDlg *GetProfileDlg(void)
{
	CProfileDlg *pProfileDlg = NULL;
	MM().Call(UPG_CMK_GET_PROFILE_DLG,(WPARAM)0,(LPARAM)&pProfileDlg);
	return pProfileDlg;
}

CWhisperDlg *GetWhisperDlg(void)
{
	CWhisperDlg *pWhisperDlg = NULL;
	MM().Call(UPG_CMK_GET_WHISPER_DLG,(WPARAM)0,(LPARAM)&pWhisperDlg);
	return pWhisperDlg;
}

CReceiveMessageDlg *GetReceiveMessageDlg(void)
{
	CReceiveMessageDlg *pReceiveMessageDlg = NULL;
	MM().Call(UPG_CMK_GET_RECEIVEMESSSAGE_DLG,(WPARAM)0,(LPARAM)&pReceiveMessageDlg);
	return pReceiveMessageDlg;
}

CReceiveAdminMessageDlg *GetReceiveAdminMessageDlg(void)
{
	CReceiveAdminMessageDlg *pReceiveAdminMessageDlg = NULL;
	MM().Call(UPG_CMK_GET_RECEIVEADMINMESSSAGE_DLG,(WPARAM)0,(LPARAM)&pReceiveAdminMessageDlg);
	return pReceiveAdminMessageDlg;
}

CSendMessageDlg *GetSendMessageDlg(void)
{
	CSendMessageDlg *pSendMessageDlg = NULL;
	MM().Call(UPG_CMK_GET_SENDMESSSAGE_DLG,(WPARAM)0,(LPARAM)&pSendMessageDlg);
	return pSendMessageDlg;
}

CMessageBox *GetMessageBoxDlg(void)
{
	CMessageBox *pMessageBoxDlg = NULL;
	MM().Call(UPG_CMK_GET_MESSSAGEBOX_DLG,(WPARAM)0,(LPARAM)&pMessageBoxDlg);
	return pMessageBoxDlg;
}

void BanishKickOut(const char *pTargetID)
{
	MM().Call(UPG_CMK_KICK_OUT, (WPARAM)0, (LPARAM)pTargetID);
}

void AddLobbyHideChatText(CString *pMsg)
{
	GM().AddHideChatText(pMsg);
}

void AddLobbyChatViewEdit(const char *str, COLORREF crNewColor/*=RGB(0,0,0)*/)
{
	ASSERT(str != NULL);
	MM().Call(UPG_CMK_ADD_LOBBY_CHATVIEW_MESSAGE,(WPARAM)crNewColor,(LPARAM)str);
}

void AddGameInfoViewEdit(const char *str, COLORREF crNewColor/*=RGB(0,0,0)*/, bool bUserChat/* = false*/)
{
	GM().AddGameInfo(str, crNewColor,bUserChat);
}

void AddGameChatViewEdit(const char *str, COLORREF crNewColor/*=RGB(0,0,0)*/, bool bUserChat/* = false*/)
{
	GM().AddGameChat(str, crNewColor,bUserChat);
}

void SetAccessRoomState(bool bAccessRoomState)
{
	s_bAccessRoomState = bAccessRoomState;
}

bool IsAccessRoomState(void)
{
	return s_bAccessRoomState;
}

void ResetBadUserTimer(void)
{
	MM().Call(UPG_CMK_RESET_BADUSERTIMER);
}

void ExitGameRequest(bool bUserAFK)
{
	//bUserAFK : Away from Keyboard - 유저가 2,3회 이상 버튼클릭이없어 나갈때는 true;
	
	GM().ExitGame(bUserAFK);
}

void SetFullScreenState(bool bFullScreen)
{
	s_bFullScreenState = bFullScreen;
}

bool IsFullScreenState(void)
{
	return s_bFullScreenState;
}

void CloseClient(void)
{
	GM().PostMainWndMessage(WM_CLOSE,(WPARAM)0,(LPARAM)0);
}

bool ChangeDisplayMode(void)
{
	return MM().Call(UPG_CMK_CHANGE_DISPLAY_MODE)?true:false;
}

void SaveScreenCapture(void)
{
	MM().Call(UPG_CMK_SCREEN_CAPTURE);
}

bool CheckBadUserReport(void)
{
	return MM().Call(UPG_CMK_CHECK_BADUSER_REPORT)?true:false;
}

LRESULT ShowNewAskChargeDlg(NEW_ASK_CHARGE_DLG_TYPE type, LPCTSTR lpMessage)
{
	return MM().Call(UPG_CMK_SHOW_NEWASKCHARGEDLG,(WPARAM)type,(LPARAM)lpMessage);
}

void ShowChanceBtn(void)
{
	MM().Call(UPG_CMK_SHOW_CHANCE_BUTTON);
}

void AddSelectUserDlg(CAddUserSelectDlg *pDlg)
{
	MM().Call(UPG_CMK_SHOW_ADDUSERSELECTDLG, 0, (LPARAM)pDlg);
}

void SubSelectUserDlg(CAddUserSelectDlg *pDlg)
{
	MM().Call(UPG_CMK_CLOSE_ADDUSERSELECTDLG, 0, (LPARAM)pDlg);
}

bool IsShowLoadingUserList(void)
{
	return MM().Call(UPG_CMK_IS_SHOW_LOADING_USERLIST)?true:false;
}

void SelChangeGameAddSelectUserDlg(IDX_GAME nGame)
{
	MM().Call(UPG_CMK_ADDUSERSELECTDLG_SELCHANGE_GAME, 0, (LPARAM)nGame);
}

void SelChangeMoneyAddSelectUserDlg(int nIndex)
{
	MM().Call(UPG_CMK_ADDUSERSELECTDLG_SELCHANGE_MONEY, 0, (LPARAM)nIndex);
}

SMALLUSERINFO *GetUserSmallInfo(const char *pID)
{
	ASSERT(pID != NULL);
	return (SMALLUSERINFO *)MM().Call(UPG_CMK_GET_USER_SMALLINFO,(WPARAM)0,(LPARAM)pID);
}

BOOL UPGIsGameItemUsing(int itemcode, BOOL *pExist)
{
	SYSTEMTIME stTime;
	g_GetSyncTime().GetAsSystemTime(stTime);

	return GameItem_IsUsingItem(g_MyGameItem, itemcode, pExist, &stTime);
}

int UPGCheckPremiumState(USERINFO* pUI)
{
	SYSTEMTIME stTime;
	g_GetSyncTime().GetAsSystemTime(stTime);
	return CheckPremiumState(pUI, &stTime);
}

GAMEITEM *UPGNotUseGameItem(int itemcode)
{
	COleDateTime time = COleDateTime::GetCurrentTime() - g_CSTimeGap;
	SYSTEMTIME st;
	time.GetAsSystemTime(st);

	for(int i=0; i<g_MyGameItem.nNum; i++) {
		ItemState state = GameItem_GetItemState(g_MyGameItem.Item[i], &st);
		if(state.iState == 1) {
			if(itemcode == GameItem_GetItemKind(g_MyGameItem.Item[i].Code) ) 
			{
				return &g_MyGameItem.Item[i];
			}
		}
	}
	return NULL;
}

CString UPGGetAutoBettingRuleName(IDX_GAME nGame,  int nAutoBet, BETRULE_KIND eBetRuleKind, RULETYPE ruleType)
{
	return GetAutoBettingRuleName( nGame, nAutoBet, eBetRuleKind, ruleType );
}

void DrawGameText(CDC &dc, IDX_GAME idxGame, LPRECT rt, UINT nFormat)
{
	if (idxGame <0 || idxGame>=MAX_GAME)
		return;

	CGame *pGame = GM().GetGame(idxGame);
	if (pGame==NULL)
		return;

	COLORREF oldcolor = dc.SetTextColor(pGame->GetGameColor());
	
	CString strGameName = pGame->GetGameName();
	if (strGameName.GetLength()>9)
	{
		int nOldExtra = SetTextCharacterExtra(dc.m_hDC, -1);	
		strGameName=" "+strGameName;
		dc.DrawText(strGameName, rt, nFormat);
		SetTextCharacterExtra(dc.m_hDC, nOldExtra);
		
	}
	else
	{
		dc.DrawText(strGameName, rt, nFormat);
	}

	dc.SetTextColor(oldcolor);

}

void OpenGameManualPage()
{
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
	{
		AfxMessageBox("준비중입니다.");
		return;
	}
#endif //UPGRADE_20120308_TOCTOC_CHANNELING

	NMBASE::UTIL::CShellMan ShellMan;	
	NMBASE::UTIL::BROWSEROPTION boption;
	ZeroMemory(&boption, sizeof(boption));
	boption.bAddressBar=FALSE;
	boption.bMenuBar=FALSE;
	boption.bResizable=FALSE;
	boption.bStatusBar=FALSE;
	boption.bToolBar=FALSE;
	boption.nHeight=720;
	boption.nWidth=1015;
	
	
	static HWND hWnd = NULL;

	CRect rt;
	GM().GetMainWnd()->GetWindowRect(&rt);

	if (hWnd==NULL || IsWindow(hWnd)==FALSE)
	{

		hWnd = ShellMan.OpenNewBrowser(getGameURL(IDX_GAMEURL_GAMEMANUAL), &boption);	
		
	}	

	if (hWnd && IsWindow(hWnd))
		::SetWindowPos(hWnd, HWND_TOP, rt.left + 40, rt.top+ 40, 0, 0, SWP_NOSIZE);

	

}

void OpenPCRoomAdvantagePage(int nPCRoomCode)
{
	NMBASE::UTIL::CShellMan ShellMan;	
	NMBASE::UTIL::BROWSEROPTION boption;
	ZeroMemory(&boption, sizeof(boption));
	boption.bAddressBar=FALSE;
	boption.bMenuBar=FALSE;
	boption.bResizable=FALSE;
	boption.bStatusBar=FALSE;
	boption.bToolBar=FALSE;
	boption.nHeight=838;
	boption.nWidth=1022;
	

	static HWND hWnd = NULL;

	CRect rt;
	GM().GetMainWnd()->GetWindowRect(&rt);

	if (hWnd==NULL || IsWindow(hWnd)==FALSE)
	{

		hWnd = ShellMan.OpenNewBrowser(getGameURL(IDX_GAMEURL_PCROOM_ADVANTAGE), &boption);	

	}	

	if (hWnd && IsWindow(hWnd))
		::SetWindowPos(hWnd, HWND_TOP, rt.left + 40, rt.top+ 40, 0, 0, SWP_NOSIZE);

}

void AskProfileToServer(UINT64 llUserKey, const char* pID, const char *pUniqNo/*=""*/)
{
	CSV_ASK_USERINFO aumsg;
	aumsg.Set(llUserKey , (char*) pID, (char *)pUniqNo);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), aumsg.pData, aumsg.GetTotalSize());	
}


void ShowProfileDialog(USERINFO *pUserinfo, CWnd *pParent, ENUM_PROFILE_OPEN_KIND enumProfileOpenKind)
{
	GM().SetProfileOpenKind(enumProfileOpenKind);
	MM().Call(UPG_CMK_SHOW_PROFILE_DIALOG, (WPARAM)pUserinfo, (LPARAM)pParent);
}
void CloseProfileDialog(void)
{
	MM().Call(UPG_CMK_CLOSE_PROFILE_DIALOG);
}

void ShowWhisperDialog(SMALLUSERINFO *pInfo, CWnd *pParent)
{
	MM().Call(UPG_CMK_SHOW_WHISPER_DIALOG, (WPARAM)pInfo, (LPARAM)pParent);
}

void CloseWhisperDialog(void)
{
	MM().Call(UPG_CMK_CLOSE_WHISPER_DIALOG);
}

void ShowReceiveMessageDialog(Camel::Dar2::SS::Memo *pMemo, CWnd *pParent)
{
	MM().Call(UPG_CMK_SHOW_RECEIVEMESSAGE_DIALOG, (WPARAM)pMemo, (LPARAM)pParent);
}

void CloseReceiveMessageDialog(void)
{
	MM().Call(UPG_CMK_CLOSE_RECEIVEMESSAGE_DIALOG);
}

void ShowSendMessageDialog( LPCTSTR lpFriendNickName, bool bGroup )
{
	MM().Call(UPG_CMK_SHOW_SENDMESSAGE_DIALOG, (WPARAM)lpFriendNickName, (LPARAM)bGroup );
}

void CloseSendMessageDialog(void)
{
	MM().Call(UPG_CMK_CLOSE_SENDMESSAGE_DIALOG);
}

void CloseMessageBoxDialog(void)
{
	MM().Call(UPG_CMK_CLOSE_MESSAGEBOX_DIALOG);
}

void SetUserListTabType(ENUM_SHOW_USERLIST_TYPE nType)
{
	MM().Call(UPG_CMK_SET_USER_LIST_TAB_TYPE, (WPARAM)nType);
}

ENUM_SHOW_USERLIST_TYPE GetUserListTabType(void)
{
	return (ENUM_SHOW_USERLIST_TYPE)MM().Call(UPG_CMK_GET_USER_LIST_TAB_TYPE);
}

void SendCreateChatting(std::list<std::string> *pListInviteID, char *pNotConnectMessage)
{
	MM().Call(UPG_CMK_SEND_CREATE_CHATTING, (WPARAM)pListInviteID, (LPARAM)pNotConnectMessage);
}

bool CheckShowCommunityWindow(void)
{
	return MM().Call(UPG_CHECK_SHOW_COMMUNITY_WINDOW)?true:false;
}

void AddChattingWhisperList(LPCTSTR lpNickName, bool bSetCurSel, bool bStartAnimation, bool bAllUser)
{
	WPARAM wParam = 0;
	if(bSetCurSel) wParam |= 0x00000001;
	if(bStartAnimation) wParam |= 0x00000002;
	if(bAllUser) wParam |= 0x00000004;
	MM().Call(UPG_ADD_CHATTING_WHISPER_LIST, wParam, (LPARAM)lpNickName);
}

bool DebugLog(LPCTSTR szLogFile, LPCTSTR szLogMsg, ...)
{
#ifndef _DEBUG
	return true;
#endif 

	CString strBuffer;
	va_list argptr;

	va_start(argptr, szLogMsg);
	strBuffer.FormatV(szLogMsg, argptr);	
	va_end(argptr);

	CFile file;

	if (!file.Open(szLogFile, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate))
	{
		return false;
	}
	file.SeekToEnd();

	CString strID, strNick, strMoney;
	CPlayer *pPlayer = GM().GetMyInfo();
	if (pPlayer)
	{
		strID = pPlayer->UI.ID;
		strNick = pPlayer->UI.NickName;
		//strMoney.Format("%s" , NMBASE::UTIL::g_MoneyMark(pPlayer->UI.GetMoney(IsTournament())));
		strMoney.Format("%s" , NMBASE::UTIL::g_MoneyMark(pPlayer->UI.GetMoney()));
	}

	COleDateTime timeNow= COleDateTime::GetCurrentTime();
	CString strMsg;
	strMsg.Format("[%s] [%s] [%s] [%s원] : %s", timeNow.Format("%y/%m/%d %H:%M:%S"), strID, strNick, strMoney, strBuffer);

	file.Write(strMsg, strMsg.GetLength());
	file.Write("\r\n",2);

	file.Close();


	return true;

}

char* ANSIToUTF8(const char * pszCode)
{
	int     nLength, nLength2;
	BSTR    bstrCode;
	char*   pszUTFCode = NULL;
	nLength = MultiByteToWideChar(CP_ACP, 0, pszCode, lstrlen(pszCode), NULL, NULL);
	bstrCode = SysAllocStringLen(NULL, nLength);
	MultiByteToWideChar(CP_ACP, 0, pszCode, lstrlen(pszCode), bstrCode, nLength);
	nLength2 = WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, pszUTFCode, 0, NULL, NULL);
	pszUTFCode = (char*)malloc(nLength2+1);
	WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, pszUTFCode, nLength2, NULL, NULL);
	return pszUTFCode;
}

char* URLencode(const char *str)
{
	char *encstr, buf[2+1];
	unsigned char c;
	int i, j;

	if(str == NULL) return NULL;
	if((encstr = (char *)malloc((strlen(str) * 3) + 1)) == NULL) 
		return NULL;
	for(i = j = 0; str[i]; i++){
		c = (unsigned char)str[i];
		if((c >= '0') && (c <= '9')) encstr[j++] = c;
		else if(c == ' ') encstr[j++] = '+';    // 웹에 맞게 추가
		else if((c >= 'A') && (c <= 'Z')) encstr[j++] = c;
		else if((c >= 'a') && (c <= 'z')) encstr[j++] = c;
		else if((c == '@') || (c == '.') || (c == '/') || (c == '\\')
			|| (c == '-') || (c == '_') || (c == ':') ) 
			encstr[j++] = c;
		else{
			sprintf(buf, "%02x", c);
			encstr[j++] = '%';
			encstr[j++] = buf[0];
			encstr[j++] = buf[1];
		}
	}
	encstr[j] = '\0';
	return encstr;
}