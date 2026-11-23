/* opendir et compagnie pour tcc sous Windows, avec la structure DIR que
 * déclare le dirent.h de tcc (celle de mingw). La poignée de _findfirst
 * (un pointeur en 64 bits) ne tient pas dans dd_handle (long) : elle est
 * rangée après dd_name, dans la place allouée en plus. */
#include <dirent.h>
#include <io.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
	intptr_t h;
} Suite;

static Suite *
suite(DIR *d)
{
	size_t n = strlen(d->dd_name) + 1;
	return (Suite *)((char *)d + ((offsetof(DIR, dd_name) + n + 15) & ~(size_t)15));
}

DIR *
opendir(const char *nom)
{
	DIR *d;
	size_t n;

	if (nom == NULL || nom[0] == '\0') {
		errno = ENOENT;
		return NULL;
	}
	n = strlen(nom);
	d = calloc(1, sizeof(DIR) + n + 3 + 16 + sizeof(Suite));
	if (d == NULL)
		return NULL;
	strcpy(d->dd_name, nom);
	if (n > 0 && d->dd_name[n - 1] != '/' && d->dd_name[n - 1] != '\\')
		strcat(d->dd_name, "\\");
	strcat(d->dd_name, "*");
	d->dd_handle = -1;
	d->dd_stat = 0;
	suite(d)->h = -1;
	/* vérifie tout de suite que le dossier existe */
	suite(d)->h = _findfirst(d->dd_name, &d->dd_dta);
	if (suite(d)->h == -1) {
		free(d);
		return NULL;
	}
	d->dd_handle = 0;
	return d;
}

struct dirent *
readdir(DIR *d)
{
	if (d->dd_stat < 0)
		return NULL;
	if (d->dd_stat > 0 || suite(d)->h == -1) {
		if (suite(d)->h == -1)
			suite(d)->h = _findfirst(d->dd_name, &d->dd_dta);
		else if (_findnext(suite(d)->h, &d->dd_dta) != 0) {
			_findclose(suite(d)->h);
			suite(d)->h = -1;
			d->dd_stat = -1;
			return NULL;
		}
		if (suite(d)->h == -1) {
			d->dd_stat = -1;
			return NULL;
		}
	}
	d->dd_stat++;
	d->dd_dir.d_name = d->dd_dta.name;
	d->dd_dir.d_namlen = (unsigned short)strlen(d->dd_dta.name);
	return &d->dd_dir;
}

int
closedir(DIR *d)
{
	if (d == NULL)
		return -1;
	if (suite(d)->h != -1)
		_findclose(suite(d)->h);
	free(d);
	return 0;
}

void
rewinddir(DIR *d)
{
	if (suite(d)->h != -1)
		_findclose(suite(d)->h);
	suite(d)->h = -1;
	d->dd_stat = 0;
}

long
telldir(DIR *d)
{
	return d->dd_stat;
}

void
seekdir(DIR *d, long pos)
{
	if (pos < 0) {
		rewinddir(d);
		d->dd_stat = -1;
		return;
	}
	rewinddir(d);
	while (d->dd_stat < pos && readdir(d) != NULL)
		;
}
