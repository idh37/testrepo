
#pragma once

template< typename T > 
class CSingleton 
{ 
private:
	static T* m_spSingleton;

public:
	CSingleton();
	virtual ~CSingleton();

	inline static T* GetSingleton();

};


template< typename T >
T* CSingleton<T>::m_spSingleton = NULL;


template< typename T >
CSingleton< T >::CSingleton() 
{ 
	ASSERT( m_spSingleton == NULL );

	T* pT = reinterpret_cast< T* >( 1 );

	//Derived Class - Singleton class
	INT offset = reinterpret_cast<INT>(pT) - reinterpret_cast<INT>(reinterpret_cast<CSingleton< T >* >( pT ) );

	m_spSingleton = reinterpret_cast< T* >( ( reinterpret_cast< INT >( this ) + offset ) );

}//Singleton< T > ::Singleton() 


template< typename T >
CSingleton< T >::~CSingleton() 
{

}