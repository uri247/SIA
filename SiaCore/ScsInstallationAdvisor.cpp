/* Copyright (c) 2013 Ness Technologies
 *
 * Module Name: ScsInstallationAdvisor.cpp
 *
 * Description:	Implementation of CScsInstallationAdvisor
 *
 * Author:      Uri London
 *
 */
#include "stdafx.h"
#include "ScsInstallationAdvisor.h"
#include "IssInstallerExecutor.h"
#include "GlobalIntfTable.h"
#include "errors.h"
#include "network.h"
#include "user.h"

wchar_t const* regpathScsUninstall  = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{78A13CBB-40C7-4E11-9405-38C5C3A98AB5}";



CScsInstallationAdvisor::CScsInstallationAdvisor()
	:m_pInstallerParamsObject(NULL)
	,m_dwIfCookie(0)
{
}

CScsInstallationAdvisor::~CScsInstallationAdvisor()
{
	ATLASSERT( m_pInstallerParamsObject == NULL );
}


HRESULT CScsInstallationAdvisor::FinalConstruct()
{
	HRESULT hr = S_OK;

	try {
		// The Installer Advisor has a single Installer Parameter structure. Instantiate it now, and keep
		// a strong reference in the class
		hr = CComObject<CSiaInstallArguments>::CreateInstance( &m_pInstallerParamsObject );
		m_pInstallerParamsObject->AddRef();
		if( FAILED(hr) ) {
			throw HrException(hr, "creating parameter object" );
		}

		CComPtr<IScsInstallationAdvisor> ifMe;
		hr = this->QueryInterface( IID_IScsInstallationAdvisor, (void**)&ifMe );
		if( FAILED(hr) ) {
			throw HrException( hr, "query self for internal interface" );
		}

		hr = TheGlobalIntfTbl()->RegisterInterfaceInGlobal( ifMe, IID_IScsInstallationAdvisor, &m_dwIfCookie );
		if( FAILED(hr) ) {
			throw HrException(hr, "register interface for marshaling");
		}
	}
    catch( Win32Exception x ) {
		hr = HRESULT_FROM_WIN32(x.error());
	}
	catch( HrException x ) {
		hr = x.error();
	}

	return hr;
}

void CScsInstallationAdvisor::FinalRelease()
{
	m_pInstallerParamsObject->Release();
	m_pInstallerParamsObject = NULL;
}


STDMETHODIMP CScsInstallationAdvisor::get_isScsInstalled(VARIANT_BOOL* pVal)
{
	HRESULT hr = S_OK;
	CRegKey key;
	LONG result;

	result = key.Open( HKEY_LOCAL_MACHINE, regpathScsUninstall, KEY_READ );
	if( result == ERROR_SUCCESS ) {
		*pVal = TRUE;
	}
	else if( result == ERROR_FILE_NOT_FOUND ) {
		*pVal = FALSE;
	}
	else {
		// tihs is a different error. returns HRESULT
		hr = HRESULT_FROM_WIN32( GetLastError() );
	}

	return hr;
}



STDMETHODIMP CScsInstallationAdvisor::get_installParams(IDispatch** pVal)
{
	HRESULT hr = m_pInstallerParamsObject->QueryInterface( pVal );
	return hr;
}


STDMETHODIMP CScsInstallationAdvisor::put_installParams(IDispatch* newVal)
{
	// Support IDispatch (e.g. Javascript object) from the user. Not impolemented yet
	HRESULT hr = E_NOTIMPL;
	return hr;
}


STDMETHODIMP CScsInstallationAdvisor::installAll()
{
	HRESULT hr = S_OK;
	try {
		IssExecutorParam execParams;
		m_pInstallerParamsObject->getExecutorParams( &execParams );
		m_executor.setParams( execParams );
		m_executor.createFile();

		// call the executor, giving it a lambda method, which will fire an event back to the calelr
		m_executor.execute( [this](long status) {
			marshalFireOnComplete(status);
		});
	}
	catch( Win32Exception x ) {
		hr = HRESULT_FROM_WIN32(x.error());
	}
	return hr;
}


STDMETHODIMP CScsInstallationAdvisor::wait()
{
	HRESULT hr = S_OK;
	try {
		m_executor.wait();
	}
	catch( Win32Exception x ) {
		hr = HRESULT_FROM_WIN32(x.error());
	}
	return hr;
}

void CScsInstallationAdvisor::marshalFireOnComplete( LONG status )
{
	HRESULT hr;
	// We may be on a different thread here. Get a pointer to ourselves and marshal a request
	// to our very own internal interface.

	CComPtr<IScsInstallationAdvisor> ifMe;
	hr = TheGlobalIntfTbl()->GetInterfaceFromGlobal( m_dwIfCookie, IID_IScsInstallationAdvisor, (void**)&ifMe );
	if( SUCCEEDED(hr) ) {
	    ifMe->_fireOnComplete( status );
	}
}


STDMETHODIMP CScsInstallationAdvisor::_fireOnComplete(LONG status)
{
	HRESULT hr = S_OK;

	try {

		// Fire through the completion port
		hr = Fire_complete( 0 );

		// Also, fire through the oncomplete event
		if( m_dispOnComplete ) {
			CComVariant avarParams[1];
			avarParams[0] = status;
			avarParams[0].vt = VT_I4;
			CComVariant varResult;
			DISPPARAMS params = { avarParams, NULL, 1, 0 };

			m_dispOnComplete->Invoke( 0, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL );
		}
	}
	catch( Win32Exception x ) {
		hr = HRESULT_FROM_WIN32( x.error() );
	}
	catch( HrException x ) {
		hr = x.error();
	}

	return hr;
}


STDMETHODIMP CScsInstallationAdvisor::get_oncomplete(VARIANT* pVal)
{
	return E_NOTIMPL;
}


STDMETHODIMP CScsInstallationAdvisor::put_oncomplete(VARIANT newVal)
{
	HRESULT hr = S_OK;

	if( newVal.vt == VT_DISPATCH )
	{
		m_dispOnComplete = newVal.pdispVal;
	}

	return hr;
}


STDMETHODIMP CScsInstallationAdvisor::get_computerName(BSTR* pVal)
{
	HRESULT hr = S_OK;

	try {
	    std::wstring fqdn = getFqdn();
		CComBSTR fqdnBstr( fqdn.c_str() );
		*pVal =  fqdnBstr.Detach();
	}
	catch( Win32Exception x ) {
		hr = HRESULT_FROM_WIN32(x.error());
	}

	return hr;
}



STDMETHODIMP CScsInstallationAdvisor::get_currentUser(BSTR* pVal)
{
	HRESULT hr = S_OK;

	try {
	    std::wstring currentUser = getCurrentUser();
		*pVal = CComBSTR( currentUser.c_str() ).Detach();
	}
	catch( Win32Exception x ) {
		hr = HRESULT_FROM_WIN32(x.error());
	}

	return hr;
}


STDMETHODIMP CScsInstallationAdvisor::test(void)
{
	_fireOnComplete( 42 );
	return S_OK;
}




STDMETHODIMP CScsInstallationAdvisor::testUser(BSTR accountDomain, BSTR accountName, BSTR accountPasswd, BSTR* pVal )
{
	bool success = verifyUserCredentials( accountDomain, accountName, accountPasswd );
	*pVal = CComBSTR( success ? L"OK" : L"FAIL" ).Detach();
	return S_OK;
}



STDMETHODIMP CScsInstallationAdvisor::checkDhcpOption15(BSTR* pVal)
{
	std::string status = dhcpOption15Check();
	*pVal = CComBSTR(status.c_str()).Detach();
	return S_OK;
}
