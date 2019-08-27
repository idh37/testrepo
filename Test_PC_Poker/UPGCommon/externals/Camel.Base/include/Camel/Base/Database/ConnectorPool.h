#ifndef __Camel_Base_Database_ConnectorPool__
#define __Camel_Base_Database_ConnectorPool__

#include "DatabaseBase.h"
#include "DatabaseHandles.h"
#include <string>
//#include <System/System.h>

_DECLARE_NAMESPACE_CAMEL_BASE_DATABASE_
namespace ConnectorPool
{
	CAMEL_BASE_DATABASE_API Handle __stdcall CreateHandle(const std::string& key, const std::string& dsn, const std::string& userid, const std::string& password, int minConnections = 1, int maxConnections = 32, int idleTime = 60);
	CAMEL_BASE_DATABASE_API Handle __stdcall CreateHandle(const std::string& key, Environment::Handle handle);
	//CAMEL_BASE_DATABASE_API Handle __stdcall CreateHandle(const std::string& key, std::string const &type, std::string const &ip, unsigned int port, std::string const &database, std::string const &userid, std::string const &password, int minConnections = 1, int maxConnections = 32, int idleTime = 60);
	CAMEL_BASE_DATABASE_API void __stdcall DestroyHandle(Handle handle);

	CAMEL_BASE_DATABASE_API Connector::Handle __stdcall Get(Handle handle);
	CAMEL_BASE_DATABASE_API Connector::Handle __stdcall GetBack(Handle handle, time_t timeLastAccess);
	CAMEL_BASE_DATABASE_API void __stdcall PutBack(Handle handle, Connector::Handle chandle);
	CAMEL_BASE_DATABASE_API size_t __stdcall GetCapacitySize(Handle handle);
	CAMEL_BASE_DATABASE_API size_t __stdcall GetUsingSize(Handle handle);
	CAMEL_BASE_DATABASE_API size_t __stdcall GetIdleSize(Handle handle);
	CAMEL_BASE_DATABASE_API size_t __stdcall GetDeadSize(Handle handle);
	CAMEL_BASE_DATABASE_API size_t __stdcall GetPoolSize(Handle handle);
	CAMEL_BASE_DATABASE_API size_t __stdcall GetAvailableSize(Handle handle);
	CAMEL_BASE_DATABASE_API size_t __stdcall GetMaxUsedSize(Handle handle);
	CAMEL_BASE_DATABASE_API size_t __stdcall GetUsedSize(Handle handle);
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_DATABASE_
#endif // __Camel_Base_Database_ConnectorPool__