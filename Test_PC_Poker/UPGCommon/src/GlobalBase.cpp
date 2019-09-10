#include "StdAfx.h"
#include "GlobalBase.h"
#include "MsgBoxDlg.h"
//#include "MsgBoxChkDlg.h"
#include "MsgBoxModelessDlg.h"
#include <math.h>
#include <CommMsgDef.h>

//NMBASE::SOCK::CSockMan			NMBASE::SOCK::GSOCKMAN();			// ���� �޴��� Ŭ����
NMBASE::UTIL::CNoChat				NoChat;
CAdminMessageDlg	*g_pAMes	= NULL; // [������ ��� �۾�]

//CTodayTipDlg		g_TipManDlg;			// ��

CString				g_ServIP;	//������ ������ IP
UINT				g_ServPort=0;		//����׿� ������ ������ ��Ʈ��ȣ
CString				g_AuthCookie;				// ���� ��Ű						[SSO �۾�]
CString				g_DataCookie;				// ������ ��Ű						[SSO �۾�]
CString				g_CpCookie;					// CP��Ű							[SSO �۾�]
CString				g_SpareParam;				// ������ ����(CP���ӵ�� ���)	[SSO �۾�]
CString				g_LoginID;	// �ڵ� �α��� ID/
CString				g_LoginPass;		// �ڵ� �α��� �н�����
int					g_LoginGroup = 0;			// �α��ν� ��û�� �׷�
int					g_LoginGameCode = 0;		// �α��ν� ��û�� �����ڵ�
CString				g_MyUniqNo;					// ����Ʈ�� ����

Ms_DestServerInfo	g_DestSvInfo	= {0,};		// ������ ������ ���� ����
int 				g_NowGroupNo;				// ������ �׷� ��ȣ
IDX_GAMERECORD		g_NowGameRecord;			// ������ ä�� ���
IDX_GROUPCLASS		g_NowGroupClass;			// ������ �׷챺(�������� ���� ����)

int 				g_NowChanNo;				// ������ ���� ä�� ��ȣ
CString				g_NowChanName;				// ������ ä�� �̸�
CString				g_strSrvInfo;				// ä�� ����

int					g_MSCode		= 0;		// 0: Korean 1: English 2:Japanese 3:Chinese
NMBASE::UTIL::CStringManager			g_StrMan;

int					g_MasterSID = -1;			// ������ ������ ���� ��ȣ
int					g_MainSrvSID = 0;			// ���� ������ ���� ��ȣ

CString				g_strMasterServerName;		// ������ ������ ���� ��ȣ
CString				g_strConnectecMasterServerName;
CString				g_strMainServerName;		// ���� ������ ���� ��ȣ



CRect				g_OrgMainDlgRect;			// ���� ���̾�α��� ���� ũ��
BOOL				g_bNoGo = FALSE;
BOOL				g_bChannel = FALSE;
int					g_MyPN = 0;					// ���� �÷��̾� ��ȣ(0-2)
int					g_MastPN = 0;				// ������ �÷��̾� ��ȣ(0-2)
ROOMINFO			g_RI={0,};					// �� ���� ����ü
NMBASE::GRAPHICGDI::CDisplay			Display;					// ���÷��� ��� ���� Ŭ����

BOOL				g_bMyObserver	= FALSE;	//�������ΰ�
BOOL				g_bAdminUser	= FALSE;	// [������ ��� �۾�]
BOOL				g_bConfirmation = FALSE;	//[������ ���� Ȯ��]

GAMEITEM_LIST		g_MyGameItem;				// �� ���� ������
COleDateTimeSpan	g_CSTimeGap;				// ������ Ŭ���̾�Ʈ�� �ð���

DWORD				g_nStartTick		= 0;	// �������� �ð� ���� ��
DWORD				g_DebugStartTick	= 0;	// �������� �ð� ���� ��
bool				g_bLobyItemFirstNavigation = false;	// [������ ����ȭ ����]

POKERGAME			g_Poker;
CString				g_TargetID;					// [�ݸ��� ���Ȱ�]

