/***************************************************************************************\
*   File:                                                                               *
*       filestream.h                                                                    *
*                                                                                       *
*   Abstract:                                                                           *
*       file streams support routines                                                   *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 11-Mar-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 51 $
#ifndef __FILESTREAM_H__
#define __FILESTREAM_H__


class IDataStream : public IDisposable {
public:
    // mode parm for Seek
    typedef enum {
        FS_SEEK_CUR,
        FS_SEEK_END,
        FS_SEEK_SET
    } fsOrigin_t;

protected:


    IDataStream()
        : IDisposable()
    {
    }
public:

    // Get the name of the file.
    virtual const TCHAR *GetName(void) = 0;
    // Get the full file path.
    virtual const TCHAR *GetFullPath(void) = 0;
    // Read data from the file to the buffer.
    virtual SIZE_T Read(void *buffer, SIZE_T len) = 0;
    // Write data from the buffer to the file.
    virtual SIZE_T Write(const void *buffer, int len) = 0;
    // Returns the length of the file.
    virtual SIZE_T Length(void) const = 0;
    // Returns offset in file.
    virtual unsigned long Tell(void) const = 0;
    // Causes any buffered data to be written to the file.
    virtual void Flush(void) = 0;
    // Seek on a file.
    virtual int Seek(long offset, fsOrigin_t origin) = 0;
    // Go back to the beginning of the file.
    virtual void Rewind(void) = 0;
};

class CPhysicalFile : public IDataStream
{
    friend class CFileSystem;

private:

    TCHAR       *m_tcFileName;
    TCHAR       *m_tcFileFullName;
    ULONG_PTR    m_ulCurrentPosition;
    SIZE_T       m_ulOverallSize;

    ULONG        m_ib;
    ULONG        m_cbBufLen;       // length of buffer if reading
    BYTE         m_cache[4096];

    HANDLE       m_hFile;
public:
    CPhysicalFile()
        : IDataStream()
    {
    }

    virtual BOOL   Init();
    virtual VOID   Free();


    virtual const TCHAR* GetName(void);
    virtual const TCHAR* GetFullPath(void);
    virtual SIZE_T Read(void* buffer, SIZE_T len);
    virtual SIZE_T Write(const void* buffer, int len);
    virtual SIZE_T Length(void) const;
    virtual ULONG  Tell(void) const;
    virtual void Flush(void);
    virtual int Seek(long offset, fsOrigin_t origin);
    virtual void Rewind(void);
};

#endif // __FILESTREAM_H__
