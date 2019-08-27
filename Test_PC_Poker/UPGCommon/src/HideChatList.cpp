// WhisperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HideChatList.h"

void CHideChatList::MAKE_IDList( std::list<std::pair<std::string, std::string> > &listIDNickName )
{
	CString	strTemp, strComp1, strComp2;
	CString strID, strNick;

	strTemp = _T("");
	strComp1 = _T("ID : ");			//- Lobby Chatting
	strComp2 = _T("## ID: ");		//- Game Room Chatting

	int nFind = 0, nFind1 = 0, nFind2 = 0, nCompLen = 0;
	list< CString >::iterator iter = m_list.begin();
	for( ; iter != m_list.end() ; iter++ )
	{
		strTemp = (*iter);

		nFind	= -1;
		nFind1	= strTemp.Find( strComp1, 0 );
		nFind2	= strTemp.Find( strComp2, 0 );

		if ( nFind1 == -1 && nFind2 == -1 )	continue;
		if ( nFind1 != -1 )
		{
			nCompLen = strComp1.GetLength();
			nFind = nFind1;
		}
		if ( nFind2 != -1 )
		{
			nCompLen = strComp2.GetLength();
			nFind = nFind2;
		}

		int nID = 0, nNick = 0;
		nID	= strTemp.Find( '(', nFind+nCompLen )+1;
		nNick	= strTemp.Find( ')', nID )+1;

		strID	= strTemp.Mid( (nFind+nCompLen), (nID-(nFind+nCompLen)-1) );
		strNick	= strTemp.Mid( nID, (nNick-nID-1) );

		listIDNickName.push_back(make_pair(string((LPCTSTR)strID), string((LPCTSTR)strNick)));
	}
}