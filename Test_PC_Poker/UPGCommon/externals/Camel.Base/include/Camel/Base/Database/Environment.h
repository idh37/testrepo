#ifndef __Camel_Base_Database_Environment__
#define __Camel_Base_Database_Environment__

#include "DatabaseBase.h"
#include "DatabaseHandles.h"
#include <string>
//#include <System/System.h>

_DECLARE_NAMESPACE_CAMEL_BASE_DATABASE_
namespace Environment
{
	namespace Type
	{
		enum _Type
		{
			MYSQL_ODBC35	= 0,
			MYSQL_ODBC51	= 1,
			MYSQL_NATIVE	= 2,
			MSSQL_ODBC		= 3,
			ORACLE_ODBC		= 4,
			LDAP			= 5,
		};
	}
	CAMEL_BASE_DATABASE_API Handle __stdcall CreateHandle();
	CAMEL_BASE_DATABASE_API void __stdcall DestroyHandle(Handle handle);

	CAMEL_BASE_DATABASE_API void __stdcall SetDatabaseType(Handle handle, Type::_Type type);
	CAMEL_BASE_DATABASE_API void __stdcall SetConnetionString(Handle handle, std::string const &connectionString);
	CAMEL_BASE_DATABASE_API void __stdcall SetDSN(Handle handle, std::string const &dsn);
	CAMEL_BASE_DATABASE_API void __stdcall SetBinDN(Handle handle, std::string const &id); // For LDAP 
	CAMEL_BASE_DATABASE_API void __stdcall SetUserID(Handle handle, std::string const &id);
	CAMEL_BASE_DATABASE_API void __stdcall SetPassword(Handle handle, std::string const &password);
	CAMEL_BASE_DATABASE_API void __stdcall SetDefaultDatabase(Handle handle, std::string const &database);
	CAMEL_BASE_DATABASE_API void __stdcall SetHostIP(Handle handle, std::string const &ip);
	CAMEL_BASE_DATABASE_API void __stdcall SetHostPort(Handle handle, unsigned int port);
	CAMEL_BASE_DATABASE_API void __stdcall SetMinConnections(Handle handle, unsigned int minConnections);
	CAMEL_BASE_DATABASE_API void __stdcall SetMaxConnections(Handle handle, unsigned int maxConnections);
	CAMEL_BASE_DATABASE_API void __stdcall SetIdleTime(Handle handle, unsigned int idleTime);
	CAMEL_BASE_DATABASE_API void __stdcall SetMySQLCharacterSet(Handle handle, const std::string& charSet);
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_DATABASE_
#endif // __Camel_Base_Database_Environment__