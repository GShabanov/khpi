/***************************************************************************************\
*   File:                                                                               *
*       utilites.h                                                                      *
*                                                                                       *
*   Abstract:                                                                           *
*       different utilites for parsing files                                            *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 4-Jan-2014                        *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 67 $

#ifndef __UTILITES_H__
#define __UTILITES_H__

typedef struct _DATA_HANDLE {
    PVOID    data;
    SIZE_T   size;
    BOOL     moreData;
} DATA_HANDLE, *PDATA_HANDLE;

typedef struct _LOAD_FILE_DESCRIPTOR {
    //
    // private fields
    //
    BOOL    bInitialized;
    PVOID   pPhysicalBuffer;
    SIZE_T  tBufferPysicalSize;
    LONG    lRefCount;

    //
    //  public fields
    //
    DATA_HANDLE      handle;
} LOAD_FILE_DESCRIPTOR, *PLOAD_FILE_DESCRIPTOR;


/*extern VOID
IoSubsystemInit();

extern BOOL
IoLoadDataFile(IN const TCHAR *tcFileName, DATA_HANDLE **dataHandle);

extern VOID
IoClose(DATA_HANDLE *dataHandle);

extern VOID
IoSubsystemFree();*/


typedef struct _GFX_INFO {
    unsigned char *pic;     // data
    COLORREF       bgColor;
    unsigned int   width;
    unsigned int   height;
} GFX_INFO, *PGFX_INFO;

typedef struct _FONT_INFO {
    ULONG PixelWidth;
    ULONG PixelHeight;
    UCHAR FirstCharacter;
    UCHAR LastCharacter;
    ULONG charCount;
    ULONG BytesPerChar;
    unsigned char *glyphData;
} FONT_INFO, *PFONT_INFO;


typedef struct _SFX_INFO {
    struct sndBuffer *soundData;
    bool              soundCompressed;        // not in Memory
    int               soundCompressionMethod; 
    int               soundLength;
} SFX_INFO, *PSFX_INFO;

extern BOOL
Load_Font(class IDataStream *pStream, unsigned int preferedWidth, struct _FONT_INFO *fontInfo);

extern BOOL
Load_WAV(unsigned char *fbuffer, unsigned int len, struct _SFX_INFO *sfxInfo);

extern BOOL
Load_Picture(class IDataStream *pStream, struct _GFX_INFO *gfxInfo);

#endif // __UTILITES_H_