// LogFile.cpp: implementation of the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCriticalSection CLogFile::Cs;

CLogFile::CLogFile()
{
	strLogFile = "Log.txt";
}

CLogFile::CLogFile(LPCTSTR fname)
{
	strLogFile = fname;
}

CLogFile::~CLogFile()
{

}

void CLogFile::SetLogFile(LPCTSTR fname)
{
	strLogFile = fname;
}


BOOL CLogFile::Write(char *pstr)
{
	Cs.Lock();

	FILE *fp = NULL;
	char date[128]={0,};
	char time[128]={0,};

	_strdate_s( date, 128 );
	_strtime_s( time, 128 );

	//fp = fopen("Log.txt", "a+" );

	//fp = fopen((LPCTSTR)strLogFile, "a+" );
	fopen_s( &fp, strLogFile.c_str(), "a+" );

	if ( NULL == fp ) 
	{ 
		Cs.Unlock(); 
		return FALSE; 
	}

	fseek(fp, 0, SEEK_END);

// 	fprintf(fp, "\n⊙ 로그 기록 일시: date[%s] time[%s]\n", date, time);
// 	fprintf(fp, "%s\n", pstr);

	fprintf(fp, "[%s %s] ", date, time);
	fprintf(fp, "%s\r\n", pstr);
	fclose(fp);

	Cs.Unlock();
	return TRUE;
}

BOOL CLogFile::Write(CString &str)
{
	return Write((char*)str.operator LPCTSTR());
}


BOOL CLogFile::Writef(char *mainmsg,...)
{
	//char buffer[1024];
	CString buffer;
	va_list argptr;	
	if( mainmsg == NULL )return FALSE;

	va_start(argptr, mainmsg);
//	vsprintf(buffer,mainmsg,argptr);
	buffer.FormatV(mainmsg,argptr);
	va_end(argptr);

	return Write(buffer);
}


BOOL CLogFile::WriteBackup(char *pData, UINT nSize)
{
	Cs.Lock();

	if(pData == NULL || nSize<=0L) { Cs.Unlock(); return FALSE; }

	std::string strTitle = _T("=======================================================================================\r\n* [ID]:[(Level)PMoney],AddPMoney(NowBetPMoney),Win/Fail/Fold,(RSF:SF:FCARD)\r\n=======================================================================================\r\n");
	TRY
	{
		CFile fileBack( strLogFile.c_str(), CFile::modeCreate|CFile::modeWrite );
		fileBack.Write( strTitle.c_str(), strTitle.length() );
		fileBack.Write( ( LPVOID )pData, nSize );
	}
	CATCH(CFileException,e)
	{
		Cs.Unlock(); return FALSE;
	}
	END_CATCH

	Cs.Unlock();
	return TRUE;
}