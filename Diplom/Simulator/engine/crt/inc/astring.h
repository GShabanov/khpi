/***************************************************************************************\
*   File:                                                                               *
*       astring.h                                                                       *
*                                                                                       *
*   Abstract:                                                                           *
*       string work routines                                                            *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 1-Feb-2015                        *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/

#ifndef __ASTRING_H__
#define __ASTRING_H__

/*
===============================================================================

    Character string

===============================================================================
*/

// these library functions should not be used for cross platform compatibility
#define strcmp          AString::Cmp      // use_idStr_Cmp
#define strncmp         use_idStr_Cmpn

#if defined( StrCmpN )
#undef StrCmpN
#endif
#define StrCmpN         use_idStr_Cmpn

#if defined( strcmpi )
#undef strcmpi
#endif
#define strcmpi         use_idStr_Icmp

#if defined( StrCmpI )
#undef StrCmpI
#endif
#define StrCmpI         use_idStr_Icmp

#if defined( StrCmpNI )
#undef StrCmpNI
#endif
#define StrCmpNI        use_idStr_Icmpn

#define stricmp         AString::Icmp     // use_idStr_Icmp
#define _stricmp        use_idStr_Icmp
#define strcasecmp      use_idStr_Icmp
#define strnicmp        use_idStr_Icmpn
#define _strnicmp       use_idStr_Icmpn
#define _memicmp        use_idStr_Icmpn
#define snprintf        use_idStr_snPrintf
#define _snprintf       use_idStr_snPrintf
#define vsnprintf       use_idStr_vsnPrintf
#define _vsnprintf      use_idStr_vsnPrintf

class idVec4;


// make AString a multiple of 16 bytes long
// don't make too large to keep memory requirements to a minimum
const int STR_ALLOC_BASE            = 20;
const int STR_ALLOC_GRAN            = 32;

typedef enum {
    MEASURE_SIZE = 0,
    MEASURE_BANDWIDTH
} Measure_t;

class AString {

public:
                  AString(void );
                  AString(const AString &text);
                  AString(const AString &text, int start, int end);
                  AString(const char *text);
                  AString(const char *text, int start, int end);
                  explicit AString(const bool b);
                  explicit AString(const char c);
                  explicit AString(const int i);
                  explicit AString(const unsigned u);
                  explicit AString(const float f);
                  ~AString( void );

    size_t        Size(void) const;
    const char *  c_str(void) const;
    operator      const char *(void) const;
    operator      const char *(void );

    char          operator[]( int index ) const;
    char &        operator[]( int index );

    const char *  operator=( const AString &text);
    void          operator=( const char *text );

    friend AString  operator+( const AString &a, const AString &b );
    friend AString  operator+( const AString &a, const char *b );
    friend AString  operator+( const char *a, const AString &b );

    friend AString  operator+( const AString &a, const float b );
    friend AString  operator+( const AString &a, const int b );
    friend AString  operator+( const AString &a, const unsigned b );
    friend AString  operator+( const AString &a, const bool b );
    friend AString  operator+( const AString &a, const char b );

    AString &       operator+=( const AString &a );
    AString &       operator+=( const char *a );
    AString &       operator+=( const float a );
    AString &       operator+=( const char a );
    AString &       operator+=( const int a );
    AString &       operator+=( const unsigned a );
    AString &       operator+=( const bool a );

                  // case sensitive compare
    friend bool   operator==( const AString &a, const AString &b );
    friend bool   operator==( const AString &a, const char *b );
    friend bool   operator==( const char *a, const AString &b );

                  // case sensitive compare
    friend bool   operator!=( const AString &a, const AString &b );
    friend bool   operator!=( const AString &a, const char *b );
    friend bool   operator!=( const char *a, const AString &b );

                  // case sensitive compare
    int           Cmp( const char *text ) const;
    int           Cmpn( const char *text, int n ) const;
    int           CmpPrefix( const char *text ) const;

                  // case insensitive compare
    int           Icmp( const char *text ) const;
    int           Icmpn( const char *text, int n ) const;
    int           IcmpPrefix( const char *text ) const;

