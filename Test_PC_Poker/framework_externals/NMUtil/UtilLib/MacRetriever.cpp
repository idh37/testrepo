/* MacRetriever.cpp
* Author: Peter Donahue
* Contact (709) 891-1129 (ask for Peter)
* peter.donahue@comdev.ca
* techinterface@yahoo.ca
*
* This class simplifies the task of retrieving the MAC address of a particular
* card under MS Windows.
*/
#include "stdafx.h"
// Uncomment this line if you are using VC++ precompiled headers through stdafx.h.
#include <stdio.h>
#include "MacRetriever.h"

namespace NMBASE
{
	namespace UTIL
	{

		CMacRetriever::CMacRetriever(void)
		{
			m_ptrSnmpExtensionInit=NULL;
			m_ptrSnmpExtensionQuery=NULL;
			m_ptrSnmpUtilOidCpy=NULL;
			m_ptrSnmpUtilOidNCmp=NULL;
			m_ptrWSAStartup=NULL;
			m_ptrWSACleanup=NULL;

			m_snmpApiDll=NULL;
			m_inetDll=NULL;
			m_wsockDll=NULL;
			m_snmpInitialized=false;

			m_ptrNetbios=NULL;
			m_netbiosDll=NULL;
			m_netbiosInitialized=false;

			m_ptrUuidCreate=NULL;
			m_uuidDll=NULL;
			m_uuidInitialized=false;
		}

		CMacRetriever::~CMacRetriever(void)
		{
			UnInitializeSNMP();
			UnInitializeNetBIOS();
			UnInitializeGuid();
		}

		// Address must be at least 13 characters long.
		bool CMacRetriever::GetMacUsingSNMP(char *address, int cardIndex)
		{
			// This function used to be implemented using GUID, but that approach doesn't work
			// for Win2000...  This new approach uses SNMP.  Hopefully it will be a little more
			// robust.
			address[0]='\0';
			if (!InitializeSNMP()) 
				return false;

			// Get the <addressNum> MAC address
			long errorStatus=0;
			long errorIndex=0;
			SnmpVarBindList bindList;
			SnmpVarBind bindVar;
			AsnObjectIdentifier tmpName;
			memset(&bindVar, 0, sizeof(bindVar));
			memset(&bindVar, 0, sizeof(bindList));

			bindList.len	= 1;
			bindList.list	= &bindVar;

			const UINT MAC_ADDRESS_OID[] = {1, 3, 6, 1, 2, 1, 2, 2, 1, 6}; 
			tmpName.idLength= 10;
			tmpName.ids		= (UINT*)MAC_ADDRESS_OID;

			if (!m_ptrSnmpUtilOidCpy(&bindVar.name, &tmpName))
			{
				strcpy_s(m_error, 256, "Error copies the variable pointed to by Oid");
				return false;
			}

			for(int i=0;i<cardIndex;++i)
			{
				if (!m_ptrSnmpExtensionQuery(SNMP_PDU_GETNEXT, &bindList, &errorStatus, &errorIndex))
				{
					strcpy_s(m_error, 256, "Error getting address from SNMP");
					return false;
				}
			}

			// Confirm that we have an address here
			if (m_ptrSnmpUtilOidNCmp(&bindVar.name, &tmpName, tmpName.idLength)) 
			{
				strcpy_s(m_error, 256, "Error getting address from SNMP");
				return false;
			}

			// bindVar should now have the address.
			unsigned char *addr=bindVar.value.asnValue.address.stream;
			if (!addr || addr==(UCHAR*)1)
			{
				strcpy_s(m_error, 256, "No address found at this index");
				return false;
			}

			// Copy addr into address
			address[0]='\0';
			CopyAddressToString(addr,address);

			return true;
		}

		void CMacRetriever::UnInitializeSNMP(void)
		{
			if (m_ptrWSACleanup) m_ptrWSACleanup();
			if (m_inetDll) FreeLibrary(m_inetDll);
			if (m_snmpApiDll) FreeLibrary(m_snmpApiDll);
			if (m_wsockDll) FreeLibrary(m_wsockDll);

			m_ptrWSACleanup=NULL;
			m_inetDll=NULL;
			m_snmpApiDll=NULL;
			m_wsockDll=NULL;

			m_snmpInitialized=false;
		}

