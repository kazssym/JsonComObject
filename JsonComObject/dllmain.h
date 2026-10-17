// dllmain.h : Declaration of module class.

class CJsonComObjectModule : public ATL::CAtlDllModuleT< CJsonComObjectModule >
{
public :
	DECLARE_LIBID(LIBID_JsonComObjectLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_JSONCOMOBJECT, "{e1603bed-3cf1-47a1-9645-c4b29bfb464f}")
};

extern class CJsonComObjectModule _AtlModule;
