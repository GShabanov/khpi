#ifndef __CSTRING_H__
#define __CSTRING_H__

/*
===============================================================================

    Character string

===============================================================================
*/


// make CString a multiple of 16 bytes long
// don't make too large to keep memory requirements to a minimum
const int STR_ALLOC_BASE            = 20;
const int STR_ALLOC_GRAN            = 32;

typedef enum {
    MEASURE_SIZE = 0,
    MEASURE_BANDWIDTH
} Measure_t;

class CString {

public:
    CString(void)  throw();
    CString(const CString &text) throw();
    CString(const CString &text, int start, int end) throw();
    CString(const TCHAR *text) throw();
    CString(const TCHAR *text, int start, int end) throw();
    CString(const char *text, size_t size) throw();
    explicit CString(const bool b) throw();
    explicit CString(const TCHAR c) throw();
    explicit CString(const int i) throw();
    explicit CString(const unsigned u) throw();
    explicit CString(const float f) throw();
    ~CString(void);

    size_t        Size(void) const;

    inline const TCHAR  *c_str(void) const {
        return m_data;
    }
    inline operator const TCHAR *(void) const {
        return c_str();
    }
    inline operator const TCHAR *(void) {
        return c_str();
    }

    TCHAR  operator[](size_t index) const {
        if ((index < 0) || (index >= m_len))
            return 0;
        //assert( ( index >= 0 ) && ( index <= len ) );
        return m_data[index];
    }

    TCHAR & operator[](size_t index) {
        if ((index < 0) || (index >= m_len))
            return m_data[0];
        //assert( ( index >= 0 ) && ( index <= len ) );
        return m_data[index];
    }

    //const TCHAR * operator=( const CString &text);
    void          operator=( const TCHAR *text );
    CString&      operator=(const CString &text);
    //const TCHAR * operator=(const CString &text);

    friend CString  operator+( const CString &a, const CString &b );
    friend CString  operator+( const CString &a, const TCHAR *b );
    friend CString  operator+( const TCHAR *a, const CString &b );

    friend CString  operator+( const CString &a, const float b );
    friend CString  operator+( const CString &a, const int b );
    friend CString  operator+( const CString &a, const unsigned b );
    friend CString  operator+( const CString &a, const bool b );
    friend CString  operator+( const CString &a, const TCHAR b );

    CString &       operator+=( const CString &a );
    CString &       operator+=( const TCHAR *a );
    CString &       operator+=( const float a );
    CString &       operator+=( const TCHAR a );
    CString &       operator+=( const int a );
    CString &       operator+=( const unsigned a );
    CString &       operator+=( const bool a );

                  // case sensitive compare
    friend bool   operator==(const CString &a, const CString &b);
    friend bool   operator==(const CString &a, const TCHAR *b);
    friend bool   operator==(const TCHAR *a, const CString &b);

                  // case sensitive compare
    friend bool   operator!=(const CString &a, const CString &b);
    friend bool   operator!=(const CString &a, const TCHAR *b);
    friend bool   operator!=(const TCHAR *a, const CString &b);

                  // case sensitive compare
    int           Cmp(const TCHAR *text) const;
    int           Cmpn(const TCHAR *text, size_t n) const;
    int           CmpPrefix(const TCHAR *text) const;

                  // case insensitive compare
    int           Icmp(const TCHAR *text) const;
    int           Icmpn(const TCHAR *text, int n) const;
    int           IcmpPrefix(const TCHAR *text ) const;

                  // compares paths and makes sure folders come first
    int           IcmpPath(const TCHAR *text) const;
    int           IcmpnPath(const TCHAR *text, int n) const;
    int           IcmpPrefixPath(const TCHAR *text) const;

    __forceinline size_t Length(void) const {
        return m_len;
    }

