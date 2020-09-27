#pragma once

#include "CETypes.h"
#include "countable.h"
#include "functor.h"
#include "scanners.h"

struct SCustomReportItem;
struct SFileReportItem;
struct SRegKReportItem;
struct SRegVReportItem;

interface IReportItem
{
	IReportItem(
		fn::functor1< void, countable< IReportItem > > pfnFix,

		scanner_id		ScannerID,
		ITEM_TYPE		Type,
		SEVERITY		Severity,
		THREAT_STATUS	Status
	);

	// <F> - File name
	// <D> - Directory name
	// <K> - Key full path
	// <S> - Subkey name
	virtual BOOL GenerateDescription( CString & strDescr ) = 0;

	SCustomReportItem * GetAsCustomItem();
	SFileReportItem * GetAsFileItem();
	SRegKReportItem * GetAsRegKItem();
	SRegVReportItem * GetAsRegVItem();

	fn::functor1< void, countable< IReportItem > > m_pfnFix;

	item_id			m_ItemID;
	scanner_id		m_ScannerID;
	ITEM_TYPE		m_Type;
	SEVERITY		m_Severity;
	THREAT_STATUS	m_Status;

	CString			m_strReserved;
};

struct SCustomReportItem : public IReportItem
{
	SCustomReportItem(
		fn::functor1< void, countable< IReportItem > > pfnFix,

		scanner_id		ScannerID,
		ITEM_TYPE		Type,
		SEVERITY		Severity,
		THREAT_STATUS	Status,

		CString			strCustomDescription
	);

	virtual BOOL GenerateDescription( CString & strDescr );

	CString m_strCustomDescription;
};

struct SFileReportItem : public IReportItem
{
	SFileReportItem(
		fn::functor1< void, countable< IReportItem > > pfnFix,

		scanner_id		ScannerID,
		ITEM_TYPE		Type,
		SEVERITY		Severity,
		THREAT_STATUS	Status,

		CString			strFile,
		CString			strDirectory
	);

	virtual BOOL GenerateDescription( CString & strDescr );

	// File data
	CString m_strFile;
	CString m_strDirectory;
};

struct SRegKReportItem : public IReportItem
{
	SRegKReportItem(
		fn::functor1< void, countable< IReportItem > > pfnFix,

		scanner_id		ScannerID,
		ITEM_TYPE		Type,
		SEVERITY		Severity,
		THREAT_STATUS	Status,

		CString			strKeyPath
	);

	virtual BOOL GenerateDescription( CString & strDescr );

	CString m_strKeyPath;
};

struct SRegVReportItem : public SRegKReportItem
{
	SRegVReportItem(
		fn::functor1< void, countable< IReportItem > > pfnFix,
		scanner_id		ScannerID,
		ITEM_TYPE		Type,
		SEVERITY		Severity,
		THREAT_STATUS	Status,

		CString			strKeyPath,
		CString			strSubkeyName,
		DWORD			dwSubkeyValueType
	);

	virtual BOOL GenerateDescription( CString & strDescr );

	CString m_strSubkeyName;
	DWORD	m_dwSubkeyValueType;
};

class CReportImpl
{

public:

	static CReportImpl & Instance()
	{
		static CReportImpl reportObj;
		return reportObj;
	}

	void Reset();

	void FixAllItems( fn::functor1< void, item_id > pfnCallPreCleanupHandler );
	FIX_RESULT FixItem( item_id id );

	item_id AddItem( countable< IReportItem > item );
	countable< IReportItem > GetItem( item_id id );

	BOOL IsValidItem( item_id id )
		{ return ( id < 0 || id >= static_cast<item_id>( m_vItems.size() ) ? FALSE : TRUE  ); }
	size_t GetItemCount();

private:

	CReportImpl() : m_AllocateValue( 1000 )
		{ Reset(); }

	inline item_id GenerateItemID()
		{ return m_item_id_next++; }

	item_id m_item_id_next;
	item_id m_AllocateValue;
	std::vector< countable< IReportItem > > m_vItems;
};

class CReport
{

public:

	CReport();
	~CReport();

	void Reset();

	void FixAllItems( fn::functor1< void, item_id > pfnCallPreCleanupHandler );
	FIX_RESULT FixItem( item_id id );

	item_id AddItem( countable< IReportItem > item );
	countable< IReportItem > GetItem( item_id id );
	BOOL IsValidItem( item_id id );
	size_t GetItemCount();

private:

	DEFINE_CRITICAL_UNIT(Report);

};

#define CREATE_REG_ITEM( fpnFix, ScannerId, severity, szFullKeyName, szValueName, dwType )\
lstrlen( szValueName ) ?\
	new SRegVReportItem(\
		fn::bind1st1( fn::mem_fun( &fpnFix ), this ),\
		ScannerId,\
		IT_REG_VALUE,\
		severity,\
		STATUS_FOUND,\
		szFullKeyName,\
		szValueName,\
		dwType\
	) :\
new SRegKReportItem(\
	fn::bind1st1( fn::mem_fun( &fpnFix ), this ),\
	ScannerId,\
	IT_REG_KEY,\
	severity,\
	STATUS_FOUND,\
	szFullKeyName\
)
