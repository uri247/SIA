/* Copyright (c) 2013 Ness Technologies
 *
 * Header:		GlobalIntfTable.h
 *
 * Description:	On occasions, we need to marshal between threads. The canonical example is when we want to marshal to the UI thread
 *				(or whatever thread that created the SCS Advisor object) in order to send an event or other manipulation that
 *              requires being on the same appartment.
 *
 * Author:      Uri London
 *
 */
#include "errors.h"

class CGlobalInterfaceTable
{
	friend IGlobalInterfaceTable* TheGlobalIntfTbl(void);

private:
    CComPtr<IGlobalInterfaceTable> m_if;

private:
    CGlobalInterfaceTable( )
    {
		HRESULT hr = CoCreateInstance( CLSID_StdGlobalInterfaceTable, NULL, CLSCTX_INPROC_SERVER, IID_IGlobalInterfaceTable, (void**)&m_if );
		if( FAILED(hr) ) {
			throw HrException(hr, "creating global interface table" );
		}
    }

public:
    operator IGlobalInterfaceTable*( ) {
        return m_if;
    }
};


IGlobalInterfaceTable* TheGlobalIntfTbl( void )
/*
 * There is only on IGlobalInterfaceTable, and this is the function that returns it.
 *
 */
{
    static CGlobalInterfaceTable l_globalIntfTbl;
    return l_globalIntfTbl;
}

