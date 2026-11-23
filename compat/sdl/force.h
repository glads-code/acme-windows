/* en-tête forcé pour SDL3 avec tcc */
#define SDL_DISABLE_WINDOWS_IME 1
/* constantes de winuser.h absentes des en-têtes de tcc */
#define MAPVK_VK_TO_VSC 0
#define MAPVK_VSC_TO_VK 1
#define MAPVK_VK_TO_CHAR 2
#define MAPVK_VSC_TO_VK_EX 3
#define MAPVK_VK_TO_VSC_EX 4
