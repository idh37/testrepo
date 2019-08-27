#pragma once

#include <string>
#include <Json/json.h>
#include "Dar2BaseMsg.h"

namespace Camel
{
	namespace Dar2
	{
		namespace DAR2Rule_JSON
		{
			// Save

			// Load
			// bool
			inline bool Load( const Json::Value& rJsonValue, bool& value)
			{
				if (rJsonValue.isNull())
				{
					return false;
				}
				if (! rJsonValue.isBool())
				{
					return false;
				}
				value = rJsonValue.asBool();
				return true;
			}

			// uchar
			inline bool Load( const Json::Value& rJsonValue, unsigned char& value)
			{
				if (rJsonValue.isNull())
				{
					return false;
				}
				if (! rJsonValue.isString())
				{
					return false;
				}
				std::string temp = rJsonValue.asString();
				if ( temp.length() != 1)
				{
					return false;
				}
				value = temp[0];
				return true;
			}
			// char
			inline bool Load( const Json::Value& rJsonValue, char& value)
			{
				if (rJsonValue.isNull())
				{
					return false;
				}
				if (! rJsonValue.isString())
				{
					return false;
				}
				std::string temp = rJsonValue.asString();
				if ( temp.length() != 1)
				{
					return false;
				}
				value = temp[0];
				return true;
			}

			// uint16
			inline bool Load( const Json::Value& rJsonValue, unsigned short& value)
			{
				if (! rJsonValue.isNumeric())
				{
					return false;
				}
				value = static_cast<unsigned short>(rJsonValue.asUInt());
				return true;
			}

			// int16
			inline bool Load( const Json::Value& rJsonValue, short& value)
			{
				if (! rJsonValue.isNumeric())
				{
					return false;
				}
				value = static_cast<unsigned short>(rJsonValue.asInt());
				return true;
			}

			// uint32
			inline bool Load( const Json::Value& rJsonValue, unsigned int& value)
			{
				if (! rJsonValue.isNumeric())
				{
					return false;
				}
				value = rJsonValue.asUInt();
				return true;
			}

			// int32
			inline bool Load( const Json::Value& rJsonValue, int& value)
			{
				if (! rJsonValue.isNumeric())
				{
					return false;
				}
				value = rJsonValue.asInt();
				return true;
			}

			// uint64
			inline bool Load( const Json::Value& rJsonValue, unsigned __int64& value)
			{
				if (! rJsonValue.isNumeric())
				{
					return false;
				}
				value = rJsonValue.asUInt64();
				return true;
			}

			// int64
			inline bool Load( const Json::Value& rJsonValue, __int64& value)
			{
				if (! rJsonValue.isNumeric())
				{
					return false;
				}
				value = rJsonValue.asInt64();
				return true;
			}

			// float32
			inline bool Load( const Json::Value& rJsonValue, float& value)
			{
				if (! rJsonValue.isNumeric())
				{
					return false;
				}
				value = rJsonValue.asFloat();
				return true;
			}

			// float64
			inline bool Load( const Json::Value& rJsonValue, double& value)
			{
				if (! rJsonValue.isNumeric())
				{
					return false;
				}
				value = rJsonValue.asDouble();
				return true;
			}

			// string
			//string
			inline bool Load( const Json::Value& rJsonValue, std::string& value)
			{
				if (! rJsonValue.isString())
				{
					return false;
				}
				value = rJsonValue.asString();
				return true;
			}
		}
	}
}