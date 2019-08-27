#ifndef __Camel_Base_Database_Connector__
#define __Camel_Base_Database_Connector__

#include "DatabaseBase.h"
#include "DatabaseHandles.h"

#include <string>
//#include <System/System.h>

_DECLARE_NAMESPACE_CAMEL_BASE_DATABASE_
namespace Connector
{
	namespace DatabaseType
	{
		enum Type
		{
			DEFAULT = 1,
			MSSQL_ODBC = 1,
			SQLLITE3
		};
	}

	CAMEL_BASE_DATABASE_API Handle __stdcall CreateHandle();
	CAMEL_BASE_DATABASE_API Handle __stdcall CreateHandle(DatabaseType::Type type);
	CAMEL_BASE_DATABASE_API void __stdcall DestroyHandle(Handle handle);
	
	CAMEL_BASE_DATABASE_API bool __stdcall Connect(Handle handle, std::string const &datasource, std::string const &userid, std::string const &password);
	CAMEL_BASE_DATABASE_API bool __stdcall Reconnect(Handle handle);
	//CAMEL_BASE_DATABASE_API bool __stdcall Connect(Handle handle, std::string const &type, std::string const &ip, unsigned int port, std::string const &database, std::string const &id, std::string const &passwd);
	CAMEL_BASE_DATABASE_API void __stdcall Disconnect(Handle handle);

	CAMEL_BASE_DATABASE_API bool Prepare(Handle handle, std::string const &query, bool isDml = false);
	CAMEL_BASE_DATABASE_API bool Prepare(Handle handle, std::string const &baseDn, std::string const &query, std::string const &outParams); // For LDAP
	
	CAMEL_BASE_DATABASE_API bool Execute(Handle handle);
	CAMEL_BASE_DATABASE_API bool ExecuteDML(Handle handle);
	CAMEL_BASE_DATABASE_API bool ExecuteSQL(Handle handle, std::string const &query);

	CAMEL_BASE_DATABASE_API bool StartTransaction(Handle handle);
	CAMEL_BASE_DATABASE_API bool Commit(Handle handle);
	CAMEL_BASE_DATABASE_API bool Rollback(Handle handle);

	CAMEL_BASE_DATABASE_API bool Fetch(Handle handle);

	CAMEL_BASE_DATABASE_API bool SetParam(Handle handle, int index, int param);
	CAMEL_BASE_DATABASE_API bool SetParam(Handle handle, int index, unsigned long param);
	CAMEL_BASE_DATABASE_API bool SetParam(Handle handle, int index, __int64 param);
	CAMEL_BASE_DATABASE_API bool SetParam(Handle handle, int index, unsigned __int64 param);
	CAMEL_BASE_DATABASE_API bool SetParam(Handle handle, int index, float param);
	CAMEL_BASE_DATABASE_API bool SetParam(Handle handle, int index, double param);
	CAMEL_BASE_DATABASE_API bool SetParam(Handle handle, int index, std::string const &param);
	CAMEL_BASE_DATABASE_API bool SetParam(Handle handle, int index, std::wstring const &param);

	CAMEL_BASE_DATABASE_API bool GetData(Handle handle, int index, int &param);
	CAMEL_BASE_DATABASE_API bool GetData(Handle handle, int index, unsigned long &param);
	CAMEL_BASE_DATABASE_API bool GetData(Handle handle, int index, __int64 &param);
	CAMEL_BASE_DATABASE_API bool GetData(Handle handle, int index, unsigned __int64 &param);
	CAMEL_BASE_DATABASE_API bool GetData(Handle handle, int index, float &param);
	CAMEL_BASE_DATABASE_API bool GetData(Handle handle, int index, double &param);
	CAMEL_BASE_DATABASE_API bool GetData(Handle handle, int index, std::string &param);
	CAMEL_BASE_DATABASE_API bool GetData(Handle handle, int index, std::wstring &param);
	CAMEL_BASE_DATABASE_API bool GetData(Handle handle, int index, char *szBuffer, int nBufferSize);

	CAMEL_BASE_DATABASE_API unsigned int GetRowsCount(Handle handle);
	CAMEL_BASE_DATABASE_API void GetLastErrorMessage(Handle handle, char *szMsg, int nBufferSize);
	CAMEL_BASE_DATABASE_API bool CheckReconnectState(Handle handle, unsigned int nativeError);
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_DATABASE_
#endif // __Camel_Base_Database_Connector__