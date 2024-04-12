/***************************************************************************************\
*   File:                                                                               *
*       waveFormat.h                                                                    *
*                                                                                       *
*   Abstract:                                                                           *
*       format for exist memory copy files                                              *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 10-Feb-2014                        *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 33 $
/*
===================================================================================

    General extended waveform format structure.
    Use this for all NON PCM formats.

===================================================================================
*/
#pragma pack(push, 1)
typedef struct _waveformatex {
    WORD    wFormatTag;        /* format type */
    WORD    nChannels;         /* number of channels (i.e. mono, stereo...) */
    DWORD   nSamplesPerSec;    /* sample rate */
    DWORD   nAvgBytesPerSec;   /* for buffer estimation */
    WORD    nBlockAlign;       /* block size of data */
    WORD    wBitsPerSample;    /* Number of bits per sample of mono data */
    WORD    cbSize;            /* The count in bytes of the size of
                               /* extra information (after cbSize) */
} waveformatex, *pwaveformatex;

/* OLD general waveform format structure (information common to all formats) */
typedef struct _waveformat {
    WORD    wFormatTag;        /* format type */
    WORD    nChannels;         /* number of channels (i.e. mono, stereo, etc.) */
    DWORD   nSamplesPerSec;    /* sample rate */
    DWORD   nAvgBytesPerSec;   /* for buffer estimation */
    WORD    nBlockAlign;       /* block size of data */
} waveformat, *pwaveformat;

/* flags for wFormatTag field of WAVEFORMAT */
enum {
    WAVE_FORMAT_TAG_PCM     = 1,
    WAVE_FORMAT_TAG_OGG     = 2
};

/* specific waveform format structure for PCM data */
typedef struct _pcmwaveformat {
    waveformat      wf;
    WORD            wBitsPerSample;
} pcmwaveformat, *ppcmwaveformat;

#ifndef mmioFOURCC
#define mmioFOURCC( ch0, ch1, ch2, ch3 )                \
        ( (dword)(byte)(ch0) | ( (dword)(byte)(ch1) << 8 ) |    \
        ( (dword)(byte)(ch2) << 16 ) | ( (dword)(byte)(ch3) << 24 ) )
#endif

#define fourcc_riff     mmioFOURCC('R', 'I', 'F', 'F')

typedef struct _waveformatextensible {
    waveformatex      Format;
    union {
        WORD wValidBitsPerSample;       /* bits of precision  */
        WORD wSamplesPerBlock;          /* valid if wBitsPerSample==0*/
        WORD wReserved;                 /* If neither applies, set to zero*/
    } Samples;
    DWORD           dwChannelMask;      /* which channels are */
                                        /* present in stream  */
    int            SubFormat;
} waveformatextensible, *pwaveformatextensible;

typedef DWORD fourcc;

/* RIFF chunk information data structure */
typedef struct _mminfo {
    fourcc          ckid;           /* chunk ID */
    DWORD           cksize;         /* chunk size */
    fourcc          fccType;        /* form type or list type */
    DWORD           dwDataOffset;   /* offset of data portion of chunk */
} mminfo, *pmminfo;

#pragma pack(pop)
