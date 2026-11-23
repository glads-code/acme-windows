/* mmreg.h minimal pour tcc : formats audio (SDL3 n'en garde que les types) */
#ifndef _MMREG_H
#define _MMREG_H
#ifndef _WAVEFORMATEX_
#define _WAVEFORMATEX_
#pragma pack(push,1)
typedef struct tWAVEFORMATEX {
	WORD wFormatTag; WORD nChannels; DWORD nSamplesPerSec; DWORD nAvgBytesPerSec;
	WORD nBlockAlign; WORD wBitsPerSample; WORD cbSize;
} WAVEFORMATEX, *PWAVEFORMATEX, *LPWAVEFORMATEX;
typedef struct {
	WAVEFORMATEX Format;
	union { WORD wValidBitsPerSample; WORD wSamplesPerBlock; WORD wReserved; } Samples;
	DWORD dwChannelMask; GUID SubFormat;
} WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;
#pragma pack(pop)
#endif
#define WAVE_FORMAT_PCM 1
#define WAVE_FORMAT_IEEE_FLOAT 3
#define WAVE_FORMAT_EXTENSIBLE 0xFFFE
#endif
