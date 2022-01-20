#pragma once
#include "cleanscanner.h"

typedef void (__stdcall * Callback)(const char* text, countable<IReportItem> item);
typedef void (__stdcall * ScannerFinished)(const scanner_id);
typedef void (__stdcall * ScannerStarted)(const scanner_id);

class CNativeNotifier :
	public INotifier
{
//private:
	public:
	CNativeNotifier(void);

	//CNativeNotifier(Callback);
	~CNativeNotifier(void);

public:
	//extern "C" __declspec(dllexport)
 	void __stdcall SetCallback(Callback handler) {
 			Handler = handler;
 	}
	void __stdcall SetCallbackScannerFinished(ScannerFinished handler) {
		ScanFinish = handler;
	}

	void __stdcall SetCallbackScannerStarted(ScannerStarted handler) {
		ScanStart = handler;
	}

	virtual void OnScan( const scanner_id scannerId, const TCHAR * szObjectName, const TCHAR * szValueName );
	virtual void OnItemFind( countable< IReportItem > item );
	virtual void OnItemFix( countable< IReportItem > item );
	virtual void OnScannerStarted( const scanner_id scannerId );
	virtual void OnScannerFinished( const scanner_id scannerId );

private:
	CString m_strItemProcessing;
	Callback Handler;
	ScannerFinished ScanFinish;
	ScannerStarted ScanStart;
};
