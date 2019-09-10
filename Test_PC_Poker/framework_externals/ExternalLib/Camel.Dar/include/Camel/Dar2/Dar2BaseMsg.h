#pragma once

#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <Camel/Base/System/MPHeap.h>
#include <Camel/Base/System/Stream.h>
namespace Camel
{
	namespace Dar2
	{
		class IDarMsg
		{
		public:
			enum ProtocolRule{
				STREAM = 0,
				JSON
			};

			enum JSONOutFormat{
				FAST = 0,
				STYLE
			};

			IDarMsg()		{	}
			virtual ~IDarMsg()	{	}

			virtual unsigned int GetMessageId() = 0;

			virtual int GetLength( Camel::Dar2::IDarMsg::ProtocolRule eRule = Camel::Dar2::IDarMsg::STREAM) = 0;
			virtual  const char* GetMessageName() = 0;

			virtual bool Save( Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM) = 0;
			virtual bool Load( Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false) = 0;

			// json format
			// packet -> string
			virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const {	return false;	};
			// string -> packet
			virtual bool LoadJSON(const std::string& strJsonFormat) {	return false;	};

			friend std::ostringstream& operator << (std::ostringstream& _Ostr, const IDarMsg* rhs)
			{
				if (NULL == rhs)
				{
					_Ostr << "(null)";
				}
				else
				{
					std::string strResult;
					rhs->SaveJSON(strResult);
					_Ostr << strResult;
				}
				return _Ostr;
			}
			friend std::ostringstream& operator << (std::ostringstream& _Ostr, const IDarMsg& rhs)
			{
				std::string strResult;
				rhs.SaveJSON(strResult);
				_Ostr << strResult;
				return _Ostr;
			}


		private:
			IDarMsg(const IDarMsg& rhs)				{};
			IDarMsg& operator=(const IDarMsg& rhs)	{	return *this;	};
		};

		// Camel�� Stream�� Read�Լ��� ���� ȣ���ϸ� ������ �������� �̽��� �ִ�. 
		// ������ �����ϱ� ���ؼ�, MassgeBlock�� ����ϵ��� ������.
		class MessageBlock
		{
		public:
			MessageBlock()
			{
				m_pBuffer = NULL;
				m_nMaxBufferSize = 0;
				m_nReadCursor = 0;
			}

			~MessageBlock()
			{
				if ( NULL != m_pBuffer)
				{
					Camel::Base::System::MPHeap::Free(NULL, m_pBuffer);
					m_pBuffer = NULL;
				}
				m_nMaxBufferSize = 0;
				m_nReadCursor = 0;
			}

		public:
			char* GetBuffer()
			{
				return m_pBuffer;
			}
			int GetLength()
			{
				return m_nMaxBufferSize - m_nReadCursor;
			}

			bool SetData( Camel::Base::System::Stream::Handle stream)
			{
				int nStreamSize = Camel::Base::System::Stream::GetLength( stream);
				m_pBuffer = static_cast<char*>(Camel::Base::System::MPHeap::Alloc(NULL, nStreamSize));
				if ( NULL == m_pBuffer)
				{
					return false;
				}
				m_nMaxBufferSize = nStreamSize;
				if ( m_nMaxBufferSize != Camel::Base::System::Stream::Peek(stream, m_pBuffer, nStreamSize))
				{
					return false;
				}
				return true;
			}
			int Read(void* pData, const int nDataSize)
			{
				// Write�� ������ �� ���� �������� �ϸ� ������.
				if ( m_nMaxBufferSize < m_nReadCursor + nDataSize)
				{
					return 0;
				}
				::memcpy( pData, m_pBuffer+m_nReadCursor, nDataSize);
				m_nReadCursor = m_nReadCursor + nDataSize;
				return nDataSize;
			}

			int Skip(unsigned int nDataSize)
			{
				// Write�� ������ �� ���� �������� �ϸ� ������.
				if ( (unsigned int) m_nMaxBufferSize < m_nReadCursor + nDataSize)
				{
					return 0;
				}
				m_nReadCursor = m_nReadCursor + nDataSize;
				return nDataSize;
			}
		private:
			char*	m_pBuffer;
			int		m_nMaxBufferSize;
			int		m_nReadCursor;
		};

		namespace Util
		{
			inline std::string GetJSON(const IDarMsg& rMsg, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST)
			{
				std::string strResult;
				// SaveJSON �Լ� ���ϰ��� false�� ���� ����ó���� �ʿ��Ѱ�??
				rMsg.SaveJSON(strResult);
				return strResult;
			}
		}
	}
}