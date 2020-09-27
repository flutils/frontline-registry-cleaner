#include "StdAfx.h"
#include "Report.h"

/* IReportItem */
IReportItem::IReportItem(
	fn::functor1< void, countable< IReportItem > > pfnFix,
	scanner_id		ScannerID,
	ITEM_TYPE		Type,
	SEVERITY		Severity,
	THREAT_STATUS	Status ) :
		m_pfnFix( pfnFix ),
		m_ItemID( -1 ),
		m_ScannerID( ScannerID ),
		m_Type( Type ),
		m_Severity( Severity ),
		m_Status( Status )
{
}

SCustomReportItem * IReportItem::GetAsCustomItem() { return dynamic_cast<SCustomReportItem*>( this ); }
SFileReportItem * IReportItem::GetAsFileItem() { return dynamic_cast<SFileReportItem*>( this ); }
SRegKReportItem * IReportItem::GetAsRegKItem() { return dynamic_cast<SRegKReportItem*>( this ); }
SRegVReportItem * IReportItem::GetAsRegVItem() { return dynamic_cast<SRegVReportItem*>( this ); }

/* SCustomReportItem */
SCustomReportItem::SCustomReportItem(
	fn::functor1< void, countable< IReportItem > > pfnFix,
	scanner_id		ScannerID,
	ITEM_TYPE		Type,
	SEVERITY		Severity,
	THREAT_STATUS	Status,
	CString			strCustomDescription ) :
		IReportItem( pfnFix, ScannerID, Type, Severity, Status ),
		m_strCustomDescription( strCustomDescription )
{
}

BOOL SCustomReportItem::GenerateDescription( CString & strDescr )
{
	strDescr = m_strCustomDescription;
	return TRUE;
}

/* SFileReportItem */
SFileReportItem::SFileReportItem(
	fn::functor1< void, countable< IReportItem > > pfnFix,
	scanner_id		ScannerID,
	ITEM_TYPE		Type,
	SEVERITY		Severity,
	THREAT_STATUS	Status,
	CString			strFile,
	CString			strDirectory ):
		IReportItem( pfnFix, ScannerID, Type, Severity, Status ),
		m_strFile( strFile ),
		m_strDirectory( strDirectory )
{
}

BOOL SFileReportItem::GenerateDescription( CString & strDescr )
{
	int iResF = strDescr.Replace( _T("<F>"), m_strFile );
	return ( strDescr.Replace( _T("<D>"), m_strDirectory ) || iResF );
}

/* SRegKReportItem */
SRegKReportItem::SRegKReportItem(
	fn::functor1< void, countable< IReportItem > > pfnFix,
	scanner_id		ScannerID,
	ITEM_TYPE		Type,
	SEVERITY		Severity,
	THREAT_STATUS	Status,
	CString			strKeyPath ):
		IReportItem( pfnFix, ScannerID, Type, Severity, Status ),
		m_strKeyPath( strKeyPath )
{
}

BOOL SRegKReportItem::GenerateDescription( CString & strDescr )
{
	return ( strDescr.Replace( _T("<K>"), m_strKeyPath ) ? TRUE : FALSE );
}

/* SRegVReportItem */
SRegVReportItem::SRegVReportItem(
	fn::functor1< void, countable< IReportItem > > pfnFix,
	scanner_id		ScannerID,
	ITEM_TYPE		Type,
	SEVERITY		Severity,
	THREAT_STATUS	Status,
	CString			strKeyPath,
	CString			strSubkeyName,
	DWORD			dwSubkeyValueType ):
		SRegKReportItem( pfnFix, ScannerID, Type, Severity, Status, strKeyPath ),
		m_strSubkeyName( strSubkeyName ),
		m_dwSubkeyValueType( dwSubkeyValueType )
{
}

BOOL SRegVReportItem::GenerateDescription( CString & strDescr )
{
	int iResS = strDescr.Replace( _T("<S>"), m_strSubkeyName );
	return ( strDescr.Replace( _T("<K>"), m_strKeyPath ) && iResS );
}	

void CReportImpl::Reset()
{
	m_vItems.clear();
	m_vItems.reserve( m_AllocateValue );

	m_item_id_next = 0;
}

FIX_RESULT CReportImpl::FixItem( item_id id )
{
	if( id < 0 || id >= static_cast<item_id>( m_vItems.size() ) )
		return FR_NOT_FOUND;

	m_vItems[id]->m_pfnFix( m_vItems[id] );

	return FR_FIXED;
}

void CReportImpl::FixAllItems( fn::functor1< void, item_id > pfnCallPreCleanupHandler )
{
	if( m_vItems.size() <= 0 )
		return;

	std::vector< countable< IReportItem > >::size_type i;
	std::vector< countable< IReportItem > >::size_type i_max = m_vItems.size();

	for( i = 0; i < i_max; ++i )
	{
		pfnCallPreCleanupHandler( static_cast<int>( i ) );
		m_vItems[i]->m_pfnFix( m_vItems[i] );
	}
}

item_id CReportImpl::AddItem( countable< IReportItem > item )
{
	item_id id = GenerateItemID();

	if( id >= m_AllocateValue )
	{
		m_AllocateValue *= 2;
		m_vItems.reserve( m_AllocateValue );
	}

	m_vItems.push_back( item );
	item->m_ItemID = id;

	return id;
}

countable< IReportItem > CReportImpl::GetItem( item_id id )
{
	if( id < 0 || id >= static_cast<item_id>( m_vItems.size() ) )
		return countable< IReportItem >( NULL );

	return m_vItems[id];
}

size_t CReportImpl::GetItemCount()
{
	return m_vItems.size();
}



CReport::CReport()
{
	INIT_CRITICAL_UNIT(Report);
}

CReport::~CReport()
{
	LockReport();
	UnlockReport();

	UNINIT_CRITICAL_UNIT(Report);
}

void CReport::Reset()
{
	LockReport();
	CReportImpl::Instance().Reset();
	UnlockReport();
}

FIX_RESULT CReport::FixItem( item_id id )
{
	LockReport();
	FIX_RESULT fr = CReportImpl::Instance().FixItem( id );
	UnlockReport();

	return fr;
}

void CReport::FixAllItems( fn::functor1< void, item_id > pfnCallPreCleanupHandler )
{
	LockReport();
	CReportImpl::Instance().FixAllItems( pfnCallPreCleanupHandler );
	UnlockReport();
}

item_id CReport::AddItem( countable< IReportItem > item )
{
	LockReport();
	item_id id = CReportImpl::Instance().AddItem( item );
	UnlockReport();

	return id;
}

countable< IReportItem > CReport::GetItem( item_id id )
{
	LockReport();
	countable< IReportItem > item = CReportImpl::Instance().GetItem( id );
	UnlockReport();

	return item;
}

BOOL CReport::IsValidItem( item_id id )
{
	LockReport();
	BOOL bRes = CReportImpl::Instance().IsValidItem( id );
	UnlockReport();

	return bRes;
}

size_t CReport::GetItemCount()
{
	LockReport();
	size_t ulRes = CReportImpl::Instance().GetItemCount();
	UnlockReport();

	return ulRes;
}