    size_t        Allocated(void) const;
    void          Empty(void);
    bool          IsEmpty(void) const;
    void          Clear( void );
    void          Append(const TCHAR a);
    void          Append(const CString &text);
    void          Append(const TCHAR *text);
    void          Append(const TCHAR *text, int len);
    void          Insert(const TCHAR a, int index);
    void          Insert(const TCHAR *text, int index);
    void          ToLower(void );
    void          ToUpper(void );
    bool          IsNumeric(void) const;
    bool          IsColor(void) const;
    bool          HasLower(void) const;
    bool          HasUpper(void) const;
    int           LengthWithoutColors(void) const;
    void          CapLength( size_t );
    void          Fill( const TCHAR ch, int newlen);

    int           Find( const TCHAR c, int start = 0, int end = -1) const;
    int           Find( const TCHAR *text, bool casesensitive = true, int start = 0, int end = -1) const;
    bool          Filter( const TCHAR *filter, bool casesensitive) const;
    int           Last(const TCHAR c) const;                     // return the index to the last occurance of 'c', returns -1 if not found
    const TCHAR *  Left(size_t len, CString &result) const;           // store the leftmost 'len' characters in the result
    const TCHAR *  Right(size_t len, CString &result) const;          // store the rightmost 'len' characters in the result
    const TCHAR *  Mid(size_t start, size_t len, CString &result) const; // store 'len' characters starting at 'start' in result
    CString       Left(size_t len) const;                          // return the leftmost 'len' characters
    CString       Right(size_t len) const;                         // return the rightmost 'len' characters
    CString       Mid(size_t start, size_t len) const;                // return 'len' characters starting at 'start'
    void          StripLeading(const TCHAR c);                   // strip char from front as many times as the char occurs
    void          StripLeading(const TCHAR *string);             // strip string from front as many times as the string occurs
    bool          StripLeadingOnce( const TCHAR *string);         // strip string from front just once if it occurs
    void          StripTrailing(const TCHAR c);                  // strip char from end as many times as the char occurs
    void          StripTrailing(const TCHAR *string);            // strip string from end as many times as the string occurs
    bool          StripTrailingOnce(const TCHAR *string);        // strip string from end just once if it occurs
    void          Strip(const TCHAR c);                          // strip char from front and end as many times as the char occurs
    void          Strip(const TCHAR *string);                    // strip string from front and end as many times as the string occurs
    void          StripTrailingWhitespace(void);                // strip trailing white space characters
    CString &     StripQuotes(void);                            // strip quotes around string
    void          Replace(const TCHAR *old, const TCHAR *nw);

    // file name methods
    int           FileNameHash(void) const;                     // hash key for the filename (skips extension)
    CString &     BackSlashesToSlashes(void);                   // convert slashes
    CString &     SetFileExtension(const TCHAR *extension);      // set the given file extension
    CString &     StripFileExtension(void);                     // remove any file extension
    CString &     StripAbsoluteFileExtension(void);             // remove any file extension looking from front (useful if there are multiple .'s)
    CString &     DefaultFileExtension(const TCHAR *extension);  // if there's no file extension use the default
    CString &     DefaultPath(const TCHAR *basepath);            // if there's no path use the default
    void          AppendPath(const TCHAR *text);                 // append a partial path
    CString &     StripFilename(void);                          // remove the filename from a path
    CString &     StripPath(void );                              // remove the path from the filename
    void          ExtractFilePath(CString &dest) const;           // copy the file path to another string
    void          ExtractFileName(CString &dest) const;           // copy the filename to another string
    void          ExtractFileBase(CString &dest) const;           // copy the filename minus the extension to another string
    void          ExtractFileExtension(CString &dest) const;      // copy the file extension to another string
    bool          CheckExtension(const TCHAR *ext);

