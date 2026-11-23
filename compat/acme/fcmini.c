/* fontconfig réduit pour acme sous Windows (voir fontconfig/fontconfig.h).
 * Inventaire : chaque fichier .ttf, .ttc, .otf de C:\Windows\Fonts et du
 * dossier des polices de l'utilisateur, ouvert avec FreeType pour lire son
 * nom PostScript, sa famille et s'il est à chasse fixe.
 * Choix d'une police : la famille demandée si elle existe, sinon Consolas
 * pour « mono » et « monospace », sinon la première police à chasse fixe. */
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "fontconfig/fontconfig.h"

#define NFAM 8

struct FcPattern {
	char *family[NFAM];	/* demandées (motif) ou famille de la police */
	int nfamily;
	char *file;
	char *psname;
	int index;
	int mono;
};

struct FcConfig {
	FcFontSet set;
};

static FcConfig *courante;

static char *
copie(const char *s)
{
	char *r = malloc(strlen(s) + 1);
	if (r)
		strcpy(r, s);
	return r;
}

FcPattern *
FcPatternCreate(void)
{
	return calloc(1, sizeof(FcPattern));
}

void
FcPatternDestroy(FcPattern *p)
{
	int i;

	if (p == NULL)
		return;
	for (i = 0; i < p->nfamily; i++)
		free(p->family[i]);
	free(p->file);
	free(p->psname);
	free(p);
}

FcBool
FcPatternAddString(FcPattern *p, const char *o, const FcChar8 *s)
{
	if (strcmp(o, FC_FAMILY) == 0) {
		if (p->nfamily == NFAM)
			return FcFalse;
		p->family[p->nfamily++] = copie((const char *)s);
	} else if (strcmp(o, FC_FILE) == 0) {
		free(p->file);
		p->file = copie((const char *)s);
	} else if (strcmp(o, FC_POSTSCRIPT_NAME) == 0) {
		free(p->psname);
		p->psname = copie((const char *)s);
	} else
		return FcFalse;
	return FcTrue;
}

FcBool
FcPatternAddInteger(FcPattern *p, const char *o, int v)
{
	if (strcmp(o, FC_INDEX) == 0)
		p->index = v;
	else if (strcmp(o, FC_SPACING) == 0)
		p->mono = v == FC_MONO;
	else
		return FcFalse;
	return FcTrue;
}

FcResult
FcPatternGetString(const FcPattern *p, const char *o, int n, FcChar8 **s)
{
	char *v = NULL;

	if (strcmp(o, FC_FAMILY) == 0) {
		if (n < p->nfamily)
			v = p->family[n];
	} else if (n == 0 && strcmp(o, FC_FILE) == 0)
		v = p->file;
	else if (n == 0 && strcmp(o, FC_POSTSCRIPT_NAME) == 0)
		v = p->psname;
	if (v == NULL)
		return FcResultNoMatch;
	*s = (FcChar8 *)v;
	return FcResultMatch;
}

FcResult
FcPatternGetInteger(const FcPattern *p, const char *o, int n, int *v)
{
	if (n != 0)
		return FcResultNoMatch;
	if (strcmp(o, FC_INDEX) == 0)
		*v = p->index;
	else if (strcmp(o, FC_SPACING) == 0)
		*v = p->mono ? FC_MONO : 0;
	else
		return FcResultNoMatch;
	return FcResultMatch;
}

static void
ajoute(FcFontSet *s, FcPattern *p)
{
	if (s->nfont == s->sfont) {
		int n = s->sfont ? 2 * s->sfont : 256;
		FcPattern **f = realloc(s->fonts, n * sizeof *f);
		if (f == NULL) {
			FcPatternDestroy(p);
			return;
		}
		s->fonts = f;
		s->sfont = n;
	}
	s->fonts[s->nfont++] = p;
}

static int
extension(const char *nom)
{
	const char *e = strrchr(nom, '.');
	return e && (_stricmp(e, ".ttf") == 0 || _stricmp(e, ".ttc") == 0 || _stricmp(e, ".otf") == 0);
}

