#pragma once

#include <Camel/Dar2/Dar2BaseMsg.h>
#include <Camel/Dar2/DAR2Rule_STREAM.h>
#if defined(__JSON_LIB_SUPPORT__)
#include <Camel/Dar2/DAR2Rule_JSON.h>
#endif // defined(__JSON_LIB_SUPPORT__)


namespace Camel
{
	namespace Dar2
	{

		#if !defined __Dar2MessageProtocol_gameinfo__MessageId_3264559152__
		#define __Dar2MessageProtocol_gameinfo__MessageId_3264559152__
		class gameinfo : public IDarMsg
		{
		public:
			static const unsigned int  msgid=3264559152;

			virtual unsigned int GetMessageId()		{	return msgid;	};
			virtual  const char* GetMessageName()	{	return "gameinfo";	};

			// member valiable declare
			int roomnum;
			int gameid;
			__int64 money;

			gameinfo()
			{
				Clear();
			}

			virtual ~gameinfo()
			{
				Clear();
			}

			virtual void Clear()
			{
				// Clear Member roomnum
				::memset(&roomnum, 0, sizeof(roomnum));
				// Clear Member gameid
				::memset(&gameid, 0, sizeof(gameid));
				// Clear Member money
				::memset(&money, 0, sizeof(money));
			}

			bool Copy(const gameinfo& rhs)
			{
				if (this == &rhs)
				{
					return false;
				}
				Clear();
				// copy Member roomnum
				roomnum = rhs.roomnum;
				// copy Member gameid
				gameid = rhs.gameid;
				// copy Member money
				money = rhs.money;
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
				// GetLength Member roomnum
				nLength += sizeof(roomnum);
				// GetLength Member gameid
				nLength += sizeof(gameid);
				// GetLength Member money
				nLength += sizeof(money);

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
				// Save Member roomnum
				if (false ==  DAR2Rule_STREAM::Save<int>( stream, roomnum))		{    return false;     }
				// Save Member gameid
				if (false ==  DAR2Rule_STREAM::Save<int>( stream, gameid))		{    return false;     }
				// Save Member money
				if (false ==  DAR2Rule_STREAM::Save<__int64>( stream, money))		{    return false;     }

				return true;
			}

			bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
			{
				// Load Member roomnum
				if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, roomnum, bRequireChangeByteOrder))		{    return false;     }
				// Load Member gameid
				if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, gameid, bRequireChangeByteOrder))		{    return false;     }
				// Load Member money
				if (false ==  DAR2Rule_STREAM::Load<__int64>( clMessageBlock, money, bRequireChangeByteOrder))		{    return false;     }
				return true;
			}

			#if defined(__JSON_LIB_SUPPORT__)
			// save JSON Format
			virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST)
			{
				Json::Value Json_root;
				if (false == SaveJSON(Json_root))	{	return false;	}
				if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
				else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
				return true;
			}

			bool SaveJSON(Json::Value& Json_root)
			{
				// Save Member roomnum
				Json_root["roomnum"] = roomnum;
				// Save Member gameid
				Json_root["gameid"] = gameid;
				// Save Member money
				Json_root["money"] = money;
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
				Clear();
				// Load Member roomnum
				if (! Json_root["roomnum"].isNull())
				{
					if (false == DAR2Rule_JSON::Load(Json_root["roomnum"], roomnum))	{	return false;	}
				}
				// Load Member gameid
				if (! Json_root["gameid"].isNull())
				{
					if (false == DAR2Rule_JSON::Load(Json_root["gameid"], gameid))	{	return false;	}
				}
				// Load Member money
				if (! Json_root["money"].isNull())
				{
					if (false == DAR2Rule_JSON::Load(Json_root["money"], money))	{	return false;	}
				}
				return true;
			}

			#endif // defined(__JSON_LIB_SUPPORT__)
		};

		#endif     // !defined __Dar2MessageProtocol_gameinfo__MessageId_3264559152__
	}
}