    // char * methods to replace library functions
    static size_t Length(const TCHAR *s);
    static TCHAR* ToLower(TCHAR *s);
    static TCHAR* ToUpper(TCHAR *s);
    static bool   IsNumeric(const TCHAR *s);
    static bool   HasLower(const TCHAR *s);
    static bool   HasUpper(const TCHAR *s);
    static int    Cmp(const TCHAR *s1, const TCHAR *s2);
    static int    Cmpn(const TCHAR *s1, const TCHAR *s2, size_t n);
    static int    Icmp(const TCHAR *s1, const TCHAR *s2);
    static int    Icmpn(const TCHAR *s1, const TCHAR *s2, size_t n);
    static int    IcmpPath( const TCHAR *s1, const TCHAR *s2 );         // compares paths and makes sure folders come first
    static int    IcmpnPath( const TCHAR *s1, const TCHAR *s2, int n ); // compares paths and makes sure folders come first
    static void   Append(TCHAR *dest, int size, const TCHAR *src);
    static void   Copynz(TCHAR *dest, const TCHAR *src, int destsize);
    static int    snPrintf(TCHAR *dest, int size, const TCHAR *fmt, ... ); // id_attribute((format(printf,3,4)));
    static int    vsnPrintf(TCHAR *dest, int size, const TCHAR *fmt, va_list argptr );
    static int    FindChar(const TCHAR *str, const TCHAR c, int start = 0, int end = -1 );
    static int    FindText(const TCHAR *str, const TCHAR *text, bool casesensitive = true, int start = 0, int end = -1 );
    static bool   Filter(const TCHAR *filter, const TCHAR *name, bool casesensitive );
    static void   StripMediaName( const TCHAR *name, CString &mediaName );
    static bool   CheckExtension( const TCHAR *name, const TCHAR *ext );
    static const char * FloatArrayToString( const float *array, const int length, const int precision );

    // hash keys
    static unsigned int  Hash(const TCHAR *string );
    static unsigned int  Hash(const TCHAR *string, size_t length);
    static unsigned int  IHash(const TCHAR *string );                    // case insensitive
    static unsigned int  IHash(const TCHAR *string, size_t length);        // case insensitive

    // character methods
    static TCHAR   ToLower(TCHAR c);
    static TCHAR   ToUpper(TCHAR c);
    static bool   CharIsPrintable( int c );
    static bool   CharIsLower( int c );
    static bool   CharIsUpper( int c );
    static bool   CharIsAlpha( int c );
    static bool   CharIsNumeric( int c );
    static bool   CharIsNewLine( char c );
    static bool   CharIsTab( char c );

    friend int    sprintf(CString &dest, const TCHAR *fmt, ... );
    friend int    vsprintf(CString &dest, const TCHAR *fmt, va_list ap );

    void          ReAllocate(size_t amount, bool keepold );             // reallocate string data buffer
    void          FreeData( void );                                   // free allocated string memory

                  // format value in the given measurement with the best unit, returns the best unit
    int           BestUnit( const TCHAR *format, float value, Measure_t measure );
                  // format value in the requested unit and measurement
    void          SetUnit( const TCHAR *format, float value, int unit, Measure_t measure );

    int                 DynamicMemoryUsed() const;
    static CString      FormatNumber(int number);

protected:
    size_t              m_len;
    TCHAR *             m_data;
    size_t              m_alloced;
    TCHAR               m_baseBuffer[ STR_ALLOC_BASE ];

    void                Init(void);                                       // initialize string using base buffer
    void                EnsureAlloced(size_t chars, bool keepold = true);   // ensure string data buffer is large anough
};

//char *                  va( const char *fmt, ... ) id_attribute((format(printf,1,2)));


__forceinline void
CString::EnsureAlloced(size_t chars, bool keepold)
{
    if ((chars * sizeof(TCHAR)) > m_alloced)
    {
        ReAllocate(chars * sizeof(TCHAR), keepold);
    }
}

__forceinline void
CString::Init(void)
{
    m_len = 0;
    m_alloced = STR_ALLOC_BASE * sizeof(TCHAR);
    m_data = m_baseBuffer;
    m_data[0] = _T('\0');
}

__forceinline
CString::CString(void)
{
    Init();
}

__forceinline
CString::CString(const CString &text)
{
    int l;

    Init();
    l = text.Length();
    EnsureAlloced(l + 1);
    _tcscpy(m_data, text.m_data);
    m_len = l;
}

