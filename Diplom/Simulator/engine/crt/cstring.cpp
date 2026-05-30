
#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include "cstring.h"

#define INTSIGNBITSET(i)		(((const unsigned long)(i)) >> 31)
#define INTSIGNBITNOTSET(i)		((~((const unsigned long)(i))) >> 31)


/*
============
CString::ReAllocate
============
*/
void CString::ReAllocate(size_t amount, bool keepold)
{
    unsigned char  *newbuffer;
    size_t          newsize;
    int             mod;

    //assert( data );
    //assert( amount > 0 );

    mod = amount % STR_ALLOC_GRAN;
    if ( !mod ) {
        newsize = amount;
    }
    else {
        newsize = amount + STR_ALLOC_GRAN - mod;
    }
    m_alloced = newsize;

    newbuffer = (unsigned char *)malloc(m_alloced);
    if (keepold && m_data)
    {
        m_data[m_len] = _T('\0');
        memcpy(newbuffer, m_data, (m_len + 1) * sizeof(TCHAR));
        //_tcscpy( (TCHAR *)newbuffer, m_data );
    }

    if ( m_data && m_data != m_baseBuffer ) {
        free(m_data);
    }

    m_data = (TCHAR *)newbuffer;
}

/*
============
CString::FreeData
============
*/
void CString::FreeData( void ) {
    if ( m_data && m_data != m_baseBuffer ) {
        free(m_data);
        m_data = m_baseBuffer;
    }
}

CString
operator+(const CString &a, const CString &b)
{
    CString result( a );
    result.Append( b );
    return result;
}

CString
operator+(const CString &a, const TCHAR *b)
{
    CString result(a);
    result.Append(b);
    return result;
}

CString
operator+( const TCHAR *a, const CString &b )
{
    CString result(a);
    result.Append(b);
    return result;
}

CString
operator+(const CString &a, const bool b)
{
    CString result(a);
    if (b) {
        result.Append(_T("true"), sizeof("true"));
    } else {
        result.Append(_T("false"), sizeof("false"));
    }
    return result;
}

CString
operator+( const CString &a, const TCHAR b)
{
    CString result(a);
    result.Append(b);
    return result;
}

CString 
operator+(const CString &a, const float b)
{
    TCHAR    text[64];
    CString   result( a );

    //sprintf( text, "%f", b );
    size_t l = _stprintf(text,  _T("%f"), b);
    result.Append(text, l);

    return result;
}

CString
operator+( const CString &a, const int b ) {
    TCHAR    text[64];
    CString   result(a);

    size_t l = _stprintf(text,  _T("%d"), b);
    result.Append(text, l);

    return result;
}

CString
operator+(const CString &a, const unsigned b)
{
    TCHAR    text[64];
    CString   result(a);

    size_t l = _stprintf(text, _T("0x%U"), b);
    result.Append(text, l);

    return result;
}

CString
CString::Left( size_t len ) const 
{
    return Mid( 0, len );
}

CString
CString::Right( size_t len ) const 
{
    if ( len >= Length() ) {
        return *this;
    }
    return Mid( Length() - len, len );
}

/*
============
CString::operator=
============
*/
void CString::operator=(const TCHAR *text)
{
    int diff;

    if (!text)
    {
        // safe behaviour if NULL
        EnsureAlloced( 1, false );
        m_data[ 0 ] = _T('\0');
        m_len = 0;
        return;
    }

    if (text == m_data)
    {
        return; // copying same thing
    }

    // check if we're aliasing
    if ( text >= m_data && text <= m_data + m_len )
    {
        diff = text - m_data;

        //assert( strlen( text ) < (unsigned)len );

        /*for ( i = 0; text[ i ]; i++ ) {
            m_data[ i ] = text[ i ];
        }*/

        _tcscpy(m_data, text);

        //m_data[i] = '\0';
        m_len -= diff;
        return;
    }

    size_t l = _tcslen( text );
    EnsureAlloced( l + 1, false );
    _tcscpy(m_data, text);
    m_len = l;
}

