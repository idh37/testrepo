#include "stdafx.h"
#include "DataTaskManager.h"
#include <process.h>
#include "Global.h"
//#include "ClientDlg.h"




#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//unsigned __stdcall DataTaskThreadProc( LPVOID lpParameter )
void DataTaskThreadProc( LPVOID lpParameter )
{
	CDataTaskManager *cllpDataTaskManager = (CDataTaskManager *)  ( lpParameter );	


	while( 1 )
	{
		if( cllpDataTaskManager->IsEmptyDataTask() == false )
		{			
			cllpDataTaskManager->ProcessDataTask();
		}
		else
		{
			SuspendThread(cllpDataTaskManager->m_LoaderThreadHandle);
		}

		::Sleep(0);

	}

	//	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

CDataTask::CDataTask()
{
	m_iDataTaskKind = DATA_TASK_KIND_NONE;		

}
CDataTask::~CDataTask()
{

}
//-----------------------------------------------------------------------------
// Name: SetDataTaskKind()
// Desc: 
//-----------------------------------------------------------------------------
void CDataTask::SetDataTaskKind(int iDataTaskKind)
{
	m_iDataTaskKind = iDataTaskKind;
}
//-----------------------------------------------------------------------------
// Name: SetFileName()
// Desc: 
//-----------------------------------------------------------------------------
void CDataTask::SetFileName(char *clpFileName)
{
	if( clpFileName == NULL ) return;
	m_strDataTaskFileName = clpFileName;
}
//-----------------------------------------------------------------------------
// Name: GetDataTaskKind()
// Desc: 
//-----------------------------------------------------------------------------
int CDataTask::GetDataTaskKind()
{
	return m_iDataTaskKind;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

CDataTaskManager::CDataTaskManager()
{
	::InitializeCriticalSection( &m_Tasks_cs );				//!< 크리티컬 섹션 초기화

	m_LoaderThreadHandle = NULL;

	m_vecDataTaskList.clear();

	NotifyProcessDataTask = NULL;
	m_bDataTaskFlag = false;
	m_bDataTaskStopFlag = false;

	m_iDataTaskKind = 0;

	m_bImmediateFalg = false;
}

CDataTaskManager::~CDataTaskManager()
{
	DestroyDataTaskManager();


}
//-----------------------------------------------------------------------------
// Name: CreateDataTaskManager()
// Desc: 
//-----------------------------------------------------------------------------
void CDataTaskManager::CreateDataTaskManager()
{
	if( m_LoaderThreadHandle != NULL )
	{
		::CloseHandle( m_LoaderThreadHandle );
		m_LoaderThreadHandle = NULL;
	}

	//m_LoaderThreadHandle = (HANDLE )_beginthreadex( NULL, 0, DataTaskThreadProc, (LPVOID)this, CREATE_SUSPENDED , NULL );
	//m_LoaderThreadHandle = (HANDLE )_beginthreadex( NULL, 0, DataTaskThreadProc, (LPVOID)this, 0 , NULL );
	m_LoaderThreadHandle = (HANDLE )_beginthread( DataTaskThreadProc, 0, (LPVOID)this);



	if( NULL == m_LoaderThreadHandle )
	{
		AfxMessageBox("CDataTaskManager::BeginLoading(): can't Create Loader Thread! ");
		return ;
	}


}
//-----------------------------------------------------------------------------
// Name: AddDataTask()
// Desc: 
//-----------------------------------------------------------------------------
void CDataTaskManager::AddDataTask(int iDataTaskKind, CDataTask *cllpDataTask, char *clpFileName)
{
	if( cllpDataTask == NULL ) return;
	if( m_bDataTaskStopFlag == true ) return ;

	cllpDataTask->SetDataTaskKind(iDataTaskKind);
	cllpDataTask->SetFileName(clpFileName);

	::EnterCriticalSection( &m_Tasks_cs );

	m_vecDataTaskList.push_back(cllpDataTask);

	m_iDataTaskKind = iDataTaskKind;

	::LeaveCriticalSection( &m_Tasks_cs );	

}
//-----------------------------------------------------------------------------
// Name: IsEmptyDataTask()
// Desc: 
//-----------------------------------------------------------------------------
bool CDataTaskManager::IsEmptyDataTask()
{
	::EnterCriticalSection( &m_Tasks_cs );
	bool bTempFlag = m_vecDataTaskList.empty();
	::LeaveCriticalSection( &m_Tasks_cs );	

	return bTempFlag;
}
//-----------------------------------------------------------------------------
// Name: ProcessDataTask()
// Desc: 
//-----------------------------------------------------------------------------
void CDataTaskManager::ProcessDataTask()
{
	::EnterCriticalSection( &m_Tasks_cs );

	vector<CDataTask *>::iterator iter = m_vecDataTaskList.begin();
	switch( (*iter)->GetDataTaskKind() )
	{		
	case DATA_TASK_KIND_LOAD :
		{

			// Immediate로 로딩 할경우 스레드가 아니기 때문에.
			// 멈추는?현상을 없에기 위해 mainrender를 해준다.
			if( m_bImmediateFalg == true )
			{
				// 종환 임시 주석
				//g_pMainDlg->MainRender();
			}

			(*iter)->Load();
		}
		break ;
	case DATA_TASK_KIND_DELETE :
		{
			(*iter)->Delete();
			delete (*iter);
			(*iter) = NULL;
		}
		break ;	
	}
	m_vecDataTaskList.erase(iter);

	if( m_bDataTaskStopFlag == true )
	{
		m_vecDataTaskList.clear();		
	}

	::LeaveCriticalSection( &m_Tasks_cs );


	if( NotifyProcessDataTask != NULL &&
		m_bDataTaskStopFlag == false )
	{
		NotifyProcessDataTask(NOTIFY_STATE_PROCESS);
	}

	if( m_bDataTaskFlag == true &&
		IsEmptyDataTask() == true )		
	{
		if( NotifyProcessDataTask != NULL &&
			m_bDataTaskStopFlag == false )
		{
			NotifyProcessDataTask(NOTIFY_STATE_END);
		}

		NotifyProcessDataTask = NULL;

		m_bDataTaskFlag = false;

	}
}
//-----------------------------------------------------------------------------
// Name: BeginDataTask()
// Desc: 
//-----------------------------------------------------------------------------
void CDataTaskManager::BeginDataTask(int iThreadPriority, bool bImmediateFalg)
{



	if( m_vecDataTaskList.empty() == true ) return;
	if( m_bDataTaskFlag == true ) return;

	m_bDataTaskFlag = true;

	m_bImmediateFalg = bImmediateFalg;

	if( bImmediateFalg == true )
	{

		while( IsEmptyDataTask() == false )
		{
			ProcessDataTask();		
		}	

	}
	else
	{
		//THREAD_PRIORITY_IDLE 
		SetThreadPriority(m_LoaderThreadHandle , iThreadPriority);
		//SetThreadPriority(m_LoaderThreadHandle , THREAD_PRIORITY_LOWEST );
		ResumeThread(m_LoaderThreadHandle);

	}




}
//-----------------------------------------------------------------------------
// Name: SetNotifyProcessDataTakFunc()
// Desc: 
//-----------------------------------------------------------------------------
void CDataTaskManager::SetNotifyProcessDataTakFunc(void (*lpFunc)(int iNotifyState))
{
	NotifyProcessDataTask = lpFunc;

}
//-----------------------------------------------------------------------------
// Name: DestroyDataTaskManager()
// Desc: 
//-----------------------------------------------------------------------------
void CDataTaskManager::DestroyDataTaskManager()
{


	m_vecDataTaskList.clear();

	if( m_LoaderThreadHandle != NULL )
	{
		::CloseHandle( m_LoaderThreadHandle );
		m_LoaderThreadHandle = NULL;
	}

	::DeleteCriticalSection( &m_Tasks_cs );					//!< 크리티컬 섹션 삭제


}
//-----------------------------------------------------------------------------
// Name: CheckDataTask()
// Desc: 
//-----------------------------------------------------------------------------
bool CDataTaskManager::CheckDataTask()
{
	return m_bDataTaskFlag;
}
//-----------------------------------------------------------------------------
// Name: SetDataTaskStopFlag()
// Desc: 
//-----------------------------------------------------------------------------
void CDataTaskManager::SetDataTaskStopFlag(bool bDataTaskStopFlag)
{
	m_bDataTaskStopFlag = bDataTaskStopFlag;
}


void CDataTaskManager::LockDataTaskManager(bool bLockFlag)
{
	if( bLockFlag == true )
	{
		::EnterCriticalSection( &m_Tasks_cs );
	}
	else
	{
		::LeaveCriticalSection( &m_Tasks_cs );	
	}	

}