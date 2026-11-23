/* wincrypt.h minimal pour tcc : aléa de CryptGenRandom (truerand de lib9) */
#ifndef _WINCRYPT_H
#define _WINCRYPT_H
typedef ULONG_PTR HCRYPTPROV;
#define PROV_RSA_FULL 1
#define CRYPT_VERIFYCONTEXT 0xF0000000
WINAPI BOOL CryptAcquireContextA(HCRYPTPROV *, LPCSTR, LPCSTR, DWORD, DWORD);
WINAPI BOOL CryptGenRandom(HCRYPTPROV, DWORD, BYTE *);
WINAPI BOOL CryptReleaseContext(HCRYPTPROV, DWORD);
#endif
