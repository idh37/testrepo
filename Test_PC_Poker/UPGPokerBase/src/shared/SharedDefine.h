/********************************************************************
	created:	2010/07/09
	created:	9:7:2010   14:33
	file Path:  Client\shared
	file base:	SharedDefine
	file ext:	h
	author:		CHOI YEONSIK
	
	purpose:	
*********************************************************************/

#pragma  once

#define TPASSERT(x)			{ if ( !( x ) ) { DebugBreak(); } }
#ifndef SAFE_NEW
 #define SAFE_NEW(p, t)		{ p = new t; TPASSERT(p != NULL); }
#endif
#define SAFE_NEW_ARRAY(p, t, s) { p = new t[s]; TPASSERT(p != NULL); }
#ifndef SAFE_DELETE
 #define SAFE_DELETE( p )	{ if ( p ) { delete p; p = NULL; } }
#endif
#define SAFE_DELETEA( p )		{ if ( p ) { delete []  ( p ); ( p ) = NULL; } }
#define CLOSE_HANDLE( h )	{ if ( h ) { CloseHandle( h ); h = NULL; } }
#define ACE_UNUSED_ARG( x )  ( x )

#ifdef _UNICODE
#define PJ_TEXT( p )			L##p
#else
#define PJ_TEXT( p )			p
#endif

#define CS( str ) #str
#define BEGIN_SELECT( e ) switch( e ) {
#define CASE_SELECT( e ) case( e ): return CS( e );
#define END_SELECT( e ) } return CS( e );

#define FORMAT					boost::format

#define dimof( a ) ( sizeof( a ) / sizeof(* ( a )))

namespace shared
{
	///////////////////////////////////////////////////////////////////////////////
	// BEGIN
	class ISharedDefine
	{
	public:
		ISharedDefine(void) : m_nReference(1)
		{
		}

		virtual ~ISharedDefine(void)
		{
		}

	public:
		void Increase(void)
		{
			TPASSERT(m_nReference > 0);
			++m_nReference;
		}

		bool Decrease(bool bRemoveMemory = true)
		{
			TPASSERT(m_nReference > 0);

			if (--m_nReference == 0)
			{
				if(bRemoveMemory)
				{
					delete this;
					return true;
				}
			}

			return false;
		}

		void Release(void)
		{
			TPASSERT(m_nReference > 0);
			while (--m_nReference != 0);
			delete this;
		}

		int GetReference(void)	const
		{
			return m_nReference;
		}

	protected:
		void SetDebugName(const char* szName)
		{
			m_szDebugName = szName;
		}

		const std::string& GetDebugName(void) const
		{
			return m_szDebugName;
		} 

	private:
		int	m_nReference;
		std::string m_szDebugName;
	};
	// END
	///////////////////////////////////////////////////////////////////////////////

}