INT64				g_MaxServerMoney=0;	//���� ���� �� �ִ� �ִ� �Ӵ� (�α��ν� �������� ����)
INT64				g_MaxServerGold=0;	//���� ���� �� �ִ� �ִ� ��� (�α��ν� �������� ����)
INT64				g_MaxServerGoldBank=0;	//���� ���� �� �ִ� �ִ� ��� ��ũ ������ (�α��ν� �������� ����)
INT64				g_DisplayMaxMoney[MAX_CODE_NEW_FAMILY]={0,};//�йи� ȸ���� �ִ� �����Ӵ� (�α��ν� �������� ����)
INT64				g_DisplayMaxGold[MAX_CODE_NEW_FAMILY]={0,};//�йи� ȸ���� �ִ� ������� (�α��ν� �������� ����)
BOOL				g_bNewlySignedUp=FALSE;

CHAT_MACRO	g_ChatMacro;				// ��ũ��(ä�� ����)
float		g_fRaceBetRatio	= 0.99f;
GAMESPEED	g_GameSpeed;

BOOL		g_bJumpItemBuying		= FALSE;
BOOL		g_bRealAdmissionBuying	= FALSE; //���� ä�� �����

BOOL		g_bAskContinue			= FALSE; 

MYGAMEINOF	g_MyGameInfo;	//���� ��� �ɼ� �� ���� �Ѵ�.
PREMOPTION	g_PremOption;//�����̾� �ɼ�
jackpot::JP_MONEYINFO g_JackPotMoneyInfo;	// ���̱ݾ� ����

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

// ���� ���� ���� ���ϱ�
double GetRadian(int x1, int y1, int x2, int y2)
{
	// �ȼ� �Ÿ�
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

	// ���� ���ϱ�
	double al = (double)abs(h)/(double)abs(w);

	double rad = atan(al);

	if(w==0 && h==0) return 0;
	else if(w>=0 && h>=0) {		// 1��и�
	}
	else if(w<0 && h>=0) {	// 2��и�
		rad = (180.0f * RADIAN) - rad;
	}
	else if(w<0 && h<0) {	// 3��и�
		rad += 180.0f * RADIAN;
	}	
	else if(w>=0 && h<0) {	// 4��и�
		rad = (360.0f * RADIAN) - rad;
	}

	return rad;
}


float GetAngle(int x1, int y1, int x2, int y2)
{
	// �ȼ� �Ÿ�
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

	// ���� ���ϱ�
	double al = (double)abs(h)/(double)abs(w);
	float angle = (float)(atan(al) / RADIAN);

	if(w==0 && h==0) return 0;
	else if(w>=0 && h>=0) {		// 1��и�
	}
	else if(w<0 && h>=0) {	// 2��и�
		angle = 180.0f - angle;
	}
	else if(w<0 && h<0) {	// 3��и�
		angle += 180.0f;
	}	
	else if(w>=0 && h<0) {	// 4��и�
		angle = 360.0f - angle;
	}

	return angle;
}

// ������ �Ÿ� ����
float GetDistance(int x1, int y1, int x2, int y2)
{
	float len = 0;

	// �ȼ� �Ÿ�
	int w = abs(x2-x1);
	int h = abs(y2-y1);

	if(w==0 && h==0) return 0;
	else if(w==0) len = (float)h;
	else if(h==0) len = (float)w;
	else len = (float)sqrt((float)(w*w + h*h));

	return len;
}

// �̵��� �ȼ� ��ǥ���� �Ѵ�.
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