/* Chaque visage de chaque police du dossier. */
static void
parcours(FcFontSet *s, FT_Library lib, const char *dossier)
{
	char motif[MAX_PATH], chemin[MAX_PATH];
	WIN32_FIND_DATAA d;
	HANDLE h;
	FT_Face face;
	FcPattern *p;
	const char *ps;
	long i, n;

	if (snprintf(motif, sizeof motif, "%s\\*", dossier) >= (int)sizeof motif)
		return;
	h = FindFirstFileA(motif, &d);
	if (h == INVALID_HANDLE_VALUE)
		return;
	do {
		if ((d.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || !extension(d.cFileName))
			continue;
		if (snprintf(chemin, sizeof chemin, "%s\\%s", dossier, d.cFileName) >= (int)sizeof chemin)
			continue;
		n = 1;
		for (i = 0; i < n; i++) {
			if (FT_New_Face(lib, chemin, i, &face) != 0)
				break;
			n = face->num_faces;
			ps = FT_Get_Postscript_Name(face);
			if (ps != NULL && face->family_name != NULL && (p = FcPatternCreate()) != NULL) {
				p->psname = copie(ps);
				p->file = copie(chemin);
				p->family[p->nfamily++] = copie(face->family_name);
				p->index = (int)i;
				p->mono = FT_IS_FIXED_WIDTH(face) != 0;
				ajoute(s, p);
			}
			FT_Done_Face(face);
		}
	} while (FindNextFileA(h, &d));
	FindClose(h);
}

FcConfig *
FcConfigCreate(void)
{
	return calloc(1, sizeof(FcConfig));
}

/* La configuration XML n'est pas lue : les dossiers sont ceux de Windows. */
FcBool
FcConfigParseAndLoadFromMemory(FcConfig *c, const FcChar8 *x, FcBool plainte)
{
	return FcTrue;
}

FcBool
FcConfigBuildFonts(FcConfig *c)
{
	FT_Library lib;
	char dossier[MAX_PATH];
	DWORD n;

	if (FT_Init_FreeType(&lib) != 0)
		return FcFalse;
	n = GetWindowsDirectoryA(dossier, MAX_PATH - 7);
	if (n > 0 && n < MAX_PATH - 7) {
		strcat(dossier, "\\Fonts");
		parcours(&c->set, lib, dossier);
	}
	n = GetEnvironmentVariableA("LOCALAPPDATA", dossier, MAX_PATH - 32);
	if (n > 0 && n < MAX_PATH - 32) {
		strcat(dossier, "\\Microsoft\\Windows\\Fonts");
		parcours(&c->set, lib, dossier);
	}
	FT_Done_FreeType(lib);
	return FcTrue;
}

FcBool
FcConfigSetCurrent(FcConfig *c)
{
	courante = c;
	return FcTrue;
}

FcBool
FcInit(void)
{
	return FcTrue;
}

FcConfig *
FcInitLoadConfigAndFonts(void)
{
	FcConfig *c = FcConfigCreate();

	if (c == NULL || !FcConfigBuildFonts(c))
		return NULL;
	courante = c;
	return c;
}

/* L'appelant libère l'ensemble mais pas la configuration : on rend une copie
 * superficielle dont FcFontSetDestroy ne libère que le tableau. */
FcFontSet *
FcConfigGetFonts(FcConfig *c, FcSetName n)
{
	FcFontSet *s;

	if (c == NULL)
		c = courante;
	if (c == NULL || n != FcSetSystem)
		return NULL;
	s = malloc(sizeof *s);
	if (s == NULL)
		return NULL;
	s->nfont = c->set.nfont;
	s->sfont = c->set.nfont;
	s->fonts = malloc((c->set.nfont + 1) * sizeof *s->fonts);
	if (s->fonts == NULL) {
		free(s);
		return NULL;
	}
	memcpy(s->fonts, c->set.fonts, c->set.nfont * sizeof *s->fonts);
	return s;
}

void
FcFontSetDestroy(FcFontSet *s)
{
	if (s) {
		free(s->fonts);
		free(s);
	}
}

FcBool
FcConfigSubstitute(FcConfig *c, FcPattern *p, FcMatchKind k)
{
	return FcTrue;
}

void
FcDefaultSubstitute(FcPattern *p)
{
}

static FcPattern *
copiepolice(const FcPattern *f)
{
	FcPattern *p = FcPatternCreate();

	if (p == NULL)
		return NULL;
	p->psname = copie(f->psname);
	p->file = copie(f->file);
	p->family[p->nfamily++] = copie(f->family[0]);
	p->index = f->index;
	p->mono = f->mono;
	return p;
}

/* Visage « Regular » de préférence : index 0 et nom PostScript sans tiret. */
static FcPattern *
famille(FcConfig *c, const char *nom, int mono)
{
	FcPattern *trouve = NULL, *f;
	int i;

	for (i = 0; i < c->set.nfont; i++) {
		f = c->set.fonts[i];
		if (_stricmp(f->family[0], nom) != 0 || (mono && !f->mono))
			continue;
		if (trouve == NULL || strchr(f->psname, '-') == NULL)
			trouve = f;
	}
	return trouve;
}

FcPattern *
FcFontMatch(FcConfig *c, FcPattern *p, FcResult *r)
{
	FcPattern *f = NULL;
	int i;

	if (c == NULL)
		c = courante;
	if (c == NULL) {
		*r = FcResultNoMatch;
		return NULL;
	}
	for (i = 0; i < p->nfamily && f == NULL; i++) {
		if (_stricmp(p->family[i], "mono") == 0 || _stricmp(p->family[i], "monospace") == 0)
			f = famille(c, "Consolas", 0);
		else
			f = famille(c, p->family[i], p->mono);
	}
	for (i = 0; i < c->set.nfont && f == NULL; i++)
		if (c->set.fonts[i]->mono)
			f = c->set.fonts[i];
	if (f == NULL && c->set.nfont > 0)
		f = c->set.fonts[0];
	if (f == NULL) {
		*r = FcResultNoMatch;
		return NULL;
	}
	*r = FcResultMatch;
	return copiepolice(f);
}