		// Load the SNMP and winsock dlls and get the required function pointers.
		bool CMacRetriever::InitializeSNMP(void)
		{
			if (m_snmpInitialized) 
				return true;

			// Load DLLs
			m_wsockDll=LoadLibrary("ws2_32.dll");
			if (!m_wsockDll)
			{
				strcpy_s(m_error, 256, "Unable to load inetmib1.dll (SNMP dll)");
				return false;
			}

			m_inetDll = LoadLibrary("inetmib1.dll");
			if (!m_inetDll)
			{
				strcpy_s(m_error, 256, "Unable to load inetmib1.dll (SNMP dll)");
				return false;
			}

			m_snmpApiDll=LoadLibrary("snmpapi.dll");
			if (!m_snmpApiDll)
			{
				strcpy_s(m_error, 256, "Unable to load snmpapi.dll (SNMP dll)");
				return false;
			}

			// Get the SNMP function pointers.
			m_ptrSnmpExtensionInit	= (t_SnmpExtensionInit)::GetProcAddress(m_inetDll, "SnmpExtensionInit");
			m_ptrSnmpExtensionQuery	= (t_SnmpExtensionQuery)::GetProcAddress(m_inetDll, "SnmpExtensionQuery");
			m_ptrSnmpUtilOidCpy		= (t_SnmpUtilOidCpy)::GetProcAddress(m_snmpApiDll, "SnmpUtilOidCpy");
			m_ptrSnmpUtilOidNCmp	= (t_SnmpUtilOidNCmp)::GetProcAddress(m_snmpApiDll, "SnmpUtilOidNCmp");

			if (!m_ptrSnmpExtensionInit || !m_ptrSnmpExtensionQuery  || !m_ptrSnmpUtilOidCpy || !m_ptrSnmpUtilOidNCmp)
			{
				strcpy_s(m_error, 256, "Error getting functions from SNMP dll.  You may not have the correct version of SNMP installed.");
				return false;
			}

			// Get the winsock pointers.
			m_ptrWSAStartup = (t_WSAStartup)::GetProcAddress(m_wsockDll, "WSAStartup");
			m_ptrWSACleanup = (t_WSACleanup)::GetProcAddress(m_wsockDll, "WSACleanup");
			if (!m_ptrWSAStartup || !m_ptrWSACleanup)
			{
				strcpy_s(m_error, 256, "Error getting functions from winsock dll.  Your winsock libs may not be installed properly.");
				// Set to null so they won't be called from UnInitialize
				m_ptrWSACleanup=NULL;
				return false;
			}

			// Call init routine for winsock.
			WSADATA sockData;
			if (m_ptrWSAStartup(MAKEWORD(2, 0), &sockData))
			{
				strcpy_s(m_error, 256, "Winsock init failed.");
				m_ptrWSACleanup=NULL;
				return false;
			}

			// Call init routine for SNMP
			HANDLE eventTrap;
			AsnObjectIdentifier id;
			if (!m_ptrSnmpExtensionInit(0, &eventTrap, &id))
			{
				strcpy_s(m_error, 256, "SNMP init failed.");
				return false;
			}

			m_snmpInitialized=true;
			return true;
		}

		typedef struct tagASTAT
		{
			ADAPTER_STATUS adapt;
			NAME_BUFFER    NameBuff [30];
		}ASTAT;

		//  This function can only handle a single address for the machine.
		bool CMacRetriever::GetMacUsingGuid(char *address, int cardIndex)
		{
			if (!InitializeGuid()) 
				return false;

			address[0]='\0';
			if (cardIndex)
			{
				strcpy_s(m_error, 256, "CardIndex must be 0 when calling GetMacUsingGuid.  If you need other MACs use GetMacUsingNetBIOS or GetMacUsingSNMP.");
				return false;
			}

			UUID uuid;
			if (m_ptrUuidCreate(&uuid) != RPC_S_OK)
			{
				strcpy_s(m_error, 256, "UuidCreate returned an invalid GUID");
				return false;
			}

			CopyAddressToString(&uuid.Data4[2], address);

			return true;
		}

