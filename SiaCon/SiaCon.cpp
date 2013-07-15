// SiaCon.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../SiaCore/SiaCore_i.h"

// {59954DB2-C7E8-454F-8CC1-E4A7F54812A0}
DEFINE_GUID( CLSID_Listener, 0x59954db2, 0xc7e8, 0x454f, 0x8c, 0xc1, 0xe4, 0xa7, 0xf5, 0x48, 0x12, 0xa0);

class CListener;
typedef IDispEventSimpleImpl<1, CListener, &__uuidof(_ISiaEvents)> SiaEventsImpl;
_ATL_FUNC_INFO FuncInfo_Complete = { CC_STDCALL, VT_EMPTY, 1, { VT_I4 }   };


std::condition_variable event;
std::mutex mu;
bool complete = false;


class CListener :public SiaEventsImpl
{
public:
    BEGIN_SINK_MAP(CListener)
        SINK_ENTRY_INFO(1, __uuidof(_ISiaEvents), 800, OnComplete, &FuncInfo_Complete )
    END_SINK_MAP()

public:
	HRESULT STDMETHODCALLTYPE OnComplete( LONG status ) {
		{
			std::unique_lock<std::mutex> lock( mu );
			complete = true;
		}
		event.notify_one();
		return S_OK;
	}
};


int wmain(int argc, wchar_t* argv[])
{
	CComPtr<IScsInstallationAdvisor> ifAdvisor;
	HRESULT hr;
	CListener listener;
	

	hr = CoInitialize( NULL );
	hr = CoCreateInstance( __uuidof(ScsInstallationAdvisor), NULL, CLSCTX_INPROC_SERVER, __uuidof(IScsInstallationAdvisor), (void**)&ifAdvisor );

	VARIANT_BOOL installed;
	hr = ifAdvisor->get_isScsInstalled( &installed );

	CComPtr<IDispatch> dispParams;
	CComPtr<ISiaInstallArguments> ifParams;
	hr = ifAdvisor->get_installParams( &dispParams );
	hr = dispParams.QueryInterface( &ifParams );

	//CComBSTR exec( "c:\\IntelSCS\\Resources\\IntelSCS\\RCS\\IntelSCSInstaller.exe" );
	CComBSTR exec( "c:\\Windows\\system32\\notepad.exe" );
	CComBSTR domain( "LONDON" );
	CComBSTR user( "HOME" );
	CComBSTR password( "makeFile12" );
	hr = ifParams->put_installerLocation( exec );
	hr = ifParams->put_serviceAccountDomain( domain );
	hr = ifParams->put_serviceAccountUser( user );
	hr = ifParams->put_serviceAccountPassword( password );

	DWORD cookie;
	AtlAdvise( ifAdvisor, (IUnknown*)&listener, __uuidof(_ISiaEvents), &cookie );


	hr = ifAdvisor->installAll();

	std::unique_lock<std::mutex> lock( mu );
	if( complete ) {
		lock.unlock();
	}
	else {
	    event.wait(lock);
	}


}

