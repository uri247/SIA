/* Copyright (c) 2013 Ness Technologies
 *
 * Header Name: ScsInstallationAdvisor.h
 *
 * Description:	Declareation of class CScsInstallationAdvisor. This is an implementation of IScsInstallationAdvisor (dual),
 *              IConnectionPointContainer and IObjectSafety - using ATL framework. Everything in this module is a standard
 *              "text book" implementation of a COM class using ATL. 
 *              This class is typically used by the UI or by automation scripts.
 *
 * Author:      Uri London
 *
 */

#pragma once
#include "resource.h"       // main symbols
#include "SiaCore_i.h"
#include "SiaInstallArguments.h"
#include "_ISiaEvents_CP.H"

using namespace ATL;

// On release build, object is marked as unsafe. However, on debug build, it is marked as safe so we can work with it a little 
// bit more smoothly.
#ifdef NDEBUG_
#define OBJSAFE_DECL
#define OBJSAFE_ENTRY
#else
#define OBJSAFE_DECL		,IObjectSafetyImpl<CScsInstallationAdvisor, INTERFACESAFE_FOR_UNTRUSTED_CALLER>
#define OBJSAFE_ENTRY		COM_INTERFACE_ENTRY(IObjectSafety)
#endif


// CScsInstallationAdvisor

class ATL_NO_VTABLE CScsInstallationAdvisor
	:public CComObjectRootEx<CComSingleThreadModel>
	,public CComCoClass<CScsInstallationAdvisor, &CLSID_ScsInstallationAdvisor>
	,public IDispatchImpl<IScsInstallationAdvisor, &IID_IScsInstallationAdvisor, &LIBID_SiaCoreLib, 1, 0>
	OBJSAFE_DECL
	,public IConnectionPointContainerImpl<CScsInstallationAdvisor>
	,public CProxy_ISiaEvents<CScsInstallationAdvisor>
{
public:
    DECLARE_REGISTRY_RESOURCEID(IDR_SCSINSTALLATIONADVISOR)
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	BEGIN_COM_MAP(CScsInstallationAdvisor)
		COM_INTERFACE_ENTRY(IScsInstallationAdvisor)
		COM_INTERFACE_ENTRY(IDispatch)
		OBJSAFE_ENTRY
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()
	BEGIN_CONNECTION_POINT_MAP(CScsInstallationAdvisor)
		CONNECTION_POINT_ENTRY(__uuidof(_ISiaEvents))
	END_CONNECTION_POINT_MAP()

public:
	CScsInstallationAdvisor();
	~CScsInstallationAdvisor();
	void FinalRelease();
	HRESULT FinalConstruct();

public:
	// IScsInstallationAdvisor
	STDMETHOD(get_isScsInstalled)(VARIANT_BOOL* pVal);
	STDMETHOD(get_installParams)(IDispatch** pVal);
	STDMETHOD(put_installParams)(IDispatch* newVal);
	STDMETHOD(installAll)();
	STDMETHOD(wait)();
	STDMETHOD(get_oncomplete)(VARIANT* pVal);
	STDMETHOD(put_oncomplete)(VARIANT newVal);
	STDMETHOD(get_computerName)(BSTR* pVal);
	STDMETHOD(get_currentUser)(BSTR* pVal);
	STDMETHOD(test)(void);
	STDMETHOD(_fireOnComplete)(LONG status);
	STDMETHOD(testUser)(BSTR accountDomain, BSTR accountName, BSTR accountPasswd, BSTR* pVal);
	STDMETHOD(checkDhcpOption15)(BSTR* pVal);

private:
	CComObject<CSiaInstallArguments>* m_pInstallerParamsObject;
	void marshalFireOnComplete( LONG status );
	CIssExecutor m_executor;
	CComPtr<IDispatch> m_dispOnComplete;
    DWORD m_dwIfCookie;
public:
};



OBJECT_ENTRY_AUTO(__uuidof(ScsInstallationAdvisor), CScsInstallationAdvisor)