// ���ذ��� ���� ��밢�� �������ϱ�
// sangle : ���� ������ ���� , rangle :  �� ������ ���� (���� ������ ��������)
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


		// ó�� �ֻ��� �����츦 ��Ͽ��� ���´�.	
		CWnd *pWnd = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);

		CString strCaption;

		while(pWnd){

			if(!pWnd->GetOwner() && pWnd->GetWindowTextLength()){

				pWnd->GetWindowText(strCaption); // �������� ĸ���� ����			

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
								//��� ������^
								if(bclose == FALSE)
								{
									//	AfxMessageBox(strCaption);
									ShowMainMessageDlg("���������� ���α׷��� ���� �Ǿ����ϴ�.\n���������� ���α׷� ���� �������� ������ �� �ֽ��ϴ�.");
									return TRUE;
								}
								else
								{
									ShowMainMessageDlg("���������� ���α׷� ���� ���ӿ� ���� �Ͻ� �� �����ϴ�. \n���������� ���α׷��� �����Ͽ� �ֽñ� �ٶ��ϴ�.");

									if( g_MainSrvSID == -1 ){
										CloseClient();
										return TRUE;
									}

									CPlayer *pMyInfo = GM().GetMyInfo();

									CSV_ASK_LOGOUT alomsg;
									// ### [ ������� ] ###
									alomsg.Set(pMyInfo->UI.UNum, pMyInfo->UI.ID);
									NMBASE::SOCK::GSOCKMAN().SendData(g_MainSrvSID, alomsg.pData, alomsg.GetTotalSize());							

									return TRUE;
								}		
							}
						}
					}				
				}
			}		
			// ������ ��Ͽ��� ���� �����츦 ���´�.		
			pWnd = pWnd->GetWindow(GW_HWNDNEXT);		
		}	

		return FALSE;
	*/
}