                  // case insensitive compare ignoring color
    int           IcmpNoColor( const char *text ) const;

                  // compares paths and makes sure folders come first
    int           IcmpPath( const char *text ) const;
    int           IcmpnPath( const char *text, int n ) const;
    int           IcmpPrefixPath( const char *text ) const;

    int           Length( void ) const;
    int           Allocated( void ) const;
    void          Empty( void );
    bool          IsEmpty( void ) const;
    void          Clear( void );
    void          Append( const char a );
    void          Append( const AString &text );
    void          Append( const char *text );
    void          Append( const char *text, int len );
    void          Insert( const char a, int index );
    void          Insert( const char *text, int index );
    void          ToLower( void );
    void          ToUpper( void );
    bool          IsNumeric( void ) const;
    bool          IsColor( void ) const;
    bool          HasLower( void ) const;
    bool          HasUpper( void ) const;
    int           LengthWithoutColors( void ) const;
    AString &       RemoveColors( void );
    void          CapLength( int );
    void          Fill( const char ch, int newlen );

    int           Find( const char c, int start = 0, int end = -1 ) const;
    int           Find( const char *text, bool casesensitive = true, int start = 0, int end = -1 ) const;
    bool          Filter( const char *filter, bool casesensitive ) const;
    int           Last( const char c ) const;                     // return the index to the last occurance of 'c', returns -1 if not found
    const char *  Left( int len, AString &result ) const;           // store the leftmost 'len' characters in the result
    const char *  Right( int len, AString &result ) const;          // store the rightmost 'len' characters in the result
    const char *  Mid( int start, int len, AString &result ) const; // store 'len' characters starting at 'start' in result
    AString       Left( int len ) const;                          // return the leftmost 'len' characters
    AString       Right( int len ) const;                         // return the rightmost 'len' characters
    AString       Mid( int start, int len ) const;                // return 'len' characters starting at 'start'
    void          StripLeading( const char c );                   // strip char from front as many times as the char occurs
    void          StripLeading( const char *string );             // strip string from front as many times as the string occurs
    bool          StripLeadingOnce( const char *string );         // strip string from front just once if it occurs
    void          StripTrailing( const char c );                  // strip char from end as many times as the char occurs
    void          StripTrailing( const char *string );            // strip string from end as many times as the string occurs
    bool          StripTrailingOnce( const char *string );        // strip string from end just once if it occurs
    void          Strip( const char c );                          // strip char from front and end as many times as the char occurs
    void          Strip( const char *string );                    // strip string from front and end as many times as the string occurs
    void          StripTrailingWhitespace( void );                // strip trailing white space characters
    AString &     StripQuotes( void );                            // strip quotes around string
    void          Replace( const char *old, const char *nw );

    // file name methods
    int           FileNameHash( void ) const;                     // hash key for the filename (skips extension)
    AString &     BackSlashesToSlashes( void );                   // convert slashes
    AString &     SetFileExtension( const char *extension );      // set the given file extension
    AString &     StripFileExtension( void );                     // remove any file extension
    AString &     StripAbsoluteFileExtension( void );             // remove any file extension looking from front (useful if there are multiple .'s)
    AString &     DefaultFileExtension( const char *extension );  // if there's no file extension use the default
    AString &     DefaultPath( const char *basepath );            // if there's no path use the default
    void          AppendPath( const char *text );                 // append a partial path
    AString &     StripFilename( void );                          // remove the filename from a path
    AString &     StripPath( void );                              // remove the path from the filename
    void          ExtractFilePath( AString &dest ) const;           // copy the file path to another string
    void          ExtractFileName( AString &dest ) const;           // copy the filename to another string
    void          ExtractFileBase( AString &dest ) const;           // copy the filename minus the extension to another string
    void          ExtractFileExtension( AString &dest ) const;      // copy the file extension to another string
    bool          CheckExtension( const char *ext );