/*
============
CString::FindChar

returns -1 if not found otherwise the index of the char
============
*/
int
CString::FindChar(const TCHAR *str, const TCHAR c, int start, int end)
{
    int i;

    if ( end == -1 ) {
        end = _tcslen(str) - 1;
    }
    for ( i = start; i <= end; i++ ) {
        if ( str[i] == c ) {
            return i;
        }
    }
    return -1;
}

/*
============
CString::FindText

returns -1 if not found otherwise the index of the text
============
*/
int
CString::FindText( const TCHAR *str, const TCHAR *text, bool casesensitive, int start, int end )
{
    int l, i, j;

    if ( end == -1 ) {
        end = _tcslen( str );
    }
    l = end - _tcslen( text );
    for ( i = start; i <= l; i++ ) {
        if ( casesensitive ) {
            for ( j = 0; text[j]; j++ ) {
                if ( str[i+j] != text[j] ) {
                    break;
                }
            }
        } else {
            for ( j = 0; text[j]; j++ ) {
                if ( _totupper( str[i+j] ) != _totupper( text[j] ) ) {
                    break;
                }
            }
        }
        if ( !text[j] ) {
            return i;
        }
    }
    return -1;
}

/*
============
CString::Filter

Returns true if the string conforms the given filter.
Several metacharacter may be used in the filter.

*          match any string of zero or more characters
?          match any single character
[abc...]   match any of the enclosed characters; a hyphen can
           be used to specify a range (e.g. a-z, A-Z, 0-9)

============
*/
bool
CString::Filter( const TCHAR *filter, const TCHAR *name, bool casesensitive )
{
    CString buf;
    int i, found, index;

    while(*filter) {
        if (*filter == '*') {
            filter++;
            buf.Empty();
            for (i = 0; *filter; i++) {
                if ( *filter == '*' || *filter == '?' || (*filter == '[' && *(filter+1) != '[') ) {
                    break;
                }
                buf += *filter;
                if ( *filter == '[' ) {
                    filter++;
                }
                filter++;
            }
            if ( buf.Length() )
            {
                index = CString(name).Find( buf.c_str(), casesensitive );
                if ( index == -1 ) {
                    return false;
                }
                name += index + _tcslen(buf);
            }
        }
        else if (*filter == '?') {
            filter++;
            name++;
        }
        else if (*filter == '[') {
            if ( *(filter+1) == '[' ) {
                if ( *name != '[' ) {
                    return false;
                }
                filter += 2;
                name++;
            }
            else {
                filter++;
                found = false;
                while(*filter && !found) {
                    if (*filter == ']' && *(filter+1) != ']') {
                        break;
                    }
                    if (*(filter+1) == '-' && *(filter+2) && (*(filter+2) != ']' || *(filter+3) == ']')) {
                        if (casesensitive) {
                            if (*name >= *filter && *name <= *(filter+2)) {
                                found = true;
                            }
                        }
                        else {
                            if ( _totupper(*name) >= _totupper(*filter) && _totupper(*name) <= _totupper(*(filter+2)) ) {
                                found = true;
                            }
                        }
                        filter += 3;
                    }
                    else {
                        if (casesensitive) {
                            if (*filter == *name) {
                                found = true;
                            }
                        }
                        else {
                            if ( _totupper(*filter) == _totupper(*name) ) {
                                found = true;
                            }
                        }
                        filter++;
                    }
                }
                if (!found) {
                    return false;
                }
                while(*filter) {
                    if ( *filter == ']' && *(filter+1) != ']' ) {
                        break;
                    }
                    filter++;
                }
                filter++;
                name++;
            }
        }
        else {
            if (casesensitive) {
                if (*filter != *name) {
                    return false;
                }
            }
            else {
                if ( _totupper(*filter) != _totupper(*name) ) {
                    return false;
                }
            }
            filter++;
            name++;
        }
    }
    return true;
}

/*
=============
CString::StripMediaName

  makes the string lower case, replaces backslashes with forward slashes, and removes extension
=============
*/
void
CString::StripMediaName( const TCHAR *name, CString &mediaName )
{
    TCHAR c;

    mediaName.Empty();

    for (c = *name; c; c = *(++name) )
    {
        // truncate at an extension
        if ( c == _T('.') ) {
            break;
        }
        // convert backslashes to forward slashes
        if ( c == _T('\\') ) {
            mediaName.Append(_T('/'));
        } else {
            mediaName.Append(CString::ToLower( c ) );
        }
    }
}

