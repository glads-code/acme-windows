/* setupapi.h minimal pour tcc : énumération des périphériques (noms des claviers et souris) */
#ifndef _SETUPAPI_H
#define _SETUPAPI_H
typedef PVOID HDEVINFO;
typedef struct _SP_DEVINFO_DATA { DWORD cbSize; GUID ClassGuid; DWORD DevInst; ULONG_PTR Reserved; } SP_DEVINFO_DATA, *PSP_DEVINFO_DATA;
#define DIGCF_PRESENT 0x02
#define DIGCF_ALLCLASSES 0x04
#define SPDRP_DEVICEDESC 0x00
#define SPDRP_MFG 0x0B
#define SPDRP_FRIENDLYNAME 0x0C
WINAPI HDEVINFO SetupDiGetClassDevsA(const GUID *, PCSTR, HWND, DWORD);
WINAPI BOOL SetupDiEnumDeviceInfo(HDEVINFO, DWORD, PSP_DEVINFO_DATA);
WINAPI BOOL SetupDiGetDeviceInstanceIdA(HDEVINFO, PSP_DEVINFO_DATA, PSTR, DWORD, PDWORD);
WINAPI BOOL SetupDiGetDeviceRegistryPropertyW(HDEVINFO, PSP_DEVINFO_DATA, DWORD, PDWORD, PBYTE, DWORD, PDWORD);
WINAPI BOOL SetupDiDestroyDeviceInfoList(HDEVINFO);
#endif
