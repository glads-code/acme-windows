/* shellapi.h pour tcc : celui de tcc, plus le glisser-déposer de fichiers */
#ifndef _GLADS_SHELLAPI_H
#define _GLADS_SHELLAPI_H
#include_next <shellapi.h>
DECLARE_HANDLE(HDROP);
WINAPI void DragAcceptFiles(HWND, BOOL);
WINAPI UINT DragQueryFileW(HDROP, UINT, LPWSTR, UINT);
WINAPI UINT DragQueryFileA(HDROP, UINT, LPSTR, UINT);
WINAPI BOOL DragQueryPoint(HDROP, POINT *);
WINAPI void DragFinish(HDROP);
#ifdef UNICODE
#define DragQueryFile DragQueryFileW
#else
#define DragQueryFile DragQueryFileA
#endif
#endif
