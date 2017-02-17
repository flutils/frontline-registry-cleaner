// MCleanEngine.h

#pragma once
#using <System.dll>
using namespace System;

#include "CleanEngine.h"
#include "NativeNotifier.h"
#include "CleanScanner.h"
#include "Defragger.h"
using namespace System;
using namespace System::ComponentModel;

namespace MCleanEngine {

	public delegate void CENotificationItemFound(String^, int itemID, int scannerID);
	public delegate void CEScannerFinished(int scannerID);
	public delegate void CEScannerStarted(int scannerID);

	public ref class ManagedCleanEngine : public IDisposable
	{
	public:
		bool Start();
		void Pause( BOOL bSuspend );
		void Stop();
		void AddItem( countable<IReportItem> item );
		bool FixItem( item_id id );
		void FixAllItems();
		void CallPreCleanupHandler( item_id id );
		void AddScanner( countable< CCleanScanner >& rspScanner );
		bool RemoveScanner( const scanner_id scannerId );
		void EnableScanner( const /*scanner_id*/int Id, const bool bEnable );
		bool IsScannerEnabled( const scanner_id Id );
		void EnableAllScanners( const bool bEnable );
		int GetScannersNumber();
		ManagedCleanEngine();
		~ManagedCleanEngine();
		!ManagedCleanEngine();
		bool IsBusy();
		
		//defragger
		bool LoadHivesInfo(System::String^ path);
		bool AnalizeHives();
		void CompactHives();
	private:
		CCleanEngine* mp_core;
		CNativeNotifier* mp_notifier;
		CDefragger* mp_defragger;

	public:
		static CENotificationItemFound ^ CENotifierItemFound;
		static CEScannerFinished ^ CEScanFinished;
		static CEScannerStarted ^ CEScanStart;
	};
}