    // char * methods to replace library functions
    static int    Length( const char *s );
    static char * ToLower( char *s );
    static char * ToUpper( char *s );
    static bool   IsNumeric( const char *s );
    static bool   HasLower( const char *s );
    static bool   HasUpper( const char *s );
    static int    LengthWithoutColors( const char *s );
    static char * RemoveColors( char *s );
    static int    Cmp( const char *s1, const char *s2 );
    static int    Cmpn( const char *s1, const char *s2, int n );
    static int    Icmp( const char *s1, const char *s2 );
    static int    Icmpn( const char *s1, const char *s2, int n );
    static int    IcmpNoColor( const char *s1, const char *s2 );
    static int    IcmpPath( const char *s1, const char *s2 );         // compares paths and makes sure folders come first
    static int    IcmpnPath( const char *s1, const char *s2, int n ); // compares paths and makes sure folders come first
    static void   Append( char *dest, int size, const char *src );
    static void   Copynz( char *dest, const char *src, int destsize );
    static int    snPrintf( char *dest, int size, const char *fmt, ... ); // id_attribute((format(printf,3,4)));
    static int    vsnPrintf( char *dest, int size, const char *fmt, va_list argptr );
    static int    FindChar( const char *str, const char c, int start = 0, int end = -1 );
    static int    FindText( const char *str, const char *text, bool casesensitive = true, int start = 0, int end = -1 );
    static bool   Filter( const char *filter, const char *name, bool casesensitive );
    static void   StripMediaName( const char *name, AString &mediaName );
    static bool   CheckExtension( const char *name, const char *ext );
    static const char * FloatArrayToString( const float *array, const int length, const int precision );

    // hash keys
    static int    Hash( const char *string );
    static int    Hash( const char *string, int length );
    static int    IHash( const char *string );                    // case insensitive
    static int    IHash( const char *string, int length );        // case insensitive

    // character methods
    static char   ToLower( char c );
    static char   ToUpper( char c );
    static bool   CharIsPrintable( int c );
    static bool   CharIsLower( int c );
    static bool   CharIsUpper( int c );
    static bool   CharIsAlpha( int c );
    static bool   CharIsNumeric( int c );
    static bool   CharIsNewLine( char c );
    static bool   CharIsTab( char c );
    static int    ColorIndex( int c );

    friend int    sprintf( AString &dest, const char *fmt, ... );
    friend int    vsprintf( AString &dest, const char *fmt, va_list ap );

    void          ReAllocate( int amount, bool keepold );             // reallocate string data buffer
    void          FreeData( void );                                   // free allocated string memory

                  // format value in the given measurement with the best unit, returns the best unit
    int           BestUnit( const char *format, float value, Measure_t measure );
                  // format value in the requested unit and measurement
    void          SetUnit( const char *format, float value, int unit, Measure_t measure );

    static void   InitMemory( void );
    static void   ShutdownMemory( void );
    static void   PurgeMemory( void );
    //static void           ShowMemoryUsage_f( const idCmdArgs &args );

    int                 DynamicMemoryUsed() const;
    static AString      FormatNumber( int number );

protected:
    int                 len;
    char *              data;
    int                 alloced;
    char                baseBuffer[ STR_ALLOC_BASE ];

    void                Init( void );                                       // initialize string using base buffer
    void                EnsureAlloced( int amount, bool keepold = true );   // ensure string data buffer is large anough
};

//char *                  va( const char *fmt, ... ) id_attribute((format(printf,1,2)));


__forceinline void
AString::EnsureAlloced(int amount, bool keepold)
{
    if (amount > alloced)
    {
        ReAllocate(amount, keepold);
    }
}

__forceinline void
AString::Init( void )
{
    len = 0;
    alloced = STR_ALLOC_BASE;
    data = baseBuffer;
    data[ 0 ] = '\0';
#ifdef ID_DEBUG_UNINITIALIZED_MEMORY
    memset( baseBuffer, 0, sizeof( baseBuffer ) );
#endif
}

__forceinline
AString::AString(void)
{
    Init();
}

__forceinline
AString::AString( const AString &text )
{
    int l;

    Init();
    l = text.Length();
    EnsureAlloced( l + 1 );
    strcpy( data, text.data );
    len = l;
}

