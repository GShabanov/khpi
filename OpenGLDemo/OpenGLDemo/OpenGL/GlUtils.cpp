
#include "pch.h"
#include "GlUtils.h"

BOOL
ReadToBuffer(const TCHAR* path, unsigned char** data, size_t* size)
{


    HANDLE  hFile =
        CreateFile(
            path,
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    DWORD            FileSizeHigh;
    DWORD            fileSize;
    DWORD            dwReaded;

    fileSize = GetFileSize(hFile, &FileSizeHigh);

    unsigned char* _data = new unsigned char[fileSize + 1];

    if (_data == NULL)
    {
        CloseHandle(hFile);
        return FALSE;
    }


    if (!ReadFile(hFile, _data, fileSize, &dwReaded, NULL))
    {
        delete[] _data;
        CloseHandle(hFile);
        return FALSE;
    }

    _data[dwReaded] = 0;

    CloseHandle(hFile);

    *data = _data;

    if (size != NULL)
        *size = (size_t)dwReaded;

    return TRUE;


}



const TCHAR*
GlSourceToStr(GLenum source)
{
    static  TCHAR string[20];

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        return _T("GL_DEBUG_SOURCE_API");

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        return _T("GL_DEBUG_SOURCE_WINDOW_SYSTEM");

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        return _T("GL_DEBUG_SOURCE_SHADER_COMPILER");

    case GL_DEBUG_SOURCE_THIRD_PARTY:
        return _T("GL_DEBUG_SOURCE_THIRD_PARTY");

    case GL_DEBUG_SOURCE_APPLICATION:
        return _T("GL_DEBUG_SOURCE_APPLICATION");

    case GL_DEBUG_SOURCE_OTHER:
        return _T("GL_DEBUG_SOURCE_OTHER");

    default:
        _stprintf(string, _T("0x%X"), source);
        return string;
    }

}

const TCHAR*
GlTypeToStr(GLenum type)
{
    static  TCHAR string[20];

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        return _T("GL_DEBUG_TYPE_ERROR");

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        return _T("GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR");

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        return _T("GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR");

    case GL_DEBUG_TYPE_PORTABILITY:
        return _T("GL_DEBUG_TYPE_PORTABILITY");

    case GL_DEBUG_TYPE_PERFORMANCE:
        return _T("GL_DEBUG_TYPE_PERFORMANCE");

    case GL_DEBUG_TYPE_OTHER:
        return _T("GL_DEBUG_TYPE_OTHER");

    case GL_DEBUG_TYPE_MARKER:
        return _T("GL_DEBUG_TYPE_MARKER");

    case GL_DEBUG_TYPE_PUSH_GROUP:
        return _T("GL_DEBUG_TYPE_PUSH_GROUP");

    case GL_DEBUG_TYPE_POP_GROUP:
        return _T("GL_DEBUG_TYPE_POP_GROUP");

    default:
        _stprintf(string, _T("0x%X"), type);
        return string;
    }

}

const TCHAR*
GlSeverityToStr(GLenum severity)
{
    static  TCHAR string[20];

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        return _T("GL_DEBUG_SEVERITY_NOTIFICATION");

    case GL_DEBUG_SEVERITY_HIGH:
        return _T("GL_DEBUG_SEVERITY_HIGH");

    case GL_DEBUG_SEVERITY_MEDIUM:
        return _T("GL_DEBUG_SEVERITY_MEDIUM");

    case GL_DEBUG_SEVERITY_LOW:
        return _T("GL_DEBUG_SEVERITY_LOW");

    default:
        _stprintf(string, _T("0x%X"), severity);
        return string;
    }

}

glm::vec3
CalcNormal(glm::vec3& vertex1, glm::vec3& vertex2, glm::vec3& vertex3)
{
    glm::vec3 vec_1 = vertex1 - vertex2;
    glm::vec3 vec_2 = vertex3 - vertex2;

    return glm::normalize(glm::cross(vec_1, vec_2));

    /*glm::vec3 normal;
    normal.x = vec_1.y * vec_2.z - vec_1.z * vec_2.y;
    normal.y = vec_1.z * vec_2.x - vec_1.x * vec_2.z;
    normal.z = vec_1.x * vec_2.y - vec_1.y * vec_2.x;

    return normal;*/
}

