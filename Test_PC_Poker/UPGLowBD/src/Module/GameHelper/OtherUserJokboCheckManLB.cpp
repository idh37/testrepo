#include "stdafx.h"
#include "OtherUserJokboCheckManLB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

COtherUserJokboCheckManLB::COtherUserJokboCheckManLB()
{
	Clear();
}

COtherUserJokboCheckManLB::~COtherUserJokboCheckManLB()
{
}

void COtherUserJokboCheckManLB::Init(NMBASE::GRAPHICGDI::xSprite *pArrowSpr)
{
	COtherUserJokboCheckMan::Init(pArrowSpr);
}

void COtherUserJokboCheckManLB::Clear(void)
{
}

void COtherUserJokboCheckManLB::Draw(CDC *pDC , NMBASE::GRAPHICGDI::CPage *pPage)
{
}

void COtherUserJokboCheckManLB::CheckJokbo(void)
{
}