__forceinline
CString::CString(const CString &text, int start, int end)
{
    int i;
    int l;

    Init();

    if (end < 0) {
        end = 0;
    }
    if (start < 0) {
        start = 0;
    }

    if (start > end)
    {
        start = 0;
        end = 0;
    }

    if (end > (int)text.Length())
    {
        end = text.Length();
    }

    if (start > (int)text.Length())
    {
        start = text.Length();
    }
    else if (start < 0)
    {
        start = 0;
    }

    l = end - start;

    if (l < 0)
    {
        l = 0;
    }

    EnsureAlloced(l + 1);

    for (i = 0; i < l; i++)
    {
        m_data[i] = text.m_data[start + i];
    }

    m_data[l] = _T('\0');
    m_len = l;
}

__forceinline
CString::CString(const TCHAR *text)
{

    Init();
    if (text)
    {
        size_t l = _tcslen(text);
        EnsureAlloced(l + 1);
        _tcscpy(m_data, text);
        m_len = l;
    }
}

__forceinline
CString::CString(const TCHAR *text, int start, int end)
{
    int l = (int)_tcslen(text);

    Init();

    if (l < 0)
    {
        l = 0;
    }

    if (end > l)
    {
        end = l;
    }
    if (start > l)
    {
        start = l;
    }
    else if (start < 0)
    {
        start = 0;
    }

    l = end - start;
    if (l < 0)
    {
        l = 0;
    }

    EnsureAlloced(l + 1);

    /*for (size_t i = 0; i < l; i++)
    {
        m_data[i] = text[start + i];
    }*/

    memcpy(m_data, text + start, l * sizeof(TCHAR));

    m_data[l] = _T('\0');
    m_len = l;
}

__forceinline
CString::CString(const char *text, size_t size)
{
    Init();

    EnsureAlloced(size + 1);

    for (size_t i = 0, j = size; i < j; i++)
    {
        m_data[i] = (TCHAR)text[i];
    }
    m_data[size] = 0;
    m_len = size;
}

__forceinline
CString::CString(const bool b)
{
    Init();
    EnsureAlloced(2);
    m_data[0] = b ? _T('1') : _T('0');
    m_data[1] = _T('\0');
    m_len = 1;
}

__forceinline
CString::CString(const TCHAR c) {
    Init();
    EnsureAlloced(2);
    m_data[0] = c;
    m_data[1] = '\0';
    m_len = 1;
}

__forceinline
CString::CString(const int i)
{
    TCHAR text[64];

    Init();
    size_t l = _sntprintf(text, sizeof(text) / sizeof(TCHAR), _T("%d"), i);
    EnsureAlloced(l + 1);
    _tcscpy(m_data, text );
    m_len = l;
}

__forceinline
CString::CString(const unsigned u)
{
    TCHAR text[64];

    Init();
    size_t l = _sntprintf(text, sizeof(text) / sizeof(TCHAR), _T("%u"), u);
    EnsureAlloced(l + 1);
    _tcscpy(m_data, text);
    m_len = l;
}

/*__forceinline
CString::CString( const float f )
{
    char text[ 64 ];
    int l;

    Init();
    l = CString::snPrintf( text, sizeof( text ), "%f", f );
    while( l > 0 && text[l-1] == '0' ) text[--l] = '\0';
    while( l > 0 && text[l-1] == '.' ) text[--l] = '\0';
    EnsureAlloced( l + 1 );
    strcpy( data, text );
    len = l;
}*/

__forceinline
CString::~CString(void)
{
    FreeData();
}

__forceinline size_t
CString::Size(void) const
{
    return sizeof(*this) + Allocated();
}


/*__forceinline const TCHAR *
CString::operator=( const CString &text )
{
    size_t l = text.m_len;
    EnsureAlloced(l + 1, false);
    memcpy(m_data, text.m_data, l * sizeof(TCHAR) );
    m_data[l] = _T('\0');
    m_len = l;

    return c_str();
}*/

__forceinline CString&
CString::operator=( const CString &text )
{
    size_t l = text.m_len;
    EnsureAlloced(l + 1, false);
    memcpy(m_data, text.m_data, l * sizeof(TCHAR) );
    m_data[l] = _T('\0');
    m_len = l;

    return *this;
}


__forceinline CString &
CString::operator+=( const float a ) {
    TCHAR    text[64];

    size_t l = _stprintf(text, _T("%f"), a);
    Append(text, l);

    return *this;
}

