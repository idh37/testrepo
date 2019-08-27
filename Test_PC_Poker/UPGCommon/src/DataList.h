#pragma once

#include <algorithm>

template <typename K, typename D>
class UPG_COMMON_EXPORT CDataList
{
public:
	struct DATAENTITY
	{
		K	Key;
		D	Data;
	};	
protected:
	class CFindKey
	{
	public:
		CFindKey(const K& key)
		{
			m_key = key;  
		}

		bool operator()(const DATAENTITY* DataEntity)
		{
			return (DataEntity->Key == m_key);
		} 

		K m_key;
	};

	list<DATAENTITY*> m_list_data;
//	typename list<DATAENTITY*>::iterator m_it;
public:
	typedef typename list<DATAENTITY*>::iterator DATAPOSITION;
	typedef typename list<DATAENTITY*>::reverse_iterator DATAPOSITION_REVERSE;
	
public:
	CDataList(void){}
	virtual ~CDataList(void){
		Clear();
	}

	typedef bool (*FN_DATA_SORT)(DATAENTITY*, DATAENTITY*);

	virtual void Sort(FN_DATA_SORT pFnSort)
	{		
		m_list_data.sort(pFnSort);
	}


	virtual int GetSize()
	{
		return m_list_data.size();
	}
	virtual void Clear()
	{

		typename list<DATAENTITY*>::iterator it = m_list_data.begin();
		for(;it!=m_list_data.end(); it++)
		{

			delete ((DATAENTITY*)(*it));
		}		
		m_list_data.clear();
	}

	virtual D* GetData(K key)
	{
		typename list<DATAENTITY*>::iterator it = find_if( m_list_data.begin(), m_list_data.end(), CFindKey(key));
		if (it == m_list_data.end())
			return NULL;

		return &((*it)->Data);
	}



	virtual DATAENTITY* AddData(K key, D *data)
	{
		DATAENTITY *pDataEntity= new DATAENTITY;
		pDataEntity->Data = *data;
		pDataEntity->Key = key;

		//ASSERT(GetData(key)==NULL);		//중복키 삽입 검사	

		m_list_data.push_back(pDataEntity);

		return pDataEntity;
	}

	virtual DATAENTITY* InsertData(DATAPOSITION pos, K key, D *data)
	{
// 		typename list<DATAENTITY*>::iterator it = find_if( m_list_data.begin(), m_list_data.end(), CFindKey(key_insertloc));
// 		if (it == m_list_data.end())
// 			return NULL;

		if (pos==m_list_data.end())
			return NULL;

		DATAENTITY *pDataEntity= new DATAENTITY;
		pDataEntity->Data = *data;
		pDataEntity->Key = key;

		ASSERT(GetData(key)==NULL);		//중복키 삽입 검사	

		m_list_data.insert(pos, pDataEntity);
		return pDataEntity;
	}

	virtual BOOL DelData(K key)
	{
		typename list<DATAENTITY*>::iterator it = find_if( m_list_data.begin(), m_list_data.end(), CFindKey(key));
		if (it == m_list_data.end())
			return FALSE;

		delete ((DATAENTITY*)(*it));

		m_list_data.erase(it);
		return TRUE;	
	}


	virtual DATAENTITY* ChangeData(K key, D *data)
	{
		typename list<DATAENTITY*>::iterator it = find_if( m_list_data.begin(), m_list_data.end(), CFindKey(key));
		if (it == m_list_data.end())
			return NULL;

		(*it)->Data = *data;

		return (*it);
	}


	virtual D *GetFirstData(DATAPOSITION &pos)
	{
		pos = m_list_data.begin();	

		if (pos==m_list_data.end())
			return NULL;

		return &(*pos)->Data;
	}

	virtual D *GetDataAt(DATAPOSITION &pos, int index)	//이 함수는 오버헤드가 크므로 자주사용금지
	{			
		pos =	m_list_data.begin();

		if (pos==m_list_data.end())
			return NULL;

		if ((int) m_list_data.size() <= index)
			return NULL;

		for(int i=0; i<index; i++) pos++;

		return &((*pos)->Data);
	}

	virtual D *GetNextData(DATAPOSITION &pos)
	{	
		if (pos==m_list_data.end())
			return NULL;

		pos++;

		if (pos==m_list_data.end())
			return NULL;

		return &(*pos)->Data;
	}

	
	virtual D *GetLastData(DATAPOSITION_REVERSE &pos_rev)	//마지막데이터를 반환
	{
		pos_rev = m_list_data.rbegin();	

		if (pos_rev==m_list_data.rend())
			return NULL;

		return &(*pos_rev)->Data;
	}


	virtual D *GetPrevData(DATAPOSITION_REVERSE &pos_rev)
	{	
		if (pos_rev==m_list_data.rend())
			return NULL;

		pos_rev++;

		if (pos_rev==m_list_data.rend())
			return NULL;

		return &(*pos_rev)->Data;
	}

};

template <typename K, typename D>
class UPG_COMMON_EXPORT CFilterDataList : public CDataList<K, D>
{


public:
	
