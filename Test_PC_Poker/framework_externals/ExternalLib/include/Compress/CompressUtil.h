#if !defined(AFX_COMPRESSUTIL_H__40435C5E_12A2_41AC_904F_C37FDB4E2B84__INCLUDED_)
#define AFX_COMPRESSUTIL_H__40435C5E_12A2_41AC_904F_C37FDB4E2B84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SockDefine.h"
#include <vector>
#include <assert.h>
#include <Afxmt.h>
#define MAX_POOLSIZE 100*1024
#define BITS 12            
#define HASHING_SHIFT BITS-8      
#define MAX_VALUE (1 << BITS) - 1 
#define MAX_CODE MAX_VALUE - 1  
#define TABLE_SIZE 5021
using namespace std;

// 2007:4:13 압축작업
// 암호화/복호화용 테이블
enum COMPRESSTYPE
{
	COMPRESSTYPE_FAIL			= -1,
	COMPRESSTYPE_NONE			= 0,

	COMPRESSTYPE_LZW_RESERVED	= 1,
	COMPRESSTYPE_LZO_RESERVED	= 2,
	COMPRESSTYPE_QLZO_RESERVED	= 3,
	COMPRESSTYPE_ZIP_RESERVED	= 4,

	COMPRESSTYPE_RESERVED		= 7,


	COMPRESSTYPE_LZW			= 10,
	COMPRESSTYPE_LZO			= 20,
	COMPRESSTYPE_QLZO			= 30,
	COMPRESSTYPE_ZIP			= 40,
};

typedef struct
{
	int Size;
}COMPRESS_HEADER;


template < class T >
class CComPressPool
{
public:
	T* m_pObjectData;
	T** m_ppFreeObject;
	CCriticalSection m_cs;
	bool m_bFreeOnDestroy;
	int  m_NumObject;
	int  m_TopObject;
	void FreeAll()
	{
		m_cs.Lock();
		int index = ( m_NumObject - 1 );
		for( m_TopObject = 0 ; m_TopObject < m_NumObject ; m_TopObject++)
		{
			m_ppFreeObject[ m_TopObject ] = &m_pObjectData[ index-- ];
		}
		m_cs.Unlock();
	}
	T* NewInstance()
	{
	
		m_cs.Lock();
		if( m_TopObject == 0 )
		{
			// 사이즈가 부족할때 2배 늘린다. 
			int ObjectNum		=	m_NumObject*2;
			T*  pObjectData		=	new T[ ObjectNum ];
			T** ppFreeObject	=	new T*[ ObjectNum ];

			for( int i = 0 ; i < m_NumObject ; i++ )
			{
				memcpy( &pObjectData[m_NumObject + i]	, &m_pObjectData[i]	 , sizeof( T ) );
				memcpy( &ppFreeObject[m_NumObject + i]	, &m_ppFreeObject[i] , sizeof( T* ) );
			}

			delete[] m_pObjectData;
			delete[] m_ppFreeObject;

			m_pObjectData = pObjectData;
			m_ppFreeObject = ppFreeObject;

			FreeAll();

			m_NumObject = ObjectNum;
			
		}
		int ObjectNo = --m_TopObject; 
		m_cs.Unlock();
		return m_ppFreeObject[ ObjectNo ];
	}

	T* FreeInstance( void * pinstance )
	{
		m_cs.Lock();
		assert( ( pinstance >= &( m_pObjectData[ 0 ] ) &&
			pinstance <= &( m_pObjectData[ m_NumObject - 1 ] ) ) );
		assert( m_TopObject < m_NumObject );

		m_ppFreeObject[ m_TopObject ] = (  T* )pinstance;
		int ObjectNo = m_TopObject;
		m_TopObject++;
		m_cs.Unlock();
		return m_ppFreeObject[ ObjectNo ];
	}

	CComPressPool( int numObject )
	{
		assert( numObject > 0 );
		m_pObjectData = new T[ numObject ];
		m_ppFreeObject = new T*[ numObject ];

		m_NumObject = numObject;
		m_bFreeOnDestroy = true;
		FreeAll();
	}

	~CComPressPool()
	{
		delete[] m_pObjectData;
		delete[] m_ppFreeObject;
	}
};


class NMSOCK_CLASS CCompress
{
public:
	CCompress(){};
	virtual ~CCompress(){};
	virtual int	Compress( unsigned char*input , unsigned int inputsize ,  unsigned char* &output ) = 0;
	virtual int	Decompress( unsigned char*input , unsigned int inputsize ,  unsigned char* &output ) = 0;
};