__forceinline
AString::AString(const AString &text, int start, int end)
{
    int i;
    int l;

    Init();
    if (end > text.Length())
    {
        end = text.Length();
    }

    if (start > text.Length())
    {
        start = text.Length();
    }
    else if ( start < 0 )
    {
        start = 0;
    }

    l = end - start;

    if (l < 0)
    {
        l = 0;
    }

    EnsureAlloced( l + 1 );

    for ( i = 0; i < l; i++ ) {
        data[ i ] = text[ start + i ];
    }

    data[ l ] = '\0';
    len = l;
}

__forceinline
AString::AString(const char *text)
{
    int l;

    Init();
    if ( text ) {
        l = strlen( text );
        EnsureAlloced( l + 1 );
        strcpy( data, text );
        len = l;
    }
}

__forceinline
AString::AString( const char *text, int start, int end )
{
    int i;
    int l = strlen( text );

    Init();
    if ( end > l ) {
        end = l;
    }
    if ( start > l ) {
        start = l;
    } else if ( start < 0 ) {
        start = 0;
    }

    l = end - start;
    if ( l < 0 ) {
        l = 0;
    }

    EnsureAlloced( l + 1 );

    for ( i = 0; i < l; i++ ) {
        data[ i ] = text[ start + i ];
    }

    data[ l ] = '\0';
    len = l;
}

__forceinline
AString::AString( const bool b )
{
    Init();
    EnsureAlloced( 2 );
    data[ 0 ] = b ? '1' : '0';
    data[ 1 ] = '\0';
    len = 1;
}

__forceinline AString::AString( const char c ) {
    Init();
    EnsureAlloced( 2 );
    data[ 0 ] = c;
    data[ 1 ] = '\0';
    len = 1;
}

__forceinline
AString::AString(const int i)
{
    char text[ 64 ];
    int l;

    Init();
    l = sprintf( text, "%d", i );
    EnsureAlloced( l + 1 );
    strcpy( data, text );
    len = l;
}

__forceinline
AString::AString(const unsigned u)
{
    char text[ 64 ];
    int l;

    Init();
    l = sprintf( text, "%u", u );
    EnsureAlloced( l + 1 );
    strcpy( data, text );
    len = l;
}

/*__forceinline
AString::AString( const float f )
{
    char text[ 64 ];
    int l;

    Init();
    l = AString::snPrintf( text, sizeof( text ), "%f", f );
    while( l > 0 && text[l-1] == '0' ) text[--l] = '\0';
    while( l > 0 && text[l-1] == '.' ) text[--l] = '\0';
    EnsureAlloced( l + 1 );
    strcpy( data, text );
    len = l;
}*/

__forceinline
AString::~AString( void )
{
    FreeData();
}

__forceinline size_t
AString::Size(void) const
{
    return sizeof(*this) + Allocated();
}

__forceinline const char *
AString::c_str(void) const
{
    return data;
}

__forceinline
AString::operator const char *( void )
{
    return c_str();
}

__forceinline
AString::operator const char *( void ) const 
{
    return c_str();
}

__forceinline char
AString::operator[]( int index ) const 
{
    //assert( ( index >= 0 ) && ( index <= len ) );
    return data[ index ];
}

__forceinline char &
AString::operator[]( int index )
{
    //assert( ( index >= 0 ) && ( index <= len ) );
    return data[ index ];
}

__forceinline const char *
AString::operator=( const AString &text )
{
    int l;

    l = text.Length();
    EnsureAlloced( l + 1, false );
    memcpy( data, text.data, l );
    data[l] = '\0';
    len = l;

    return c_str();
}

AString
operator+( const AString &a, const AString &b )
{
    AString result( a );
    result.Append( b );
    return result;
}

AString
operator+( const AString &a, const char *b )
{
    AString result( a );
    result.Append( b );
    return result;
}

AString
operator+( const char *a, const AString &b )
{
    AString result( a );
    result.Append( b );
    return result;
}

AString
operator+( const AString &a, const bool b )
{
    AString result( a );
    result.Append( b ? "true" : "false" );
    return result;
}


