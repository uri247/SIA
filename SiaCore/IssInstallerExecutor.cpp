#include "stdafx.h"
#include "IssInstallerExecutor.h"
#include "resource.h"

class AutoResourceData
{
public:
	AutoResourceData();
	~AutoResourceData();

};

std::string get_tmpfile_name( )
{
	char path_buffer[MAX_PATH+1];
	char filename_buffer[MAX_PATH+1];
	DWORD result;

	result = GetTempPathA( _countof(path_buffer)-1, path_buffer );
	if( result == 0 || result > MAX_PATH ) {
		throw Win32Exception( "createTmpFile" );
	}

	result = GetTempFileNameA( path_buffer, "sia", 0, filename_buffer );
	if( result == 0 ) {
		throw Win32Exception( "createTmpFile" );
	}

	return std::string(filename_buffer);
}


std::string get_programfile_dir()
{
	HRESULT hr;
	char buff[MAX_PATH+1];
	hr = SHGetFolderPathA(NULL, CSIDL_PROGRAM_FILES, NULL, SHGFP_TYPE_CURRENT, buff );
	if( FAILED(hr) ) {
		throw HrException(hr, "SHGetFolderPathA");
	}
	return std::string(buff);
}



char const* LoadTextResource( int name, wchar_t const* type, DWORD& size )
{
	HMODULE handle = GetModuleHandle(L"SiaCore");
	HRSRC rc = FindResource(handle, MAKEINTRESOURCE(name), type);
	size = ::SizeofResource(handle, rc);
	HGLOBAL datarc = LoadResource(handle, rc);
	char const* data = static_cast<char const*>(LockResource(datarc));
	return data;
}


void CIssExecutor::setParams( IssExecutorParam const& params )
{
	// this is a structure copy by design.
	m_params = params; 
}

void CIssExecutor::createFile( )
{
	DWORD size;
	std::ofstream tmp_file;
	std::string program_file;
	int resource_name;

	// get name and open the file
	m_fname = get_tmpfile_name();
	tmp_file.open( m_fname );

	program_file = get_programfile_dir();

	// find out which ISS we should start with, and then load it
	if( m_params.createDatabase ) {
		resource_name = IDR_WITH_DATABASE;
	}
	else {
		resource_name = IDR_NO_DATABASE;
	}

	char const* fmt = LoadTextResource( resource_name, L"TEXTFILE", size );

	// allocate the buffer
	std::unique_ptr<char[]> buffer( new char[size+1000] );

	// then, printf
	if( resource_name == IDR_WITH_DATABASE ) {
		sprintf_s( &buffer[0], size+1000, fmt, 
			m_params.serviceAccountDomain.c_str(), 
			m_params.serviceAccountName.c_str(), 
			m_params.serviceAccountPassword.c_str(),
			m_params.sqlServer.c_str(),
			m_params.sqlSchema.c_str(),
			m_params.sqlServer.c_str(),
			m_params.sqlSchema.c_str(),
			program_file.c_str()
			);
	}
	else {
	    sprintf_s( &buffer[0], size+1000, fmt,
			m_params.serviceAccountDomain.c_str(), 
			m_params.serviceAccountName.c_str(), 
			m_params.serviceAccountPassword.c_str(),
			program_file.c_str()
			);
	}

	tmp_file << &buffer[0];

	tmp_file.close();
}

void CIssExecutor::wait_thread()
{
	DWORD result;
	long code;
	result = WaitForSingleObject( m_hIssProcess, INFINITE );
	if( result == WAIT_OBJECT_0 ) {
		code = 0;
	}
	else {
		code = GetLastError();
	}
	m_callback( code );
}

void CIssExecutor::execute( std::function<void(long)> callback )
{
	DWORD dwresult;
	BOOL result;
	char current_dir[MAX_PATH+1];
	char args[2000];

	m_callback = callback;

	dwresult = GetCurrentDirectoryA( _countof(current_dir)-1, current_dir );
	if( !dwresult ) {
		throw Win32Exception( "GetCurrentDirectoryA in execute" );
	}

	sprintf_s( args, "/s /f1%s /f2%s\\%s", m_fname.c_str(), current_dir, "rcs.log" );

	SHELLEXECUTEINFOA si;
	memset( &si, 0, sizeof(si) );
	si.cbSize = sizeof(si);
	si.fMask = SEE_MASK_NOCLOSEPROCESS;
	si.hwnd = NULL;
	si.lpVerb = "open";
	si.lpFile = m_params.installerLocation.c_str();
	si.lpParameters = args;
	si.lpDirectory = 0;
	si.nShow = SW_SHOW;
	si.hInstApp = 0;
	result = ShellExecuteExA(&si);

	if( !result ) {
		throw Win32Exception( "Creating the Intel Scs process" );
	}

	m_hIssProcess = si.hProcess;
	std::thread th( &CIssExecutor::wait_thread, this );
	th.detach();


}


void CIssExecutor::wait( )
{
	DWORD result;
	result = WaitForSingleObject( m_hIssProcess, INFINITE );
	if( result != WAIT_OBJECT_0 ) {
		throw Win32Exception("Waiting for Scs Installer to terminate");
	}
}

