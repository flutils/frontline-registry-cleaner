// This is the main DLL file.

#include "stdafx.h"
#include <atlstr.h>
#include <atlfile.h>
#include "FLCleanEngine.h"
#include "CETypes.h"
#using <mscorlib.dll>
using namespace System::Runtime::InteropServices;
static void __stdcall MHandlerItemFound(const char*, countable<IReportItem> item);
static void __stdcall MHandlerScanFinished(const scanner_id scannerID);
static void __stdcall MHandlerScanStarted(const scanner_id scannerID);

FLCleanEngine::ManagedCleanEngine::ManagedCleanEngine()
{
	mp_notifier = new CNativeNotifier();
	mp_core = new CCleanEngine(mp_notifier);
	mp_defragger = new CDefragger();
	mp_notifier->SetCallback(MHandlerItemFound);
	mp_notifier->SetCallbackScannerFinished(MHandlerScanFinished);
	mp_notifier->SetCallbackScannerStarted(MHandlerScanStarted);
}

bool FLCleanEngine::ManagedCleanEngine::Start()
{	
	if(mp_core->StartEngine() == ERR_ALREADY_RUNNING)
		return false;
	else
		return true;
}

void FLCleanEngine::ManagedCleanEngine::Pause( BOOL bSuspend )
{
	mp_core->Pause(bSuspend);
}

void FLCleanEngine::ManagedCleanEngine::Stop()
{
	mp_core->Stop();
}

void FLCleanEngine::ManagedCleanEngine::AddItem( countable<IReportItem> item )
{
	mp_core->AddItem(item);
}

bool FLCleanEngine::ManagedCleanEngine::FixItem( item_id id )
{
	return mp_core->FixItem(id);
}

void FLCleanEngine::ManagedCleanEngine::FixAllItems()
{
	mp_core->FixAllItems();
}

void FLCleanEngine::ManagedCleanEngine::CallPreCleanupHandler( item_id id )
{
	mp_core->CallPreCleanupHandler(id);
}

void FLCleanEngine::ManagedCleanEngine::AddScanner( countable< CCleanScanner >& rspScanner )
{
	try
	{
	mp_core->AddScanner(rspScanner);
	}
	catch(...)
	{
	}
}

bool FLCleanEngine::ManagedCleanEngine::RemoveScanner( const scanner_id scannerId )
{
	return mp_core->RemoveScanner(scannerId);
}

void FLCleanEngine::ManagedCleanEngine::EnableScanner( const int Id, const bool bEnable )
{
	mp_core->EnableScanner(scanner_id(Id), bEnable);
}

bool FLCleanEngine::ManagedCleanEngine::IsScannerEnabled( const scanner_id Id )
{
	return mp_core->IsScannerEnabled(Id);
}

void FLCleanEngine::ManagedCleanEngine::EnableAllScanners( const bool bEnable )
{
	mp_core->EnableAllScanners(bEnable);
}

FLCleanEngine::ManagedCleanEngine::~ManagedCleanEngine()
{		
}

int FLCleanEngine::ManagedCleanEngine::GetScannersNumber()
{
	return static_cast<int>(mp_core->GetScannersNum());
}

bool FLCleanEngine::ManagedCleanEngine::IsBusy()
{
	return mp_core->IsBusy();
}

bool FLCleanEngine::ManagedCleanEngine::AnalizeHives()
{
	return mp_defragger->AnalyzeHives(); 
}

void FLCleanEngine::ManagedCleanEngine::CompactHives()
{
	mp_defragger->CompactHives();
}

bool FLCleanEngine::ManagedCleanEngine::LoadHivesInfo(System::String^ path)
{
	const char* str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(path);
	bool result = mp_defragger->LoadHivesInfo(str2);
	Marshal::FreeHGlobal((System::IntPtr)(void*)str2);
	return result;
}

static void __stdcall MHandlerItemFound(const char* text, countable<IReportItem> item)
{
	if(FLCleanEngine::ManagedCleanEngine::CENotifierItemFound != nullptr)
	{
		FLCleanEngine::ManagedCleanEngine::CENotifierItemFound(gcnew System::String(text), item->m_ItemID, item->m_ScannerID);
	}
}

static void __stdcall MHandlerScanFinished(const scanner_id scannerID)
{
	if(FLCleanEngine::ManagedCleanEngine::CEScanFinished != nullptr)
	{
		FLCleanEngine::ManagedCleanEngine::CEScanFinished((int)scannerID);
	}
}

static void __stdcall MHandlerScanStarted(const scanner_id scannerID)
{
	if(FLCleanEngine::ManagedCleanEngine::CEScanStart != nullptr)
	{
		FLCleanEngine::ManagedCleanEngine::CEScanStart((int)scannerID);
	}

}
FLCleanEngine::ManagedCleanEngine::!ManagedCleanEngine()
{
	
}