#pragma once

#include "resource.h"

extern class CMainModule _AtlModule;


class CMainWindow;
typedef IDispEventSimpleImpl<1, CMainWindow, &__uuidof(DWebBrowserEvents2)> DWebBrowserEvents2Impl;



class CMainWindow
	:public CWindowImpl<CMainWindow>
	,DWebBrowserEvents2Impl
{
public:
    BEGIN_MSG_MAP(CMainWindow)
		MESSAGE_HANDLER( WM_CREATE, onCreate )
        MESSAGE_HANDLER( WM_DESTROY, onDestroy )
		MESSAGE_HANDLER( WM_SIZE, onSize )
    END_MSG_MAP()
	BEGIN_SINK_MAP(CMainWindow)
	END_SINK_MAP()

public:
    CMainWindow( );
	LRESULT onCreate( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& fhandled );
    LRESULT onDestroy( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& fhandled );
	LRESULT onSize( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& fhandled );

private:
	CAxWindow2 m_axwnd;
};



class CMainModule : public CAtlExeModuleT< CMainModule >
{
public:
	HRESULT PreMessageLoop( int nCmdShow );
	HRESULT PostMessageLoop();
	HRESULT regClass();

public: 
	CMainModule() :m_dwRegister(0) { }

private:
	CMainWindow m_wndMain;
	DWORD m_dwRegister;

private:
	HRESULT setIeEmulation();
};