/*
=============
CString::CheckExtension
=============
*/
bool
CString::CheckExtension( const TCHAR *name, const TCHAR *ext )
{
    const TCHAR *s1 = name + Length( name ) - 1;
    const TCHAR *s2 = ext + Length( ext ) - 1;
    int c1, c2, d;

    do {
        c1 = *s1--;
        c2 = *s2--;

        d = c1 - c2;
        while( d ) {
            if ( c1 <= 'Z' && c1 >= 'A' ) {
                d += ('a' - 'A');
                if ( !d ) {
                    break;
                }
            }
            if ( c2 <= 'Z' && c2 >= 'A' ) {
                d -= ('a' - 'A');
                if ( !d ) {
                    break;
                }
            }
            return false;
        }
    } while( s1 > name && s2 > ext );

    return ( s1 >= name );
}


/*
============
CString::Last

returns -1 if not found otherwise the index of the char
============
*/
int
CString::Last( const TCHAR c ) const
{
    int i;
    
    for( i = Length(); i > 0; i-- ) {
        if ( m_data[ i - 1 ] == c ) {
            return i - 1;
        }
    }

    return -1;
}

/*
============
CString::StripLeading
============
*/
void
CString::StripLeading( const TCHAR c ) {
    while( m_data[ 0 ] == c ) {
        memmove( &m_data[ 0 ], &m_data[ 1 ], m_len );
        m_len--;
    }
}

/*
============
CString::StripLeading
============
*/
void
CString::StripLeading( const TCHAR *string )
{
    int l;

    l = _tcslen( string );
    if ( l > 0 ) {
        while ( !Cmpn( string, l ) ) {
            memmove( m_data, m_data + l, m_len - l + 1 );
            m_len -= l;
        }
    }
}

/*
============
CString::StripLeadingOnce
============
*/
bool CString::StripLeadingOnce( const TCHAR *string )
{

    size_t l = _tcslen( string );
    if ( ( l > 0 ) && !Cmpn( string, l ) ) {
        memmove( m_data, m_data + l, m_len - l + 1 );
        m_len -= l;
        return true;
    }
    return false;
}

/*
============
CString::StripTrailing
============
*/
void CString::StripTrailing( const TCHAR c )
{
    
    for (size_t i = Length(); i > 0 && m_data[ i - 1 ] == c; i-- ) {
        m_data[ i - 1 ] = _T('\0');
        m_len--;
    }
}

/*
============
CString::StripLeading
============
*/
void CString::StripTrailing( const TCHAR *string ) {

    size_t l = _tcslen( string );
    if ( l > 0 ) {
        while ( ( m_len >= l ) && !Cmpn( string, m_data + m_len - l, l ) ) {
            m_len -= l;
            m_data[m_len] = _T('\0');
        }
    }
}

/*
============
CString::StripTrailingOnce
============
*/
bool CString::StripTrailingOnce( const TCHAR *string )
{

    size_t l = _tcslen( string );
    if ((l > 0) && (m_len >= l) && !Cmpn(string, m_data + m_len - l, l))
    {
        m_len -= l;
        m_data[m_len] = _T('\0');
        return true;
    }
    return false;
}

/*
============
CString::Replace
============
*/
void CString::Replace( const TCHAR *old, const TCHAR *nw )
{
    CString   oldString( m_data );

    size_t oldLen = _tcslen( old );
    size_t newLen = _tcslen( nw );

    // Work out how big the new string will be
    size_t count = 0;
    for (size_t i = 0; i < oldString.Length(); i++ )
    {
        if( !CString::Cmpn( &oldString[i], old, oldLen ) ) {
            count++;
            i += (oldLen - 1);
        }
    }

    if (count)
    {
        EnsureAlloced( m_len + (( newLen - oldLen ) * count) + (2 * sizeof(TCHAR)), false );

        size_t j = 0;
        // Replace the old data with the new data
        for (size_t i = 0; i < oldString.Length(); i++ )
        {
            if( !CString::Cmpn(&oldString[i], old, oldLen ) )
            {
                memcpy(m_data + j, nw, newLen * sizeof(TCHAR) );
                i += (oldLen - 1);
                j += newLen;
            }
            else
            {
                m_data[j] = oldString[i];
                j++;
            }
        }
        m_data[j] = 0;
        m_len = _tcslen(m_data);
    }
}