__forceinline CString &
CString::operator+=( const int a )
{
    TCHAR    text[64];
    size_t l = _stprintf( text, _T("%d"), a );
    Append(text, l);
    return *this;
}

__forceinline CString &
CString::operator+=( const unsigned a ) {
    TCHAR    text[64];

    size_t l = _stprintf( text, _T("%u"), a );
    Append(text, l);
    return *this;
}

__forceinline CString &
CString::operator+=(const CString &a)
{
    Append(a);
    return *this;
}

__forceinline CString &
CString::operator+=(const TCHAR *a)
{
    Append(a);
    return *this;
}

__forceinline CString &CString::operator+=( const TCHAR a )
{
    Append(a);
    return *this;
}

__forceinline CString &CString::operator+=( const bool a )
{
    //Append(a ? _T("true") : _T("false"));
    if (a) {
        Append(_T("true"), sizeof("true"));
    } else {
        Append(_T("false"), sizeof("false"));
    }
    return *this;
}

__forceinline bool 
operator==(const CString &a, const CString &b)
{
    return (!CString::Cmp( a.m_data, b.m_data ) );
}

__forceinline bool 
operator==( const CString &a, const TCHAR *b )
{
    //assert( b );
    return (!CString::Cmp( a.m_data, b ) );
}

__forceinline bool 
operator==(const TCHAR *a, const CString &b)
{
    //assert( a );
    return (!CString::Cmp(a, b.m_data ) );
}

__forceinline bool
operator!=(const CString &a, const CString &b)
{
    return !( a == b );
}

__forceinline bool
operator!=(const CString &a, const TCHAR *b )
{
    return !( a == b );
}

__forceinline bool
operator!=( const TCHAR *a, const CString &b )
{
    return !( a == b );
}

__forceinline
int CString::Cmp( const TCHAR *text ) const {
    //assert( text );
    return CString::Cmp(m_data, text );
}

__forceinline int
CString::Cmpn( const TCHAR *text, size_t n ) const
{
    //assert( text );
    return CString::Cmpn(m_data, text, n );
}

__forceinline int CString::CmpPrefix( const TCHAR *text ) const {
    //assert( text );
    return CString::Cmpn(m_data, text, _tcslen(text));
}

__forceinline int CString::Icmp( const TCHAR *text ) const {
    //assert( text );
    return CString::Icmp(m_data, text );
}

__forceinline int CString::Icmpn( const TCHAR *text, int n ) const {
    //assert( text );
    return CString::Icmpn(m_data, text, n );
}

__forceinline int CString::IcmpPrefix( const TCHAR *text ) const {
    //assert( text );
    return CString::Icmpn(m_data, text, _tcslen(text));
}

__forceinline size_t CString::Allocated(void) const {
    if (m_data != m_baseBuffer)
    {
        return m_alloced;
    }
    else
    {
        return 0;
    }
}

__forceinline void CString::Empty( void ) {
    EnsureAlloced(1 * sizeof(TCHAR));
    m_data[0] = _T('\0');
    m_len = 0;
}

__forceinline bool CString::IsEmpty( void ) const {
    return (CString::Cmp( m_data, _T("")) == 0 );
}

__forceinline void CString::Clear( void ) {
    FreeData();
    Init();
}

__forceinline void
CString::Append(const TCHAR a)
{
    EnsureAlloced( m_len + 2);
    m_data[m_len] = a;
    m_len++;
    m_data[m_len] = _T('\0');
}

__forceinline void
CString::Append(const CString &text)
{
    size_t newLen = text.Length();

    newLen += m_len;

    EnsureAlloced(newLen + 1);
    memcpy(m_data + m_len, text.m_data, text.m_len * sizeof(TCHAR));
    /*for (size_t i = 0; i < text.m_len; i++ ) {
        m_data[m_len + i] = text[i];
    }*/
    m_len = newLen;
    m_data[m_len] = _T('\0');
}

