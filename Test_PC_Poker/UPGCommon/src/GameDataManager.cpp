#include "StdAfx.h"
#include "GameDataManager.h"
#include "ThreadLock.h"

CGameDataManager::GAME_DATA::GAME_DATA()
:	m_nType(GDT_UNKNOWN),
	m_pData(NULL)
{
}

CGameDataManager::GAME_DATA::~GAME_DATA()
{
}

void CGameDataManager::GAME_DATA::Release()
{
	if(m_pData) 
	{
		switch(m_nType)
		{
		case GDT_STRING:
			delete ((std::string *)m_pData);
			break;
		case GDT_DOUBLE:
			delete ((double *)m_pData);
			break;
		case GDT_UINT64:
			delete ((UINT64 *)m_pData);
			break;
		case GDT_INT64:
			delete ((INT64 *)m_pData);
			break;
		}
	}
	m_pData = NULL;
}

CGameDataManager::CGameDataManager(void)
{
	InitializeCriticalSection(&m_cLock);
}

CGameDataManager::~CGameDataManager(void)
{
	DeleteCriticalSection(&m_cLock);
}

void CGameDataManager::Release(void)
{
	CThreadLock lock(&m_cLock);
	MAP_GAME_DATA::iterator start = m_mapData.begin();
	MAP_GAME_DATA::iterator end = m_mapData.end();

	for(;start != end;start++)
	{
		start->second.Release();
	}

	m_mapData.clear();
}

CGameDataManager& CGameDataManager::GetInstance()
{
	static CGameDataManager s_cGameDataManager;
	return s_cGameDataManager;
}

bool CGameDataManager::IsKey(const std::string &strKey)
{
	CThreadLock lock(&m_cLock);
	if(m_mapData.find(strKey) == m_mapData.end()) return false;
	return true;
}

CGameDataManager::GAME_DATA &CGameDataManager::GetData(const std::string &strKey)
{
	CThreadLock lock(&m_cLock);
	return m_mapData[strKey];
}

void CGameDataManager::SetINT64(const std::string &strKey, const INT64 nData)
{
	CThreadLock lock(&m_cLock);
	if(IsKey(strKey))
	{
		GAME_DATA &data = GetData(strKey);
		ASSERT(data.m_nType == GDT_INT64);
		*((INT64 *)data.m_pData) = nData;
	}
	else
	{
		GAME_DATA data;
		data.m_nType = GDT_INT64;
		data.m_pData = new INT64;
		*((INT64 *)data.m_pData) = nData;

		m_mapData[strKey] = data;
	}
}

void CGameDataManager::SetBool(const std::string &strKey, const bool bData)
{
	CThreadLock lock(&m_cLock);
	SetINT64(strKey, bData?1:0);
}

void CGameDataManager::SetUINT64(const std::string &strKey, const UINT64 nData)
{
	CThreadLock lock(&m_cLock);
	if(IsKey(strKey))
	{
		GAME_DATA &data = GetData(strKey);
		ASSERT(data.m_nType == GDT_UINT64);
		*((UINT64 *)data.m_pData) = nData;
	}
	else
	{
		GAME_DATA data;
		data.m_nType = GDT_UINT64;
		data.m_pData = new UINT64;
		*((UINT64 *)data.m_pData) = nData;

		m_mapData[strKey] = data;
	}
}

void CGameDataManager::SetDouble(const std::string &strKey, const double dbData)
{
	CThreadLock lock(&m_cLock);
	if(IsKey(strKey))
	{
		GAME_DATA &data = GetData(strKey);
		ASSERT(data.m_nType == GDT_DOUBLE);
		*((double *)data.m_pData) = dbData;
	}
	else
	{
		GAME_DATA data;
		data.m_nType = GDT_DOUBLE;
		data.m_pData = new double;
		*((double *)data.m_pData) = dbData;

		m_mapData[strKey] = data;
	}
}

void CGameDataManager::SetString(const std::string &strKey, const std::string strData)
{
	CThreadLock lock(&m_cLock);
	if(IsKey(strKey))
	{
		GAME_DATA &data = GetData(strKey);
		ASSERT(data.m_nType == GDT_STRING);
		*((std::string *)data.m_pData) = strData;
	}
	else
	{
		GAME_DATA data;
		data.m_nType = GDT_STRING;
		data.m_pData = new std::string;
		*((std::string *)data.m_pData) = strData;

		m_mapData[strKey] = data;
	}
}

INT64 &CGameDataManager::GetINT64(const std::string &strKey)
{
	CThreadLock lock(&m_cLock);
	ASSERT(IsKey(strKey));
	GAME_DATA &data = GetData(strKey);
	ASSERT(data.m_nType == GDT_INT64);
	return *((INT64 *)data.m_pData);
}

bool CGameDataManager::GetBool(const std::string &strKey)
{
	CThreadLock lock(&m_cLock);
	return GetINT64(strKey)?true:false;
}

UINT64 &CGameDataManager::GetUINT64(const std::string &strKey)
{
	CThreadLock lock(&m_cLock);
	ASSERT(IsKey(strKey));
	GAME_DATA &data = GetData(strKey);
	ASSERT(data.m_nType == GDT_UINT64);
	return *((UINT64 *)data.m_pData);
}

double &CGameDataManager::GetDouble(const std::string &strKey)
{
	CThreadLock lock(&m_cLock);
	ASSERT(IsKey(strKey));
	GAME_DATA &data = GetData(strKey);
	ASSERT(data.m_nType == GDT_DOUBLE);
	return *((double *)data.m_pData);
}

std::string &CGameDataManager::GetString(const std::string &strKey)
{
	CThreadLock lock(&m_cLock);
	ASSERT(IsKey(strKey));
	GAME_DATA &data = GetData(strKey);
	ASSERT(data.m_nType == GDT_STRING);
	return *((std::string *)data.m_pData);
}

CGameDataManager& GDM()
{
	return CGameDataManager::GetInstance();
}