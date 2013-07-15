#pragma once
#include "resource.h"
#include "SiaCore_i.h"
#include "IssInstallerExecutor.h"
using namespace ATL;


// CSiaInstallArguments

class ATL_NO_VTABLE CSiaInstallArguments :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSiaInstallArguments, &CLSID_SiaInstallArguments>,
	public IDispatchImpl<ISiaInstallArguments, &IID_ISiaInstallArguments, &LIBID_SiaCoreLib, 1, 0>
{
	BEGIN_COM_MAP(CSiaInstallArguments)
		COM_INTERFACE_ENTRY(ISiaInstallArguments)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

public:
    CSiaInstallArguments();
    ~CSiaInstallArguments();
	HRESULT CSiaInstallArguments::FinalConstruct( );
	void FinalRelease() { }

public:
	// ISiaInstallArguments
	STDMETHOD(get_createDatabase)(VARIANT_BOOL* pVal);
	STDMETHOD(put_createDatabase)(VARIANT_BOOL newVal);
	STDMETHOD(get_installService)(VARIANT_BOOL* pVal);
	STDMETHOD(put_installService)(VARIANT_BOOL newVal);
	STDMETHOD(get_installConsole)(VARIANT_BOOL* pVal);
	STDMETHOD(put_installConsole)(VARIANT_BOOL newVal);
	STDMETHOD(get_serviceAccountDomain)(BSTR* pVal);
	STDMETHOD(put_serviceAccountDomain)(BSTR newVal);
	STDMETHOD(get_serviceAccountUser)(BSTR* pVal);
	STDMETHOD(put_serviceAccountUser)(BSTR newVal);
	STDMETHOD(get_serviceAccountPassword)(BSTR* pVal);
	STDMETHOD(put_serviceAccountPassword)(BSTR newVal);
	STDMETHOD(get_installerLocation)(BSTR* pVal);
	STDMETHOD(put_installerLocation)(BSTR newVal);
	STDMETHOD(get_sqlServer)(BSTR* pVal);
	STDMETHOD(put_sqlServer)(BSTR newVal);
	STDMETHOD(get_sqlSchema)(BSTR* pVal);
	STDMETHOD(put_sqlSchema)(BSTR newVal);

public:
	void getExecutorParams( IssExecutorParam* params );

private:
	bool m_createDatabase;
	bool m_installService;
	bool m_installConsole;
	CComBSTR m_serviceAccountDomain;
	CComBSTR m_serviceAccountName;
	CComBSTR m_serviceAccountPassword;
	CComBSTR m_installerLocation;
	CComBSTR m_sqlServer;
	CComBSTR m_sqlSchema;
};