__forceinline void
CString::Append(const TCHAR *text)
{
    int newLen;

    if (text)
    {
        size_t textLeng = _tcslen(text);
        newLen = m_len + textLeng;
        EnsureAlloced(newLen + 1);
        /*for (size_t i = 0; text[i]; i++)
        {
            m_data[m_len + i] = text[i];
        }*/
        memcpy(m_data + m_len, text, textLeng * sizeof(TCHAR));
        m_len = newLen;
        m_data[m_len] = _T('\0');
    }
}

__forceinline void
CString::Append(const TCHAR *text, int l)
{
    if (text && l )
    {
        size_t newLen = m_len + l;
        EnsureAlloced(newLen + 1);
        memcpy(m_data + m_len, text, l * sizeof(TCHAR));
        /*for (size_t i = 0; text[ i ] && i < l; i++ ) {
            m_data[m_len + i] = text[i];
        }*/
        m_len = newLen;
        m_data[m_len] = '\0';
    }
}

__forceinline void
CString::Insert(const TCHAR a, int index)
{
    if (index < 0 )
    {
        index = 0;
    }
    else if (index > (int)m_len)
    {
        index = m_len;
    }

    EnsureAlloced(m_len + 1 + 1);
    for (int i = (int)m_len; i >= index; i-- )
    {
        m_data[i + 1] = m_data[i];
    }

    m_data[index] = a;
    m_len++;
}

__forceinline void
CString::Insert(const TCHAR *text, int index)
{
    if (index < 0)
    {
        index = 0;
    }
    else if (index > (int)m_len)
    {
        index = m_len;
    }

    size_t l = _tcslen(text);
    EnsureAlloced(m_len + l + 1);
    for (int i = (int)m_len; i >= index; i-- ) {
        m_data[i+l] = m_data[i];
    }
    for (size_t i = 0; i < l; i++ )
    {
        m_data[index+i] = text[i];
    }
    m_len += l;
}

__forceinline void
CString::ToLower(void)
{
    /*for (size_t i = 0; m_data[i]; i++ )
    {
        if (CharIsUpper(m_data[i]))
        {
            m_data[i] += ( _T('a') - _T('A') );
        }
    }*/

    for (size_t i = 0; i < this->m_len; i++) {
        m_data[i] = m_data[i] - 'A' + 'a';
    }
}

__forceinline void
CString::ToUpper(void)
{
    /*for (size_t i = 0; m_data[i]; i++ )
    {
        if (CharIsLower(m_data[i]))
        {
            m_data[i] -= ( _T('a') - _T('A') );
        }
    }*/
    for (size_t i = 0; i < this->m_len; i++) {
        m_data[i] = m_data[i] - 'a' + 'A';
    }
}

__forceinline bool CString::IsNumeric( void ) const {
    return CString::IsNumeric(m_data );
}

__forceinline bool CString::HasLower( void ) const {
    return CString::HasLower(m_data );
}

__forceinline bool CString::HasUpper( void ) const {
    return CString::HasUpper(m_data );
}

__forceinline void CString::CapLength(size_t newlen)
{
    if ( m_len <= newlen ) {
        return;
    }
    m_data[newlen] = 0;
    m_len = newlen;
}

__forceinline void CString::Fill( const TCHAR ch, int newlen ) {
    EnsureAlloced( (newlen + 1) * sizeof(TCHAR) );
    m_len = newlen;
    //memset(m_data, ch, m_len );
    m_data[m_len] = 0;
}

__forceinline int CString::Find( const TCHAR c, int start, int end ) const {
    if ( end == -1 ) {
        end = m_len;
    }
    return CString::FindChar(m_data, c, start, end );
}

__forceinline int CString::Find( const TCHAR *text, bool casesensitive, int start, int end ) const {
    if ( end == -1 ) {
        end = m_len;
    }
    return CString::FindText(m_data, text, casesensitive, start, end );
}

__forceinline bool CString::Filter( const TCHAR *filter, bool casesensitive ) const {
    return CString::Filter( filter, m_data, casesensitive );
}

__forceinline const TCHAR *
CString::Left(size_t len, CString &result) const
{
    return Mid( 0, len, result );
}

__forceinline const TCHAR *
CString::Right(size_t len, CString &result) const {
    if ( len >= Length() ) {
        result = *this;
        return result;
    }
    return Mid( Length() - len, len, result );
}

