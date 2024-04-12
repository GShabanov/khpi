/***************************************************************************************\
*   File:                                                                               *
*       filesystem.cpp                                                                  *
*                                                                                       *
*   Abstract:                                                                           *
*       file system manegment                                                           *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 16-Mar-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 43 $
#include "stdafx.h"
#include "..\fs\inc\filesystem.h"
#include "..\fs\inc\filestream.h"

BOOL
CFileSystem::Init()
{
    return TRUE;
}

VOID
CFileSystem::Free()
{
}

BOOL 
CFileSystem::Restart()
{
    return TRUE;
}

IDataStream *
CFileSystem::OpenFileRead(const TCHAR *relativePath,  int searchFlags, BOOL allowCopyFiles, const char* gamedir)
{
    UNREFERENCED_PARAMETER(gamedir);
    UNREFERENCED_PARAMETER(searchFlags);

    DWORD dwSharing = 0;

    if (allowCopyFiles)
    {
        dwSharing = FILE_SHARE_READ;
    }

    HANDLE fileHandle = ::CreateFile(
        (LPCSTR)relativePath,
        GENERIC_READ,
        dwSharing, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, NULL);

    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        return NULL;
    }

    CPhysicalFile *pFsStream = new CPhysicalFile();

    if (pFsStream == NULL)
    {
        CloseHandle(fileHandle);
        return NULL;
    }

    if (pFsStream->Init() != TRUE)
    {
        delete pFsStream;
        CloseHandle(fileHandle);
        return NULL;
    }

    DWORD dwFileSizeLow = 0;
    DWORD dwFileSizeHigh = 0;
    dwFileSizeLow = GetFileSize(fileHandle, &dwFileSizeHigh);

    pFsStream->m_hFile = fileHandle;
    pFsStream->m_ulOverallSize = dwFileSizeLow;

    pFsStream->m_tcFileFullName = (TCHAR *)relativePath;
    pFsStream->m_tcFileName = (TCHAR*)relativePath;

    return pFsStream;
}