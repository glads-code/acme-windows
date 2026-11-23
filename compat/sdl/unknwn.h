/* unknwn.h minimal pour tcc : IUnknown et conventions COM */
#ifndef _UNKNWN_H
#define _UNKNWN_H
#include <windows.h>
#include <basetyps.h>
#ifndef STDMETHODCALLTYPE
#define STDMETHODCALLTYPE __stdcall
#endif
typedef WCHAR OLECHAR;
typedef OLECHAR *BSTR;
WINAPI BSTR SysAllocString(const OLECHAR *);
WINAPI void SysFreeString(BSTR);
#ifndef __IUnknown_INTERFACE_DEFINED__
#define __IUnknown_INTERFACE_DEFINED__
typedef struct IUnknown IUnknown;
typedef IUnknown *LPUNKNOWN;
typedef struct IUnknownVtbl {
	HRESULT (STDMETHODCALLTYPE *QueryInterface)(IUnknown *, REFIID, void **);
	ULONG (STDMETHODCALLTYPE *AddRef)(IUnknown *);
	ULONG (STDMETHODCALLTYPE *Release)(IUnknown *);
} IUnknownVtbl;
struct IUnknown { const IUnknownVtbl *lpVtbl; };
#endif
#endif
