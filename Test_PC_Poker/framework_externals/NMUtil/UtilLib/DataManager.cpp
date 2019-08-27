#include "StdAfx.h"
#include "DataManager.h"
#include "ThreadLock.h"

namespace NMBASE
{
	namespace UTIL
	{

		CDataManager& GDMAN()
		{
			return CDataManager::GetInstance();
		}


		CDataManager::CDATA::CDATA()
			:	m_nType(GDT_UNKNOWN),
			m_pData(NULL)
		{
		}

		CDataManager::CDATA::~CDATA()
		{
		}

		void CDataManager::CDATA::Release()
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

		CDataManager::CDataManager(void)
		{
			InitializeCriticalSection(&m_cLock);
		}

		CDataManager::~CDataManager(void)
		{
			DeleteCriticalSection(&m_cLock);
		}

		void CDataManager::Release(void)
		{
			CThreadLock lock(&m_cLock);
			MAP_CDATA_IT start = m_mapData.begin();
			MAP_CDATA_IT end = m_mapData.end();

			for(; start != end; ++start)
			{
				start->second.Release();
			}

			m_mapData.clear();
		}

		CDataManager& CDataManager::GetInstance()
		{
			static CDataManager s_cDataManager;
			return s_cDataManager;
		}

		bool CDataManager::IsKey(const std::string &strKey)
		{
			CThreadLock lock(&m_cLock);
			if(m_mapData.find(strKey) == m_mapData.end())
				return false;

			return true;
		}

		CDataManager::CDATA &CDataManager::GetData(const std::string &strKey)
		{
			CThreadLock lock(&m_cLock);
			return m_mapData[strKey];
		}

		void CDataManager::SetINT64(const std::string &strKey, const INT64 nData)
		{
			CThreadLock lock(&m_cLock);
			if(IsKey(strKey))
			{
				CDATA &data = GetData(strKey);
				ASSERT(data.m_nType == GDT_INT64);
				*((INT64 *)data.m_pData) = nData;
			}
			else
			{
				CDATA data;
				data.m_nType = GDT_INT64;
				data.m_pData = new INT64;
				*((INT64 *)data.m_pData) = nData;

				m_mapData[strKey] = data;
			}
		}

		void CDataManager::SetBool(const std::string &strKey, const bool bData)
		{
			CThreadLock lock(&m_cLock);
			SetINT64(strKey, bData?1:0);
		}

		void CDataManager::SetUINT64(const std::string &strKey, const UINT64 nData)
		{
			CThreadLock lock(&m_cLock);
			if(IsKey(strKey))
			{
				CDATA &data = GetData(strKey);
				ASSERT(data.m_nType == GDT_UINT64);
				*((UINT64 *)data.m_pData) = nData;
			}
			else
			{
				CDATA data;
				data.m_nType = GDT_UINT64;
				data.m_pData = new UINT64;
				*((UINT64 *)data.m_pData) = nData;

				m_mapData[strKey] = data;
			}
		}

		void CDataManager::SetDouble(const std::string &strKey, const double dbData)
		{
			CThreadLock lock(&m_cLock);
			if(IsKey(strKey))
			{
				CDATA &data = GetData(strKey);
				ASSERT(data.m_nType == GDT_DOUBLE);
				*((double *)data.m_pData) = dbData;
			}
			else
			{
				CDATA data;
				data.m_nType = GDT_DOUBLE;
				data.m_pData = new double;
				*((double *)data.m_pData) = dbData;

				m_mapData[strKey] = data;
			}
		}

		void CDataManager::SetString(const std::string &strKey, const std::string strData)
		{
			CThreadLock lock(&m_cLock);
			if(IsKey(strKey))
			{
				CDATA &data = GetData(strKey);
				ASSERT(data.m_nType == GDT_STRING);
				*((std::string *)data.m_pData) = strData;
			}
			else
			{
				CDATA data;
				data.m_nType = GDT_STRING;
				data.m_pData = new std::string;
				*((std::string *)data.m_pData) = strData;

				m_mapData[strKey] = data;
			}
		}

		INT64 &CDataManager::GetINT64(const std::string &strKey)
		{
			CThreadLock lock(&m_cLock);
			ASSERT(IsKey(strKey));
			CDATA &data = GetData(strKey);
			ASSERT(data.m_nType == GDT_INT64);
			return *((INT64 *)data.m_pData);
		}

		bool CDataManager::GetBool(const std::string &strKey)
		{
			CThreadLock lock(&m_cLock);
			return GetINT64(strKey)?true:false;
		}

		UINT64 &CDataManager::GetUINT64(const std::string &strKey)
		{
			CThreadLock lock(&m_cLock);
			ASSERT(IsKey(strKey));
			CDATA &data = GetData(strKey);
			ASSERT(data.m_nType == GDT_UINT64);
			return *((UINT64 *)data.m_pData);
		}

		double &CDataManager::GetDouble(const std::string &strKey)
		{
			CThreadLock lock(&m_cLock);
			ASSERT(IsKey(strKey));
			CDATA &data = GetData(strKey);
			ASSERT(data.m_nType == GDT_DOUBLE);
			return *((double *)data.m_pData);
		}

		std::string &CDataManager::GetString(const std::string &strKey)
		{
			CThreadLock lock(&m_cLock);
			ASSERT(IsKey(strKey));
			CDATA &data = GetData(strKey);
			ASSERT(data.m_nType == GDT_STRING);
			return *((std::string *)data.m_pData);
		}

	}//namespace UTIL

}//namespace NMBASE
