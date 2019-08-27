#include "StdAfx.h"
#include "StringUtil.h"

#include <math.h>

namespace NMBASE
{
	namespace UTIL
	{
		LPCTSTR g_NumberToHangul( INT64 number )
		{
			static CString s_strTemp; // 한글표시
			s_strTemp = "";
			CString strtemp;
			INT64 t = number; 

			BOOL bMinus = FALSE;
			if( t < 0 )
			{
				bMinus = TRUE;
				t *= -1;
			}

			INT64 v = t / (INT64)10000000000000000;	// 경 단위
			if(v > 0) 
			{
				t = t - v*(INT64)10000000000000000;
				strtemp.Format("%I64d경", v);
				s_strTemp += strtemp;
			}

			v = t / (INT64)1000000000000;	// 조 단위
			if(v > 0)
			{
				t = t - v*(INT64)1000000000000;
				strtemp.Format("%I64d조", v);
				s_strTemp += strtemp;
			}

			v = t / (INT64)100000000;	// 억 단위
			if(v > 0)
			{
				t = t - v*(INT64)100000000;
				strtemp.Format("%I64d억", v);
				s_strTemp += strtemp;
			}

			v = t / (INT64)10000;	// 만 단위
			if(v > 0)
			{
				t = t - v*(INT64)10000;
				strtemp.Format("%I64d만", v);
				s_strTemp += strtemp;
			}

			if( t > 0 ) 
			{
				strtemp.Format("%I64d", t);
				s_strTemp += strtemp;
			}

			if( number == 0 )
			{
				s_strTemp = "0";
			}

			if( bMinus )
			{
				s_strTemp.Insert(0,"-");
			}

			return (LPCTSTR)s_strTemp;
		}


		LPCTSTR g_MakeCommaMoney(INT64 nMoney)
		{
			static CString s_strVal("0");
			s_strVal ="";
			bool minus = false;

			if(nMoney<0)
			{		
				minus = true;
				nMoney = -nMoney;
			}

			s_strVal.Format("%I64d",nMoney);


			for(int i=18; i>0; i-=3)
			{
				if(s_strVal.GetLength()>i)
				{
					s_strVal.Insert(s_strVal.GetLength()-i,",");
				}
			}

			if(s_strVal=="")
				s_strVal= "0";

			if( minus )
				s_strVal.Insert(0,"-");

			return (LPCTSTR)s_strVal;
		}


		LPCTSTR g_MoneyMark(INT64 nMoney, int nUnit /*= 5*/)
		{	
			static CString s_strTemp; // 한글표시
			s_strTemp = "";
			CString strtemp;
			INT64 t = nMoney; 

			BOOL bMinus = FALSE;
			if( t < 0 )
			{
				bMinus = TRUE;
				t *= -1;
			}

			INT64 v = t / (INT64)10000000000000000;	// 경 단위
			int nTemp = 0;
			if(v > 0) 
			{
				t = t - v*(INT64)10000000000000000;
				strtemp.Format("%I64d경", v);
				s_strTemp += strtemp;
				nTemp++;
			}

			if(nUnit == nTemp) goto GOTO_MONEY_MARK_END;

			v = t / (INT64)1000000000000;	// 조 단위
			if(v > 0)
			{
				t = t - v*(INT64)1000000000000;
				strtemp.Format("%I64d조", v);
				s_strTemp += strtemp;
				nTemp++;
			}

			if(nUnit == nTemp) goto GOTO_MONEY_MARK_END;

			v = t / (INT64)100000000;	// 억 단위
			if(v > 0) 
			{
				t = t - v*(INT64)100000000;
				strtemp.Format("%I64d억", v);
				s_strTemp += strtemp;
				nTemp++;
			}

			if(nUnit == nTemp) goto GOTO_MONEY_MARK_END;

			v = t / (INT64)10000;	// 만 단위
			if(v > 0) 
			{
				t = t - v*(INT64)10000;
				strtemp.Format("%I64d만", v);
				s_strTemp += strtemp;
				nTemp++;
			}

			if(nUnit == nTemp) goto GOTO_MONEY_MARK_END;

			if( t > 0 ) 
			{
				strtemp.Format("%I64d", t);
				s_strTemp += strtemp;
			}

GOTO_MONEY_MARK_END:

			if( nMoney == 0 )
			{
				s_strTemp = "0";
			}

			if( bMinus )
			{
				s_strTemp.Insert(0,"-");
			}

			return (LPCTSTR)s_strTemp;	
		}

