#pragma once


// C++ Headers
#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <condition_variable>

// Windows Headers
#define WIN32_LEAN_AND_MEAN    1
#define _WIN32_WINNT           0x0501
#include <Windows.h>


// ATL Library
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#include <atlbase.h>
#include <atlstr.h>
#include <atlcom.h>

