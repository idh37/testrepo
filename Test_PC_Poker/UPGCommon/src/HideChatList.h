#pragma once

class UPG_COMMON_EXPORT CHideChatList
{
public: 
	CHideChatList(){ Clear();};
	virtual ~CHideChatList(){ Clear();};

	
	void Clear(){
		m_list.clear();
		m_Data= "";
	};
	void AddData( CString* str ){
		PushBack( str );
		if( GetLineNum() > 90 ){
			PopFront();
		}
	}
	void PushBack( CString* str ){
		m_list.push_back( *str );
	}
	void PopFront()	{
		m_list.pop_front();
	}
	int GetLineNum()	{
		return m_list.size();
	}
	CString& GetData()	{
		m_Data = "";
		list< CString >::iterator iter = m_list.begin();
		for( ; iter != m_list.end() ; iter++ )	{
			m_Data += (*iter);
		}
		return m_Data;
	}

	void	MAKE_IDList( std::list<std::pair<std::string, std::string> > &listIDNickName );
private:
	CString m_Data;
	list< CString > m_list;
};