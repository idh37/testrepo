
#ifndef _GS_SERVERGROUP_API_H_
#define _GS_SERVERGROUP_API_H_

#ifndef GSCONNECTOR_EXE
	#ifdef GSCONNECTOR_EXPORTS
		#define GSCONNECTOR_API __declspec(dllexport)
		#define GSCONNECTOR_API_TEMPLATE 
	#else
		#define GSCONNECTOR_API __declspec(dllimport)
		#define GSCONNECTOR_API_TEMPLATE extern

		#ifndef _DEBUG
			#pragma comment(lib, "Pigeon.GS.Connector.lib")
		#else
			#pragma comment(lib, "Pigeon.GS.ConnectorD.lib")
		#endif
	#endif
#else
	#define GSCONNECTOR_API
#endif


#include <Camel/Base/System/ThreadPool.h>
#include <Camel/Base/System/SharedPtr.h>
#include <Camel/Dar2/Dar2BaseMsg.h>
#include <Camel/Dar2/DAR2Rule_STREAM.h>

#include "Pigeon.GS.ConnectorAPIError.h"
#include "Pigeon.GS.Allocator.h"
#include "Pigeon.GS.Connector_InnerProtocol.h"

#include <string>


namespace Pigeon
{
	namespace GS
	{
		namespace System
		{
			GSCONNECTOR_API void __stdcall GetVersion(unsigned long *majorVersion, unsigned long *minorVersion, unsigned long *buildVersion1, unsigned long *buildVersion2);
			GSCONNECTOR_API void __stdcall GetProductVersion(unsigned long *majorVersion, unsigned long *minorVersion, unsigned long *buildVersion1, unsigned long *buildVersion2);
		}

		namespace Environment
		{
			typedef struct _Handle {} *THandle, *Handle;

			GSCONNECTOR_API Handle		__stdcall	CreateHandle  ( Camel::Base::System::ThreadPool::Handle hRecvThreadPool );
			GSCONNECTOR_API void		__stdcall	DestroyHandle ( Handle hEnv );

			GSCONNECTOR_API bool		__stdcall	SetTimeout	( Handle hEnv, int nTimeout );
			GSCONNECTOR_API int		__stdcall	GetTimeout	( Handle hEnv );

			GSCONNECTOR_API Camel::Base::System::ThreadPool::Handle __stdcall	GetNetworkThreadPool ( Handle hEnv );
		}	// namespace Environment


		namespace Connector
		{
			typedef struct _Handle {} *THandle, *Handle;

			/////////////////////////////////////////////////////////////////////////////////////////////////
			// �⺻ ������
			/////////////////////////////////////////////////////////////////////////////////////////////////
			GSCONNECTOR_API Handle	__stdcall CreateHandle (Environment::Handle hEnv);
			GSCONNECTOR_API void	__stdcall DestroyHandle( Handle hCon );

			GSCONNECTOR_API bool	__stdcall Connect( Handle hCon, const char* szAddr, unsigned short nPort );
			GSCONNECTOR_API void	__stdcall Disconnect( Handle hCon );
			GSCONNECTOR_API bool	__stdcall ReConnect( Handle hCon );

			typedef bool (__stdcall	*OnDisconnected )	( void* /*pContext*/);
			GSCONNECTOR_API bool __stdcall SetOnDisconnected(Handle hCon, OnDisconnected pCallback, void* pData);

			/////////////////////////////////////////////////////////////////////////////////////////////////
			// Logic... 
			/////////////////////////////////////////////////////////////////////////////////////////////////


			/////////////////////////////////////////////////////////////////////////////////////////////////
			// GS������ ������ ��Ͻ�Ų��.
			// hHandle : Ŀ���� �ڵ�, stastus : ���� ���� ����
			GSCONNECTOR_API bool __stdcall RegisterServerEx( GS::Connector::Handle hHandle, IN const Camel::Dar2::GSConnector::_ServerStatusEx& status );

			/////////////////////////////////////////////////////////////////////////////////////////////////
			// GS�������� ������ ���� ��Ų��.
			// hHandle : Ŀ���� �ڵ�, stastus : ���� ���� ����
			GSCONNECTOR_API bool __stdcall UnRegisterServerEx( GS::Connector::Handle hHandle );

			/////////////////////////////////////////////////////////////////////////////////////////////////
			// GS������ ���� ���¸� ������Ʈ �Ѵ�.
			// hHandle : Ŀ���� �ڵ�, stastus : ���� ���� ����
			GSCONNECTOR_API bool __stdcall UpdateServerEx( GS::Connector::Handle hHandle, IN const Camel::Dar2::GSConnector::_ServerStatusEx& status );