/*
============
CString::Mid
============
*/
const TCHAR *
CString::Mid( size_t start, size_t len, CString &result) const
{

    result.Empty();

    size_t i = Length();
    if ( i == 0 || len <= 0 || start >= i ) {
        return NULL;
    }

    if ( start + len >= i ) {
        len = i - start;
    }

    result.Append(&m_data[start], len);
    return result;
}

/*
============
CString::Mid
============
*/
CString
CString::Mid(size_t start, size_t len ) const
{
    CString result;

    size_t i = Length();
    if ( i == 0 || len <= 0 || start >= i ) {
        return result;
    }

    if ( start + len >= i ) {
        len = i - start;
    }

    result.Append( &m_data[ start ], len );
    return result;
}

/*
============
CString::StripTrailingWhitespace
============
*/
void
CString::StripTrailingWhitespace( void )
{
    int i;
    
    // cast to unsigned char to prevent stripping off high-ASCII characters
    for( i = Length(); i > 0 && (unsigned char)(m_data[ i - 1 ]) <= ' '; i-- ) {
        m_data[ i - 1 ] = '\0';
        m_len--;
    }
}

/*
============
CString::StripQuotes

Removes the quotes from the beginning and end of the string
============
*/
CString &
CString::StripQuotes(void)
{
    if ( m_data[0] != _T('\"') )
    {
        return *this;
    }
    
    // Remove the trailing quote first
    if ( m_data[m_len-1] == _T('\"') )
    {
        m_data[m_len-1] = '\0';
        m_len--;
    }

    // Strip the leading quote now
    m_len--;  
    memmove( &m_data[ 0 ], &m_data[ 1 ], m_len );
    m_data[m_len] = '\0';
    
    return *this;
}

/*
============
CString::SetFileExtension
============
*/
CString &
CString::SetFileExtension( const TCHAR *extension )
{
    StripFileExtension();
    if ( *extension != _T('.') ) {
        Append(_T('.'));
    }
    Append( extension );
    return *this;
}

/*
============
CString::StripFileExtension
============
*/
CString &
CString::StripFileExtension( void )
{
    int i;

    for ( i = m_len-1; i >= 0; i-- ) {
        if ( m_data[i] == _T('.') ) {
            m_data[i] = _T('\0');
            m_len = i;
            break;
        }
    }
    return *this;
}

/*
============
CString::StripAbsoluteFileExtension
============
*/
CString &
CString::StripAbsoluteFileExtension( void )
{

    for (size_t i = 0; i < m_len; i++ ) {
        if ( m_data[i] == _T('.') ) {
            m_data[i] = _T('\0');
            m_len = i;
            break;
        }
    }

    return *this;
}

/*
==================
CString::DefaultFileExtension
==================
*/
CString &
CString::DefaultFileExtension( const TCHAR *extension )
{
    int i;

    // do nothing if the string already has an extension
    for ( i = m_len-1; i >= 0; i-- ) {
        if ( m_data[i] == _T('.') ) {
            return *this;
        }
    }
    if ( *extension != _T('.') ) {
        Append( _T('.') );
    }
    Append( extension );
    return *this;
}

/*
==================
CString::DefaultPath
==================
*/
CString &
CString::DefaultPath( const TCHAR *basepath )
{
    if (((*this)[(size_t)0] == _T('/')) || ((*this)[(size_t)0] == _T('\\')))
    {
        // absolute path location
        return *this;
    }

    *this = basepath + *this;
    return *this;
}

