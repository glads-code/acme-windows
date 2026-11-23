/* Ce que SDL3 tire de ses fichiers C++ et des en-têtes Windows complets,
 * absents avec tcc : GameInput indisponible (comme SDL sans gameinput.h),
 * et les GUID du bouton de la barre des tâches. */
#include <windows.h>
#include <objbase.h>
#include <stdbool.h>

const GUID CLSID_TaskbarList = { 0x56fdf344, 0xfd6d, 0x11d0, { 0x95, 0x8a, 0x00, 0x60, 0x97, 0xc9, 0xa0, 0x90 } };
const GUID IID_ITaskbarList3 = { 0xea1afb91, 0x9e28, 0x4b86, { 0x90, 0xe9, 0x9e, 0x9f, 0x8a, 0x5e, 0xef, 0xaf } };

typedef struct SDL_VideoDevice SDL_VideoDevice;
extern bool SDL_SetError(const char *fmt, ...);
bool WIN_InitGameInput(SDL_VideoDevice *v) { return SDL_SetError("That operation is not supported"); }
bool WIN_UpdateGameInputEnabled(SDL_VideoDevice *v) { return SDL_SetError("That operation is not supported"); }
void WIN_UpdateGameInput(SDL_VideoDevice *v) { }
void WIN_QuitGameInput(SDL_VideoDevice *v) { }
