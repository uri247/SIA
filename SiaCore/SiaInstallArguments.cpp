#include "stdafx.h"
#include "SiaInstallArguments.h"

CComBSTR empty_bstr(L"");

CSiaInstallArguments::CSiaInstallArguments()
/*
 * Set default value for the installer parameter
 *
 */
	:m_createDatabase(false)
	,m_installService(false)
	,m_installConsole(false)
	,m_serviceAccountDomain(L"")
	,m_serviceAccountName(L"")
	,m_serviceAccountPassword(L"")
	,m_sqlServer(L"")
	,m_sqlSchema(L"")
	,m_installerLocation(L"IntelSCSInstaller.exe")
{
}


CSiaInstallArguments::~CSiaInstallArguments()
{
}


HRESULT CSiaInstallArguments::FinalConstruct( )
{
	return S_OK;
}


STDMETHODIMP CSiaInstallArguments::get_createDatabase(VARIANT_BOOL* pVal)
{
	*pVal = m_createDatabase ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}


STDMETHODIMP CSiaInstallArguments::put_createDatabase(VARIANT_BOOL newVal)
{
	m_createDatabase = (newVal == VARIANT_TRUE);
	return S_OK;
}


STDMETHODIMP CSiaInstallArguments::get_installService(VARIANT_BOOL* pVal)
{
	*pVal = m_installService ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}


STDMETHODIMP CSiaInstallArguments::put_installService(VARIANT_BOOL newVal)
{
	m_installService = (newVal == VARIANT_TRUE);
	return S_OK;
}


STDMETHODIMP CSiaInstallArguments::get_installConsole(VARIANT_BOOL* pVal)
{
	*pVal = m_installConsole ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}


STDMETHODIMP CSiaInstallArguments::put_installConsole(VARIANT_BOOL newVal)
{
	m_installConsole = ( newVal == VARIANT_TRUE );
	return S_OK;
}


STDMETHODIMP CSiaInstallArguments::get_serviceAccountDomain(BSTR* pVal)
{
	*pVal = m_serviceAccountDomain.Copy();
	return S_OK;
}


STDMETHODIMP CSiaInstallArguments::put_serviceAccountDomain(BSTR newVal)
{
	m_serviceAccountDomain = newVal ? newVal : empty_bstr;
	return S_OK;
}


STDMETHODIMP CSiaInstallArguments::get_serviceAccountUser(BSTR* pVal)
{
	*pVal = m_serviceAccountName.Copy();
	return S_OK;
}


STDMETHODIMP CSiaInstallArguments::put_serviceAccountUser(BSTR newVal)
{
	m_serviceAccountName = newVal ? newVal : empty_bstr;
	return S_OK;
}


STDMETHODIMP CSiaInstallArguments::get_serviceAccountPassword(BSTR* pVal)
{
	*pVal = m_serviceAccountPassword.Copy();
	return S_OK;
}


STDMETHODIMP CSiaInstallArguments::put_serviceAccountPassword(BSTR newVal)
{
	m_serviceAccountPassword = newVal ? newVal : empty_bstr;
	return S_OK;
}



STDMETHODIMP CSiaInstallArguments::get_installerLocation(BSTR* pVal)
{
	*pVal = m_installerLocation;
	return S_OK;
}


STDMETHODIMP CSiaInstallArguments::put_installerLocation(BSTR newVal)
{
	m_installerLocation = newVal ? newVal : empty_bstr;
	return S_OK;
}


STDMETHODIMP CSiaInstallArguments::get_sqlServer(BSTR* pVal)
{
	*pVal = m_sqlServer;
	return S_OK;
}


STDMETHODIMP CSiaInstallArguments::put_sqlServer(BSTR newVal)
{
	m_sqlServer = newVal ? newVal : empty_bstr;
	return S_OK;
}


STDMETHODIMP CSiaInstallArguments::get_sqlSchema(BSTR* pVal)
{
	*pVal = m_sqlSchema;
	return S_OK;
}


STDMETHODIMP CSiaInstallArguments::put_sqlSchema(BSTR newVal)
{
	m_sqlSchema = newVal ? newVal : empty_bstr;
	return S_OK;
}

void CSiaInstallArguments::getExecutorParams( IssExecutorParam* params )
/*
 * Merely translate structure from internal fields (also: unicode, com) to the executor format (c++11 friendly)
 */
{
	params->createDatabase = this->m_createDatabase;
	params->installService = this->m_installService;
	params->installConsole = this->m_installConsole;
	params->serviceAccountDomain = CW2A(m_serviceAccountDomain);
	params->serviceAccountName = CW2A(m_serviceAccountName);
	params->serviceAccountPassword = CW2A(m_serviceAccountPassword);
	params->installerLocation = CW2A(m_installerLocation);
	params->sqlServer = CW2A(m_sqlServer);
	params->sqlSchema = CW2A(m_sqlSchema);
}