AString
operator+( const AString &a, const char b ) {
    AString result( a );
    result.Append( b );
    return result;
}

AString 
operator+( const AString &a, const float b )
{
    char    text[ 64 ];
    AString   result( a );

    sprintf( text, "%f", b );
    result.Append( text );

    return result;
}

AString
operator+( const AString &a, const int b ) {
    char    text[ 64 ];
    AString   result( a );

    sprintf( text, "%d", b );
    result.Append( text );

    return result;
}

AString
operator+( const AString &a, const unsigned b ) {
    char    text[ 64 ];
    AString   result( a );

    sprintf( text, "%u", b );
    result.Append( text );

    return result;
}

__forceinline AString &
AString::operator+=( const float a ) {
    char text[ 64 ];

    sprintf( text, "%f", a );
    Append( text );

    return *this;
}

__forceinline AString &AString::operator+=( const int a ) {
    char text[ 64 ];

    sprintf( text, "%d", a );
    Append( text );

    return *this;
}

__forceinline AString &AString::operator+=( const unsigned a ) {
    char text[ 64 ];

    sprintf( text, "%u", a );
    Append( text );

    return *this;
}

__forceinline AString &AString::operator+=( const AString &a ) {
    Append( a );
    return *this;
}

__forceinline AString &AString::operator+=( const char *a )
{
    Append( a );
    return *this;
}

__forceinline AString &AString::operator+=( const char a )
{
    Append( a );
    return *this;
}

__forceinline AString &AString::operator+=( const bool a )
{
    Append( a ? "true" : "false" );
    return *this;
}

__forceinline bool 
operator==( const AString &a, const AString &b )
{
    return ( !AString::Cmp( a.data, b.data ) );
}

__forceinline bool 
operator==( const AString &a, const char *b )
{
    //assert( b );
    return ( !AString::Cmp( a.data, b ) );
}

__forceinline bool 
operator==(const char *a, const AString &b)
{
    //assert( a );
    return ( !AString::Cmp( a, b.data ) );
}

__forceinline bool
operator!=( const AString &a, const AString &b )
{
    return !( a == b );
}

__forceinline bool operator!=( const AString &a, const char *b ) {
    return !( a == b );
}

__forceinline bool operator!=( const char *a, const AString &b ) {
    return !( a == b );
}

__forceinline int AString::Cmp( const char *text ) const {
    //assert( text );
    return AString::Cmp( data, text );
}

__forceinline int
AString::Cmpn( const char *text, int n ) const
{
    //assert( text );
    return AString::Cmpn( data, text, n );
}

__forceinline int AString::CmpPrefix( const char *text ) const {
    //assert( text );
    return AString::Cmpn( data, text, strlen( text ) );
}

__forceinline int AString::Icmp( const char *text ) const {
    //assert( text );
    return AString::Icmp( data, text );
}

__forceinline int AString::Icmpn( const char *text, int n ) const {
    //assert( text );
    return AString::Icmpn( data, text, n );
}

__forceinline int AString::IcmpPrefix( const char *text ) const {
    //assert( text );
    return AString::Icmpn( data, text, strlen( text ) );
}
__forceinline int AString::Length( void ) const {
    return len;
}

__forceinline int AString::Allocated( void ) const {
    if ( data != baseBuffer ) {
        return alloced;
    } else {
        return 0;
    }
}

__forceinline void AString::Empty( void ) {
    EnsureAlloced( 1 );
    data[0] = '\0';
    len = 0;
}

__forceinline bool AString::IsEmpty( void ) const {
    return ( AString::Cmp( data, "" ) == 0 );
}

__forceinline void AString::Clear( void ) {
    FreeData();
    Init();
}

__forceinline void
AString::Append( const char a )
{
    EnsureAlloced( len + 2 );
    data[ len ] = a;
    len++;
    data[ len ] = '\0';
}

__forceinline void
AString::Append( const AString &text )
{
    int newLen;
    int i;

    newLen = len + text.Length();
    EnsureAlloced( newLen + 1 );
    for ( i = 0; i < text.len; i++ ) {
        data[ len + i ] = text[ i ];
    }
    len = newLen;
    data[ len ] = '\0';
}

