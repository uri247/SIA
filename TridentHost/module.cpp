/* Copyright (c) 2013 Ness Technologies
 *
 * Module Name: module.cpp:w
 *
 * Description:	module level functions and structure, including definition of the _AtlModule global and the 
 *              methods of CMainModule, and the function WinMain
 *              Message loop (this is a Win32 program) is run by ATL's WinMain function
 *
 * Author:      Uri London
 *
 */

#include "stdafx.h"
#include "TridentHost.h"
#include "../SiaCore/SiaCore_i.h"

CMainModule _AtlModule;

wchar_t const* regpathFeatureControl = L"Software\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION";
wchar_t const* regvalEmulation = L"ScsIA.exe";

//
// I need to tell Trident that we are on IE10.
// TODO: right now our web pages only support IE10. It wouldn't be terribly hard to support IE9, and with some degrigation
// on the user experience (no sliding effect) we can also support IE8, and even below.
//
HRESULT CMainModule::setIeEmulation()
{
	LONG result;
	CRegKey key;

	result = key.Open( HKEY_CURRENT_USER, regpathFeatureControl, KEY_ALL_ACCESS );
	if( result == ERROR_SUCCESS ) {
	    result = key.SetDWORDValue( regvalEmulation, 10000 );
	}

	return HRESULT_FROM_WIN32( result );
}



typedef HRESULT (__stdcall *DllGetClassObjectFunction)(REFCLSID rclsid, REFIID riid, void** ppv );


//
// Method:		CMainModule::regClass
//
// Description:	This function tries to workaround the registry requirement. Here I've a custom implementation
//              of CoCreateInstance via LoadLibrary, class object, and Create.
//
// TODO:		Unfortunately, this function works for the Creation once Trident has the guid, but DOES NOT
//				WORK for finding the GUID when the JavaScript is using the ProgID. Eventually I'll have to 
//				figure out why the regfree assembly doesn't work yet.
HRESULT CMainModule::regClass()
{
	HRESULT hr = S_OK;
	HMODULE hmodule = NULL;
	DllGetClassObjectFunction fn = NULL;
	CComPtr<IClassFactory> ifFactory;

	if( SUCCEEDED(hr) ) {
		hmodule = LoadLibrary( L"SiaCore.dll" );
		if( !hmodule ) {
			hr = HRESULT_FROM_WIN32(GetLastError());
		}
	}

	if( SUCCEEDED(hr) ) {
		fn = (DllGetClassObjectFunction)GetProcAddress( hmodule, "DllGetClassObject" );
		if( !fn ) {
			hr = HRESULT_FROM_WIN32(GetLastError());
		}
	}

	if( SUCCEEDED(hr) ) {
		hr = fn( __uuidof(ScsInstallationAdvisor), IID_IClassFactory, (void**)&ifFactory );
	}


	if( SUCCEEDED(hr) ) {
		hr = CoRegisterClassObject( __uuidof(ScsInstallationAdvisor), ifFactory, CLSCTX_INPROC_SERVER, REGCLS_MULTIPLEUSE, &m_dwRegister );
	}

	return hr;
}




HRESULT CMainModule::PreMessageLoop( int nCmdShow )
{
	HRESULT hr = S_OK;

	if( SUCCEEDED(hr) ) {
	    hr = CAtlExeModuleT<CMainModule>::PreMessageLoop( nCmdShow );
	}

	if( SUCCEEDED(hr) ) {
		hr = setIeEmulation();
	}

	if( SUCCEEDED(hr) ) {
		hr = regClass();
	}

	if( SUCCEEDED(hr) )
	{
		AtlAxWinInit();
		RECT rect = { 200, 200, 920, 831 };
		m_wndMain.Create(NULL, rect, L"Trident Host", WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME, 0 );
		if( m_wndMain.m_hWnd == NULL ) {
			hr = HRESULT_FROM_WIN32( GetLastError() );
		}
	}

	if( SUCCEEDED(hr) ) {
	    m_wndMain.ShowWindow(nCmdShow);
	}

	return hr;
}


HRESULT CMainModule::PostMessageLoop()
{
	if( m_dwRegister ) {
		CoRevokeClassObject( m_dwRegister );
		m_dwRegister = 0;
	}

	AtlAxWinTerm();
	return CAtlExeModuleT<CMainModule>::PostMessageLoop();
}



int APIENTRY wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	return _AtlModule.WinMain(nCmdShow);
}

