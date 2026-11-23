/* imm.h minimal pour tcc (IME désactivé dans SDL3) */
#ifndef _IMM_H
#define _IMM_H
typedef HANDLE HIMC;
typedef HANDLE HIMCC;
typedef struct tagCOMPOSITIONFORM { DWORD dwStyle; POINT ptCurrentPos; RECT rcArea; } COMPOSITIONFORM, *LPCOMPOSITIONFORM;
typedef struct tagCANDIDATEFORM { DWORD dwIndex; DWORD dwStyle; POINT ptCurrentPos; RECT rcArea; } CANDIDATEFORM, *LPCANDIDATEFORM;
#define CFS_RECT 1
#define CFS_POINT 2
#define CFS_EXCLUDE 128
WINAPI BOOL ImmSetCompositionWindow(HIMC, LPCOMPOSITIONFORM);
WINAPI BOOL ImmSetCandidateWindow(HIMC, LPCANDIDATEFORM);
WINAPI HIMC ImmGetContext(HWND);
WINAPI BOOL ImmReleaseContext(HWND, HIMC);
WINAPI HIMC ImmAssociateContext(HWND, HIMC);
#endif
