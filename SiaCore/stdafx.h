#pragma once

// C++ Headers
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <memory>
#include <thread>
#include <functional>

// Windows Headers
#define WIN32_LEAN_AND_MEAN		1
#define _WIN32_WINNT			0x0501
#define WINVER					0x0501
#define SCRICT					1
#define SECURITY_WIN32			1
#include <Windows.h>
#include <shellapi.h>
#include <security.h>
#include <dhcpsapi.h>
#include <DhcpCSdk.h>
#include <IPHlpApi.h>
#include <ShlObj.h>

// ATL Headers
#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>


