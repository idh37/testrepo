
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
			// 기본 프레임
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
			// GS서버에 서버를 등록시킨다.
			// hHandle : 커넥터 핸들, stastus : 현재 서버 상태
			GSCONNECTOR_API bool __stdcall RegisterServerEx( GS::Connector::Handle hHandle, IN const Camel::Dar2::GSConnector::_ServerStatusEx& status );

			/////////////////////////////////////////////////////////////////////////////////////////////////
			// GS서버에서 서버를 제외 시킨다.
			// hHandle : 커넥터 핸들, stastus : 현재 서버 상태
			GSCONNECTOR_API bool __stdcall UnRegisterServerEx( GS::Connector::Handle hHandle );

			/////////////////////////////////////////////////////////////////////////////////////////////////
			// GS서버에 서버 상태를 업데이트 한다.
			// hHandle : 커넥터 핸들, stastus : 현재 서버 상태
			GSCONNECTOR_API bool __stdcall UpdateServerEx( GS::Connector::Handle hHandle, IN const Camel::Dar2::GSConnector::_ServerStatusEx& status );

			/////////////////////////////////////////////////////////////////////////////////////////////////
			// GS서버에서 특정 서버들을 요청. 결과 값을 가져온다.( 클라이언트에서 내가 적합한 서버를 가져올때 사용 )
			// hHandle : 커넥터 핸들, nMaxList : 결과값 최대 갯수, 
			// ServerKind : 어떤 서버 종류인가를 판별하는 옵션( SS : 0, Relay : 1 )
			// MajorVersion / MinorVersion 받아오려는 적합한 버전.( 이 두개의 버전값만 비교해서 동일한 버전의 서버만 가져온다. )
			// RequestOption: 0 - 가중치 / 서버 빈 자리 고려순서( 기본 로드 밸런싱 전략에 따른 서버 ), 1 - 가장 로드가 적은 서버, 2 - 가장 바쁜 서버( 꽉차지 않은 ). 3. 모든 서버
			// RequestParam: 어떤 서버를 가져올 것인지 넘겨줄 파라미터 옵션

			// ex ) Version 6.9.123.456 Relay서버 3대 받아오려면.. 
			// GetServerListEx( handle, 3, Camel::Dar2::GSConnector::SS, 6, 9, 123, 456, SERVER_QUERY_CMD::BALANCED, 0, buf );
			// Version에 상관없이 모든 서버를 받아오려면..
			// GetServerListEx( handle, 3, Camel::Dar2::GSConnector::SS, 0, 0, 0, 0, SERVER_QUERY_CMD::ALL, 0, buf );
			// 
			enum SERVER_QUERY_CMD
			{
				BALANCED = 0,	// 적정 서버를 밸런싱 해서 받아온다.
				ALL = 3,		// 모든 서버를 받아온다.
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
			// 아래 4개의 API는 2012 / 1 / 26이후 구버전이 되었음...
			//////////////////////////////////////////////////////////////////////////

			// GS서버에 서버를 등록시킨다.
			// hHandle : 커넥터 핸들, stastus : 현재 서버 상태
			GSCONNECTOR_API bool __stdcall RegisterServer( GS::Connector::Handle hHandle, IN const Camel::Dar2::GSConnector::_ServerStatus& status );

			/////////////////////////////////////////////////////////////////////////////////////////////////
			// GS서버에서 서버를 제외 시킨다.
			// hHandle : 커넥터 핸들, stastus : 현재 서버 상태
			GSCONNECTOR_API bool __stdcall UnRegisterServer( GS::Connector::Handle hHandle );

			/////////////////////////////////////////////////////////////////////////////////////////////////
			// GS서버에 서버 상태를 업데이트 한다.
			// hHandle : 커넥터 핸들, stastus : 현재 서버 상태
			GSCONNECTOR_API bool __stdcall UpdateServer( GS::Connector::Handle hHandle, IN const Camel::Dar2::GSConnector::_ServerStatus& status );

			/////////////////////////////////////////////////////////////////////////////////////////////////
			// GS서버에서 특정 서버들을 요청. 결과 값을 가져온다.( 클라이언트에서 내가 적합한 서버를 가져올때 사용 )
			// hHandle : 커넥터 핸들, nMaxList : 결과값 최대 갯수, ProtocolVersion: 어떤 버전을 서비스하는 서버를 뽑아올 것인가. 
			// RequestOption: 0 - 가중치 / 서버 빈 자리 고려순서( 기본 로드 밸런싱 전략에 따른 서버 ), 1 - 가장 로드가 적은 서버, 2 - 가장 바쁜 서버( 꽉차지 않은 ). 3. 모든 서버
			// RequestParam: 어떤 서버를 가져올 것인지 넘겨줄 파라미터 옵션
			// ServerKind : 어떤 서버 종류인가를 판별하는 옵션( SS : 0, Relay : 1 )
			// ex ) Relay서버 한대 받아오려면.. GetServerList( handle, 1, 0, 1, 0, 0, buf );
			// 
			GSCONNECTOR_API bool __stdcall GetServerList( GS::Connector::Handle hHandle, IN const int MaxList, IN const Camel::Dar2::GSConnector::PIGEON_SERVER_KIND ServerKind, IN const int ProtocolVer, IN const int RequestOption, IN const int RequestParamReserved, OUT const Camel::Dar2::GSConnector::_ServerList** pResultBuf );
		}

	// GS 커넥터용 핸들
	#define GS_CONNECTOR_HANDLE GS::Connector::Handle
	};
}

#endif

