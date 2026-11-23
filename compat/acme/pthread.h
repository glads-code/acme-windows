/* pthread.h minimal pour tcc sous Windows : ce dont se servent lib9 et
 * libthread de plan9port, sur les verrous SRW, les variables de condition
 * et le TLS de Windows (Vista et suivants). Tout est en ligne : pas de
 * bibliothèque à lier en plus. */
#ifndef GLADS_PTHREAD_H
#define GLADS_PTHREAD_H

/* Sans windows.h : il déclarerait Rectangle (GDI), qui entre en conflit
 * avec le Rectangle de draw.h. Les fonctions Windows utiles sont déclarées ici. */
#include <stddef.h>
#include <stdint.h>
#include <process.h>
#include <errno.h>

typedef unsigned long DWORD;
#define WINAPI __stdcall
#ifndef INFINITE
#define INFINITE 0xFFFFFFFF
#endif
#ifndef TLS_OUT_OF_INDEXES
#define TLS_OUT_OF_INDEXES 0xFFFFFFFF
#endif
__declspec(dllimport) DWORD WINAPI TlsAlloc(void);
__declspec(dllimport) void *WINAPI TlsGetValue(DWORD);
__declspec(dllimport) int WINAPI TlsSetValue(DWORD, void *);
__declspec(dllimport) DWORD WINAPI GetCurrentThreadId(void);
__declspec(dllimport) int WINAPI CloseHandle(void *);
__declspec(dllimport) void *WINAPI GetProcessHeap(void);
__declspec(dllimport) void *WINAPI HeapAlloc(void *, DWORD, size_t);
__declspec(dllimport) int WINAPI HeapFree(void *, DWORD, void *);

/* Types de Windows absents des en-têtes de tcc : un pointeur chacun. */
typedef struct { void *p; } pthread_mutex_t;
typedef struct { void *p; } pthread_cond_t;
typedef DWORD pthread_t;
typedef DWORD pthread_key_t;
typedef int pthread_mutexattr_t;
typedef int pthread_condattr_t;
typedef int pthread_attr_t;

#define PTHREAD_MUTEX_INITIALIZER { 0 }
#define PTHREAD_COND_INITIALIZER { 0 }
#define PTHREAD_MUTEX_NORMAL 0
#define PTHREAD_MUTEX_DEFAULT 0

__declspec(dllimport) void WINAPI AcquireSRWLockExclusive(void *);
__declspec(dllimport) void WINAPI ReleaseSRWLockExclusive(void *);
__declspec(dllimport) unsigned char WINAPI TryAcquireSRWLockExclusive(void *);
__declspec(dllimport) int WINAPI SleepConditionVariableSRW(void *, void *, DWORD, unsigned long);
__declspec(dllimport) void WINAPI WakeConditionVariable(void *);
__declspec(dllimport) void WINAPI WakeAllConditionVariable(void *);

static __inline int pthread_mutexattr_init(pthread_mutexattr_t *a) { *a = 0; return 0; }
static __inline int pthread_mutexattr_destroy(pthread_mutexattr_t *a) { return 0; }
static __inline int pthread_mutexattr_settype(pthread_mutexattr_t *a, int t) { *a = t; return 0; }

static __inline int pthread_mutex_init(pthread_mutex_t *m, const pthread_mutexattr_t *a) { m->p = 0; return 0; }
static __inline int pthread_mutex_destroy(pthread_mutex_t *m) { return 0; }
static __inline int pthread_mutex_lock(pthread_mutex_t *m) { AcquireSRWLockExclusive(&m->p); return 0; }
static __inline int pthread_mutex_unlock(pthread_mutex_t *m) { ReleaseSRWLockExclusive(&m->p); return 0; }
static __inline int pthread_mutex_trylock(pthread_mutex_t *m) { return TryAcquireSRWLockExclusive(&m->p) ? 0 : EBUSY; }

static __inline int pthread_cond_init(pthread_cond_t *c, const pthread_condattr_t *a) { c->p = 0; return 0; }
static __inline int pthread_cond_destroy(pthread_cond_t *c) { return 0; }
static __inline int pthread_cond_wait(pthread_cond_t *c, pthread_mutex_t *m) { SleepConditionVariableSRW(&c->p, &m->p, INFINITE, 0); return 0; }
static __inline int pthread_cond_signal(pthread_cond_t *c) { WakeConditionVariable(&c->p); return 0; }
static __inline int pthread_cond_broadcast(pthread_cond_t *c) { WakeAllConditionVariable(&c->p); return 0; }

static __inline int pthread_key_create(pthread_key_t *k, void (*fin)(void *))
{
	/* le destructeur n'est pas appelé (libthread n'en passe pas) */
	DWORD i = TlsAlloc();
	if (i == TLS_OUT_OF_INDEXES)
		return EAGAIN;
	*k = i;
	return 0;
}
static __inline void *pthread_getspecific(pthread_key_t k) { return TlsGetValue(k); }
static __inline int pthread_setspecific(pthread_key_t k, const void *v) { return TlsSetValue(k, (void *)v) ? 0 : EINVAL; }

static __inline pthread_t pthread_self(void) { return GetCurrentThreadId(); }
static __inline int pthread_equal(pthread_t a, pthread_t b) { return a == b; }

typedef struct { void *(*f)(void *); void *a; } glads_pthread_depart;

static unsigned __stdcall glads_pthread_lance(void *p)
{
	glads_pthread_depart d = *(glads_pthread_depart *)p;
	HeapFree(GetProcessHeap(), 0, p);
	d.f(d.a);
	return 0;
}

/* Les fils sont toujours détachés : leur poignée est fermée tout de suite. */
static __inline int pthread_create(pthread_t *t, const pthread_attr_t *attr, void *(*f)(void *), void *a)
{
	glads_pthread_depart *d;
	uintptr_t h;
	unsigned id;

	d = HeapAlloc(GetProcessHeap(), 0, sizeof *d);
	if (d == NULL)
		return EAGAIN;
	d->f = f;
	d->a = a;
	h = _beginthreadex(NULL, 0, glads_pthread_lance, d, 0, &id);
	if (h == 0) {
		HeapFree(GetProcessHeap(), 0, d);
		return EAGAIN;
	}
	CloseHandle((void *)h);
	if (t)
		*t = id;
	return 0;
}
static __inline int pthread_detach(pthread_t t) { return 0; }
static __inline void pthread_exit(void *r) { _endthreadex(0); }

static __inline int pthread_attr_init(pthread_attr_t *a) { *a = 0; return 0; }
static __inline int pthread_attr_destroy(pthread_attr_t *a) { return 0; }
static __inline int pthread_attr_setstacksize(pthread_attr_t *a, size_t n) { return 0; }

#endif
