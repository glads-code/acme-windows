/* fontconfig réduit, pour acme sous Windows compilé avec tcc : les seules
 * fonctions qu'appellent fontsrv et fontpipe, sur un inventaire des polices
 * de Windows fait avec FreeType (fcmini.c). */
#ifndef GLADS_FONTCONFIG_H
#define GLADS_FONTCONFIG_H

typedef unsigned char FcChar8;
typedef int FcBool;
#define FcTrue 1
#define FcFalse 0

typedef enum { FcResultMatch, FcResultNoMatch, FcResultTypeMismatch, FcResultNoId, FcResultOutOfMemory } FcResult;
typedef enum { FcMatchPattern, FcMatchFont, FcMatchScan } FcMatchKind;
typedef enum { FcSetSystem = 0, FcSetApplication = 1 } FcSetName;

#define FC_FAMILY "family"
#define FC_FILE "file"
#define FC_INDEX "index"
#define FC_POSTSCRIPT_NAME "postscriptname"
#define FC_SPACING "spacing"
#define FC_MONO 100

typedef struct FcPattern FcPattern;
typedef struct FcConfig FcConfig;
typedef struct FcFontSet { int nfont; int sfont; FcPattern **fonts; } FcFontSet;

FcBool FcInit(void);
FcConfig *FcInitLoadConfigAndFonts(void);
FcConfig *FcConfigCreate(void);
FcBool FcConfigParseAndLoadFromMemory(FcConfig *, const FcChar8 *, FcBool);
FcBool FcConfigBuildFonts(FcConfig *);
FcBool FcConfigSetCurrent(FcConfig *);
FcFontSet *FcConfigGetFonts(FcConfig *, FcSetName);
void FcFontSetDestroy(FcFontSet *);

FcPattern *FcPatternCreate(void);
void FcPatternDestroy(FcPattern *);
FcBool FcPatternAddString(FcPattern *, const char *, const FcChar8 *);
FcBool FcPatternAddInteger(FcPattern *, const char *, int);
FcResult FcPatternGetString(const FcPattern *, const char *, int, FcChar8 **);
FcResult FcPatternGetInteger(const FcPattern *, const char *, int, int *);
FcBool FcConfigSubstitute(FcConfig *, FcPattern *, FcMatchKind);
void FcDefaultSubstitute(FcPattern *);
FcPattern *FcFontMatch(FcConfig *, FcPattern *, FcResult *);

#endif