/*
====================
CString::AppendPath
====================
*/
void
CString::AppendPath( const TCHAR *text )
{
    int pos;
    int i = 0;

    if ( text && text[i] ) {
        pos = m_len;
        EnsureAlloced( m_len + _tcslen( text ) + 2 );

        if ( pos ) {
            if ( m_data[ pos-1 ] != '/' ) {
                m_data[ pos++ ] = '/';
            }
        }
        if ( text[i] == '/' ) {
            i++;
        }

        for ( ; text[ i ]; i++ ) {
            if ( text[ i ] == '\\' ) {
                m_data[ pos++ ] = '/';
            } else {
                m_data[ pos++ ] = text[ i ];
            }
        }
        m_len = pos;
        m_data[ pos ] = _T('\0');
    }
}

/*
==================
CString::StripFilename
==================
*/
CString &
CString::StripFilename( void )
{
    size_t pos;

    pos = Length() - 1;
    while( ( pos > 0 ) && ( ( *this )[ pos ] != _T('/') ) && ( ( *this )[ pos ] != '\\' ) ) {
        pos--;
    }

    if ( pos < 0 ) {
        pos = 0;
    }

    CapLength( pos );
    return *this;
}

/*
==================
CString::StripPath
==================
*/
CString &
CString::StripPath( void )
{
    size_t pos;

    pos = Length();
    while( ( pos > 0 ) && ( ( *this )[ pos - 1 ] != '/' ) && ( ( *this )[ pos - 1 ] != '\\' ) ) {
        pos--;
    }

    *this = Right( Length() - pos );
    return *this;
}

/*
====================
CString::ExtractFilePath
====================
*/
void 
CString::ExtractFilePath(CString &dest ) const {

    //
    // back up until a \ or the start
    //
    size_t pos = Length();
    while( ( pos > 0 ) && ( ( *this )[ pos - 1 ] != '/' ) && ( ( *this )[ pos - 1 ] != '\\' ) ) {
        pos--;
    }

    Left( pos, dest );
}

/*
====================
CString::ExtractFileName
====================
*/
void
CString::ExtractFileName(CString &dest ) const
{
    //
    // back up until a \ or the start
    //
    size_t pos = Length(); // - 1;

    if (pos > 0)
    {
        pos--;
        while( ( pos > 0 ) && ( ( *this )[ pos - 1 ] != '/' ) && ( ( *this )[ pos - 1 ] != '\\' ) ) {
            pos--;
        }

        Right( Length() - pos, dest );
    }
}

/*
====================
CString::ExtractFileBase
====================
*/
void
CString::ExtractFileBase(CString &dest ) const
{
    //
    // back up until a \ or the start
    //
    size_t pos = Length();
    if (pos > 0)
    {
        pos--;
        while( ( pos > 0 ) && ( ( *this )[ pos - 1 ] != '/' ) && ( ( *this )[ pos - 1 ] != '\\' ) ) {
            pos--;
        }

        size_t start = pos;
        while( ( pos < Length() ) && ( ( *this )[ pos ] != '.' ) ) {
            pos++;
        }

        Mid( start, pos - start, dest );
    }
}

/*
====================
CString::ExtractFileExtension
====================
*/
void
CString::ExtractFileExtension(CString &dest ) const
{
    //
    // back up until a . or the start
    //
    size_t pos = Length();
    if (pos > 0)
    {
        pos--;
        while( ( pos > 0 ) && ( ( *this )[ pos - 1 ] != '.' ) ) {
            pos--;
        }

        if ( !pos ) {
            // no extension
            dest.Empty();
        } else {
            Right( Length() - pos, dest );
        }
    }
}


/*
=====================================================================

  char * methods to replace library functions

=====================================================================
*/

/*
============
CString::IsNumeric

Checks a string to see if it contains only numerical values.
============
*/
bool
CString::IsNumeric( const TCHAR *s )
{
    int     i;
    bool    dot;

    if ( *s == '-' ) {
        s++;
    }

    dot = false;
    for ( i = 0; s[i]; i++ ) {
        if ( !_istdigit( s[i] ) ) {
            if ( ( s[ i ] == '.' ) && !dot ) {
                dot = true;
                continue;
            }
            return false;
        }
    }

    return true;
}

