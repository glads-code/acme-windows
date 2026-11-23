# acme-windows

acme (port SDL3 de [blakemcbride/Acme](https://github.com/blakemcbride/Acme), dérivé de plan9port) pour Windows x86-64, compilé avec **tcc seulement** : SDL3 réduite (vidéo, rendu logiciel), FreeType, les bibliothèques Plan 9 du port, puis acme. Pas de gcc, de mingw, de MSVC, d'harfbuzz ni de vrai fontconfig.

Compilation sous Windows, avec le tcc natif (`C:\tcc-dev\tcc.exe`, voir le README de rcsh) et les outils de 9pm, sans make ni CMake :

    rcsh compiler.rc < NUL

Résultat : `dist/acme.exe`, seul fichier à copier (environ 10 minutes dans la VM, un cœur). Prérequis en plus de 9pm : `curl`, `git` (pour `git apply`) et le `tar` de Windows.

État : compilé et lancé dans la VM Windows 11 (il démarre et tourne sans erreur), pas encore vu à l'écran : une session ssh n'affiche aucune fenêtre.

## Contenu

- `compiler.rc` : télécharge les sources (empreintes SHA-256), produit les listes d'imports des DLL Windows (`tcc -impdef`), puis compile SDL3, FreeType, les bibliothèques Plan 9 et acme.
- `sdl/` : ce que CMake génère pour SDL3 avec cette configuration, figé : `SDL_build_config.h`, `SDL_revision.h`, et la liste des 219 fichiers à compiler (`sources.txt`).
- `acme/bibliotheques.txt` : fichiers de chaque bibliothèque Plan 9 (dossier, archive, source), relevés dans leurs Makefile.
- `sdl3-tcc.patch` : SDL3 sans C++, sans Direct3D, sans OLE (glisser-déposer par `WM_DROPFILES`), `MONITORINFOEXW` à plat pour tcc.
- `acme-tcc.patch` : macro `USED` de `libc.h` pour tcc.
- `compat/sdl/` : en-têtes Windows absents de tcc, réduits à ce que SDL3 utilise ; IME désactivé (`force.h`) ; `sdl-tcc.c` (GameInput indisponible, GUID de la barre des tâches).
- `compat/acme/` : `pthread.h` sur les verrous SRW et le TLS de Windows, `dirent.c`, `consoleapi.h` (ConPTY), `wincrypt.h`, `utime.h`, `plan9-target.h` (racine Plan 9 que réclame lib9), et `fcmini.c` : fontconfig réduit, qui inventorie `C:\Windows\Fonts` avec FreeType (« mono » donne Consolas).
- `latin1.h` : table de composition des caractères, produite par `mklatinkbd -r lib/keyboard` (outil d'Acme).

## Licences

- Nos fichiers (`compiler.rc`, `compat/` sauf `compat/sdl/shobjidl.h`, `acme/bibliotheques.txt`, `sdl/sources.txt`) : domaine public, par la dédicace CC0 1.0 (`LICENSE`). En droit français, on ne peut pas renoncer à tous ses droits d'auteur : CC0 prévoit alors une licence libre sans condition.
- Repris de SDL3, sous licence zlib (avis en tête de chaque fichier) : `compat/sdl/shobjidl.h` (déclaration d'`ITaskbarList3`), `sdl/SDL_build_config.h`, `sdl/SDL_revision.h`, et `sdl3-tcc.patch`, qui modifie SDL3.
- Repris d'Acme et de plan9port, sous leur licence (de type MIT, `LICENSE` d'Acme) : `latin1.h`, produit à partir de `lib/keyboard`, et `acme-tcc.patch`, qui modifie Acme.
- Le binaire `acme.exe` assemble Acme et plan9port (licence de type MIT), SDL3 (zlib), FreeType (licence FreeType : « Portions of this software are copyright © The FreeType Project (www.freetype.org). All rights reserved. ») et le runtime de tcc (LGPL). Une release qui le contient doit joindre ces licences.
