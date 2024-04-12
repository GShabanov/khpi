/***************************************************************************************\
*   File:                                                                               *
*       filesystem.h                                                                    *
*                                                                                       *
*   Abstract:                                                                           *
*       file system manegment                                                           *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 16-Mar-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 51 $
#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

class IFileSystem : public IObject {

public:
    // modes for OpenFileByMode. used as bit mask internally
    typedef enum {
        FS_READ     = 0,
        FS_WRITE    = 1,
        FS_APPEND   = 2
    } fsMode_t;

protected:
    ~IFileSystem() {}




public:
    IFileSystem()
        : IObject()
    {
    }

    virtual BOOL Init() = 0;

    virtual BOOL Restart() = 0;
    
    // Opens a file for writing, will create any needed subdirectories.
    class IDataStream *OpenFileWrite(const char *relativePath, const char *basePath = "fs_savepath");
    // Opens a file for writing at the end.
    class IDataStream *OpenFileAppend(const char *filename, bool sync = false, const char *basePath = "fs_basepath");
    // Opens a file for reading from a full OS path.
    class IDataStream *OpenExplicitFileRead(const char *OSPath);
    // Opens a file for writing to a full OS path.
    class IDataStream *OpenExplicitFileWrite(const char *OSPath);

    // Opens a file for reading.
    virtual class IDataStream*
        OpenFileRead(const TCHAR* relativePath, int searchFlags, BOOL allowCopyFiles, const char* gamedir = NULL) = 0;

    // Opens a file for reading, writing, or appending depending on the value of mode.
    class IDataStream*
        OpenFileByMode(const char* relativePath, fsMode_t mode)
    {
        if (mode == FS_READ) {
            return OpenFileRead(relativePath, 0, TRUE);
        }
        if (mode == FS_WRITE) {
            return OpenFileWrite(relativePath);
        }
        if (mode == FS_APPEND) {
            return OpenFileAppend(relativePath, true);
        }

        return NULL;
    }
};

class CFileSystem : public IFileSystem
{
private:
public:
    CFileSystem()
        : IFileSystem()
    {
    }

    virtual BOOL   Init();
    virtual VOID   Free();

    virtual BOOL Restart();
    virtual class IDataStream* OpenFileRead(const TCHAR* relativePath, int searchFlags, BOOL allowCopyFiles, const char* gamedir);

};
#endif // __FILESYSTEM_H__