			/////////////////////////////////////////////////////////////////////////////////////////////////
			// GS�������� Ư�� �������� ��û. ��� ���� �����´�.( Ŭ���̾�Ʈ���� ���� ������ ������ �����ö� ��� )
			// hHandle : Ŀ���� �ڵ�, nMaxList : ����� �ִ� ����, 
			// ServerKind : � ���� �����ΰ��� �Ǻ��ϴ� �ɼ�( SS : 0, Relay : 1 )
			// MajorVersion / MinorVersion �޾ƿ����� ������ ����.( �� �ΰ��� �������� ���ؼ� ������ ������ ������ �����´�. )
			// RequestOption: 0 - ����ġ / ���� �� �ڸ� �������( �⺻ �ε� �뷱�� ������ ���� ���� ), 1 - ���� �ε尡 ���� ����, 2 - ���� �ٻ� ����( ������ ���� ). 3. ��� ����
			// RequestParam: � ������ ������ ������ �Ѱ��� �Ķ���� �ɼ�

			// ex ) Version 6.9.123.456 Relay���� 3�� �޾ƿ�����.. 
			// GetServerListEx( handle, 3, Camel::Dar2::GSConnector::SS, 6, 9, 123, 456, SERVER_QUERY_CMD::BALANCED, 0, buf );
			// Version�� ������� ��� ������ �޾ƿ�����..
			// GetServerListEx( handle, 3, Camel::Dar2::GSConnector::SS, 0, 0, 0, 0, SERVER_QUERY_CMD::ALL, 0, buf );
			// 
			enum SERVER_QUERY_CMD
			{
				BALANCED = 0,	// ���� ������ �뷱�� �ؼ� �޾ƿ´�.
				ALL = 3,		// ��� ������ �޾ƿ´�.
			};
			GSCONNECTOR_API bool __stdcall GetServerListEx( IN GS::Connector::Handle hHandle, 
															IN const int MaxList, 
															IN const Camel::Dar2::GSConnector::PIGEON_SERVER_KIND ServerKind, 
															IN const unsigned int MajorVersion, 
															IN const unsigned int MinorVersion, 
															IN const unsigned int BuildVer1, 
															IN const unsigned int BuildVer2, 
															IN const int RequestOption, 
															IN const int RequestParamReserved, 
															OUT const Camel::Dar2::GSConnector::_ServerListEx** pResultBuf );







			//////////////////////////////////////////////////////////////////////////
			// �Ʒ� 4���� API�� 2012 / 1 / 26���� �������� �Ǿ���...
			//////////////////////////////////////////////////////////////////////////

			// GS������ ������ ��Ͻ�Ų��.
			// hHandle : Ŀ���� �ڵ�, stastus : ���� ���� ����
			GSCONNECTOR_API bool __stdcall RegisterServer( GS::Connector::Handle hHandle, IN const Camel::Dar2::GSConnector::_ServerStatus& status );

			/////////////////////////////////////////////////////////////////////////////////////////////////
			// GS�������� ������ ���� ��Ų��.
			// hHandle : Ŀ���� �ڵ�, stastus : ���� ���� ����
			GSCONNECTOR_API bool __stdcall UnRegisterServer( GS::Connector::Handle hHandle );

			/////////////////////////////////////////////////////////////////////////////////////////////////
			// GS������ ���� ���¸� ������Ʈ �Ѵ�.
			// hHandle : Ŀ���� �ڵ�, stastus : ���� ���� ����
			GSCONNECTOR_API bool __stdcall UpdateServer( GS::Connector::Handle hHandle, IN const Camel::Dar2::GSConnector::_ServerStatus& status );

			/////////////////////////////////////////////////////////////////////////////////////////////////
			// GS�������� Ư�� �������� ��û. ��� ���� �����´�.( Ŭ���̾�Ʈ���� ���� ������ ������ �����ö� ��� )
			// hHandle : Ŀ���� �ڵ�, nMaxList : ����� �ִ� ����, ProtocolVersion: � ������ �����ϴ� ������ �̾ƿ� ���ΰ�. 
			// RequestOption: 0 - ����ġ / ���� �� �ڸ� �������( �⺻ �ε� �뷱�� ������ ���� ���� ), 1 - ���� �ε尡 ���� ����, 2 - ���� �ٻ� ����( ������ ���� ). 3. ��� ����
			// RequestParam: � ������ ������ ������ �Ѱ��� �Ķ���� �ɼ�
			// ServerKind : � ���� �����ΰ��� �Ǻ��ϴ� �ɼ�( SS : 0, Relay : 1 )
			// ex ) Relay���� �Ѵ� �޾ƿ�����.. GetServerList( handle, 1, 0, 1, 0, 0, buf );
			// 
			GSCONNECTOR_API bool __stdcall GetServerList( GS::Connector::Handle hHandle, IN const int MaxList, IN const Camel::Dar2::GSConnector::PIGEON_SERVER_KIND ServerKind, IN const int ProtocolVer, IN const int RequestOption, IN const int RequestParamReserved, OUT const Camel::Dar2::GSConnector::_ServerList** pResultBuf );
		}

	// GS Ŀ���Ϳ� �ڵ�
	#define GS_CONNECTOR_HANDLE GS::Connector::Handle
	};
}

#endif

