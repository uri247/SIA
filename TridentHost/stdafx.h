#pragma once

// windows
#define _WIN32_WINNT            0x0601
#define WIN32_LEAN_AND_MEAN     1
#include <windows.h>
#include <ShlObj.h>
#include <objbase.h>
#include <direct.h>

// CRT
#include <cstdlib>
#include <memory.h>
#include <cstring>


// ATL
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define _ATL_APARTMENT_THREADED
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atlhost.h>
