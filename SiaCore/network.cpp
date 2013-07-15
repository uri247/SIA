/* Copyright (c) 2013 Ness Technologies
 *
 * Module:		network.cpp
 *
 * Description: A collection of user and network functions
 *
 * Author:      Uri London
 *
 */
#include "stdafx.h"
#include "errors.h"

std::wstring getFqdn( )
{
	BOOL result;
	wchar_t buffer[400];
	DWORD size = _countof(buffer) - 1;

	result = GetComputerNameEx( ComputerNameDnsFullyQualified, buffer, &size );
	if( !result ) {
		throw Win32Exception( "Trying to get computer name" );
	}

	return std::wstring(buffer);
}

std::wstring getCurrentUser( )
{
	BOOL result;
	wchar_t buffer[400];
	ULONG size = _countof(buffer) - 1;

	result = GetUserNameEx( NameSamCompatible, buffer, &size );
	if( !result ) {
		throw Win32Exception( "Get Current User" );
	}

	return std::wstring(buffer);
}


// This is a specialization of default_delete, so unique_ptr will work with it. We free memory allocated by Dhcp functions
// with DhcpRpcFreeMemory. Using this technique, I can return memory having the caller responsible freeing the memory in
// a policy the callee dictates.
template<>
struct std::default_delete<DHCP_OPTION> {
public:
	void operator()( DHCP_OPTION* ptr ) {
		DhcpRpcFreeMemory( ptr );
	}
};


//
// Note: caller will free memory within the destructor of return type.
//
std::unique_ptr<DHCP_OPTION> dhcpGetOption_ViaServerAPI(int num)
{
	DWORD result;
	DHCP_OPTION* raw_ptr = NULL;

    result = DhcpGetOptionInfo( L"192.168.24.51", num, &raw_ptr );
	if( result != ERROR_SUCCESS ) {
		throw Win32Exception("dhcp get option");
	}

	std::unique_ptr<DHCP_OPTION> option(raw_ptr);
	return option;
}


std::string dhcpOption15Check_ViaServerAPI()
{
	char const* status;

	try {
	    std::unique_ptr<DHCP_OPTION> option = dhcpGetOption_ViaServerAPI(15);
		ATLASSERT( option->OptionType == DhcpUnaryElementTypeOption );
		wchar_t* val = option->DefaultValue.Elements[0].Element.StringDataOption;
		status = "OK";
	}
	catch( Win32Exception x ) {
		if( x.error() == ERROR_ACCESS_DENIED ) {
			status = "access denied";
		}
		else {
			status = "error";
		}
	}

	return std::string(status);
}


std::string getDomainName( wchar_t* adapterName )
{
	DWORD result;
	BYTE buffer[1000];
	DWORD size = _countof(buffer);
	std::string domainName;

	DHCPCAPI_PARAMS option15Param = { 0, OPTION_DOMAIN_NAME, FALSE, 0, 0 };
	DHCPCAPI_PARAMS_ARRAY recdParams = { 1, &option15Param };
	DHCPCAPI_PARAMS_ARRAY sendParams = { 0, NULL };

	result = DhcpRequestParams( DHCPCAPI_REQUEST_SYNCHRONOUS, NULL, adapterName, NULL, sendParams, recdParams, buffer, &size, NULL );
	
	// result may be 2 (not found) or 0 (ok).
	if( result == ERROR_SUCCESS ) {
		domainName = std::string((char*)option15Param.Data);
	}
	else {
		domainName = std::string();
	}
	return domainName;
}

//
// Function:	Iterate through all network adapters, for call the callback routine with adapter information
//
void enumAdapters( std::function<bool(IP_ADAPTER_INFO&)> callback )
{
	// 20 adapaters on one machine is enough!
	IP_ADAPTER_INFO rgInfo[20];
	DWORD size = sizeof(rgInfo);
	DWORD result;

	result = GetAdaptersInfo( rgInfo, &size );
	if( result != ERROR_SUCCESS ) {
		throw Win32Exception(result, "GetAdaptersInfo");
	}

	bool totalOption15 = false;
	for( IP_ADAPTER_INFO* pinfo = rgInfo; pinfo && !totalOption15; pinfo = pinfo->Next ) {
		bool cont = callback( *pinfo );
		if( !cont )
			break;
	}
}


std::string dhcpOption15Check()
{
	char const* status;
	DWORD result;
	bool found = false;

	enumAdapters( [&found](IP_ADAPTER_INFO& info) {
		std::string domain = getDomainName( ATL::CA2W(info.AdapterName) );
		found = (domain.length() > 0);
		return !found;
	});

	if( found )
		status = "OK";
	else
		status = "can't find domain name suffix on any adapter";

	return std::string(status);
};
