#include "StdAfx.h"
#include "VipJackpotMoney.h"

CVipJackpotMoney::CVipJackpotMoney()
{
}

CVipJackpotMoney::~CVipJackpotMoney()
{
}

void CVipJackpotMoney::OnCreated(void)
{
	SetUseLogic(true);
}

void CVipJackpotMoney::Logic(const DWORD &dwTime)
{
	if (!GDM().IsKey(GDK_JACKPOT_MONEY))
	{
		SetNumber(0);
		return;
	}

	INT64 &n64JackPotMoney	  = GDM().GetINT64(GDK_JACKPOT_MONEY);
	INT64 &n64TmpJackPotMoney = GDM().GetINT64(GDK_JACKPOT_MONEY_TEMP);

	INT64 minus = n64TmpJackPotMoney - n64JackPotMoney;
	INT64 plus = 0;

	if(n64TmpJackPotMoney > n64JackPotMoney) 
	{
		if(		minus > 10000000000)plus = 987654321;
		else if(minus > 1000000000) plus = 98765431;
		else if(minus > 100000000)  plus = 9876541;
		else if(minus > 10000000)   plus = 987651;
		else if(minus > 1000000)    plus = 98761;
		else if(minus > 100000)	    plus = 9871;
		else if(minus > 10000)	    plus = 981;
		else if(minus > 1000)       plus = 91; 
		else if(minus > 50 )        plus = 9;
		else plus = 1;

		n64JackPotMoney+=plus; // 증가 보여주기
	}
	else  if(n64TmpJackPotMoney < n64JackPotMoney) 
	{
		if(		minus < -100000000000000000) plus = 9999999987654321;
		else if(minus < -10000000000000000)  plus = 999999987654321;
		else if(minus < -1000000000000000)   plus = 99999987654321;
		else if(minus < -100000000000000)    plus = 9999987654321;
		else if(minus < -10000000000000)     plus = 999987654321;
		else if(minus < -1000000000000)      plus = 99987654321;
		else if(minus < -100000000000)       plus = 9987654321;
		else if(minus < -10000000000)        plus = 987654321;
		else if(minus < -1000000000)         plus = 98765431;
		else if(minus < -100000000)	         plus = 9876521;
		else if(minus < -10000000)           plus = 987651;
		else if(minus < -1000000)            plus = 98761;
		else if(minus < -100000)	         plus = 9871;
		else if(minus < -10000)	             plus = 981;
		else if(minus < -1000)               plus = 91; 
		else if(minus < -50 )                plus = 9;
		else plus = 1;

		n64JackPotMoney-=plus; // 증가 보여주기
	}
	//INT64 jackPotMoneyTemp = GetNumber() ;

	//INT64 plus = 0;
	//if(		jackPotMoneyTemp > 10000000000000)	plus = 98765431;
	//else if(jackPotMoneyTemp > 1000000000000)	plus = 9876521;
	//else if(jackPotMoneyTemp > 100000000000)	plus = 987651;
	//else if(jackPotMoneyTemp > 10000000000)		plus = 98761;
	//else if(jackPotMoneyTemp > 1000000000)		plus = 98761;
	//else if(jackPotMoneyTemp > 100000000)		plus = 9871;
	//else if(jackPotMoneyTemp > 10000000)		plus = 981;
	//else if(jackPotMoneyTemp > 1000000)			plus = 981;
	//else if(jackPotMoneyTemp > 100000)			plus = 91;
	//else if(jackPotMoneyTemp > 10000)			plus = 9;
	//else if(jackPotMoneyTemp > 1000)			plus = 1; 
	//else if(jackPotMoneyTemp > 50 )				plus = 1;
	//else plus = 1;

	//jackPotMoneyTemp+=plus; // 증가 보여주기

	SetNumber(n64JackPotMoney);
}