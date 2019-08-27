#include "stdafx.h"
#include <ipHlpapi.h>
#include "IPAdapterInfo.h"
#include "MacRetriever.h"

namespace NMBASE
{
	namespace UTIL
	{

		LPCTSTR GetGateway()
		{
			PIP_ADAPTER_INFO pAdapterInfo;
			PIP_ADAPTER_INFO pAdapter = NULL;
			DWORD dwRetVal = 0;

			pAdapterInfo = (IP_ADAPTER_INFO *)malloc( sizeof( IP_ADAPTER_INFO ) );
			ULONG ulOutBufLen = sizeof( IP_ADAPTER_INFO );

			// Make an initial call to GetAdaptersInfo to get
			// the necessary size into the ulOutBufLen variable
			if ( GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW )
			{
				free( pAdapterInfo );
				pAdapterInfo = ( IP_ADAPTER_INFO * )malloc( ulOutBufLen ); 
			}

			if ( ERROR_SUCCESS == ( dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen ) ) )
			{
				CString strGateway;
				pAdapter = pAdapterInfo;
				while( pAdapter )
				{
					strGateway = pAdapter->GatewayList.IpAddress.String;
					pAdapter = pAdapter->Next;		// Progress through linked list
				}
				free( pAdapterInfo );
				return (LPCTSTR)strGateway;
			}
			else
			{
				free( pAdapterInfo );

				CString strTemp;
				strTemp = _T("0.0.0.0");

				return (LPCTSTR)strTemp;
			}
		}

		bool GetMacUsingIPHLP( LPCTSTR lpszMacAddr )
		{
			PIP_ADAPTER_INFO pAdapterInfo;
			PIP_ADAPTER_INFO pAdapter = NULL;
			DWORD dwRetVal = 0;

			pAdapterInfo = (IP_ADAPTER_INFO *)malloc( sizeof( IP_ADAPTER_INFO ) );
			ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

			// Make an initial call to GetAdaptersInfo to get
			// the necessary size into the ulOutBufLen variable
			if ( GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW )
			{
				free( pAdapterInfo );
				pAdapterInfo = ( IP_ADAPTER_INFO * )malloc( ulOutBufLen ); 
			}

			if ( ERROR_SUCCESS == ( dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen ) ) )
			{
				pAdapter = pAdapterInfo;
				while( pAdapter )
				{
					BYTE *MACData = pAdapter->Address;
					if( MACData )
					{
						wsprintf((LPTSTR)lpszMacAddr, _T("%02X%02X%02X%02X%02X%02X"), MACData[0] , MACData[1] , MACData[2] , MACData[3] , MACData[4] , MACData[5]);

						free( pAdapterInfo );
						return true;
					}

					pAdapter = pAdapter->Next;		// Progress through linked list
				}
				free( pAdapterInfo );
				return true;
			}
			else
			{
				free( pAdapterInfo );
				wsprintf((LPTSTR)lpszMacAddr, _T("000000000000"));
				return false;
			}
		}

		LPCTSTR GetMACAddress()
		{
			int i = 0;
			char address[13] = {0, };
			CMacRetriever mac;
			CString strMAC = _T("00-00-00-00-00-00");

			// 1. Using ipHlpapi
			if(GetMacUsingIPHLP(strMAC.GetBuffer()))
			{
				// GetMacUsingIPHLP = lpszMacAddr;
				// Release GetBuffer()
				strMAC.ReleaseBuffer();
				return (LPCTSTR)strMAC;
			}
			// Release GetBuffer() 
			strMAC.ReleaseBuffer();

			// 2. Using NetBIOS
			for(i = 0; i < 6; ++i)
			{
				if (mac.GetMacUsingNetBIOS(address, i)) 
				{
					// Ignore NULL addresses returned by other network interfaces 
					if (strncmp(address, "4445535400", 10) == 0)
						continue;
					// Ignore all dial-up networking adapters 
					if (strncmp(address, "000000000000", 12) == 0)
						continue;

					//wsprintf((LPTSTR)lpszMacAddr, _T("%s"), address);
					strMAC.Format(_T("%s"), address);

					return (LPCTSTR)strMAC;
				}
			}

			// 3. Using SNMP
			for(i = 0; i < 6; ++i)
			{
				if (mac.GetMacUsingSNMP(address, i)) 
				{
					// Ignore NULL addresses returned by other network interfaces 
					if (strncmp(address, "4445535400", 10) == 0)
						continue;
					// Ignore all dial-up networking adapters 
					if (strncmp(address, "000000000000", 12) == 0)
						continue;

					//wsprintf((LPTSTR)lpszMacAddr, _T("%s"), address);
					strMAC.Format(_T("%s"), address);

					return (LPCTSTR)strMAC;
				}
			}

			// 4. Using GUID
			for(i = 0; i < 6; ++i)
			{
				if (mac.GetMacUsingGuid(address, i)) 
				{
					// Ignore NULL addresses returned by other network interfaces 
					if (strncmp(address, "4445535400", 10) == 0)
						continue;
					// Ignore all dial-up networking adapters 
					if (strncmp(address, "000000000000", 12) == 0)
						continue;

					strMAC.Format(_T("%s"), address);

					return (LPCTSTR)strMAC;
				}
			}

			return (LPCTSTR)strMAC;
		}

	}//namespace UTIL

}//namespace NMBASE