//	dest_str �� compare_str�� ���ڰ� ���� �Ǿ� �ִ����� �˻��Ѵ�
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
	//���ǿ��� ���� �ǻ� (Ŭ����)
	else if ( MacroString.Find( "_AppPopup_", 0 ) >= 0 ) 
	{
		CString strURL = GetPostedData( "URL=", lpszURL );
		GDM().SetString(GDK_SSOWEB_URL, (LPCTSTR)strURL);
		int nType = atoi( (LPCTSTR)GetPostedData( "Type=", lpszURL ));
		GDM().SetINT64(GDK_SSOWEB_TYPE, nType);
		GDM().SetINT64(GDK_SSOWEB_ITEMCODE, atoi( (LPCTSTR)GetPostedData( "Itemcode=", lpszURL ) ));

		// [�����ۼ� Tab �߰� ����]
		int Tab				 = 0;
		Tab = atoi( (LPCTSTR)GetPostedData( "tab=", lpszURL ) );

		*pbCancel = TRUE;

		if ( nType == 1 ) // ������ ������
		{
			MM().Call(UPG_CMK_AVATAR_SHOP, Tab, (LPARAM) strURL.operator LPCTSTR());
		}
		else if ( nType == 2 )
		{
			MM().Call(UPG_CMK_CHANCEITEMSHOP);
		}
		else if ( nType == 3 )
		{
			MM().Call(UPG_CMK_ITEMSHOP, 1);	///1������ ��Ŀ������ �����ش�.
		}
	}
	// [���Ӿ�����â ������] 2006.04.28
	else if ( MacroString.Find( "Status=", 0 ) >= 0 )
	{
		CString status = GetPostedData1( "Status=", lpszURL );
		int nStatus = 0;

		if ( status == "_buyend_" )//���Ű� ������
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
			GM().PostMainWndMessage(UM_CLOSEALLPOPUP);	//�˾�(����, ��������â)�� ������� ���δ��̾�α׿� �˸���.
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

		if ( nStatus == 1 ) // ������ ���� ����
		{
			if ( nType == 1 ) // ��� ������ 
			{
				CSV_ASK_GAMEITEMLIST agmsg;
				agmsg.Set();
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), agmsg.pData, agmsg.GetTotalSize());

				
				GM().PostMainWndMessage(UM_CLOSEALLPOPUP);	//�˾�(����, ��������â)�� ������� ���δ��̾�α׿� �˸���.
			}
			else if ( nType == 2 ) // �ƹ�Ÿ ������
			{
				CSV_ASK_GAME_CONTINUE msg;
				msg.Set(GM().GetMyInfo()->UI.ID, 0);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());				
			}			
		}
		else if ( nStatus == 2 ) // ������ ��� ����
		{
			if ( nType == 1 )
			{		
				//���� ä�� �����								
				CSV_ASK_GAMEITEMLIST agmsg;
				agmsg.Set( 0, nItemNo );
	
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), agmsg.pData, agmsg.GetTotalSize());
			}
		}
		else if ( nStatus == 3 ) // �������� �� �̵�
		{
			//%%%���� ����
// 			if ( g_pMainDlg->m_NewGameItemDlg.m_hWnd != NULL )
// 			{
// 				g_pMainDlg->m_NewGameItemDlg.ShowTab( 2 );
// 			}	
		}
	}	
	else if ( MacroString.Find( "_�ݱ�_",0 ) >= 0 )
	{		
		GM().PostMainWndMessage(UM_CLOSEALLPOPUP);	//�˾�(����, ��������â)�� ������� ���δ��̾�α׿� �˸���.
	}
	else if( MacroString.Find("_�귿����_") >= 0 ){
		if( IDYES == AfxMessageBox( "�귿 ���� �ƹ�Ÿ�� ���� ���� �� ���Ű� �����մϴ�.\n���� ������ �����Ͻðڽ��ϱ�?", MB_YESNO ) ){
			g_bNoGo = FALSE;			
			GM().PostMainWndMessage(WM_COMMAND, IDCANCEL, 0);	

			ShellExecute(NULL,  "open", (char*)(LPCTSTR)getGameURL(IDX_GAMEURL_ROULETTE), NULL, NULL, SW_SHOWNORMAL);
		}		
		else{
			//g_pLobyDlg->LobyItemNavigate(TRUE);
		}

		*pbCancel =TRUE;
	}	
	else if( MacroString.Find( "_�Ƚɴݱ�_", 0 ) >= 0 )
	{
		// �Ӵ� ������Ʈ ����. // ���� �������� ����� �ƴ��� üũ�ϰ� ��ٸ� �Ӵϸ� ������Ʈ �Ѵ�.
		CSV_ASK_GAME_CONTINUE msg;
		msg.Set(GM().GetMyInfo()->UI.ID, 0);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	}

	//��Ŀ����â ĳ�������Ǵݱ�
	else if ( MacroString.Find("javascript:f_charge_complete()") >=0)
	{
		*pbCancel = TRUE;
		//��Ŀ����â�� �ݴ´�. 
		//MM().Call(UPG_CMK_GAMEITEM_DLG_CLOSE);
		GM().PostMainWndMessage(UM_CLOSEALLPOPUP);	//�˾�(����, ��������â)�� ������� ���δ��̾�α׿� �˸���.
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
	CString str; // �ѱ�ǥ��
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
		if( bName == TRUE )rstr.Format("����");
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

//è�Ǿ� ä�ο��� ����� ���� �Ӵ� üũ �� ����  ó��
BOOL  g_Ask_RealChannel_JoinRoomMsg()
{
	return TRUE;
}

int g_GetURLChannelNo()
{
	//�� URL ���� �����  ä�ι�ȣ ���ϱ�
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
	return CString("�׷��̸� ����");	
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

void ShowDirectGoDlg(void)	///������� �ٷΰ��� ȣ��
{
	MM().Call(UPG_CMK_SHOW_DIRECTGO_DLG);
}

void ShowSelectStartDlg(void)	///���ý���
{
	MM().Call(UPG_CMK_SHOW_SELECTSTART_DLG);
}

void ShowResetSelectStartDlg(void)	///���ý���
{
	MM().Call(UPG_CMK_SHOW_RESETSELECTSTART_DLG);
}

//������� �ٷΰ��� ��Ŷ�� ������.
bool SendSelectStart(void)
{
	return GM().SendQuickJoiner()?true:false;
}

LRESULT ShowQuickJoinerDlg(void)
{
	return GM().ShowQuickJoinerDlg();
}

//������� �ٷΰ��� ��Ŷ�� ������.
//������� �ٷΰ��Ⱑ �����Ǿ� ���� �ʴٸ� false�� �����Ѵ�.
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
	//bUserAFK : Away from Keyboard - ������ 2,3ȸ �̻� ��ưŬ���̾��� �������� true;
	
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
		AfxMessageBox("�غ����Դϴ�.");
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
	strMsg.Format("[%s] [%s] [%s] [%s��] : %s", timeNow.Format("%y/%m/%d %H:%M:%S"), strID, strNick, strMoney, strBuffer);

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
		else if(c == ' ') encstr[j++] = '+';    // ���� �°� �߰�
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