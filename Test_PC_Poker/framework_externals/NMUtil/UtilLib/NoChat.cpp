// NoChat.cpp: implementation of the CNoChat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NoChat.h"
#include <vector>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <mmsystem.h>
//#pragma comment(lib,"winmm.lib")

namespace NMBASE
{
	namespace UTIL
	{
		//////////////////////////////////////////////////////////////////////
		// Construction/Destruction
		//////////////////////////////////////////////////////////////////////

		CNoChat::CNoChat()
		{
			ZeroMemory(AddTime, sizeof(AddTime));
		}

		CNoChat::~CNoChat()
		{

		}

		BOOL CNoChat::CheckNoChat(CString &str)
		{
			return CheckNoChat((LPCTSTR)str);
		}

		BOOL CNoChat::CheckNoChat(LPCTSTR pstr)
		{

#ifdef _GAMETEST
			return FALSE;
#endif
			if(bNoChat == TRUE)
			{
				DWORD tick = timeGetTime() - NoChatTimeTick;
				if(tick < 0 || tick > 1000*120) // 120�ʰ� ä�� ����
				{
					bNoChat = FALSE;
				}
				else return TRUE;
			}

			// ä�� ������ ���ۿ� ���
			AddChat(pstr);

			int totline=0;
			NoChatTimeTick = timeGetTime();
			bNoChat = TRUE;

			// ���ۿ� ��ϵ� �� ���μ��� �˻�
			for(int i=0; i<10; i++)
			{
				if(History[i].GetLength()>0)
				{
					// 5���� �˻��Ͽ� ��� ���� ���ڿ��� ��� ����� ����
					totline++;
					if(totline<5 && History[0].Compare(History[i])!=0) bNoChat=FALSE;
				}
				else { if(i<5) bNoChat=FALSE; break; }
			}

			// 5���� ��� ���� ���ڿ��̴��� ������� ���ڿ��� �ð������� �дٸ� ���谡 �ƴ�
			if(bNoChat) 
			{
				DWORD interval = AddTime[0] - AddTime[4];
				// 5�� ���� ���ڿ����� �ð����� 5�� �̻��̶��
				if(interval<0 || interval > 1000*120 || History[0].GetLength()<20) bNoChat = FALSE;
			}

			if( !bNoChat && History[0].GetLength() > 20 )
			{
				bNoChat = CheckNoChat2();
			}

			// ���������� ���� �ð��� �˻�
			if(!bNoChat && totline > 5)
			{
				DWORD interval = AddTime[0] - AddTime[5];
				// 5���� 3�� �̳��� ������ ���� ����� ����
				if(interval>=0 && interval < 1000*3) bNoChat = TRUE;
			}

			return FALSE;
		}

		BOOL CNoChat::CheckNoChat2()
		{
			std::vector<CString> vecCheckstr;
			std::vector<DWORD> vecTime;
			int nCnt = 0;
			for(size_t i=1; i<10; i++)
			{
				if( nCnt < 4 && 
					History[i].GetLength() > 20 )
				{
					nCnt++;
					vecCheckstr.push_back(History[i]);
					vecTime.push_back(AddTime[i]);
				}
			}
			nCnt = 1;
			if( vecCheckstr.size() > 0 )
			{
				for( size_t i = 0; i < vecCheckstr.size(); i++)
				{
					if( History[0].Compare( vecCheckstr[i] ) == 0 )
					{
						nCnt++;
					}
					else
					{
						if( ChatClac( History[0], vecCheckstr[i] ) == TRUE )
							nCnt++;
					}
				}

				DWORD interval = 0;
				if( vecTime.size() > 0 )
				{
					interval = vecTime[0] - vecTime[vecTime.size() - 1];
				}

				if( nCnt >= 3 && 
					interval < 1000*120 )
				{
					return TRUE;
				}
			}			
			return FALSE;
		}

		BOOL CNoChat::ChatClac( CString strSource, CString strTarget )
		{
			std::vector< float > vecSourceCnt;
			while( strSource.GetLength() > 0 )
			{
				char cTemp = strSource[0];
				int nSourceCnt = strSource.Remove(cTemp);
				int nTargetRemove = strTarget.Remove(cTemp);

				if( nSourceCnt > nTargetRemove )
				{
					if( nSourceCnt > 0 )
						vecSourceCnt.push_back( 1 - ( (float)( nSourceCnt - nTargetRemove ) / (float)nSourceCnt ) );
				}
				else
				{
					if( nTargetRemove > 0 )
						vecSourceCnt.push_back( 1 - ( (float)(nTargetRemove - nSourceCnt ) / (float)nTargetRemove ) );
				}
			}

			float fResult = 0;
			for( size_t i = 0; i < vecSourceCnt.size(); i++ )
			{
				fResult += vecSourceCnt[i];
			}

			if( ( fResult / (float)vecSourceCnt.size() ) > 0.6 )
			{
				return TRUE;
			}
			return FALSE;
		}

		void CNoChat::AddChat(LPCTSTR pstr)
		{
			// �����丮 ����
			for(int i=9; i>0; i--)
			{
				AddTime[i] = AddTime[i-1];
				History[i] = History[i-1];
			}
			History[0].Format("%s",pstr);
			AddTime[0] = timeGetTime();
		}

	}//namespace UTIL

}//namespace NMBASE