		LPCTSTR g_HistoryMoneyMark(INT64 nMoney)
		{
			static CString s_strTemp; // 한글표시
			s_strTemp = "";
			CString strtemp;
			INT64 t = nMoney; 

			BOOL bMinus = FALSE;
			if( t < 0 )
			{
				bMinus = TRUE;
				t *= -1;
			}

			INT64 v = t / (INT64)10000000000000000;	// 경 단위
			if(v > 0)
			{
				t = t - v*(INT64)10000000000000000;
				strtemp.Format("%I64d경", v);
				s_strTemp += strtemp;

				if( bMinus )
				{
					s_strTemp.Insert(0,"-");		
				}
				return (LPCTSTR)s_strTemp;
			}

			v = t / (INT64)1000000000000;	// 조 단위
			if(v > 0) 
			{
				t = t - v*(INT64)1000000000000;
				strtemp.Format("%I64d조", v);
				s_strTemp += strtemp;
				if( bMinus )
				{
					s_strTemp.Insert(0,"-");		
				}
				return (LPCTSTR)s_strTemp;
			}

			v = t / (INT64)100000000;	// 억 단위
			if(v > 0)
			{
				t = t - v*(INT64)100000000;
				strtemp.Format("%I64d억", v);
				s_strTemp += strtemp;
				if( bMinus )
				{
					s_strTemp.Insert(0,"-");		
				}
				return (LPCTSTR)s_strTemp;
			}

			v = t / (INT64)10000;	// 만 단위
			if(v > 0) 
			{
				t = t - v*(INT64)10000;
				strtemp.Format("%I64d만", v);
				s_strTemp += strtemp;
				if( bMinus )
				{
					s_strTemp.Insert(0,"-");		
				}
				return (LPCTSTR)s_strTemp;
			}

			if( t > 0 ) 
			{
				strtemp.Format("%I64d", t);
				s_strTemp += strtemp;
				if( bMinus )
				{
					s_strTemp.Insert(0,"-");		
				}
				return s_strTemp;
			}

			if( nMoney == 0 )
			{
				s_strTemp = "0";
			}

			if( bMinus )
			{
				s_strTemp.Insert(0,"-");		
			}

			return (LPCTSTR)s_strTemp;	
		}

		LPCTSTR g_ProfileMoneyMark(INT64 nMoney)
		{
			static CString s_strTemp; // 한글표시
			s_strTemp = "";
			CString strtemp;
			INT64 t = nMoney; 

			BOOL bMinus = FALSE;
			if( t < 0 )
			{
				bMinus = TRUE;
				t *= -1;
			}

			INT64 v = t / (INT64)10000000000000000;	// 경 단위
			if(v > 0)
			{
				t = t - v*(INT64)10000000000000000;
				strtemp.Format("%I64d경", v);
				s_strTemp += strtemp;

				if( bMinus )
				{
					s_strTemp.Insert(0,"-");		
				}		
			}

			v = t / (INT64)1000000000000;	// 조 단위
			if(v > 0) 
			{
				t = t - v*(INT64)1000000000000;
				strtemp.Format("%I64d조", v);
				s_strTemp += strtemp;
				if( bMinus )
				{
					s_strTemp.Insert(0,"-");		
				}		
			}

			v = t / (INT64)100000000;	// 억 단위
			if(v > 0) 
			{
				t = t - v*(INT64)100000000;
				strtemp.Format("%I64d억", v);
				s_strTemp += strtemp;
				if( bMinus )
				{
					s_strTemp.Insert(0,"-");		
				}		
			}

			v = t / (INT64)10000;	// 만 단위
			if(v > 0)
			{
				t = t - v*(INT64)10000;
				strtemp.Format("%I64d만", v);
				s_strTemp += strtemp;
				if( bMinus )
				{
					s_strTemp.Insert(0,"-");		
				}	
			}


			if( nMoney < 10000 )
			{
				s_strTemp.Format("%I64d", nMoney);
			}

			if( nMoney == 0)
			{
				s_strTemp = "0";
			}

			if( bMinus )
			{
				s_strTemp.Insert(0,"-");		
			}	

			return (LPCTSTR)s_strTemp;	
		}