/*
============
CString::HasLower

Checks if a string has any lowercase chars
============
*/
bool
CString::HasLower( const TCHAR *s )
{
    if ( !s ) {
        return false;
    }
    
    while ( *s ) {
        if ( CharIsLower( *s ) ) {
            return true;
        }
        s++;
    }
    
    return false;
}

/*
============
CString::HasUpper
    
Checks if a string has any uppercase chars
============
*/
bool
CString::HasUpper( const TCHAR *s )
{
    if ( !s ) {
        return false;
    }
    
    while ( *s ) {
        if ( CharIsUpper( *s ) ) {
            return true;
        }
        s++;
    }
    
    return false;
}

/*
================
CString::Cmp
================
*/
int
CString::Cmp( const TCHAR *s1, const TCHAR *s2 )
{
    int c1, c2, d;

    do {
        c1 = *s1++;
        c2 = *s2++;

        d = c1 - c2;
        if ( d ) {
            return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
        }
    } while( c1 );

    return 0;       // strings are equal
}

/*
================
CString::Cmpn
================
*/
int
CString::Cmpn(const TCHAR *s1, const TCHAR *s2, size_t n)
{
    int c1, c2, d;

    //assert( n >= 0 );

    do {
        c1 = *s1++;
        c2 = *s2++;

        if ( !n-- ) {
            return 0;       // strings are equal until end point
        }

        d = c1 - c2;
        if ( d ) {
            return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
        }
    } while( c1 );

    return 0;       // strings are equal
}

/*
================
CString::Icmp
================
*/
int
CString::Icmp(const TCHAR *s1, const TCHAR *s2)
{
    int c1, c2, d;

    do {
        c1 = *s1++;
        c2 = *s2++;

        d = c1 - c2;
        while( d ) {
            if ( c1 <= 'Z' && c1 >= 'A' ) {
                d += ('a' - 'A');
                if ( !d ) {
                    break;
                }
            }
            if ( c2 <= 'Z' && c2 >= 'A' ) {
                d -= ('a' - 'A');
                if ( !d ) {
                    break;
                }
            }
            return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
        }
    } while( c1 );

    return 0;       // strings are equal
}

/*
================
CString::Icmpn
================
*/
int CString::Icmpn( const TCHAR *s1, const TCHAR *s2, size_t n ) {
    int c1, c2, d;

    //assert( n >= 0 );

    do {
        c1 = *s1++;
        c2 = *s2++;

        if ( !n-- ) {
            return 0;       // strings are equal until end point
        }

        d = c1 - c2;
        while( d ) {
            if ( c1 <= 'Z' && c1 >= 'A' ) {
                d += ('a' - 'A');
                if ( !d ) {
                    break;
                }
            }
            if ( c2 <= 'Z' && c2 >= 'A' ) {
                d -= ('a' - 'A');
                if ( !d ) {
                    break;
                }
            }
            return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
        }
    } while( c1 );

    return 0;       // strings are equal
}


/*
================
CString::IcmpPath
================
*/
int CString::IcmpPath( const TCHAR *s1, const TCHAR *s2 ) {
    int c1, c2, d;

#if 0
//#if !defined( _WIN32 )
    idLib::common->Printf( "WARNING: IcmpPath used on a case-sensitive filesystem?\n" );
#endif

    do {
        c1 = *s1++;
        c2 = *s2++;

        d = c1 - c2;
        while( d ) {
            if ( c1 <= 'Z' && c1 >= 'A' ) {
                d += ('a' - 'A');
                if ( !d ) {
                    break;
                }
            }
            if ( c1 == '\\' ) {
                d += ('/' - '\\');
                if ( !d ) {
                    break;
                }
            }
            if ( c2 <= 'Z' && c2 >= 'A' ) {
                d -= ('a' - 'A');
                if ( !d ) {
                    break;
                }
            }
            if ( c2 == '\\' ) {
                d -= ('/' - '\\');
                if ( !d ) {
                    break;
                }
            }
            // make sure folders come first
            while( c1 ) {
                if ( c1 == '/' || c1 == '\\' ) {
                    break;
                }
                c1 = *s1++;
            }
            while( c2 ) {
                if ( c2 == '/' || c2 == '\\' ) {
                    break;
                }
                c2 = *s2++;
            }
            if ( c1 && !c2 ) {
                return -1;
            } else if ( !c1 && c2 ) {
                return 1;
            }
            // same folder depth so use the regular compare
            return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
        }
    } while( c1 );

    return 0;
}