__forceinline void
AString::Append( const char *text )
{
    int newLen;
    int i;

    if ( text ) {
        newLen = len + strlen( text );
        EnsureAlloced( newLen + 1 );
        for ( i = 0; text[ i ]; i++ ) {
            data[ len + i ] = text[ i ];
        }
        len = newLen;
        data[ len ] = '\0';
    }
}

__forceinline void
AString::Append( const char *text, int l )
{
    int newLen;
    int i;

    if ( text && l ) {
        newLen = len + l;
        EnsureAlloced( newLen + 1 );
        for ( i = 0; text[ i ] && i < l; i++ ) {
            data[ len + i ] = text[ i ];
        }
        len = newLen;
        data[ len ] = '\0';
    }
}

__forceinline void
AString::Insert( const char a, int index )
{
    int i, l;

    if ( index < 0 ) {
        index = 0;
    } else if ( index > len ) {
        index = len;
    }

    l = 1;
    EnsureAlloced( len + l + 1 );
    for ( i = len; i >= index; i-- ) {
        data[i+l] = data[i];
    }
    data[index] = a;
    len++;
}

__forceinline void
AString::Insert( const char *text, int index )
{
    int i, l;

    if ( index < 0 ) {
        index = 0;
    } else if ( index > len ) {
        index = len;
    }

    l = strlen( text );
    EnsureAlloced( len + l + 1 );
    for ( i = len; i >= index; i-- ) {
        data[i+l] = data[i];
    }
    for ( i = 0; i < l; i++ ) {
        data[index+i] = text[i];
    }
    len += l;
}

__forceinline void AString::ToLower( void ) {
    for (int i = 0; data[i]; i++ ) {
        if ( CharIsUpper( data[i] ) ) {
            data[i] += ( 'a' - 'A' );
        }
    }
}

__forceinline void AString::ToUpper( void ) {
    for (int i = 0; data[i]; i++ ) {
        if ( CharIsLower( data[i] ) ) {
            data[i] -= ( 'a' - 'A' );
        }
    }
}

__forceinline bool AString::IsNumeric( void ) const {
    return AString::IsNumeric( data );
}

__forceinline bool AString::IsColor( void ) const {
    return AString::IsColor( data );
}

__forceinline bool AString::HasLower( void ) const {
    return AString::HasLower( data );
}

__forceinline bool AString::HasUpper( void ) const {
    return AString::HasUpper( data );
}

__forceinline AString &AString::RemoveColors( void ) {
    AString::RemoveColors( data );
    len = Length( data );
    return *this;
}

__forceinline int AString::LengthWithoutColors( void ) const {
    return AString::LengthWithoutColors( data );
}

__forceinline void AString::CapLength( int newlen ) {
    if ( len <= newlen ) {
        return;
    }
    data[ newlen ] = 0;
    len = newlen;
}

__forceinline void AString::Fill( const char ch, int newlen ) {
    EnsureAlloced( newlen + 1 );
    len = newlen;
    memset( data, ch, len );
    data[ len ] = 0;
}

__forceinline int AString::Find( const char c, int start, int end ) const {
    if ( end == -1 ) {
        end = len;
    }
    return AString::FindChar( data, c, start, end );
}

__forceinline int AString::Find( const char *text, bool casesensitive, int start, int end ) const {
    if ( end == -1 ) {
        end = len;
    }
    return AString::FindText( data, text, casesensitive, start, end );
}

__forceinline bool AString::Filter( const char *filter, bool casesensitive ) const {
    return AString::Filter( filter, data, casesensitive );
}

__forceinline const char *
AString::Left( int len, AString &result ) const
{
    return Mid( 0, len, result );
}

__forceinline const char *
AString::Right( int len, AString &result ) const {
    if ( len >= Length() ) {
        result = *this;
        return result;
    }
    return Mid( Length() - len, len, result );
}

AString
AString::Left( int len ) const 
{
    return Mid( 0, len );
}