__forceinline void CString::Strip( const TCHAR c ) {
    StripLeading( c );
    StripTrailing( c );
}

__forceinline void CString::Strip( const TCHAR *string ) {
    StripLeading( string );
    StripTrailing( string );
}

__forceinline bool CString::CheckExtension( const TCHAR *ext ) {
    return CString::CheckExtension( m_data, ext );
}

__forceinline size_t CString::Length(const TCHAR *s) {
    size_t i = 0;
    for (; s[i]; i++ ) {}
    return i;
}

__forceinline TCHAR *CString::ToLower( TCHAR *s ) {
    for ( size_t i = 0; s[i]; i++ ) {
        if ( CharIsUpper( s[i] ) ) {
            s[i] += ( _T('a') - _T('A') );
        }
    }
    return s;
}

__forceinline TCHAR *CString::ToUpper( TCHAR *s ) {
    for ( size_t i = 0; s[i]; i++ ) {
        if ( CharIsLower( s[i] ) ) {
            s[i] -= ( _T('a') - _T('A') );
        }
    }
    return s;
}

__forceinline unsigned int CString::Hash( const TCHAR *string ) {
    size_t hash = 0;
    for (size_t i = 0; *string != _T('\0'); i++ )
    {
        hash += ( *string++ ) * ( i + 119 );
    }
    return hash;
}

__forceinline unsigned int CString::Hash( const TCHAR *string, size_t length )
{
    size_t hash = 0;
    for (size_t i = 0; i < length; i++ ) {
        hash += ( *string++ ) * ( i + 119 );
    }
    return hash;
}

__forceinline unsigned int CString::IHash( const TCHAR *string ) {
    size_t hash = 0;
    for (size_t i = 0; *string != _T('\0'); i++ ) {
        hash += ToLower( *string++ ) * ( i + 119 );
    }
    return hash;
}

__forceinline unsigned int CString::IHash( const TCHAR *string, size_t length ) {
    size_t hash = 0;
    for (size_t i = 0; i < length; i++ ) {
        hash += ToLower( *string++ ) * ( i + 119 );
    }
    return hash;
}

__forceinline TCHAR CString::ToLower( TCHAR c ) {
    if ( c <= _T('Z') && c >= _T('A') ) {
        return ( c + ( _T('a') - _T('A') ) );
    }
    return c;
}

__forceinline TCHAR CString::ToUpper( TCHAR c ) {
    if ( c >= 'a' && c <= 'z' ) {
        return ( c - ( 'a' - 'A' ) );
    }
    return c;
}

__forceinline bool CString::CharIsPrintable( int c ) {
    // test for regular ascii and western European high-ascii chars
    return ( c >= 0x20 && c <= 0x7E ) || ( c >= 0xA1 && c <= 0xFF );
}

__forceinline bool CString::CharIsLower( int c ) {
    // test for regular ascii and western European high-ascii chars
    return ( c >= 'a' && c <= 'z' ) || ( c >= 0xE0 && c <= 0xFF );
}

__forceinline bool CString::CharIsUpper( int c ) {
    // test for regular ascii and western European high-ascii chars
    return ( c <= 'Z' && c >= 'A' ) || ( c >= 0xC0 && c <= 0xDF );
}

__forceinline bool CString::CharIsAlpha( int c ) {
    // test for regular ascii and western European high-ascii chars
    return ( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) ||
             ( c >= 0xC0 && c <= 0xFF ) );
}

/*__forceinline bool CString::CharIsNumeric( int c ) {
    return ( c <= '9' && c >= '0' );
}

__forceinline bool CString::CharIsNewLine( TCHAR c ) {
    return ( c == _T('\n') || c == _T('\r') || c == _T('\v') );
}

__forceinline bool CString::CharIsTab( TCHAR c ) {
    return ( c == _T('\t') );
}

__forceinline int CString::ColorIndex( int c ) {
    return ( c & 15 );
}

__forceinline int CString::DynamicMemoryUsed() const {
    return ( m_data == m_baseBuffer ) ? 0 : m_alloced;
}*/

#endif /* __CSTRING_H__ */
