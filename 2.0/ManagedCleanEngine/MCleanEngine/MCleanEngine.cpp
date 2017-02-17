// This is the main DLL file.

#include "stdafx.h"
#include <atlstr.h>
#include <atlfile.h>
#include "MCleanEngine.h"
#include "CETypes.h"
#using <mscorlib.dll>
using namespace System::Runtime::InteropServices;
static void __stdcall MHandlerItemFound(const char*, countable<IReportItem> item);
static void __stdcall MHandlerScanFinished(const scanner_id scannerID);
static void __stdcall MHandlerScanStarted(const scanner_id scannerID);

MCleanEngine::ManagedCleanEngine::ManagedCleanEngine()
{
	mp_notifier = new CNativeNotifier();
	mp_core = new CCleanEngine(mp_notifier);
	mp_defragger = new CDefragger();
	mp_notifier->SetCallback(MHandlerItemFound);
	mp_notifier->SetCallbackScannerFinished(MHandlerScanFinished);
	mp_notifier->SetCallbackScannerStarted(MHandlerScanStarted);
}

bool MCleanEngine::ManagedCleanEngine::Start()
{	
	if(mp_core->StartEngine() == ERR_ALREADY_RUNNING)
		return false;
	else
		return true;
}

void MCleanEngine::ManagedCleanEngine::Pause( BOOL bSuspend )
{
	mp_core->Pause(bSuspend);
}

void MCleanEngine::ManagedCleanEngine::Stop()
{
	mp_core->Stop();
}

void MCleanEngine::ManagedCleanEngine::AddItem( countable<IReportItem> item )
{
	mp_core->AddItem(item);
}

bool MCleanEngine::ManagedCleanEngine::FixItem( item_id id )
{
	return mp_core->FixItem(id);
}

void MCleanEngine::ManagedCleanEngine::FixAllItems()
{
	mp_core->FixAllItems();
}

void MCleanEngine::ManagedCleanEngine::CallPreCleanupHandler( item_id id )
{
	mp_core->CallPreCleanupHandler(id);
}

void MCleanEngine::ManagedCleanEngine::AddScanner( countable< CCleanScanner >& rspScanner )
{
	try
	{
	mp_core->AddScanner(rspScanner);
	}
	catch(...)
	{
	}
}

bool MCleanEngine::ManagedCleanEngine::RemoveScanner( const scanner_id scannerId )
{
	return mp_core->RemoveScanner(scannerId);
}

void MCleanEngine::ManagedCleanEngine::EnableScanner( const int Id, const bool bEnable )
{
	mp_core->EnableScanner(scanner_id(Id), bEnable);
}

bool MCleanEngine::ManagedCleanEngine::IsScannerEnabled( const scanner_id Id )
{
	return mp_core->IsScannerEnabled(Id);
}

void MCleanEngine::ManagedCleanEngine::EnableAllScanners( const bool bEnable )
{
	mp_core->EnableAllScanners(bEnable);
}

MCleanEngine::ManagedCleanEngine::~ManagedCleanEngine()
{		
}

int MCleanEngine::ManagedCleanEngine::GetScannersNumber()
{
	return static_cast<int>(mp_core->GetScannersNum());
}

bool MCleanEngine::ManagedCleanEngine::IsBusy()
{
	return mp_core->IsBusy();
}

bool MCleanEngine::ManagedCleanEngine::AnalizeHives()
{
	return mp_defragger->AnalyzeHives(); 
}

void MCleanEngine::ManagedCleanEngine::CompactHives()
{
	mp_defragger->CompactHives();
}

bool MCleanEngine::ManagedCleanEngine::LoadHivesInfo(System::String^ path)
{
	const char* str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(path);
	bool result = mp_defragger->LoadHivesInfo(str2);
	Marshal::FreeHGlobal((System::IntPtr)(void*)str2);
	return result;
}

static void __stdcall MHandlerItemFound(const char* text, countable<IReportItem> item)
{
	if(MCleanEngine::ManagedCleanEngine::CENotifierItemFound != nullptr)
	{
		MCleanEngine::ManagedCleanEngine::CENotifierItemFound(gcnew System::String(text), item->m_ItemID, item->m_ScannerID);
	}
}

static void __stdcall MHandlerScanFinished(const scanner_id scannerID)
{
	if(MCleanEngine::ManagedCleanEngine::CEScanFinished != nullptr)
	{
		MCleanEngine::ManagedCleanEngine::CEScanFinished((int)scannerID);
	}
}

static void __stdcall MHandlerScanStarted(const scanner_id scannerID)
{
	if(MCleanEngine::ManagedCleanEngine::CEScanStart != nullptr)
	{
		MCleanEngine::ManagedCleanEngine::CEScanStart((int)scannerID);
	}

}
MCleanEngine::ManagedCleanEngine::!ManagedCleanEngine()
{
	
}