AString
AString::Right( int len ) const 
{
    if ( len >= Length() ) {
        return *this;
    }
    return Mid( Length() - len, len );
}

__forceinline void AString::Strip( const char c ) {
    StripLeading( c );
    StripTrailing( c );
}

__forceinline void AString::Strip( const char *string ) {
    StripLeading( string );
    StripTrailing( string );
}

__forceinline bool AString::CheckExtension( const char *ext ) {
    return AString::CheckExtension( data, ext );
}

__forceinline int AString::Length( const char *s ) {
    int i;
    for ( i = 0; s[i]; i++ ) {}
    return i;
}

__forceinline char *AString::ToLower( char *s ) {
    for ( int i = 0; s[i]; i++ ) {
        if ( CharIsUpper( s[i] ) ) {
            s[i] += ( 'a' - 'A' );
        }
    }
    return s;
}

__forceinline char *AString::ToUpper( char *s ) {
    for ( int i = 0; s[i]; i++ ) {
        if ( CharIsLower( s[i] ) ) {
            s[i] -= ( 'a' - 'A' );
        }
    }
    return s;
}

__forceinline int AString::Hash( const char *string ) {
    int i, hash = 0;
    for ( i = 0; *string != '\0'; i++ ) {
        hash += ( *string++ ) * ( i + 119 );
    }
    return hash;
}

__forceinline int AString::Hash( const char *string, int length ) {
    int i, hash = 0;
    for ( i = 0; i < length; i++ ) {
        hash += ( *string++ ) * ( i + 119 );
    }
    return hash;
}

__forceinline int AString::IHash( const char *string ) {
    int i, hash = 0;
    for( i = 0; *string != '\0'; i++ ) {
        hash += ToLower( *string++ ) * ( i + 119 );
    }
    return hash;
}

__forceinline int AString::IHash( const char *string, int length ) {
    int i, hash = 0;
    for ( i = 0; i < length; i++ ) {
        hash += ToLower( *string++ ) * ( i + 119 );
    }
    return hash;
}

__forceinline bool AString::IsColor( const char *s ) {
    return ( s[0] == C_COLOR_ESCAPE && s[1] != '\0' && s[1] != ' ' );
}

__forceinline char AString::ToLower( char c ) {
    if ( c <= 'Z' && c >= 'A' ) {
        return ( c + ( 'a' - 'A' ) );
    }
    return c;
}

__forceinline char AString::ToUpper( char c ) {
    if ( c >= 'a' && c <= 'z' ) {
        return ( c - ( 'a' - 'A' ) );
    }
    return c;
}

__forceinline bool AString::CharIsPrintable( int c ) {
    // test for regular ascii and western European high-ascii chars
    return ( c >= 0x20 && c <= 0x7E ) || ( c >= 0xA1 && c <= 0xFF );
}

__forceinline bool AString::CharIsLower( int c ) {
    // test for regular ascii and western European high-ascii chars
    return ( c >= 'a' && c <= 'z' ) || ( c >= 0xE0 && c <= 0xFF );
}

__forceinline bool AString::CharIsUpper( int c ) {
    // test for regular ascii and western European high-ascii chars
    return ( c <= 'Z' && c >= 'A' ) || ( c >= 0xC0 && c <= 0xDF );
}

__forceinline bool AString::CharIsAlpha( int c ) {
    // test for regular ascii and western European high-ascii chars
    return ( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) ||
             ( c >= 0xC0 && c <= 0xFF ) );
}

__forceinline bool AString::CharIsNumeric( int c ) {
    return ( c <= '9' && c >= '0' );
}

__forceinline bool AString::CharIsNewLine( char c ) {
    return ( c == '\n' || c == '\r' || c == '\v' );
}

__forceinline bool AString::CharIsTab( char c ) {
    return ( c == '\t' );
}

__forceinline int AString::ColorIndex( int c ) {
    return ( c & 15 );
}

__forceinline int AString::DynamicMemoryUsed() const {
    return ( data == baseBuffer ) ? 0 : alloced;
}

#endif /* !__STR_H__ */
