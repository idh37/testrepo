#include "stdafx.h"
#include "OtherUserJokboCheckMan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

COtherUserJokboCheckMan::COtherUserJokboCheckMan()
{
	m_pArrowSpr = NULL;
}

COtherUserJokboCheckMan::~COtherUserJokboCheckMan()
{
}

void COtherUserJokboCheckMan::Init(NMBASE::GRAPHICGDI::xSprite *pArrowSpr)
{
	m_pArrowSpr = pArrowSpr;
}