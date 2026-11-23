# acme-windows

acme (port SDL3 de [blakemcbride/Acme](https://github.com/blakemcbride/Acme), dérivé de plan9port) pour Windows x86-64, compilé avec **tcc seulement** : SDL3 réduite (vidéo, rendu logiciel), FreeType, les bibliothèques Plan 9 du port, puis acme. Pas de gcc, de mingw, de MSVC, d'harfbuzz ni de vrai fontconfig.

État : `./compiler.rc` produit `dist/acme.exe` (tcc croisé, sur deb). acme démarre dans la VM Windows 11 sans erreur, mais il n'a pas encore été vu à l'écran (pas de session graphique). La compilation sous Windows avec le tcc natif reste à faire.

## Contenu

- `compiler.rc` : télécharge les sources (empreintes SHA-256), construit le tcc croisé, puis tout le reste.
- `sdl3-tcc.patch` : SDL3 sans C++, sans Direct3D, sans OLE (glisser-déposer par `WM_DROPFILES`), `MONITORINFOEXW` à plat pour tcc.
- `acme-tcc.patch` : macro `USED` de `libc.h` pour tcc.
- `compat/sdl/` : en-têtes Windows absents de tcc, réduits à ce que SDL3 utilise ; IME désactivé (`force.h`) ; `sdl-tcc.c` (GameInput indisponible, GUID de la barre des tâches).
- `compat/acme/` : `pthread.h` sur les verrous SRW et le TLS de Windows, `dirent.c`, `consoleapi.h` (ConPTY), `wincrypt.h`, `utime.h`, et `fcmini.c` : fontconfig réduit, qui inventorie `C:\Windows\Fonts` avec FreeType (« mono » donne Consolas).
- `def/` : listes d'imports des DLL Windows, produites par `tcc -impdef` sur les DLL de Windows 11.
- `latin1.h` : table de composition des caractères, produite par `mklatinkbd -r lib/keyboard` (outil d'Acme).