class NMSOCK_CLASS CLZW : public CCompress
{
	int				m_Code_Value[ TABLE_SIZE ]; 
	unsigned int	m_Prefix_Code[ TABLE_SIZE ]; 
	unsigned char	m_Append_Character[ TABLE_SIZE ];
	unsigned char	m_Decode_Stack[4000]; 

	int m_Bit_Count;
	unsigned long m_Bit_Buffer;
	int m_BufferIndex;
	int m_BufferSize;

public:
	CLZW(){}
	virtual ~CLZW(){};
	void			output_code(unsigned char* output,unsigned int code);
	unsigned int	input_code(unsigned char* &input );
	int				find_match(unsigned int hash_prefix,unsigned int hash_character);
	unsigned char*	decode_string(unsigned char *buffer,unsigned int code);

	int				Compress( unsigned char*input , unsigned int inputsize ,  unsigned char* &output );
	int				Decompress( unsigned char*input , unsigned int inputsize ,  unsigned char* &output );

};


enum HeaderFields 
{
	COMPSIZE	= 0, 
	UNCOMPSIZE	= 1, 
	COMPRESSIBLE= 2, 
	RESERVED1	= 3, 
	RESERVED2	= 4,
	RESERVED3	= 5,
	MAX_SIZE	= 6,
};

class NMSOCK_CLASS QuickLZ  : public CCompress
{
public:
	QuickLZ(){};
	virtual ~QuickLZ(){};

	char m_Destination[MAX_POOLSIZE + 36000];

	unsigned int Read( void* src , unsigned int bytes );
	void Write( unsigned int f , void* dest , unsigned int bytes );
	bool MemCompare( void* m1 , void* m2 , unsigned int size );
	bool MemCompare( void* m1 , char* str);

	void MemSet( void* dest , unsigned char val , unsigned int size );
	void MemCopy( void* dest , void* src , unsigned int size );
	void MemCopy( void* dest , char* str );

	void MemCopyUP( char* dest ,  const char* src , unsigned int n );
	unsigned int GetHeaderField( char* src , unsigned int start , HeaderFields field );
	unsigned int GetCompressedSize( char* src );
	unsigned int GetDecompressedSize( char* src , unsigned int start );


	unsigned int UnsafeCompress( char* src , unsigned int start , char* dest , unsigned int size );
	unsigned int UnsafeDecompress( char* src , unsigned int start , char* dest );

	int Compress( unsigned char*input , unsigned int inputsize ,  unsigned char* &output );
	int Decompress( unsigned char*input , unsigned int inputsize ,  unsigned char* &output );
};



class NMSOCK_CLASS CLZO : public CCompress
{
public:
	CLZO(){};
	virtual ~CLZO(){};
	
	char m_Code_Value[MAX_POOLSIZE];

	int Compress( unsigned char*input , unsigned int inputsize ,  unsigned char* &output );
	int Decompress( unsigned char*input , unsigned int inputsize ,  unsigned char* &output );


};  

class NMSOCK_CLASS CZIP: public CCompress
{
public:
	CZIP(){};
	virtual ~CZIP(){};

	int Compress( unsigned char*input , unsigned int inputsize ,  unsigned char* &output );
	int Decompress( unsigned char*input , unsigned int inputsize ,  unsigned char* &output );
};

#define MAKE_SECOND(x)	(float)x/1000000.0f


class NMSOCK_CLASS CHRTimer
{
public:
	CHRTimer()
	{
		m_isStart=FALSE;
	}
	~CHRTimer() {}
		
	BOOL	INIT()
	{
			return QueryPerformanceFrequency(&m_liFrequency);
	}
	void	START()
	{
		if( !m_isStart )
		{
			m_isStart = TRUE;
			QueryPerformanceCounter(&m_liStartCount);
		}
	}
	void	END()
	{
		if( m_isStart )
		{
			QueryPerformanceCounter(&m_liEndCount);
			m_isStart = FALSE;
		}
	}

	int		GetMicroSeconds()
	{
		if( !m_isStart ) return (int)((m_liEndCount.QuadPart - m_liStartCount.QuadPart) * 1000000 / m_liFrequency.QuadPart);
		else return 0L;
	}

private:
	
	BOOL			m_isStart;

	LARGE_INTEGER	m_liFrequency;
	LARGE_INTEGER	m_liStartCount;
	LARGE_INTEGER	m_liEndCount;
};

void UtilCompress( int type , unsigned char* pDst, DWORD& compressed_size, unsigned char* original_ptr,unsigned int original_size );
void UtilDecompress( int type , unsigned char* pDst, DWORD& compressed_size, unsigned char* original_ptr,unsigned int original_size );

#endif // !defined(AFX_COMPRESSUTIL_H__40435C5E_12A2_41AC_904F_C37FDB4E2B84__INCLUDED_)