	typedef bool(*FN_FILTER)(D*);
	typedef int KEY_FILTER;
	enum { KEY_FILTER_NULL = -1 };


protected:	

	//필터 데이터
	map< KEY_FILTER, list<DATAENTITY*> > m_map_condtion_list;
	map< KEY_FILTER, FN_FILTER > m_map_filter_func;

public:	

	CFilterDataList()
	{
	}

	virtual ~CFilterDataList()
	{
		Clear();
		ClearFilter();
	}

	virtual void ClearFilter()
	{
		
		m_map_condtion_list.clear();
		m_map_filter_func.clear();

	}


	virtual void Clear()
	{
		map< KEY_FILTER, list<DATAENTITY*> >::iterator it = m_map_condtion_list.begin();
		for(; it!=m_map_condtion_list.end(); it++)
		{
			it->second.clear();
		}

		CDataList::Clear();
	}

	virtual KEY_FILTER AddFilter(FN_FILTER fnFilter)
	{
		KEY_FILTER key = (KEY_FILTER) m_map_condtion_list.size();

		list<DATAENTITY*> filterlist;

		//데이터가 이미 있다면 fnFilter 을 만족하는 데이터 추출	
		if (CDataList::GetSize()>0)
		{	
			list<DATAENTITY*>::iterator list_it = m_list_data.begin();
			for(; list_it!=m_list_data.end(); list_it++)
			{
				D *pData = &((*list_it)->Data);
				if (fnFilter( pData ))
				{					
					filterlist.push_back( *list_it );
				}
			}
		}

		//필터를 추가		
		m_map_condtion_list.insert( pair<KEY_FILTER, list<DATAENTITY*> >(key , filterlist));
		m_map_filter_func.insert( pair<KEY_FILTER, FN_FILTER >(key , fnFilter));

		return key;
	}
	

	virtual int GetSize()
	{
		return  CDataList::GetSize();
	}
	virtual int GetSize(KEY_FILTER nFilterKey)
	{
		if (KEY_FILTER_NULL==nFilterKey)
			return CDataList::GetSize();

		if (m_map_condtion_list.find(nFilterKey)==m_map_condtion_list.end())
			return 0;

		return m_map_condtion_list[nFilterKey].size();
	}

	virtual D *GetFirstData(DATAPOSITION &pos)
	{
		return CDataList::GetFirstData(pos);
	}
	virtual D *GetFirstData(KEY_FILTER nFilterKey, DATAPOSITION &pos)
	{
		if (KEY_FILTER_NULL==nFilterKey)
			return CDataList::GetFirstData(pos);

		if (m_map_condtion_list.find(nFilterKey)==m_map_condtion_list.end())
			return NULL;

		pos =	m_map_condtion_list[nFilterKey].begin();
		if (pos == m_map_condtion_list[nFilterKey].end())
		{
			return NULL;
		}

		return &(*pos)->Data;
	}

	virtual D *GetDataAt(DATAPOSITION &pos, int index)
	{
		return CDataList::GetDataAt(pos, index);
	}
	virtual D *GetNextData(DATAPOSITION &pos)
	{
		return CDataList::GetNextData(pos);
	}
	
	virtual D *GetDataAt(KEY_FILTER nFilterKey, DATAPOSITION &pos, int index) //이 함수는 오버헤드가 크므로 자주사용금지
	{
		if (KEY_FILTER_NULL==nFilterKey)
			return CDataList::GetDataAt(pos, index);

		if (m_map_condtion_list.find(nFilterKey)==m_map_condtion_list.end())
			return NULL;	
	

		pos =	m_map_condtion_list[nFilterKey].begin();
		if (pos == m_map_condtion_list[nFilterKey].end())
		{
			return NULL;
		}

		if (index >= (int)m_map_condtion_list[nFilterKey].size())
			return NULL;


		for(int i=0; i<index; i++) pos ++;

		return &((*pos)->Data);

	}
	virtual D *GetNextData(KEY_FILTER nFilterKey, DATAPOSITION &pos)
	{		
		if (KEY_FILTER_NULL==nFilterKey)
			return CDataList::GetNextData(pos);

		if (m_map_condtion_list.find(nFilterKey)==m_map_condtion_list.end())
			return NULL;	

		if (pos==m_map_condtion_list[nFilterKey].end())
			return NULL;

		pos++;
		if (pos==m_map_condtion_list[nFilterKey].end())
			return NULL;

		return &(*pos)->Data;

	}

