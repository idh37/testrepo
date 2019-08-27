/**    
@file    GameDataManager.h
@date    2011/2/15
@author  PJH(gamking@cj.com)
@brief   Data 관리자 ( 게임에서 사용하는 데이터 값을 Command/value형식으로 저장한다. )
@brief   이전에는 CGameDataManager라는 클래스로 사용됨.
*/

#pragma once

#include "UtilDefine.h"
#include <map>
#include <string>
using namespace std;

namespace NMBASE
{
	namespace UTIL
	{
		class NMUTIL_CLASS CDataManager
		{
		public:
			CDataManager(void);
			virtual ~CDataManager(void);

		public:
			static CDataManager& GetInstance();

		public:
			void Release(void);

			void SetINT64(const std::string &strKey,  const INT64 nData);
			void SetUINT64(const std::string &strKey, const UINT64 nData);
			void SetDouble(const std::string &strKey, const double dbData);
			void SetString(const std::string &strKey, const std::string strData);
			void SetBool(const std::string &strKey,   const bool bData);

			INT64  &GetINT64(const std::string &strKey);
			UINT64 &GetUINT64(const std::string &strKey);
			double &GetDouble(const std::string &strKey);
			std::string &GetString(const std::string &strKey);
			bool GetBool(const std::string &strKey);
			bool IsKey(const std::string &strKey);

		protected:
			enum EDATA_TYPE
			{
				GDT_UNKNOWN = -1,
				GDT_INT64,
				GDT_UINT64,
				GDT_DOUBLE,
				GDT_STRING
			};

			class CDATA
			{
			public:
				EDATA_TYPE	m_nType;
				void*		m_pData;
			public:
				CDATA();
				virtual ~CDATA();

				void Release();
			};

			typedef std::map<string, CDATA> MAP_CDATA;
			typedef MAP_CDATA::iterator		MAP_CDATA_IT;

			MAP_CDATA			m_mapData;
			CRITICAL_SECTION	m_cLock;



		protected:
			CDATA	&GetData(const std::string &strKey);
		};

		NMUTIL_API CDataManager& GDMAN();

	}//namespace UTIL

}//namespace NMBASE
