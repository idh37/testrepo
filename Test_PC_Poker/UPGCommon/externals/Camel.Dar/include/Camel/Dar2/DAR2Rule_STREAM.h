#pragma once

#include <string>
#include <Camel/Base/System/Stream.h>
#include "Dar2BaseMsg.h"

namespace Camel
{
	namespace Dar2
	{
		template<typename _Ty>
		inline void ChangeToByteOrder(_Ty* p)
		{
			register int i = 0;
			register int j = sizeof(_Ty) - 1;

			while (i < j)
			{
				std::swap(reinterpret_cast<char *>(p)[i], reinterpret_cast<char *>(p)[j]);
				++i;
				--j;
			}
		}

		namespace DAR2Rule_STREAM
		{
			inline bool Skip( MessageBlock& rMessageBlock, unsigned int skipsize)
			{
				if ( rMessageBlock.Skip(skipsize) != skipsize)
				{
					return false;
				}
				return true;
			}

			template<typename _Ty>
			inline bool Save( Camel::Base::System::Stream::Handle stream, _Ty& value)
			{
				if (  Camel::Base::System::Stream::Write(stream, &value, sizeof(_Ty)) != sizeof(_Ty))
				{
					return false;
				}
				return true;
			}

			template<typename _Ty>
			inline bool Save( Camel::Base::System::Stream::Handle stream, _Ty* value, int nArraySize)
			{
				int nBufferSize = sizeof(_Ty)*nArraySize;
				if (  Camel::Base::System::Stream::Write(stream, value, sizeof(_Ty)*nArraySize) != nBufferSize)
				{
					return false;
				}
				return true;
			}

			template<typename _Ty>
			inline bool Load( MessageBlock& rMessageBlock, _Ty& value, bool bRequireChangeByteOrder)
			{
				if ( rMessageBlock.Read(&value, sizeof(_Ty)) != sizeof(_Ty))
				{
					return false;
				}
				if ( sizeof(_Ty) != 1 && bRequireChangeByteOrder == true)
				{
					ChangeToByteOrder<_Ty>( &value);
				}
				return true;
			}

			template<typename _Ty>
			inline bool Load( MessageBlock& rMessageBlock, _Ty* value, int nArraySize, bool bRequireChangeByteOrder)
			{
				int nBufferSize = sizeof(_Ty)*nArraySize;
				if ( rMessageBlock.Read(value, nBufferSize) != nBufferSize)
				{
					return false;
				}
				if ( sizeof(_Ty) != 1 && bRequireChangeByteOrder == true)
				{
					for (int i = 0; i < nArraySize; ++i)
					{
						ChangeToByteOrder<_Ty>( &value[i]);
					}
				}
				return true;
			}

		//string
			template<>
			inline bool Save(Base::System::Stream::Handle stream, std::string& strValue )
			{
				int nSize = strValue.length();
				if ( false == Save<int>(stream, nSize))
				{
					return false;
				}
				if ( false == Save<const char>(stream, strValue.c_str(), nSize))
				{
					return false;
				}
				return true;
			}

			template<>
			inline bool Save(Base::System::Stream::Handle stream, const std::string& strValue )
			{
				int nSize = strValue.length();
				if ( false == Save<int>(stream, nSize))
				{
					return false;
				}
				if ( false == Save<const char>(stream, strValue.c_str(), nSize))
				{
					return false;
				}
				return true;
			}

			template<>
			inline bool Load( MessageBlock& rMessageBlock, std::string& strValue, bool bRequireChangeByteOrder)
			{
				int nSize = 0;
				if ( false == Load<int>(rMessageBlock, nSize, bRequireChangeByteOrder))
				{
					return false;
				}
				int nMaxLength = rMessageBlock.GetLength();
				if ( nMaxLength < nSize)
				{
					return false;
				}

				if ( 0 < nSize)
				{
					strValue.resize(nSize);
					if ( false == Load<char>(rMessageBlock, &strValue[0], nSize, bRequireChangeByteOrder))
					{
						return false;
					}				
					return true;
				}
				else if (0 == nSize)
				{
					return true;
				}
				return false;
			}
		}
	}
}