	virtual D *GetLastData(DATAPOSITION_REVERSE &pos_rev)
	{
		return CDataList::GetLastData(pos_rev);
	}
	virtual D *GetLastData(KEY_FILTER nFilterKey, DATAPOSITION_REVERSE &pos_rev)
	{
		if (KEY_FILTER_NULL==nFilterKey)
			return CDataList::GetLastData(pos_rev);

		if (m_map_condtion_list.find(nFilterKey)==m_map_condtion_list.end())
			return NULL;

		pos_rev =	m_map_condtion_list[nFilterKey].rbegin();
		if (pos_rev == m_map_condtion_list[nFilterKey].rend())
		{
			return NULL;
		}

		return &(*pos_rev)->Data;
	}

	
	virtual D *GetPrevData(DATAPOSITION_REVERSE &pos_rev)
	{
		return CDataList::GetPrevData(pos_rev);
	}

	
	virtual D *GetPrevData(KEY_FILTER nFilterKey, DATAPOSITION_REVERSE &pos_rev)
	{		
		if (KEY_FILTER_NULL==nFilterKey)
			return CDataList::GetPrevData(pos_rev);

		if (m_map_condtion_list.find(nFilterKey)==m_map_condtion_list.end())
			return NULL;	

		if (pos_rev==m_map_condtion_list[nFilterKey].rend())
			return NULL;

		pos_rev++;
		if (pos_rev==m_map_condtion_list[nFilterKey].rend())
			return NULL;

		return &(*pos_rev)->Data;

	}


	virtual DATAENTITY* AddData(K key, D *data)
	{
		DATAENTITY *pDE = CDataList::AddData(key, data);
		if (pDE)
		{
			//조건을 만족할 경우에만 추가
			map< KEY_FILTER, list<DATAENTITY*> >::iterator it = m_map_condtion_list.begin();
			for(; it!=m_map_condtion_list.end(); it++)
			{
				if (m_map_filter_func[it->first](data))
				{
					it->second.push_back( pDE );
				}
			}
		}
		return pDE;
	}

	virtual DATAENTITY* InsertData(DATAPOSITION pos, K key, D *data)
	{
		DATAENTITY *pDE = CDataList::InsertData(pos, key, data);

		if (pDE)
		{

			//조건을 만족할 경우에만 다시 리스트 생성
			map< KEY_FILTER, list<DATAENTITY*> >::iterator it = m_map_condtion_list.begin();
			for(; it!=m_map_condtion_list.end(); it++)
			{
				if (m_map_filter_func[it->first](data))	//추가될 데이터가 해당 필터의 조건이 맞으면 해당 필터리스트 삭제 후 다시 재생성
				{
					it->second.clear();	//해당 필터 삭제

					//필터의 조건에 일치하는 리스트 재생성
					list<DATAENTITY*>::iterator list_it = m_list_data.begin();
					for(; list_it!=m_list_data.end(); list_it++)
					{
						D *pData = &((*list_it)->Data);
						if (m_map_filter_func[it->first]( pData ))	//전체 리스트
						{					
							it->second.push_back( *list_it );
						}
					}
				}
			}
		}
		return pDE;
	}



	virtual BOOL DelData(K key)
	{
		//필터 먼저 삭제			
		map< KEY_FILTER, list<DATAENTITY*> >::iterator it_map = m_map_condtion_list.begin();
		for(; it_map!=m_map_condtion_list.end(); it_map++)
		{
			typename list<DATAENTITY*>::iterator it = find_if( it_map->second.begin(), it_map->second.end(), CFindKey(key));
			if (it != it_map->second.end())
			{
				it_map->second.erase(it);
			}
		}
		return CDataList::DelData(key);
	}	


	virtual DATAENTITY *ChangeData(K key, D *data)
	{
		DATAENTITY *pDE = CDataList::ChangeData(key,data);
		if (pDE)
		{			
			map< KEY_FILTER, list<DATAENTITY*> >::iterator it_map = m_map_condtion_list.begin();
			for(; it_map!=m_map_condtion_list.end(); it_map++)
			{			
				//새로운 데이터가 조건을 만족할경우 추가 또는 교체
				typename list<DATAENTITY*>::iterator it_list = find_if( it_map->second.begin(), it_map->second.end(), CFindKey(key));

				if (m_map_filter_func[it_map->first](data))
				{					
					//새로운 데이터가 조건을 만족할경우 추가 또는 교체					
					if (it_list == it_map->second.end())
					{
						//없으면 추가
						it_map->second.push_back( pDE );
					}
					else
					{
						//있으면 교체	
						(*it_list)->Data = *data;						
					}
				}
				else
				{
					//새로운 데이터가 조건을 만족하지 않을경우 제거					
					if (it_list != it_map->second.end())
					{
						//있으면 삭제
						it_map->second.erase(it_list);
					}

				}				
			
			}

		}
		return pDE;
	}


	virtual void Sort(FN_DATA_SORT pFnSort)
	{
		CDataList::Sort(pFnSort);

		map< KEY_FILTER, list<DATAENTITY*> >::iterator it_map = m_map_condtion_list.begin();
		for(; it_map!=m_map_condtion_list.end(); it_map++)
		{
			it_map->second.sort(pFnSort);
		}
	}


	virtual void Sort(KEY_FILTER nFilterKey, FN_DATA_SORT pFnSort)
	{		

		if (KEY_FILTER_NULL==nFilterKey)
		{
			CDataList::Sort(pFnSort);
			return;
		}

		if (m_map_condtion_list.find(nFilterKey)==m_map_condtion_list.end())
			return;

		m_map_condtion_list[nFilterKey].sort(pFnSort);
	}


};
