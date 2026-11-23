/* windowsx.h minimal pour tcc : lecture des messages souris, style des fenêtres */
#ifndef _WINDOWSX_H
#define _WINDOWSX_H
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#define GetWindowStyle(h) ((DWORD)GetWindowLong(h, GWL_STYLE))
#define GetWindowExStyle(h) ((DWORD)GetWindowLong(h, GWL_EXSTYLE))
#endif
