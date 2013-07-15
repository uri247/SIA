#pragma once
#include "errors.h"

struct IssExecutorParam
{
	bool createDatabase;
	bool installService;
	bool installConsole;
	std::string serviceAccountDomain;
	std::string serviceAccountName;
	std::string serviceAccountPassword;
	std::string installerLocation;
	std::string sqlServer;
	std::string sqlSchema;
};


class CIssExecutor
{
private:
	IssExecutorParam m_params;
	std::string m_fname;
	std::string m_logname;
	HANDLE m_hIssProcess;
	std::function<void(long)> m_callback;

public:
	CIssExecutor( ) { }

	void setParams( IssExecutorParam const& params );
    void createFile( );
	void execute( std::function<void(long)> );
	void wait( );

	void wait_thread( );
};
