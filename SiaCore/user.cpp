#include "stdafx.h"
#include "user.h"



bool verifyUserCredentials( wchar_t const* domain, wchar_t const* userName, wchar_t const* password )
{
    BOOL result;
	HANDLE htoken;
	DWORD error;

	result = LogonUser( userName, domain, password, LOGON32_LOGON_NETWORK, LOGON32_PROVIDER_DEFAULT, &htoken );
	if( !result ) {
	    error = GetLastError();
	}

	CloseHandle( htoken );
	return result ? true : false;
}