/*
================
CString::IcmpnPath
================
*/
int
CString::IcmpnPath( const TCHAR *s1, const TCHAR *s2, int n ) {
    int c1, c2, d;

#if 0
//#if !defined( _WIN32 )
    idLib::common->Printf( "WARNING: IcmpPath used on a case-sensitive filesystem?\n" );
#endif

    //assert( n >= 0 );

    do {
        c1 = *s1++;
        c2 = *s2++;

        if ( !n-- ) {
            return 0;       // strings are equal until end point
        }

        d = c1 - c2;
        while( d ) {
            if ( c1 <= 'Z' && c1 >= 'A' ) {
                d += ('a' - 'A');
                if ( !d ) {
                    break;
                }
            }
            if ( c1 == '\\' ) {
                d += ('/' - '\\');
                if ( !d ) {
                    break;
                }
            }
            if ( c2 <= 'Z' && c2 >= 'A' ) {
                d -= ('a' - 'A');
                if ( !d ) {
                    break;
                }
            }
            if ( c2 == '\\' ) {
                d -= ('/' - '\\');
                if ( !d ) {
                    break;
                }
            }
            // make sure folders come first
            while( c1 ) {
                if ( c1 == '/' || c1 == '\\' ) {
                    break;
                }
                c1 = *s1++;
            }
            while( c2 ) {
                if ( c2 == '/' || c2 == '\\' ) {
                    break;
                }
                c2 = *s2++;
            }
            if ( c1 && !c2 ) {
                return -1;
            } else if ( !c1 && c2 ) {
                return 1;
            }
            // same folder depth so use the regular compare
            return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
        }
    } while( c1 );

    return 0;
}

/*
=============
CString::Copynz
 
Safe strncpy that ensures a trailing zero
=============
*/
void
CString::Copynz( TCHAR *dest, const TCHAR *src, int destsize ) {
    if ( !src ) {
        return;
    }
    if ( destsize < 1 ) {
        return;
    }

    _tcsncpy( dest, src, destsize-1 );
    dest[destsize-1] = 0;
}

/*
================
CString::Append

  never goes past bounds or leaves without a terminating 0
================
*/
void
CString::Append( TCHAR *dest, int size, const TCHAR *src ) {
    int     l1;

    l1 = _tcslen( dest );
    if ( l1 >= size )
    {
        // TODO: do error
    }
    else
    {
        CString::Copynz( dest + l1, src, size - l1 );
    }
}


/*
================
CString::snPrintf
================
*/
int CString::snPrintf( TCHAR *dest, int size, const TCHAR *fmt, ...) {
    int len;
    va_list argptr;
    TCHAR buffer[32000]; // big, but small enough to fit in PPC stack

    va_start( argptr, fmt );
    len = _vstprintf( buffer, fmt, argptr );
    va_end( argptr );

    if (len >= sizeof( buffer ) )
    {
        //idLib::common->Error( "CString::snPrintf: overflowed buffer" );
    }
    if (len >= size )
    {
        //idLib::common->Warning( "CString::snPrintf: overflow of %i in %i\n", len, size );
        len = size;
    }
    CString::Copynz( dest, buffer, size );
    return len;
}

/*
============
CString::vsnPrintf

vsnprintf portability:

C99 standard: vsnprintf returns the number of characters (excluding the trailing
'\0') which would have been written to the final string if enough space had been available
snprintf and vsnprintf do not write more than size bytes (including the trailing '\0')

win32: _vsnprintf returns the number of characters written, not including the terminating null character,
or a negative value if an output error occurs. If the number of characters to write exceeds count, then count 
characters are written and -1 is returned and no trailing '\0' is added.

CString::vsnPrintf: always appends a trailing '\0', returns number of characters written (not including terminal \0)
or returns -1 on failure or if the buffer would be overflowed.
============
*/
int CString::vsnPrintf( TCHAR *dest, int size, const TCHAR *fmt, va_list argptr ) {
    int ret;

    if (size < 0)
        return -1;

    ret = _vsntprintf( dest, size-1, fmt, argptr );

    dest[size-1] = 0;
    if ( ret < 0 || ret >= size ) {
        return -1;
    }
    return ret;
}