		//조, 억, 만 
		int g_NumberToParse(INT64 money, int &Num1, int &Num2, int &Num3, int &Num4) 
		{
			INT64 t = money; 

			INT64 v = t / (INT64)1000000000000;	// 조 단위
			if(v > 0)
			{
				t = t - v*(INT64)1000000000000;
				Num1 =  (int) v;
			}

			v = t / (INT64)100000000;	// 억 단위
			if(v > 0) 
			{
				t = t - v*(INT64)100000000;
				Num2 = (int) v;
			}

			v = t / (INT64)10000;	// 만 단위
			if(v > 0) 
			{
				t = t - v*(INT64)10000;
				Num3 = (int) v;
			}

			if( t > 0 )
			{
				Num4 = (int) t;
			}

			return 0;
		}

		LPCTSTR g_HistoryMoneyMarkEx(INT64 nMoney)
		{
			static CString s_strTemp; // 한글표시
			s_strTemp = "";
			CString str1;
			INT64 t = nMoney; 
			INT64 Money = nMoney;	

			BOOL bMinus = FALSE;
			if( t < 0 ) 
			{
				bMinus = TRUE;		
				Money = nMoney * -1;
			}

			int num[4] = {0,};	
			g_NumberToParse(Money, num[3], num[2], num[1], num[0]);

			if (num[3] > 0)
			{
				str1.Format("%d조",num[3]);
				s_strTemp += str1;
			}
			if (num[2] > 0)
			{
				str1.Format("%d억원",num[2]);
				s_strTemp += str1;
			}
			else if (num[1] > 0)
			{
				str1.Format("%d만원",num[1]);
				s_strTemp += str1;
			}
			else if (num[0] > 0)
			{
				str1.Format("%d원",num[0]);
				s_strTemp += str1;
			}	

			if( nMoney == 0 )
			{
				s_strTemp = "0원";
			}

			if( bMinus )
			{
				s_strTemp.Insert(0,"-");		
			}	

			return (LPCTSTR)s_strTemp;	
		}

		LPCTSTR g_HigherMonetaryunit(INT64 nMoney)
		{
			static CString s_strTemp; // 한글표시
			s_strTemp = "";
			CString str1;
			INT64 t = nMoney; 
			INT64 Money = nMoney;	

			BOOL bMinus = FALSE;
			if( t < 0 )
			{
				bMinus = TRUE;		
				Money = nMoney * -1;
			}

			int num[4] = {0,};	
			g_NumberToParse(Money, num[3], num[2], num[1], num[0]);

			if (num[3] > 0)
			{
				str1.Format("%d조",num[3]);
				s_strTemp += str1;
			}
			else if (num[2] > 0)
			{
				str1.Format("%d억",num[2]);
				s_strTemp += str1;
			}
			else if (num[1] > 0)
			{
				str1.Format("%d만",num[1]);
				s_strTemp += str1;
			}
			else if (num[0] > 0)
			{
				str1.Format("%d원",num[0]);
				s_strTemp += str1;
			}	

			if( nMoney == 0 )
			{
				s_strTemp = "0원";
			}

			if( bMinus )
			{
				s_strTemp.Insert(0,"-");		
			}	

			return (LPCTSTR)s_strTemp;	
		}

		LPCTSTR g_MakeShortMoneyStr(INT64 nMoney)		//머니스트링이 길면 짧게 만든다.
		{
			static CString s_strTemp; // 한글표시
			s_strTemp = "";
			s_strTemp.Format("%I64d", nMoney);

			int len=s_strTemp.GetLength();

			if (len<=8)
			{		
				return g_NumberToHangul(nMoney);
			}
			else
			{
				int div = len/4;

				INT64 b = (INT64) pow(10.0f,(div-1)*4);

				INT64 pm=(nMoney/b)*b;		

				return g_NumberToHangul(pm);
			}

		}

