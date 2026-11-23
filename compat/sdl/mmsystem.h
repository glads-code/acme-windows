/* mmsystem.h minimal pour tcc : résolution du minuteur */
#ifndef _MMSYSTEM_H
#define _MMSYSTEM_H
typedef UINT MMRESULT;
WINAPI MMRESULT timeBeginPeriod(UINT);
WINAPI MMRESULT timeEndPeriod(UINT);
WINAPI DWORD timeGetTime(void);
#endif
