#pragma once

class CSiaCoreModule : public ATL::CAtlDllModuleT< CSiaCoreModule >
{
public :
	DECLARE_LIBID(LIBID_SiaCoreLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SIACORE, "{453C4929-36BB-42BD-85B4-653F1F3C8292}")
};

extern class CSiaCoreModule _AtlModule;


