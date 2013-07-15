/* Copyright (c) 2013 Ness Technologies
 *
 * Module Name: TridentHost.cpp
 *
 * Description:	Trident a.k.a. Microsoft HTML control, a.k.a. WebOC. We have a small wrapper Windows application that
 *              host the mshtml control, then navigate to that start.html web application
 *
 * Author:      Uri London
 *
 */
#include "stdafx.h"
#include "TridentHost.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name



CMainWindow::CMainWindow( )
{
    CWndClassInfo& wci = GetWndClassInfo( );
    if( !wci.m_atom ) {
        wci.m_wc.hIcon = LoadIcon( _AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(IDI_TRIDENTHOST) );
        wci.m_wc.hIconSm = LoadIcon( _AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(IDI_SMALL) );
        wci.m_bSystemCursor = TRUE;
        wci.m_lpszCursorID = IDC_CROSS;
    }
}


LRESULT CMainWindow::onDestroy( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& fhandled )
{
    PostQuitMessage( 0 );
    return ERROR_SUCCESS;
}


LRESULT CMainWindow::onCreate( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& fhandled )
{
	HRESULT hr;
	RECT rect;

	GetClientRect(&rect);
	m_axwnd.Create( m_hWnd, rect, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 1 );
	ATLASSERT( m_axwnd.m_hWnd  );

	CComPtr<IUnknown> unkControl;
	CComPtr<IWebBrowser2> ifBrz;

	hr = m_axwnd.CreateControlEx(
		L"about:blank",
		NULL,
		NULL,
		&unkControl,
		__uuidof(DWebBrowserEvents2),
		(IUnknown*)(DWebBrowserEvents2Impl*)this
		);

	if( SUCCEEDED(hr) ) {
		hr = unkControl.QueryInterface( &ifBrz );
	}

	wchar_t start[MAX_PATH];
	_wgetcwd( start, MAX_PATH );
	//wcscat_s(start, L"\\..\\html\\start.html");
	wcscat_s(start, L"\\start.html");


	//CComBSTR url( L"www.london.org.il" );
	CComBSTR url( start);
	if( SUCCEEDED(hr) ) {
		hr = ifBrz->Navigate( url, NULL, NULL, NULL, NULL );
	}

	return 0;
}


LRESULT CMainWindow::onSize( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& fhandled )
{
    WORD width = LOWORD(lparam);
    WORD height = HIWORD(lparam);

	::MoveWindow( m_axwnd, 0, 0, width, height, false );

	return 0;
}
