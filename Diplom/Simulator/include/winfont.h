/***************************************************************************************\
*   File:                                                                               *
*       winfont.h                                                                       *
*                                                                                       *
*   Abstract:                                                                           *
*       window font resource file definition                                            *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 15-Apr-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 50 $
#ifndef __WINFONT_H__
#define __WINFONT_H__
//
// Define OEM bitmapped font check values.
//

#define OEM_FONT_VERSION 0x200
#define OEM_FONT_TYPE 0
#define OEM_FONT_ITALIC 0
#define OEM_FONT_UNDERLINE 0
#define OEM_FONT_STRIKEOUT 0
#define OEM_FONT_CHARACTER_SET 255
#define OEM_FONT_FAMILY (3 << 4)

#pragma pack(push)
#pragma pack(1)
typedef struct {
    WORD    Version;          // Always 17985 for the Nonce
    DWORD   Size;             // Size of whole file
    char    Copyright[60];
    WORD    Type;             // Raster Font if Type & 1 == 0
    WORD    Points;           // Nominal Point size
    WORD    VertRes;          // Nominal Vertical resolution
    WORD    HorizRes;         // Nominal Horizontal resolution
    WORD    Ascent;           // Height of Ascent
    WORD    IntLeading;       // Internal (Microsoft) Leading
    WORD    ExtLeading;       // External (Microsoft) Leading
    BYTE    Italic;           // Italic font if set
    BYTE    Underline;        // Etc.
    BYTE    StrikeOut;        // Etc.
    WORD    Weight;           // Weight: 200 = regular
    BYTE    CharSet;          // ANSI=0. other=255
    WORD    PixelWidth;         // Fixed width. 0 ==> Variable
    WORD    PixelHeight;        // Fixed Height
    BYTE    Family;           // Pitch and Family
    WORD    AvgWidth;         // Width of character 'X'
    WORD    MaxWidth;         // Maximum width
    BYTE    FirstCharacter;   // First character defined in font
    BYTE    LastCharacter;    // Last character defined in font
    BYTE    DefaultCharacter; // Sub. for out of range chars.
    BYTE    BreakCharacter;   // Word Break Character
    WORD    WidthBytes;       // No.Bytes/row of Bitmap
    DWORD   Device;           // Pointer to Device Name string
    DWORD   Face;             // Pointer to Face Name String
    DWORD   BitsPointer;        // Pointer to Bit Map
    DWORD   BitsOffset;     // Offset to Bit Map
    UCHAR   filler;
    } OEM_FONT_FILE_HEADER, *POEM_FONT_FILE_HEADER;       // Above pointers all rel. to start of file

typedef struct _FONT_MAP {
    USHORT Width;
    USHORT Offset;
} FONT_MAP, *PFONT_MAP;

// the original 3.0 header:

typedef struct {
    WORD    Version;          // Always 17985 for the Nonce
    DWORD   Size;             // Size of whole file
    char    Copyright[60];
    WORD    fsType;           // Type field for the font
    WORD    fsPoints;         // Point size of font
    WORD    fsVertRes;        // Vertical digitization
    WORD    fsHorizRes;       // Horizontal digitization
    WORD    fsAscent;         // Baseline offset from char cell top
    WORD    fsInternalLeading;    // Internal leading included in font
    WORD    fsExternalLeading;    // Prefered extra space between lines
    BYTE    fsItalic;         // Flag specifying if italic
    BYTE    fsUnderline;          // Flag specifying if underlined
    BYTE    fsStrikeOut;          // Flag specifying if struck out
    WORD    fsWeight;         // Weight of font
    BYTE    fsCharSet;        // Character set of font
    WORD    fsPixWidth;       // Width field for the font
    WORD    fsPixHeight;          // Height field for the font
    BYTE    fsPitchAndFamily;     // Flag specifying pitch and family
    WORD    fsAvgWidth;       // Average character width
    WORD    fsMaxWidth;       // Maximum character width
    BYTE    fsFirstChar;          // First character in the font
    BYTE    fsLastChar;       // Last character in the font
    BYTE    fsDefaultChar;        // Default character for out of range
    BYTE    fsBreakChar;          // Character to define wordbreaks
    WORD    fsWidthBytes;         // Number of bytes in each row
    DWORD   fsDevice;         // Offset to device name
    DWORD   fsFace;           // Offset to face name
    DWORD   fsBitsPointer;        // Bits pointer
    DWORD   fsBitsOffset;         // Offset to the begining of the bitmap
    BYTE    fsDBfiller;       // Word alignment for the offset table

    DWORD   fsFlags;          // Bit flags
    WORD    fsAspace;         // Global A space, if any
    WORD    fsBspace;         // Global B space, if any
    WORD    fsCspace;         // Global C space, if any
    DWORD   fsColorPointer;       // offset to color table, if any
    DWORD   fsReserved[4];        //
    BYTE    fsCharOffset;         // Area for storing the char. offsets

    } OEM_FONT_FILE_HEADER30;

typedef struct tagFFH {
    WORD        fhVersion        ;
    DWORD       fhSize           ;
    char        fhCopyright[60]  ;
    WORD        fhType           ;
    WORD        fhPoints         ;
    WORD        fhVertRes        ;
    WORD        fhHorizRes       ;
    WORD        fhAscent         ;
    WORD        fhInternalLeading;
    WORD        fhExternalLeading;
    BYTE        fhItalic         ;
    BYTE        fhUnderline      ;
    BYTE        fhStrikeOut      ;
    WORD        fhWeight         ;
    BYTE        fhCharSet        ;
    WORD        fhPixWidth       ;
    WORD        fhPixHeight      ;
    BYTE        fhPitchAndFamily ;
    WORD        fhAvgWidth       ;
    WORD        fhMaxWidth       ;
    BYTE        fhFirstChar      ;
    BYTE        fhLastChar       ;
    BYTE        fhDefaultChar    ;
    BYTE        fhBreakChar      ;
    WORD        fhWidthBytes     ;
    DWORD       fhDevice         ;
    DWORD       fhFace           ;
    DWORD       fhBitsPointer    ;
    } FFH;
#pragma pack(pop)

#endif // __WINFONT_H__