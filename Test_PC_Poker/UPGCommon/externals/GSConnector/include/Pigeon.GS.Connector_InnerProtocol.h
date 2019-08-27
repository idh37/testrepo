#pragma once

// DarCompiler 2.0.0.1 ver.

#include <Camel/Dar2/Dar2BaseMsg.h>
#include <Camel/Dar2/Dar2Rule_STREAM.h>
#if defined(__JSON_LIB_SUPPORT__)
#include <Camel/Dar2/Dar2Rule_JSON.h>
#endif // defined(__JSON_LIB_SUPPORT__)


namespace Camel
{
	namespace Dar2
	{
		namespace GSConnector
		{

			#if !defined __DarEnum_GSConnector_CURRENT_SERVER_ERROR_STATUS__MessageId_3522638927__
			#define __DarEnum_GSConnector_CURRENT_SERVER_ERROR_STATUS__MessageId_3522638927__
			enum CURRENT_SERVER_ERROR_STATUS
			{
				SERVER_NORMAL = 0,
				SERVER_UNDER_CONSTRUCTION = 1,
				SERVER_FAIL = 2,
			};

			// Camel::Dar2::GSConnector::CURRENT_SERVER_ERROR_STATUS enum => string
			inline const char* EnumToString(Camel::Dar2::GSConnector::CURRENT_SERVER_ERROR_STATUS eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::GSConnector::SERVER_NORMAL == eEnumValue)
				{
					return "Camel::Dar2::GSConnector::SERVER_NORMAL";
				}
				if ( Camel::Dar2::GSConnector::SERVER_UNDER_CONSTRUCTION == eEnumValue)
				{
					return "Camel::Dar2::GSConnector::SERVER_UNDER_CONSTRUCTION";
				}
				if ( Camel::Dar2::GSConnector::SERVER_FAIL == eEnumValue)
				{
					return "Camel::Dar2::GSConnector::SERVER_FAIL";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::GSConnector::CURRENT_SERVER_ERROR_STATUS string => enum
			inline Camel::Dar2::GSConnector::CURRENT_SERVER_ERROR_STATUS StringToEnum(const char* szStringValue, Camel::Dar2::GSConnector::CURRENT_SERVER_ERROR_STATUS eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::GSConnector::SERVER_NORMAL", szStringValue) == 0) 
				{
					return Camel::Dar2::GSConnector::SERVER_NORMAL;
				}
				if ( strcmp("Camel::Dar2::GSConnector::SERVER_UNDER_CONSTRUCTION", szStringValue) == 0) 
				{
					return Camel::Dar2::GSConnector::SERVER_UNDER_CONSTRUCTION;
				}
				if ( strcmp("Camel::Dar2::GSConnector::SERVER_FAIL", szStringValue) == 0) 
				{
					return Camel::Dar2::GSConnector::SERVER_FAIL;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_GSConnector_CURRENT_SERVER_ERROR_STATUS__MessageId_3522638927__
		}
		namespace GSConnector
		{

			#if !defined __DarEnum_GSConnector_PIGEON_SERVER_KIND__MessageId_444888039__
			#define __DarEnum_GSConnector_PIGEON_SERVER_KIND__MessageId_444888039__
			enum PIGEON_SERVER_KIND
			{
				PIGEON_SS = 0,
				VOICE_RELAY = 1,
			};

			// Camel::Dar2::GSConnector::PIGEON_SERVER_KIND enum => string
			inline const char* EnumToString(Camel::Dar2::GSConnector::PIGEON_SERVER_KIND eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::GSConnector::PIGEON_SS == eEnumValue)
				{
					return "Camel::Dar2::GSConnector::PIGEON_SS";
				}
				if ( Camel::Dar2::GSConnector::VOICE_RELAY == eEnumValue)
				{
					return "Camel::Dar2::GSConnector::VOICE_RELAY";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::GSConnector::PIGEON_SERVER_KIND string => enum
			inline Camel::Dar2::GSConnector::PIGEON_SERVER_KIND StringToEnum(const char* szStringValue, Camel::Dar2::GSConnector::PIGEON_SERVER_KIND eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::GSConnector::PIGEON_SS", szStringValue) == 0) 
				{
					return Camel::Dar2::GSConnector::PIGEON_SS;
				}
				if ( strcmp("Camel::Dar2::GSConnector::VOICE_RELAY", szStringValue) == 0) 
				{
					return Camel::Dar2::GSConnector::VOICE_RELAY;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_GSConnector_PIGEON_SERVER_KIND__MessageId_444888039__
		}

		#if !defined __DarMessageProtocol_GSConnector__ServerStatus__MessageId_1993037826__
		#define __DarMessageProtocol_GSConnector__ServerStatus__MessageId_1993037826__
		namespace GSConnector
		{
			class _ServerStatus : public IDarMsg
			{
			public:
				static const unsigned int  msgid=1993037826U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "GSConnector::_ServerStatus";	};

				// const member valiable declare

				// member valiable declare
				int nMaxCapacity;
				int nCurCapacity;
				int nWeightingFactor;
				Camel::Dar2::GSConnector::CURRENT_SERVER_ERROR_STATUS nCurStatus;
				std::string strIP;
				unsigned int nPort;
				int nServerVer;
				Camel::Dar2::GSConnector::PIGEON_SERVER_KIND nKind;

				// Optional member valiable declare

				_ServerStatus()
				{
					Init();
				}

				virtual ~_ServerStatus()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					nMaxCapacity = static_cast<int>(0);
					nCurCapacity = static_cast<int>(0);
					nWeightingFactor = static_cast<int>(0);
					nCurStatus = static_cast<Camel::Dar2::GSConnector::CURRENT_SERVER_ERROR_STATUS>(0);
					strIP = "";
					nPort = static_cast<unsigned int>(0);
					nServerVer = static_cast<int>(0);
					nKind = static_cast<Camel::Dar2::GSConnector::PIGEON_SERVER_KIND>(0);

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					::memset(&nMaxCapacity, 0, sizeof(nMaxCapacity));
					::memset(&nCurCapacity, 0, sizeof(nCurCapacity));
					::memset(&nWeightingFactor, 0, sizeof(nWeightingFactor));
					::memset(&nCurStatus, 0, sizeof(nCurStatus));
					strIP = "";
					::memset(&nPort, 0, sizeof(nPort));
					::memset(&nServerVer, 0, sizeof(nServerVer));
					::memset(&nKind, 0, sizeof(nKind));

					// Optional member variable clear
				}

				bool Copy(const _ServerStatus& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					nMaxCapacity = rhs.nMaxCapacity;
					nCurCapacity = rhs.nCurCapacity;
					nWeightingFactor = rhs.nWeightingFactor;
					nCurStatus = rhs.nCurStatus;
					strIP = rhs.strIP;
					nPort = rhs.nPort;
					nServerVer = rhs.nServerVer;
					nKind = rhs.nKind;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member nMaxCapacity
					nLength += sizeof(nMaxCapacity);
					// GetLength Member nCurCapacity
					nLength += sizeof(nCurCapacity);
					// GetLength Member nWeightingFactor
					nLength += sizeof(nWeightingFactor);
					// GetLength Member nCurStatus
					nLength += sizeof(nCurStatus);
					// GetLength Member strIP
					nLength += sizeof(int);
					nLength += strIP.length();
					// GetLength Member nPort
					nLength += sizeof(nPort);
					// GetLength Member nServerVer
					nLength += sizeof(nServerVer);
					// GetLength Member nKind
					nLength += sizeof(nKind);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, nMaxCapacity))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, nCurCapacity))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, nWeightingFactor))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::GSConnector::CURRENT_SERVER_ERROR_STATUS>( stream, nCurStatus))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, strIP))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<unsigned int>( stream, nPort))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, nServerVer))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::GSConnector::PIGEON_SERVER_KIND>( stream, nKind))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, nMaxCapacity, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, nCurCapacity, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, nWeightingFactor, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::GSConnector::CURRENT_SERVER_ERROR_STATUS>( clMessageBlock, nCurStatus, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, strIP, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<unsigned int>( clMessageBlock, nPort, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, nServerVer, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::GSConnector::PIGEON_SERVER_KIND>( clMessageBlock, nKind, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member nMaxCapacity
					Json_root["nMaxCapacity"] = nMaxCapacity;
					// Save Member nCurCapacity
					Json_root["nCurCapacity"] = nCurCapacity;
					// Save Member nWeightingFactor
					Json_root["nWeightingFactor"] = nWeightingFactor;
					// Save Member nCurStatus
					Json_root["nCurStatus"] = nCurStatus;
					// Save Member strIP
					Json_root["strIP"] = strIP;
					// Save Member nPort
					Json_root["nPort"] = nPort;
					// Save Member nServerVer
					Json_root["nServerVer"] = nServerVer;
					// Save Member nKind
					Json_root["nKind"] = nKind;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member nMaxCapacity
					if (! Json_root["nMaxCapacity"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["nMaxCapacity"], nMaxCapacity))	{	return false;	}
					}
					// Load Member nCurCapacity
					if (! Json_root["nCurCapacity"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["nCurCapacity"], nCurCapacity))	{	return false;	}
					}
					// Load Member nWeightingFactor
					if (! Json_root["nWeightingFactor"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["nWeightingFactor"], nWeightingFactor))	{	return false;	}
					}
					// Load Member nCurStatus
					if (! Json_root["nCurStatus"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["nCurStatus"], temp))	{	return false;	}
						nCurStatus = static_cast<Camel::Dar2::GSConnector::CURRENT_SERVER_ERROR_STATUS>(temp);
					}
					// Load Member strIP
					if (! Json_root["strIP"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["strIP"], strIP))	{	return false;	}
					}
					// Load Member nPort
					if (! Json_root["nPort"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["nPort"], nPort))	{	return false;	}
					}
					// Load Member nServerVer
					if (! Json_root["nServerVer"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["nServerVer"], nServerVer))	{	return false;	}
					}
					// Load Member nKind
					if (! Json_root["nKind"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["nKind"], temp))	{	return false;	}
						nKind = static_cast<Camel::Dar2::GSConnector::PIGEON_SERVER_KIND>(temp);
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_GSConnector__ServerStatus__MessageId_1993037826__

		#if !defined __DarMessageProtocol_GSConnector__ServerList__MessageId_2460366844__
		#define __DarMessageProtocol_GSConnector__ServerList__MessageId_2460366844__
		namespace GSConnector
		{
			class _ServerList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2460366844U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "GSConnector::_ServerList";	};

				// const member valiable declare

				// member valiable declare
				std::vector<Camel::Dar2::GSConnector::_ServerStatus* > AvaliableList;

				// Optional member valiable declare

				_ServerList()
				{
					Init();
				}

				virtual ~_ServerList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::vector<Camel::Dar2::GSConnector::_ServerStatus* >::iterator iter = AvaliableList.begin(); iter != AvaliableList.end(); ++iter)
					{
						Camel::Dar2::GSConnector::_ServerStatus* pItem = (*iter);
						delete pItem;
					}
					AvaliableList.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::vector<Camel::Dar2::GSConnector::_ServerStatus* >::iterator iter = AvaliableList.begin(); iter != AvaliableList.end(); ++iter)
					{
						Camel::Dar2::GSConnector::_ServerStatus* pItem = (*iter);
						delete pItem;
					}
					AvaliableList.clear();

					// Optional member variable clear
				}

				bool Copy(const _ServerList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::vector<Camel::Dar2::GSConnector::_ServerStatus* >::const_iterator iter = rhs.AvaliableList.begin(); iter != rhs.AvaliableList.end(); ++iter)
					{
						Camel::Dar2::GSConnector::_ServerStatus* pItem = new Camel::Dar2::GSConnector::_ServerStatus;
						pItem->Copy(*(*iter));
						AvaliableList.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member AvaliableList
					nLength += sizeof(int);
					for ( std::vector<Camel::Dar2::GSConnector::_ServerStatus* >::iterator iter = AvaliableList.begin(); iter != AvaliableList.end(); ++iter)
					{
						Camel::Dar2::GSConnector::_ServerStatus* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_AvaliableListSize = AvaliableList.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_AvaliableListSize ))      {      return false;     }
					for ( std::vector<Camel::Dar2::GSConnector::_ServerStatus* >::iterator iter = AvaliableList.begin(); iter != AvaliableList.end(); ++iter)
					{
						Camel::Dar2::GSConnector::_ServerStatus* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_AvaliableListSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_AvaliableListSize, bRequireChangeByteOrder))      {      return false;     }
					AvaliableList.reserve( n_AvaliableListSize);
					for ( unsigned int nIndex = 0; nIndex < n_AvaliableListSize ; ++nIndex)
					{
						Camel::Dar2::GSConnector::_ServerStatus* pTemp = new Camel::Dar2::GSConnector::_ServerStatus;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						AvaliableList.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member AvaliableList
					{
						Json::Value JsonValue_AvaliableList;
						for ( std::vector<Camel::Dar2::GSConnector::_ServerStatus* >::const_iterator iter = AvaliableList.begin(); iter != AvaliableList.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::GSConnector::_ServerStatus* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_AvaliableList.append( JsonValue_temp);
							}
						Json_root["AvaliableList"] = JsonValue_AvaliableList;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member AvaliableList
					if ( (! Json_root["AvaliableList"].isNull()) && (Json_root["AvaliableList"].isArray()))
					{
						unsigned int n_AvaliableListSize = Json_root["AvaliableList"].size();
						for ( unsigned int nIndex = 0; nIndex < n_AvaliableListSize ; ++nIndex)
						{
							Camel::Dar2::GSConnector::_ServerStatus* temp = new Camel::Dar2::GSConnector::_ServerStatus;
							if (false ==  temp->LoadJSON( Json_root["AvaliableList"][nIndex]))		{    delete temp;	return false;     }
							AvaliableList.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_GSConnector__ServerList__MessageId_2460366844__

		#if !defined __DarMessageProtocol_GSConnector__ServerStatusEx__MessageId_2670188466__
		#define __DarMessageProtocol_GSConnector__ServerStatusEx__MessageId_2670188466__
		namespace GSConnector
		{
			class _ServerStatusEx : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2670188466U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "GSConnector::_ServerStatusEx";	};

				// const member valiable declare

				// member valiable declare
				int nMaxCapacity;
				int nCurCapacity;
				int nWeightingFactor;
				Camel::Dar2::GSConnector::CURRENT_SERVER_ERROR_STATUS nCurStatus;
				std::string strIP;
				unsigned int nPort;
				Camel::Dar2::GSConnector::PIGEON_SERVER_KIND nKind;
				unsigned int nMajorVer;
				unsigned int nMinorVer;
				unsigned int nBuildVer1;
				unsigned int nBuildVer2;

				// Optional member valiable declare

				_ServerStatusEx()
				{
					Init();
				}

				virtual ~_ServerStatusEx()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					nMaxCapacity = static_cast<int>(0);
					nCurCapacity = static_cast<int>(0);
					nWeightingFactor = static_cast<int>(0);
					nCurStatus = static_cast<Camel::Dar2::GSConnector::CURRENT_SERVER_ERROR_STATUS>(0);
					strIP = "";
					nPort = static_cast<unsigned int>(0);
					nKind = static_cast<Camel::Dar2::GSConnector::PIGEON_SERVER_KIND>(0);
					nMajorVer = static_cast<unsigned int>(0);
					nMinorVer = static_cast<unsigned int>(0);
					nBuildVer1 = static_cast<unsigned int>(0);
					nBuildVer2 = static_cast<unsigned int>(0);

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					::memset(&nMaxCapacity, 0, sizeof(nMaxCapacity));
					::memset(&nCurCapacity, 0, sizeof(nCurCapacity));
					::memset(&nWeightingFactor, 0, sizeof(nWeightingFactor));
					::memset(&nCurStatus, 0, sizeof(nCurStatus));
					strIP = "";
					::memset(&nPort, 0, sizeof(nPort));
					::memset(&nKind, 0, sizeof(nKind));
					::memset(&nMajorVer, 0, sizeof(nMajorVer));
					::memset(&nMinorVer, 0, sizeof(nMinorVer));
					::memset(&nBuildVer1, 0, sizeof(nBuildVer1));
					::memset(&nBuildVer2, 0, sizeof(nBuildVer2));

					// Optional member variable clear
				}

				bool Copy(const _ServerStatusEx& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					nMaxCapacity = rhs.nMaxCapacity;
					nCurCapacity = rhs.nCurCapacity;
					nWeightingFactor = rhs.nWeightingFactor;
					nCurStatus = rhs.nCurStatus;
					strIP = rhs.strIP;
					nPort = rhs.nPort;
					nKind = rhs.nKind;
					nMajorVer = rhs.nMajorVer;
					nMinorVer = rhs.nMinorVer;
					nBuildVer1 = rhs.nBuildVer1;
					nBuildVer2 = rhs.nBuildVer2;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member nMaxCapacity
					nLength += sizeof(nMaxCapacity);
					// GetLength Member nCurCapacity
					nLength += sizeof(nCurCapacity);
					// GetLength Member nWeightingFactor
					nLength += sizeof(nWeightingFactor);
					// GetLength Member nCurStatus
					nLength += sizeof(nCurStatus);
					// GetLength Member strIP
					nLength += sizeof(int);
					nLength += strIP.length();
					// GetLength Member nPort
					nLength += sizeof(nPort);
					// GetLength Member nKind
					nLength += sizeof(nKind);
					// GetLength Member nMajorVer
					nLength += sizeof(nMajorVer);
					// GetLength Member nMinorVer
					nLength += sizeof(nMinorVer);
					// GetLength Member nBuildVer1
					nLength += sizeof(nBuildVer1);
					// GetLength Member nBuildVer2
					nLength += sizeof(nBuildVer2);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, nMaxCapacity))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, nCurCapacity))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, nWeightingFactor))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::GSConnector::CURRENT_SERVER_ERROR_STATUS>( stream, nCurStatus))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, strIP))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<unsigned int>( stream, nPort))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::GSConnector::PIGEON_SERVER_KIND>( stream, nKind))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<unsigned int>( stream, nMajorVer))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<unsigned int>( stream, nMinorVer))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<unsigned int>( stream, nBuildVer1))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<unsigned int>( stream, nBuildVer2))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, nMaxCapacity, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, nCurCapacity, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, nWeightingFactor, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::GSConnector::CURRENT_SERVER_ERROR_STATUS>( clMessageBlock, nCurStatus, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, strIP, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<unsigned int>( clMessageBlock, nPort, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::GSConnector::PIGEON_SERVER_KIND>( clMessageBlock, nKind, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<unsigned int>( clMessageBlock, nMajorVer, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<unsigned int>( clMessageBlock, nMinorVer, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<unsigned int>( clMessageBlock, nBuildVer1, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<unsigned int>( clMessageBlock, nBuildVer2, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member nMaxCapacity
					Json_root["nMaxCapacity"] = nMaxCapacity;
					// Save Member nCurCapacity
					Json_root["nCurCapacity"] = nCurCapacity;
					// Save Member nWeightingFactor
					Json_root["nWeightingFactor"] = nWeightingFactor;
					// Save Member nCurStatus
					Json_root["nCurStatus"] = nCurStatus;
					// Save Member strIP
					Json_root["strIP"] = strIP;
					// Save Member nPort
					Json_root["nPort"] = nPort;
					// Save Member nKind
					Json_root["nKind"] = nKind;
					// Save Member nMajorVer
					Json_root["nMajorVer"] = nMajorVer;
					// Save Member nMinorVer
					Json_root["nMinorVer"] = nMinorVer;
					// Save Member nBuildVer1
					Json_root["nBuildVer1"] = nBuildVer1;
					// Save Member nBuildVer2
					Json_root["nBuildVer2"] = nBuildVer2;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member nMaxCapacity
					if (! Json_root["nMaxCapacity"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["nMaxCapacity"], nMaxCapacity))	{	return false;	}
					}
					// Load Member nCurCapacity
					if (! Json_root["nCurCapacity"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["nCurCapacity"], nCurCapacity))	{	return false;	}
					}
					// Load Member nWeightingFactor
					if (! Json_root["nWeightingFactor"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["nWeightingFactor"], nWeightingFactor))	{	return false;	}
					}
					// Load Member nCurStatus
					if (! Json_root["nCurStatus"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["nCurStatus"], temp))	{	return false;	}
						nCurStatus = static_cast<Camel::Dar2::GSConnector::CURRENT_SERVER_ERROR_STATUS>(temp);
					}
					// Load Member strIP
					if (! Json_root["strIP"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["strIP"], strIP))	{	return false;	}
					}
					// Load Member nPort
					if (! Json_root["nPort"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["nPort"], nPort))	{	return false;	}
					}
					// Load Member nKind
					if (! Json_root["nKind"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["nKind"], temp))	{	return false;	}
						nKind = static_cast<Camel::Dar2::GSConnector::PIGEON_SERVER_KIND>(temp);
					}
					// Load Member nMajorVer
					if (! Json_root["nMajorVer"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["nMajorVer"], nMajorVer))	{	return false;	}
					}
					// Load Member nMinorVer
					if (! Json_root["nMinorVer"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["nMinorVer"], nMinorVer))	{	return false;	}
					}
					// Load Member nBuildVer1
					if (! Json_root["nBuildVer1"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["nBuildVer1"], nBuildVer1))	{	return false;	}
					}
					// Load Member nBuildVer2
					if (! Json_root["nBuildVer2"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["nBuildVer2"], nBuildVer2))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_GSConnector__ServerStatusEx__MessageId_2670188466__

		#if !defined __DarMessageProtocol_GSConnector__ServerListEx__MessageId_2609107302__
		#define __DarMessageProtocol_GSConnector__ServerListEx__MessageId_2609107302__
		namespace GSConnector
		{
			class _ServerListEx : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2609107302U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "GSConnector::_ServerListEx";	};

				// const member valiable declare

				// member valiable declare
				std::vector<Camel::Dar2::GSConnector::_ServerStatusEx* > AvaliableList;

				// Optional member valiable declare

				_ServerListEx()
				{
					Init();
				}

				virtual ~_ServerListEx()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::vector<Camel::Dar2::GSConnector::_ServerStatusEx* >::iterator iter = AvaliableList.begin(); iter != AvaliableList.end(); ++iter)
					{
						Camel::Dar2::GSConnector::_ServerStatusEx* pItem = (*iter);
						delete pItem;
					}
					AvaliableList.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::vector<Camel::Dar2::GSConnector::_ServerStatusEx* >::iterator iter = AvaliableList.begin(); iter != AvaliableList.end(); ++iter)
					{
						Camel::Dar2::GSConnector::_ServerStatusEx* pItem = (*iter);
						delete pItem;
					}
					AvaliableList.clear();

					// Optional member variable clear
				}

				bool Copy(const _ServerListEx& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::vector<Camel::Dar2::GSConnector::_ServerStatusEx* >::const_iterator iter = rhs.AvaliableList.begin(); iter != rhs.AvaliableList.end(); ++iter)
					{
						Camel::Dar2::GSConnector::_ServerStatusEx* pItem = new Camel::Dar2::GSConnector::_ServerStatusEx;
						pItem->Copy(*(*iter));
						AvaliableList.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member AvaliableList
					nLength += sizeof(int);
					for ( std::vector<Camel::Dar2::GSConnector::_ServerStatusEx* >::iterator iter = AvaliableList.begin(); iter != AvaliableList.end(); ++iter)
					{
						Camel::Dar2::GSConnector::_ServerStatusEx* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_AvaliableListSize = AvaliableList.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_AvaliableListSize ))      {      return false;     }
					for ( std::vector<Camel::Dar2::GSConnector::_ServerStatusEx* >::iterator iter = AvaliableList.begin(); iter != AvaliableList.end(); ++iter)
					{
						Camel::Dar2::GSConnector::_ServerStatusEx* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_AvaliableListSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_AvaliableListSize, bRequireChangeByteOrder))      {      return false;     }
					AvaliableList.reserve( n_AvaliableListSize);
					for ( unsigned int nIndex = 0; nIndex < n_AvaliableListSize ; ++nIndex)
					{
						Camel::Dar2::GSConnector::_ServerStatusEx* pTemp = new Camel::Dar2::GSConnector::_ServerStatusEx;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						AvaliableList.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member AvaliableList
					{
						Json::Value JsonValue_AvaliableList;
						for ( std::vector<Camel::Dar2::GSConnector::_ServerStatusEx* >::const_iterator iter = AvaliableList.begin(); iter != AvaliableList.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::GSConnector::_ServerStatusEx* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_AvaliableList.append( JsonValue_temp);
							}
						Json_root["AvaliableList"] = JsonValue_AvaliableList;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member AvaliableList
					if ( (! Json_root["AvaliableList"].isNull()) && (Json_root["AvaliableList"].isArray()))
					{
						unsigned int n_AvaliableListSize = Json_root["AvaliableList"].size();
						for ( unsigned int nIndex = 0; nIndex < n_AvaliableListSize ; ++nIndex)
						{
							Camel::Dar2::GSConnector::_ServerStatusEx* temp = new Camel::Dar2::GSConnector::_ServerStatusEx;
							if (false ==  temp->LoadJSON( Json_root["AvaliableList"][nIndex]))		{    delete temp;	return false;     }
							AvaliableList.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_GSConnector__ServerListEx__MessageId_2609107302__
	}
}
