/**    
@file    SockDefine.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   SocketCamel Import / Export Define
*/
#pragma once

namespace NMBASE
{
	namespace SOCKCAMEL
	{

	#ifndef NMSOCKCAMEL_API
		#ifdef NMSOCKCAMEL_EXPORTS
			#define NMSOCKCAMEL_API		__declspec(dllexport )
		#else
			#define NMSOCKCAMEL_API		__declspec(dllimport )
		#endif 
	#endif

	#ifndef NMSOCKCAMEL_CLASS 
		#define NMSOCKCAMEL_CLASS NMSOCKCAMEL_API
	#endif



	///< SockCmnd.h
	/**      
	@brief	Camel 패킷 데이터 구조체
	*/
	typedef struct _tagCAMELNOTIFY
	{
		void* pData;
		void* rHeader;	// Camel::Dar::PacketHeader
		void* hStream;	// Camel::Base::System::Stream::Handle
		int nPacketSize;

	}SCAMELNOTIFY;


	typedef struct _tagCAMELERROR
	{
		void* pData;		// Base::Connector*
		void* hStream;		// Camel::Base::System::Stream::Handle
		int   nErrorCode;	// Error Code

	}SCAMELERROR;


	} //namespace SOCK
}  // namespace NMBASE