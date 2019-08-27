/* MacRetriever.h
* Author: Peter Donahue
* Contact (709) 891-1129 (ask for Peter)
* peter.donahue@comdev.ca
* techinterface@yahoo.ca
*
* This class simplifies the task of retrieving the MAC address of a particular
* card under MS Windows.
*/

#pragma once


#include <windows.h>
#include <stdlib.h>
#include <snmp.h>
#include <nb30.h>
#include <rpc.h>
#include <rpcdce.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>

namespace NMBASE
{
	namespace UTIL
	{

		typedef SNMPAPI (WINAPI *t_SnmpExtensionInit)(DWORD dwUptimeReference, HANDLE *phSubagentTrapEvent, AsnObjectIdentifier *pFirstSupportedRegion);
		typedef SNMPAPI (WINAPI *t_SnmpExtensionQuery)(BYTE bPduType, SnmpVarBindList *pVarBindList, AsnInteger32 *pErrorStatus, AsnInteger32 *pErrorIndex);
		typedef SNMPAPI (WINAPI *t_SnmpUtilOidCpy)(AsnObjectIdentifier * pOidDst, AsnObjectIdentifier * pOidSrc);
		typedef SNMPAPI (WINAPI *t_SnmpUtilOidNCmp)(AsnObjectIdentifier * pOidDst, AsnObjectIdentifier * pOidSrc, UINT nSubIds);
		typedef int (WINAPI *t_WSAStartup)(WORD wVersionRequested, LPWSADATA lpWSAData);
		typedef int (WINAPI *t_WSACleanup)();

		typedef UCHAR (WINAPI *t_Netbios)(PNCB pncb);

		typedef HRESULT (WINAPI *t_UuidCreate)(UUID *puuid);

		class CMacRetriever
		{
		public:
			CMacRetriever(void);
			~CMacRetriever(void);

			bool GetMacUsingGuid(char *address, int cardIndex);
			bool GetMacUsingNetBIOS(char *address, int cardIndex);
			bool GetMacUsingSNMP(char *address, int cardIndex); // The call to get the MAC address.
			const char* GetErrorMessage() {return m_error;}; // Returns the most recent error (should only be called after a function fails)

			static void CopyAddressToString(unsigned char hexAddr[6], char *strAddr);

			// These will be called automatically, but are public in case you would like to 
			// unload the DLLs before this object goes out of scope.
			bool InitializeGuid();
			void UnInitializeGuid();
			bool InitializeSNMP();  
			void UnInitializeSNMP();
			bool InitializeNetBIOS();  
			void UnInitializeNetBIOS();

		protected:
			// function pointers for SNMP and winsock
			t_SnmpExtensionInit m_ptrSnmpExtensionInit;
			t_SnmpExtensionQuery m_ptrSnmpExtensionQuery;
			t_SnmpUtilOidCpy m_ptrSnmpUtilOidCpy;
			t_SnmpUtilOidNCmp m_ptrSnmpUtilOidNCmp;
			t_WSAStartup m_ptrWSAStartup;
			t_WSACleanup m_ptrWSACleanup;

			// dll handles for SNMP and winsock
			HINSTANCE m_snmpApiDll;
			HINSTANCE m_inetDll;
			HINSTANCE m_wsockDll;
			bool m_snmpInitialized;

			// function pointer for Netbios
			bool m_netbiosInitialized;
			HINSTANCE m_netbiosDll;
			t_Netbios m_ptrNetbios;

			//function pointer for UuidCreate
			bool m_uuidInitialized;
			HINSTANCE m_uuidDll;
			t_UuidCreate m_ptrUuidCreate;

			char m_error[256];  //  This string will contain an error message if any function fails.

		};

	}//namespace UTIL

}//namespace NMBASE