/*
============
sprintf

Sets the value of the string using a printf interface.
============
*/
int sprintf( CString &string, const TCHAR *fmt, ... ) {
    int l;
    va_list argptr;
    TCHAR buffer[32000];
    
    va_start( argptr, fmt );
    l = CString::vsnPrintf( buffer, sizeof(buffer)-1, fmt, argptr );
    va_end( argptr );
    buffer[sizeof(buffer) / sizeof(TCHAR) - 1] = 0;

    string = buffer;
    return l;
}

/*
============
vsprintf

Sets the value of the string using a vprintf interface.
============
*/
int vsprintf( CString &string, const TCHAR *fmt, va_list argptr )
{
    TCHAR buffer[32000];
    
    size_t l = CString::vsnPrintf( buffer, sizeof(buffer)-1, fmt, argptr );
    buffer[sizeof(buffer) / sizeof(TCHAR) - 1] = 0;
    
    string = buffer;
    return l;
}

/*
============
va

does a varargs printf into a temp buffer
NOTE: not thread safe
============
*/
TCHAR *va( const TCHAR *fmt, ... ) {
    va_list argptr;
    static int index = 0;
    static TCHAR string[4][16384];   // in case called by nested functions
    TCHAR *buf;

    buf = string[index];
    index = (index + 1) & 3;

    va_start( argptr, fmt );
    _vstprintf( buf, fmt, argptr );
    va_end( argptr );

    return buf;
}



/*
================
CString::ShowMemoryUsage_f
================
*/
/*void CString::ShowMemoryUsage_f( const idCmdArgs &args ) {
#ifdef USE_STRING_DATA_ALLOCATOR
    idLib::common->Printf( "%6d KB string memory (%d KB free in %d blocks, %d empty base blocks)\n",
        stringDataAllocator.GetBaseBlockMemory() >> 10, stringDataAllocator.GetFreeBlockMemory() >> 10,
            stringDataAllocator.GetNumFreeBlocks(), stringDataAllocator.GetNumEmptyBaseBlocks() );
#endif
}*/

/*
================
CString::FormatNumber
================
*/
struct formatList_t {
    int         gran;
    int         count;
};

// elements of list need to decend in size
formatList_t formatList[] = {
    { 1000000000, 0 },
    { 1000000, 0 },
    { 1000, 0 }
};

int numFormatList = sizeof(formatList) / sizeof( formatList[0] );


CString CString::FormatNumber( int number ) {
    CString string;
    bool hit;

    // reset
    for ( int i = 0; i < numFormatList; i++ ) {
        formatList_t *li = formatList + i;
        li->count = 0;
    }

    // main loop
    do {
        hit = false;

        for ( int i = 0; i < numFormatList; i++ ) {
            formatList_t *li = formatList + i;

            if ( number >= li->gran ) {
                li->count++;
                number -= li->gran;
                hit = true;
                break;
            }
        }
    } while ( hit );

    // print out
    bool found = false;

    for ( int i = 0; i < numFormatList; i++ ) {
        formatList_t *li = formatList + i;

        if ( li->count ) {
            if ( !found ) {
                string += va( _T("%i,"), li->count );
            } else {
                string += va( _T("%3.3i,"), li->count );
            }
            found = true;
        }
        else if ( found ) {
            string += va( _T("%3.3i,"), li->count );
        }
    }

    if ( found ) {
        string += va( _T("%3.3i"), number );
    }
    else {
        string += va( _T("%i"), number );
    }

    // pad to proper size
    int count = 11 - string.Length();

    for ( int i = 0; i < count; i++ ) {
        string.Insert(_T(" "), 0 );
    }

    return string;
}

