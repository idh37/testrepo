
#pragma once

#include <string>
#include <vector>
#include "GraphicDXDefine.h"


using namespace std;

#define		DATA_TASK_KIND_NONE			0
#define		DATA_TASK_KIND_LOAD			1
#define		DATA_TASK_KIND_DELETE		2


#define		NOTIFY_STATE_PROCESS		0
#define		NOTIFY_STATE_END			1


class NMGRAPHICDX_CLASS CDataTask
{
public :
	void						SetDataTaskKind(int iDataTaskKind);
	void						SetFileName(char *clpFileName);
	virtual void				Load() = 0;
	virtual void				Delete() = 0;
	int							GetDataTaskKind();
public :	
protected :
protected :
	int							m_iDataTaskKind;
	string						m_strDataTaskFileName;
public :
	CDataTask();
	virtual ~CDataTask();	
};


class NMGRAPHICDX_CLASS CDataTaskManager
{
public :
	void						CreateDataTaskManager();
	void						DestroyDataTaskManager();
	void						AddDataTask(int iDataTaskKind, CDataTask *cllpDataTask, char *clpFileName);
	bool						IsEmptyDataTask();

	void						ProcessDataTask();

	void						BeginDataTask(int iThreadPriority, bool bImmediateFalg = false);

	void						SetNotifyProcessDataTakFunc(void (*lpFunc)(int iNotifyState));
	void						SetDataTaskStopFlag(bool bDataTaskStopFlag);

	bool						CheckDataTask();	

	void						LockDataTaskManager(bool bLockFlag);


public :
	HANDLE						m_LoaderThreadHandle;
	int							m_iDataTaskKind;
protected :
protected :
	vector<CDataTask *>			m_vecDataTaskList;

	void (*NotifyProcessDataTask)(int iNotifyState);

	bool						m_bDataTaskFlag;

	CRITICAL_SECTION			m_Tasks_cs;

	bool						m_bDataTaskStopFlag;

	bool						m_bImmediateFalg;
public :
	CDataTaskManager();
	virtual ~CDataTaskManager();	
};
