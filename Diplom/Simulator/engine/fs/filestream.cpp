/***************************************************************************************\
*   File:                                                                               *
*       filestream.cpp                                                                  *
*                                                                                       *
*   Abstract:                                                                           *
*       file streams support routines                                                   *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 11-Mar-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 43 $
#include "stdafx.h"
#include "..\fs\inc\filestream.h"

#define FILE_CACHE_SIZE  0x8000
BOOL
CPhysicalFile::Init()
{
    this->m_ulCurrentPosition = 0;
    this->m_ulOverallSize = 0;
    this->m_hFile = 0;

    this->m_cbBufLen = 0;
    this->m_ib = 0;

    return TRUE;
}

VOID
CPhysicalFile::Free()
{
    if (this->m_hFile != 0 && this->m_hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(this->m_hFile);
    }
}

const TCHAR *
CPhysicalFile::GetName(void)
{
    return this->m_tcFileName;
}

const TCHAR *
CPhysicalFile::GetFullPath(void)
{
    return this->m_tcFileFullName;
}

SIZE_T
CPhysicalFile::Read(void *buffer, SIZE_T len)
{
    if (this->m_hFile == 0 || this->m_hFile == INVALID_HANDLE_VALUE)
        return 0;

    ULONG cbReadRequestSize = len;
    ULONG cbT, cbRead;

    while (len > 0)
    {
        // Assert if we are beyond the bufferlen and Not sizeof(this->ab) which
        // would imply a seek happened...
        //ASSERT((this->ib <= this->cbBufLen) || (this->ib == sizeof(this->ab)));

        if (this->m_ib < this->m_cbBufLen)
        {
            cbT = this->m_cbBufLen - this->m_ib;

            if (cbT > len)
                cbT = len;

            memcpy(buffer, &this->m_cache[this->m_ib], cbT);
            this->m_ib += cbT;
            len -= cbT;

            this->m_ulCurrentPosition += cbT;

            if (len == 0)
                break;

            (BYTE *&)buffer += cbT;
        }

        // Buffer's empty.  Handle rest of large reads directly...
        //
        if (len > sizeof(this->m_cache))
        {
            cbT = len - len % sizeof(this->m_cache);
            if (!ReadFile(this->m_hFile, buffer, cbT, &cbRead, NULL))
            {
                return 0;
            }

            this->m_ulCurrentPosition += cbRead;

            len -= cbRead;
            (BYTE *&)buffer += cbRead;

            if (cbT != cbRead)
                break;          // end of file
        }

        if (len == 0)
            break;

        // was the last read a partial read?  if so we are done
        //
        if (this->m_cbBufLen > 0 && this->m_cbBufLen < sizeof(this->m_cache))
        {
            break;
        }

        // Read an entire buffer's worth.  We may try to read past EOF,
        // so we must only check for != 0...
        //
        if (!ReadFile(this->m_hFile, this->m_cache, sizeof(this->m_cache), &cbRead, NULL))
        {
            return 0;
        }

        if (cbRead == 0)
            break;

        this->m_ulCurrentPosition += cbRead;

        this->m_ib = 0;
        this->m_cbBufLen = cbRead;
    }

    return cbReadRequestSize - len;
}

SIZE_T
CPhysicalFile::Write(const void *buffer, int len)
{
    this->m_ib = sizeof(this->m_cache);
    this->m_cbBufLen = 0;     // Say we have not read it yet.

    if (this->m_hFile == 0 || this->m_hFile == INVALID_HANDLE_VALUE)
        return 0;

    DWORD   dwWritten = (DWORD)len;

    if (WriteFile(
        this->m_hFile,
        buffer,
        dwWritten,
        &dwWritten,
        NULL) != TRUE)
    {
        return 0;
    }

    this->m_ulCurrentPosition += dwWritten;

    return dwWritten;
}

SIZE_T
CPhysicalFile::Length(void) const
{
    return this->m_ulOverallSize;
}

ULONG
CPhysicalFile::Tell(void) const
{
    return this->m_ulCurrentPosition;
}

void
CPhysicalFile::Flush(void)
{
    ::FlushFileBuffers(this->m_hFile);
}

int
CPhysicalFile::Seek(long offset, fsOrigin_t origin)
{
    this->m_ib = sizeof(this->m_cache);
    this->m_cbBufLen = 0;     // Say we have not read it yet.

    if (origin == FS_SEEK_SET)
    {
        if ((offset < 0) || 
            ((SIZE_T)offset > this->m_ulOverallSize))
        {
            return -1;
        }

        this->m_ulCurrentPosition = offset;

        ::SetFilePointer(this->m_hFile, offset, NULL, FILE_BEGIN); 
        return 0;
    }

    if (origin == FS_SEEK_END)
    {
        if ((offset < 0) || 
            ((this->m_ulOverallSize - offset) < 0))
        {
            return -1;
        }

        this->m_ulCurrentPosition = this->m_ulOverallSize - offset;

        ::SetFilePointer(this->m_hFile, offset, NULL, FILE_END); 
        return 0;
    }

    if (origin == FS_SEEK_CUR)
    {
        if (((this->m_ulCurrentPosition + offset) > this->m_ulOverallSize) || 
            ((this->m_ulCurrentPosition + offset) < 0))
        {
            return -1;
        }

        this->m_ulCurrentPosition += offset;

        ::SetFilePointer(this->m_hFile, offset, NULL, FILE_CURRENT); 
    }

    return -1;
}

void
CPhysicalFile::Rewind(void)
{
    this->m_ib = sizeof(this->m_cache);
    this->m_cbBufLen = 0;     // Say we have not read it yet.

    this->m_ulCurrentPosition = 0;
    ::SetFilePointer(this->m_hFile, 0, NULL, FILE_BEGIN); 
}
