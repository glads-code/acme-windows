/* objbase.h minimal pour tcc : initialisation COM */
#ifndef _OBJBASE_H
#define _OBJBASE_H
#include <unknwn.h>
typedef enum tagCOINIT { COINIT_APARTMENTTHREADED = 2, COINIT_MULTITHREADED = 0,
	COINIT_DISABLE_OLE1DDE = 4, COINIT_SPEED_OVER_MEMORY = 8 } COINIT;
#ifndef CLSCTX_INPROC_SERVER
#define CLSCTX_INPROC_SERVER 1
#define CLSCTX_ALL 23
#endif
extern const GUID CLSID_TaskbarList, IID_ITaskbarList3;
WINAPI HRESULT CoInitializeEx(LPVOID, DWORD);
WINAPI void CoUninitialize(void);
WINAPI HRESULT CoCreateInstance(REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID *);
WINAPI void CoTaskMemFree(LPVOID);
WINAPI LPVOID CoTaskMemAlloc(SIZE_T);
#endif
