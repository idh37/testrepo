#pragma once

#include "DialogDefine.h"
#include <list>

using namespace std;


class NMDIALOG_CLASS CHideChatList
{
public: 
	CHideChatList(){ Clear();};
	virtual ~CHideChatList(){ Clear();};


	void Clear()
	{
		m_list.clear();
		m_Data= "";
	};
	void AddData( CString* str )
	{
		PushBack( str );
		if( GetLineNum() > 90 )
		{
			PopFront();
		}
	}
	void PushBack( CString* str )
	{
		m_list.push_back( *str );
	}
	void PopFront()	
	{
		m_list.pop_front();
	}
	int GetLineNum()	
	{
		return m_list.size();
	}
	CString& GetData()	
	{
		m_Data = "";
		list< CString >::iterator iter = m_list.begin();
		for( ; iter != m_list.end() ; iter++ )	
		{
			m_Data += (*iter);
		}
		return m_Data;
	}

	void	MAKE_IDList( void *pDlg );
private:
	CString m_Data;
	list< CString > m_list;
};