		// 조억, 억만, 만원
		LPCTSTR g_MakeShortMoneyStrEx(INT64 nMoney)		//머니스트링이 길면 짧게 만든다.
		{		
			static CString s_strTemp; // 한글표시
			s_strTemp = "";
			CString str1;
			INT64 t = nMoney; 
			INT64 Money = nMoney;	

			BOOL bMinus = FALSE;
			if( t < 0 ) {
				bMinus = TRUE;		
				Money = nMoney * -1;
			}

			int num[4] = {0,};	
			g_NumberToParse(Money, num[3], num[2], num[1], num[0]);

			if( nMoney == 0 )
			{
				s_strTemp = "0"; 
				return (LPCTSTR)s_strTemp;
			}

			if (num[3] > 0)
			{
				str1.Format("%d조",num[3]);
				s_strTemp += str1;
				if (num[2] > 0)
				{
					str1.Format("%d억",num[2]);
					s_strTemp += str1;
				}

			}
			else if (num[2] > 0)
			{
				str1.Format("%d억",num[2]);
				s_strTemp += str1;
				if (num[1] > 0)
				{
					str1.Format("%d만",num[1]);
					s_strTemp += str1;
				}

			}
			else if (num[1] > 0)
			{
				str1.Format("%d만",num[1]);
				s_strTemp += str1;
				if (num[0] > 0)
				{
					str1.Format("%d",num[0]);
					s_strTemp += str1;
				}	
				//return s_strTemp;
			}
			else if (num[0] > 0)
			{
				str1.Format("%d",num[0]);
				s_strTemp += str1;
			}	


			if( bMinus )
			{
				s_strTemp.Insert(0,"-");		
			}	

			return (LPCTSTR)s_strTemp;	
		}
		LPCTSTR g_MakeShortMoneyStrText(INT64 nMoney)
		{
			static CString s_strTemp; // 한글표시
			s_strTemp = "";
			CString str1;
			INT64 t = nMoney; 
			INT64 Money = nMoney;	

			BOOL bMinus = FALSE;
			if( t < 0 ) {
				bMinus = TRUE;		
				Money = nMoney * -1;
			}

			int num[4] = {0,};
			int n1000 = 0;
			int n100 = 0;
			int n10 = 0;
			g_NumberToParse(Money, num[3], num[2], num[1], num[0]);

			if( nMoney == 0 )
			{
				s_strTemp = "0"; 
				return (LPCTSTR)s_strTemp;
			}

			if (num[3] > 0)
			{
				str1.Format("%d조",num[3]);
				s_strTemp += str1;
				if (num[2] > 0)
				{
					n1000 = num[2]/1000;
					n100 = num[2]%1000/100;
					n10 = num[2]%100/10;
					if(0!=n1000){
						str1.Format("%d천",n1000);
						s_strTemp += str1;
					}
					if(0!=n100){
						str1.Format("%d백",n100);
						s_strTemp += str1;
					}
					if(0!=n10){
						str1.Format("%d십",n10);
						s_strTemp += str1;
					}
					s_strTemp += "억";
				}

			}
			else if (num[2] > 0)
			{
				str1.Format("%d억",num[2]);
				s_strTemp += str1;
				if (num[1] > 0)
				{
					n1000 = num[1]/1000;
					n100 = num[1]%1000/100;
					n10 = num[1]%100/10;;
					if(0!=n1000){
						str1.Format("%d천",n1000);
						s_strTemp += str1;
					}
					if(0!=n100){
						str1.Format("%d백",n100);
						s_strTemp += str1;
					}
					if(0!=n10){
						str1.Format("%d십",n10);
						s_strTemp += str1;
					}
					s_strTemp += "만";
				}

			}
			else if (num[1] > 0)
			{
				str1.Format("%d만",num[1]);
				s_strTemp += str1;
				if (num[0] > 0)
				{
					n1000 = num[0]/1000;
					n100 = num[0]%1000/100;
					n10 = num[0]%100/10;
					if(0!=n1000){
						str1.Format("%d천",n1000);
						s_strTemp += str1;
					}
					if(0!=n100){
						str1.Format("%d백",n100);
						s_strTemp += str1;
					}
					if(0!=n10){
						str1.Format("%d십",n10);
						s_strTemp += str1;
					}
				}	
				//return s_strTemp;
			}
			else if (num[0] > 0)
			{
				n1000 = num[0]/1000;
				n100 = num[0]%1000/100;
				n10 = num[0]%100/10;;
				if(0!=n1000){
					str1.Format("%d천",n1000);
					s_strTemp += str1;
				}
				if(0!=n100){
					str1.Format("%d백",n100);
					s_strTemp += str1;
				}
				if(0!=n10){
					str1.Format("%d십",n10);
					s_strTemp += str1;
				}
			}	


			if( bMinus )
			{
				s_strTemp.Insert(0,"-");		
			}	

			return (LPCTSTR)s_strTemp;	
		}


		char* g_NumberToString(int  nNumber)
		{			
			static char buffer[128] = {0, };
			_itoa_s(nNumber, buffer, 10);

			return buffer;
		}

		char* g_NumberToString(unsigned int nNumber)
		{
			static char buffer[128] = {0, };
			_itoa_s(nNumber, buffer, 10);

			return buffer;
		}

		


	}//namespace UTIL

}//namespace NMBASE