		bool CMacRetriever::GetMacUsingNetBIOS(char *address, int cardIndex)
		{
			// Taken from MSDN article on finding the MAC address through NetBios.
			address[0]='\0';
			if (!InitializeNetBIOS()) return false;

			NCB ncb;
			ASTAT Adapter;
			memset(&ncb,0,sizeof(ncb));
			ncb.ncb_command=NCBRESET;
			ncb.ncb_lana_num = (UCHAR)cardIndex;
			if (m_ptrNetbios(&ncb) != NRC_GOODRET)
			{
				switch(ncb.ncb_retcode)
				{
				case 0x23:
					strcpy_s(m_error, 256, "No NetBIOS interface found at this index");
					break;

				default:
					strcpy_s(m_error, 256, "Error calling NetBIOS reset function");
				}
				return false;
			}

			memset(&ncb,0,sizeof(ncb));
			ncb.ncb_command=NCBASTAT;
			ncb.ncb_lana_num=(UCHAR)cardIndex;
			strcpy_s((char*)ncb.ncb_callname, NCBNAMSZ, "*               ");
			ncb.ncb_buffer=(unsigned char*)&Adapter;
			ncb.ncb_length=sizeof(Adapter);
			if (m_ptrNetbios(&ncb) != NRC_GOODRET)
			{
				strcpy_s(m_error, 256, "MAC address retrieval from NetBIOS failed");
				return false;
			}

			CopyAddressToString(Adapter.adapt.adapter_address, address);    
			return true;
		}

		void CMacRetriever::CopyAddressToString(unsigned char hexAddr[6], char *strAddr)
		{
#pragma warning( push )
#pragma warning( disable:4996 )
			sprintf(strAddr, "%02x%02x%02x%02x%02x%02x",
				hexAddr[0],
				hexAddr[1],
				hexAddr[2],
				hexAddr[3],
				hexAddr[4],
				hexAddr[5]);
#pragma warning( pop )
			/*
			char tmp[3];

			strAddr[0]='\0';
			for(int i=0;i<6;++i){
			itoa(hexAddr[i], tmp, 16);
			if (hexAddr[i] <= 0x0f) strcat(strAddr, "0");
			strcat(strAddr, tmp);
			}
			*/
		}

		void CMacRetriever::UnInitializeNetBIOS(void)
		{
			if (m_netbiosDll) FreeLibrary(m_netbiosDll);
			m_netbiosDll=NULL;
			m_netbiosInitialized=false;
		}

		bool CMacRetriever::InitializeNetBIOS(void)
		{

			if (m_netbiosInitialized)
				return true;

			// Load DLLs
			m_netbiosDll=LoadLibrary("Netapi32.dll");
			if (!m_netbiosDll)
			{
				strcpy_s(m_error, 256, "Unable to load Netapi32.dll (Netbios dll)");
				return false;
			}

			// Get the NetBIOS function pointer.
			m_ptrNetbios=(t_Netbios)::GetProcAddress(m_netbiosDll, "Netbios");

			if (!m_ptrNetbios)
			{
				strcpy_s(m_error, 256, "Error getting function from NetBIOS dll");
				return false;
			}

			m_netbiosInitialized=true;

			return true;
		}

		void CMacRetriever::UnInitializeGuid(void)
		{
			if (m_uuidDll) 
				FreeLibrary(m_uuidDll);

			m_uuidDll=NULL;
			m_uuidInitialized=false;
		}

		bool CMacRetriever::InitializeGuid(void)
		{

			// We need a function to return a valid GUID (UUID including MAC address)
			if (m_uuidInitialized) 
				return true;

			// Load DLLs
			m_uuidDll=LoadLibrary("rpcrt4.dll");
			if (!m_uuidDll)
			{
				strcpy_s(m_error, 256, "Unable to load rpcrt.dll (RPC dll for UuidCreate)");
				return false;
			}

			// Get the UuidCreate function pointer.
			m_ptrUuidCreate=(t_UuidCreate)::GetProcAddress(m_uuidDll, "UuidCreateSequential");
			if (!m_ptrUuidCreate)
			{
				// We may be on an older OS.  Try to get old function
				m_ptrUuidCreate=(t_UuidCreate)::GetProcAddress(m_uuidDll, "UuidCreate");
			}
			if (!m_ptrUuidCreate)
			{
				strcpy_s(m_error, 256, "Error getting UuidCreate function from rpcrt4.dll");
				return false;
			}

			m_uuidInitialized=true;

			return true;
		}

	}//namespace UTIL

}//namespace NMBASE