#include "stdafx.h"
#include "SortHeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSortHeaderCtrl::CSortHeaderCtrl() : m_iSortColumn(-1)
{
}

CSortHeaderCtrl::~CSortHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CSortHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CSortHeaderCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortHeaderCtrl message handlers

void CSortHeaderCtrl::SetSortArrow( const int iSortColumn, const BOOL bSortAscending )
{
	// change the item to owner drawn.
	if (m_iSortColumn != iSortColumn && (m_iSortColumn != -1))
	{
		HD_ITEM hditem;
		hditem.mask = HDI_FORMAT;
		VERIFY( GetItem( m_iSortColumn, &hditem ) );
		hditem.fmt &= ~(HDF_SORTDOWN|HDF_SORTUP);
		VERIFY( SetItem( m_iSortColumn, &hditem ) );
	}
	if (bSortAscending)
	{
		HD_ITEM hditem;
		hditem.mask = HDI_FORMAT;
		VERIFY( GetItem( iSortColumn, &hditem ) );
		hditem.fmt &= ~(HDF_SORTDOWN|HDF_SORTUP);
		hditem.fmt |= HDF_SORTUP;
		VERIFY( SetItem( iSortColumn, &hditem ) );
	}
	else
	{
		HD_ITEM hditem;
		hditem.mask = HDI_FORMAT;
		VERIFY( GetItem( iSortColumn, &hditem ) );
		hditem.fmt &= ~(HDF_SORTDOWN|HDF_SORTUP);
		hditem.fmt |= HDF_SORTDOWN;
		VERIFY( SetItem( iSortColumn, &hditem ) );
	}
	m_iSortColumn = iSortColumn;
}
