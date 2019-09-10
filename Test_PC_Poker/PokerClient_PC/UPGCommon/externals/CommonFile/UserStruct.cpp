#include "StdAfx.h"
#include "UserStruct.h"


// ������Ŀ ���� ����
INT64 MoneyGrade[GRADE_MAX] = {
	0,
	1000000000,
	10000000000,
	100000000000,
	1000000000000,
};
 


int GetMoneyGrade(INT64 Money )
{	
	for( int i = 1; i < GRADE_MAX; i ++ )
	{
		if( Money < MoneyGrade[i] ) 
			return i-1;
	}
	
	return GRADE_MAX-1;
}

INT64 GetGradeToUserMoney(int grade)
{
	if(grade<0 || grade > GRADE_MAX ) return 0;
	if(grade == GRADE_MAX) return 100000000000000000;
	return MoneyGrade[grade];
}
int GetWinPro(int winnum, int loosenum, int drawnum)
{
	// �·� ���
	int totnum = winnum + loosenum + drawnum;
	int winpro;
	if(totnum == 0) winpro = 0;
	else winpro = (int)(((double)winnum / totnum)*100);

	return winpro;
}


int GetWinPro(int winnum, int loosenum)
{
	// �·� ���
	int totnum = winnum + loosenum;
	int winpro;
	if(totnum == 0) winpro = 0;
	else winpro = (int)(((double)winnum / totnum)*100);
	
	return winpro;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////                               [���� ������ �۾�]                               //////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
state.iState =  1;	// ��� ������ ������
				2;	// ������� ������
				3;	// ��� �Ұ����� ������
				4:  // 2���� ���վ�����
*/
ItemState GameItem_GetItemState(GAMEITEM &item, SYSTEMTIME* pCurTime) 
{
	ItemState state;
	memset(&state, 0, sizeof(ItemState));

	// �ùٸ� ����Ÿ�� �ƴϴ�
	if(item.UseDay < 0 || item.UseNum < 0) {
		state.iState = 0;	// �߸��� ������
		return state;
	}

	// ��� ȸ���� ������ ��� ���� ���θ� �Ǵ�
	if(item.UseNum > 0) {
		// ����� ȸ���� ��� ������ ȸ���� �ʰ��ߴٸ� �� �� �������̴�
		if(item.Use >= item.UseNum) {
			state.iState = 3;	// ��� �Ұ����� ������
		}
	}

	// ��� �Ⱓ���� ������ ��� ���� ���θ� �Ǵ�
	if(item.UseDay > 0) { 
		if(item.Use > 0) { // ���Ǿ��� �������̶�� ������ ��¥�� �ִ�
			// �������� ��ȿ �Ⱓ�� �д����� ���
			int period = item.UseDay * 24 * 60;
			// ���� �ð� ���
			COleDateTime nt;
			if(pCurTime) nt = *pCurTime;					// ���Ƿ� ������ ����ð�(Ŭ���̾�Ʈ��)
			else nt = COleDateTime::GetCurrentTime();	// ������ ���� �ð�
			COleDateTime st(item.StartDate);				// ��ŸƮ�ð�
			COleDateTimeSpan TimeSpan = nt - st;		// ����ð����� ���̸� ���Ѵ�
			int passed = (int)TimeSpan.GetTotalMinutes();

			if(period > passed) {
				// ���� �ð� ���
				int leftmin = (int)(period - passed);
				COleDateTimeSpan sp(0, 0, leftmin, 0);

				state.LeftTime = sp;
				state.EndDate = nt + sp;

				// ������� �������̴�
				state.bNowUsing = TRUE;
				state.iState = 2;	// ������� ������
			}
			else {
				// �پ� �������̴�
				state.bNowUsing = FALSE;

				state.iState = 3;	// ��� �Ұ����� ������
			}
		}
		else {
			state.iState = 1;	// ��� ������ ������
		}
	}

	return state;
}

// �������� ������ �˾Ƴ��� ���� �Լ�(�������� �����ڵ带 ���� �ڵ�� ���)
int GameItem_GetItemKind(int itemcode)
{
	int kind = 0;

	if(itemcode >= ITEM_SCODE_SECRET && itemcode <= ITEM_ECODE_SECRET)  // TODO : elseif(...??
		kind = ITEM_SCODE_SECRET;
	else if(itemcode >= ITEM_SCODE_SUPERMASTER && itemcode <= ITEM_ECODE_SUPERMASTER) 
		kind = ITEM_SCODE_SUPERMASTER;
	else if(itemcode >= ITEM_SCODE_PLUSPOINT && itemcode <= ITEM_ECODE_PLUSPOINT) 
		kind = ITEM_SCODE_PLUSPOINT;
	else if(itemcode == ITEM_SCODE_INVINCIBLE) // õ�Ϲ��� ������ 
		kind = ITEM_SCODE_INVINCIBLE;
	else if(itemcode >= ITEM_SCODE_QUICKJOINER && itemcode <= ITEM_ECODE_QUICKJOINER)//������� �ٷ�����
		kind = ITEM_SCODE_QUICKJOINER;
	else if( itemcode >= ITEM_SCODE_POKER_HELP && itemcode <= ITEM_ECODE_POKER_HELP )//���� �����
		kind = ITEM_SCODE_POKER_HELP;

	return kind;
}

// Ư�� ������ �������� ��������� �˾Ƴ�(����/Ŭ���̾�Ʈ ����)
BOOL GameItem_IsUsingItem(GAMEITEM_LIST &itemlist, int itemcode, BOOL *pExist, SYSTEMTIME* pCurTime)
{
	int kind = GameItem_GetItemKind(itemcode);
	if(kind == 0)
		return FALSE;

	BOOL bRtn = FALSE;
	for(int i=0; i<itemlist.nNum; i++) {
		// ��û�� �����۰� ���� ������ ���������� �˻�
		if( kind == GameItem_GetItemKind(itemlist.Item[i].Code) ) {		
			// �������� ���¸� ����(pCurTime�� NULL�̶�� �ý����� ���� �ð��� �������� �Ǵ�)
			ItemState state = GameItem_GetItemState(itemlist.Item[i], pCurTime);

			// ��� ������ �������̸� �������� ǥ��
			if(state.iState == 1) 
				*pExist = TRUE;

			// ���� ������ΰ�?
			if(state.bNowUsing) 
				bRtn = TRUE;
		}
	}
	return bRtn;
}

CString GameItem_GetItemName(GAMEITEM& item)
{
	CString str;
	str = GameItem_GetItemName(item.Code);
	if(item.UseDay > 0) {
		CString strday;
		strday.Format(" (%d��)", item.UseDay);
		str += strday;
	}
	return str;
}

CString GameItem_GetItemName(int code)
{
	CString str;

	if(code >= ITEM_SCODE_SECRET && code <= ITEM_ECODE_SECRET) {
		str.Format("������� ������");
	}
	else if(code >= ITEM_SCODE_SUPERMASTER && code <= ITEM_ECODE_SUPERMASTER) {
		str.Format("���� ���� ������");
	}
	else if(code >= ITEM_SCODE_PLUSPOINT && code <= ITEM_ECODE_PLUSPOINT) {
		str.Format("�÷��� ����Ʈ ������");
	}
	else if(code == ITEM_SCODE_INVINCIBLE)  {// [õ�Ϲ��� ������]
		str.Format("õ�Ϲ��� ������");
	}
	else if(code >= ITEM_SCODE_QUICKJOINER && code <= ITEM_ECODE_QUICKJOINER) {//������� �ٷ�����
		str.Format("������� �ٷ����� ������");
	}
	else if( code >= ITEM_SCODE_POKER_HELP && code <= ITEM_ECODE_POKER_HELP )
	{
		str.Format("���� ����� ������");
	}
	else {
		str = "�� �� ���� ������";
	}

	return str;
}

CString GameItem_GetItemHelp(int code)
{
	CString str;

	if(code >= ITEM_SCODE_SECRET && code <= ITEM_ECODE_SECRET) {
		str.Format("������Ŀ2 �Ϲ� ä�ο��� ��������� ���� �� ���Ǵ� �������Դϴ�.");
	}
	else if(code >= ITEM_SCODE_SUPERMASTER && code <= ITEM_ECODE_SUPERMASTER) {
		str.Format("������ ���� ������ �ο��Ͽ� ������ �����߹� ��ų �� �ֵ��� ���ְ� �� ����Ʈ�� ���� ���� �÷��ִ� �������Դϴ�.");
	}
	else if(code >= ITEM_SCODE_PLUSPOINT && code <= ITEM_ECODE_PLUSPOINT) {
		str.Format("���ӿ��� �¸��Ͽ��� ��� ���붧 ���� �� ���� ������ ���� �� �ֵ��� ���ִ� �������Դϴ�.");
	}
	else if(code == ITEM_SCODE_INVINCIBLE) { // [õ�Ϲ��� ������]
		str.Format("���۹�+����������+�������������� ����� �� �ֵ��� ���ִ� �������Դϴ�.");
	}
	else {
		str = "";
	}
	return str;
}

int GameItem_GetItemImageIndex(int code) 
{
	int image = 0;
	if(code >= ITEM_SCODE_SUPERMASTER && code <= ITEM_ECODE_SUPERMASTER) image = 2;
	if(code >= ITEM_SCODE_PLUSPOINT   && code <= ITEM_ECODE_PLUSPOINT  ) image = 3;
	if(code >= ITEM_SCODE_SECRET      && code <= ITEM_ECODE_SECRET     ) image = 5; //�����
	if(code == ITEM_SCODE_INVINCIBLE ) image = 6; // [õ�Ϲ��� ������]
	return image;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////                               [�����̾� �۾�]	                                //////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//�Լ� ��ü
int CheckPremiumState(USERINFO* pUI, SYSTEMTIME* pCurTime)
{
	if(!pUI) return 0;

	if(pUI->PremMenuNo > 0) {
		// ���� �ð� ���
		COleDateTime nt;
		if(pCurTime) nt = *pCurTime;		// ���Ƿ� ������ ����ð�(Ŭ���̾�Ʈ��)
		else nt = COleDateTime::GetCurrentTime();	// ������ ���� �ð�

		// �����̾� ���� �Ͻ�
		COleDateTime et(pUI->PremDate); // ��ŸƮ�ð�

		// ���� �����̾� �Ⱓ�� ���Ѵ�
		COleDateTimeSpan TimeSpan = et - nt;
		double period = TimeSpan.GetTotalMinutes();
		
		if(period >= 0) {
			return pUI->PremMenuNo;
		}
	}

	return 0;
}


//�йи� ���� (�Լ���ü)
int IsPremiumMoreMini(USERINFO* pUI, SYSTEMTIME* pCurTime)
{
	if( pUI == NULL ) return 0;

	if( CheckPremiumState( pUI, pCurTime) == 0 ) return 0;
	
	return  (pUI->PremMenuNo >= CODE_NEW_FAMILY_SILVER);	
}

//�йи� ����
char *g_GetPremName(int PremMenuNo, BOOL bEnglish)
{
	static char* PremSvrName[]={"�Ϲ�", "�̴�","�ǹ�", "���", "���̾Ƹ��", "?"};			// 2011.01.27. �йи� ����
	static char* PremSvrNameEng[]={ "NORMAL", "MINI","SILVER", "GOLD", "DIAMOND", "?"};		

	static char pUnknown[]="?";

	if (PremMenuNo<0 || PremMenuNo>=MAX_CODE_NEW_FAMILY)
		return pUnknown;

	if (bEnglish)
	{
		return PremSvrNameEng[PremMenuNo];
	}
	
	return PremSvrName[PremMenuNo];
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
CString Log_USERINFO(USERINFO *pUI)
{	
	CString strLog;
	if (pUI==NULL) return strLog;

	strLog.Format("ID=%s, NN=%s", pUI->ID, pUI->NickName);

	size_t nRecordCount = _countof(pUI->AllGameRecord);
	for (size_t i = 0; i < nRecordCount; ++i)
	{
		strLog.AppendFormat(", ChipKind[%d] %s", i, (LPCTSTR)pUI->AllGameRecord[i].Log()); 
	}

	return strLog;
}

//���̽� ���� �ݾ� ���ϱ� 
#include <math.h>

//< CYS 110512 > ����Ʈ ���� �α� �߰�
CString GetAutoBettingRuleName(IDX_GAME idxGame,  int nAutoBet, BETRULE_KIND eBetRuleKind/* ����, ����,Ʈ */, RULETYPE ruleType )
{
	//< CYS 110512
	if ( eBetRuleKind == BETRULE_KIND_LIMIT )
	{
		std::string strRuleName = "����Ʈ";
		
		return strRuleName.c_str();
	}
	//> CYS 110512

	if (idxGame==IDX_GAME_NP || idxGame==IDX_GAME_NPT)
	{
		CString rstr,temp_str;

		if( nAutoBet <= 0 )
		{
			rstr.Format("����");
		}
		else if ( nAutoBet == 400 )
		{
			rstr.Format("4������");
		}
		else{
			temp_str.Format("%d",nAutoBet);

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


		if (rstr.IsEmpty()) rstr="???";

		return rstr;

	}
	else if (idxGame ==IDX_GAME_DP)
	{
		CString rstr,temp_str;

		if( nAutoBet <= 0 )
		{
			rstr.Format("����");
		}
		else{
			if ( ruleType == RULETYPE_5POKER )
			{
				if ( nAutoBet > 500 )
					nAutoBet -= 200;
			}
			temp_str.Format("%d",nAutoBet);

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

		if (rstr.IsEmpty()) rstr="???";
		return rstr;
	}
	else if (idxGame ==IDX_GAME_BD || idxGame == IDX_GAME_BDT)
	{
		CString rstr;

		switch(nAutoBet) 
		{
		case 0://����
			rstr.Format("����");
			break;	
		case 401: //< CYS 111010 >
			rstr.Format("1H");
			break;
		case 490:
			rstr.Format("��ħ��F");
			break;
		case 501://111
			rstr.Format("��ħ1");
			break;
		case 502://222
			rstr.Format("��ħ2");
			break;
		case 503://333
			rstr.Format("��ħ3");
			break;	
		case 504://444
			rstr.Format("��ħ4");
			break;	
		case 505://555
			rstr.Format("��ħ5");
			break;			
		case 601:
			rstr.Format("����1");
			break;
		case 602:
			rstr.Format("����2");
			break;
		case 603:
			rstr.Format("����3");
			break;
		case 604:
			rstr.Format("����4");
			break;
		case 605:
			rstr.Format("����5");
			break;
			//< CYS 100802
		case 590:
			{
				rstr.Format("��ħF");
				break;
			}
			//> CYS 100802
		}
		
		if (rstr.IsEmpty()) rstr="???";
		return rstr;
	}
	else if (idxGame == IDX_GAME_SD || idxGame == IDX_GAME_SDT)
	{
		std::string strRule;

		switch( nAutoBet ) 
		{
		case 0:
			{
				strRule = "����";
				break;	
			}
		case 491:
			{
				strRule = "111";
				break;
			}
		default:
			{
				strRule = "?";
				break;
			}
		}
		
		if ( strRule.empty() ) strRule="???";
		CString str;
		str.Format("%s", strRule.c_str() );
		return str;
	}
	else 
	{

		CString rstr,temp_str;

		if( nAutoBet <= 400 )
		{
			rstr.Format("����");
		}
		else if( nAutoBet == 401 ) //< CYS 111010 >
		{
			rstr.Format("1H");
		}
		else{
			temp_str.Format("%d",nAutoBet);

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

		if (rstr.IsEmpty()) rstr="???";
		return rstr;

	}
}


CString GetCardSettingName(IDX_GAME idxGame, char card_setting )
{
	if (IDX_GAME_BD == idxGame || IDX_GAME_BDT == idxGame)
	{
		switch (card_setting)
		{
		case 0:
			return CString("����");
		case 1:
			return CString("��ħ");
		case 2:
			return CString("����");
		}
	}
	else if (IDX_GAME_NP == idxGame || IDX_GAME_NPT == idxGame)
	{
		switch (card_setting)
		{
		case 6:
			return CString("6��");
		case 5:
			return CString("����");
		}
	}
	else
	{
		switch (card_setting)
		{
		case 6:
			return CString("6��");
		case 5:
			return CString("5��");
		case 4:
			return CString("����");
		}
	}
	return CString("?");
}

CString GetBetMoneyMultiName(IDX_GAME idxGame, float fBetMoneyMulti )
{
	if ( 1.0 == fBetMoneyMulti )
		return CString("����");
	else if ( 3.5 == fBetMoneyMulti )
		return CString("�̴�");
	else if ( 8.5 == fBetMoneyMulti )
		return CString("���");
	else if ( 20.0 == fBetMoneyMulti )
		return CString("�ƽ�");

	CString str;
	str.Format("%f��", fBetMoneyMulti);
	return str;
}

//���������� ����Ƚ�� ī��Ʈ�� �����̳� �Ϻ��̳�.. ������ ���� 
COUNT_BASE g_GetChanceItemCountBase(int ChaneItemCode)
{
	//switch((ITEM_SCODE_REWARD)ChaneItemCode)
	//{
	////case ITEM_SCODE_REWARD_LOWGRADE: 
	////	return DAILY_COUNT;
	////case ITEM_SCODE_REWARD_HIGHGRADE: 
	////	return DAILY_COUNT;	
	////case ITEM_SCODE_REWARD_ALLINKING: 
	////	return DAILY_COUNT;	
	////case ITEM_SCODE_REWARD_LUCKAVATA:
	////	return MONTHLY_COUNT;
	////case ITEM_SCODE_REWARD_SPECIALLUCKITEM:
	////	return MONTHLY_COUNT;
	////case ITEM_SCODE_REWARD_ALLINCHANCE:
	////	return DAILY_COUNT;	
	//}

	return UNCOUNTABLE;  //UNCOUNTABLE �� ��쿡�� DB�� ������� �ʴ´�. 
}


CString NumberToOrientalString( INT64 number )
{
	CString strrtn; // �ѱ�ǥ��
	CString strtemp;
	INT64 t = number; 

	BOOL bMinus = FALSE;
	if( t < 0 ) {
		bMinus = TRUE;
		t *= -1;
	}

	INT64 v = t / (INT64)10000000000000000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)10000000000000000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;
	}

	v = t / (INT64)1000000000000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)1000000000000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;
	}

	v = t / (INT64)100000000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)100000000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;
	}

	v = t / (INT64)10000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)10000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;
	}

	if( t > 0 ) {
		strtemp.Format("%I64d", t);
		strrtn += strtemp;
	}

	if( number == 0 )
	{
		strrtn = "0";
	}

	if( bMinus )
	{
		strrtn.Insert(0,"-");
		//CString temp;
		//temp.Format( "-%s", strrtn );
		//strrtn = temp;
	}


	//NUMBERFMT nFmt = { 0, 0, 4, ".", ",", 0 };
	//TCHAR    szVal[32];
	//TCHAR     szOut[32];
	//sprintf ( szVal,"%I64d", number );
	//GetNumberFormat ( NULL, NULL, szVal, &nFmt, szOut, 32 );

	return strrtn;

}
// ������Ŀ ���� ����

CString GetGradeName(int lv)
{
	switch (lv)
	{
	case GRADE_PYOUNGMIN:
		{
			return "���";
		}break;
	case GRADE_GOSU:
		{
			return "����";
		}break;
	case GRADE_YOUNGWOONG:
		{
			return "����";
		}break;
	case GRADE_JIZON:
		{
			return "����";
		}break;
	case GRADE_DOSIN:
		{
			return "����";
		}break;
	default:
		{
			return "������";
		}break;
	}
	return "������";
}



#if defined(__NEWPOKER__)
INT64	g_DS_Level[50] = {
	0,							// 1		
	1000000,					// 2
	6000000,					// 3
	18000000,					// 4
	40000000,					// 5
	75000000,					// 6
	126000000,					// 7
	196000000,					// 8
	288000000,					// 9
	405000000,					// 10 ��� ����
	550000000,					// 11
	1275000000,					// 12
	2900000000,					// 13
	5875000000,					// 14
	10650000000,				// 15
	17675000000,				// 16
	27400000000,				// 17
	40275000000,				// 18
	56750000000,				// 19
	77275000000,				// 20 ī�嵦 1�� ����
	108300000000,				// 21
	163450000000,				// 22
	239600000000,				// 23
	340650000000,				// 24
	470500000000,				// 25
	575500000000,				// 26
	693500000000,				// 27
	824500000000,				// 28
	968500000000,				// 29
	1162500000000,				// 30 ī�嵦 1�� ���� total = 2
	1456500000000,				// 31
	1950500000000,				// 32
	2844500000000,				// 33
	4438500000000,				// 34
	7132500000000,				// 35
	11426500000000,				// 36
	17920500000000,				// 37
	27314500000000,				// 38
	40408500000000,				// 39
	58102500000000,				// 40 ���̵� ��
	85796500000000,				// 41
	133490500000000,			// 42
	221184500000000,			// 43
	378878500000000,			// 44
	646572500000000,			// 45
	1074266500000000,			// 46
	1721960500000000,			// 47
	2659654500000000,			// 48
	3967348500000000,			// 49
	5735042500000000,			// 50 ī�嵦 1 ���� total = 3
};


int DS_GetLevel(INT64 Money,int winnum)
{
	if(Money < 0 || winnum < 0)return 0;

	if(winnum > 1000)
	{
		winnum = 1000;
	}
	Money = (Money * winnum) / 1000;

	int i = 0;
	for( ; i < 50; i ++ )
	{
		if( Money < g_DS_Level[i] ) break;
	}
	if(i >= 50)return 50;
	return i;
}

INT64 DS_GetLevel(int lv)
{
	for( int i = 0; i < 50; i ++ )
	{
		if(i == lv)return g_DS_Level[i];
	}

	return 0;
}

int GetLevelPoint(int point, int winnum, int loosenum)
{
	// �·� ���
	int totnum = winnum + loosenum;
	float winpro;
	if(totnum == 0) winpro = 0;
	else winpro = ((float)winnum / totnum)*100;

	int levelpoint = (int)(point * winpro/100);
	if(levelpoint < 0) levelpoint = 0;

	return levelpoint;
}




#endif	//defined(__NEWPOKER__)