#ifndef	__CLIENT_CONNECTOR_API_H__
#define __CLIENT_CONNECTOR_API_H__

#include "SockCDefine.h"
#include "Camel/Dar/DarBaseMsg.h"
#include "Camel/Dar/DarDispatcher.h"

namespace Client
{
	namespace Connector
	{
		typedef struct _Handle 
		{

		} *THandle, *Handle;

		NMSOCKC_API Handle __stdcall CreateHandle ();
		NMSOCKC_API void   __stdcall DestroyHandle( Handle hCon );

		NMSOCKC_API bool   __stdcall Connect( Handle hCon, const char* szAddr, unsigned short nPort );
		NMSOCKC_API void   __stdcall Disconnect( Handle hCon );

		typedef bool (__stdcall	*OnRecievedCallback )	( void* , Camel::Dar::PacketHeader& rHeader, Camel::Base::System::Stream::Handle, int nPacketSize);
		typedef bool (__stdcall	*OnDisconnectedCallback )	( void* );

		NMSOCKC_API bool __stdcall SetOnRecieved( Handle hCon, OnRecievedCallback pCallback, void* pData);
		NMSOCKC_API bool __stdcall SetOnDisconnected( Handle hCon, OnDisconnectedCallback pCallback, void* pData);

		NMSOCKC_API bool __stdcall Send( Handle hCon, Camel::Dar::IDarMsg* pMessage);
	}
}

#endif	//	__CLIENT_CONNECTOR_API_H__