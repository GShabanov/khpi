/***************************************************************************************\
*   File:                                                                               *
*       sound.h                                                                         *
*                                                                                       *
*   Abstract:                                                                           *
*       base sound interface                                                            *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 4-Feb-2014                        *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 51 $
#ifndef __SOUND_H__
#define __SOUND_H__


class IWaveSource : public IDisposable
/*===================================================================================*/
/*                                                                                   */
/* Дескриптор хранимого звукового источника (фактически распарсенный звуковой файл)  */
/*                                                                                   */
/*===================================================================================*/
{
protected:

    SIZE_T          m_MemSize;       // size of the wave data in memory
    DWORD           m_dwSize;        // size in samples
    void           *m_pbData;

    waveformatextensible  m_Wfx;        // current format for data

public:
    IWaveSource(struct _waveformatextensible* pwfx)
        : IDisposable()
        , m_dwSize(0)        // size in samples
        , m_pbData(0)
        , m_MemSize(0)       // size of the wave data in memory
    {
        memset(&m_Wfx, 0, sizeof(waveformatextensible));

        if (pwfx != 0)
        {
            m_Wfx = *pwfx;
        }
    }

    virtual BOOL Init(class IDataStream* dataStream) = 0;

    virtual VOID Free() = 0;

    virtual void* Reference()
    {
        IDisposable::Reference();

        return m_pbData;
    }

    virtual const waveformatextensible& GetWaveFormat() const
    {
        return m_Wfx;
    }

    SIZE_T  GetMemorySize(void) { return m_MemSize; }
    DWORD   GetOutputSize(void) { return m_dwSize; }

};


class ISound : public IDisposable
/*===================================================================================*/
/*                                                                                   */
/* Дескриптор звука связанный со звуковой поверхностью и имеющий свои характеристики */
/*                                                                                   */
/*===================================================================================*/
{
protected:

    class IWaveSource      *m_pWaveSource;
    DWORD                   m_SeekBase;

public:
    ISound()
        : IDisposable()
        , m_pWaveSource(0)
        , m_SeekBase(0)
    {
    }

    LIST_ENTRY              m_ChainedEntry;

    virtual BOOL Init(class IWaveSource* waveSource) = 0;

    virtual DWORD Read(BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead) = 0;

    virtual int ResetFile(void) = 0;

    virtual int Close(void) = 0;

    virtual const waveformatextensible& GetWaveFormat() const
    {
        return m_pWaveSource->GetWaveFormat();
    }

};


class CMemoryWaveSource : public IWaveSource
/*===================================================================================*/
/*                                                                                   */
/* Дескриптор хранимого в памяти звука (псевдодескриптор)                            */
/*                                                                                   */
/*===================================================================================*/
{
private:

public:
    CMemoryWaveSource(struct _waveformatextensible* pwfx)
        : IWaveSource(pwfx)
    {
    }

    VOID* Reference();

    virtual BOOL   Init(class IDataStream* dataStream);
    virtual VOID   Free();
};


class CSoundWaveFile : public  ISound
/* ==================================================================================*/
/*                                                                                   */
/* Используем как эммуляцию псевдохранилища для MemoryWaveSource                     */
/*                                                                                   */
/*===================================================================================*/
{
private:

    int    ResampleSound();
    SIZE_T  m_DataCursor;

public:
    CSoundWaveFile::CSoundWaveFile(void)
        : ISound()
        , m_DataCursor(0)
    {

    }

    virtual BOOL   Init(class IWaveSource* waveSource);
    virtual VOID   Free();

    virtual DWORD  Read(BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead);
    virtual int    ResetFile();
    virtual int    Close(void);


private:

    int             OpenOGG(const char* strFileName, WAVEFORMATEX* pwfx = NULL);
    int             ReadOGG(BYTE* pBuffer, int dwSizeToRead, int *pdwSizeRead);
    int             CloseOGG(void);
};


#endif // __SOUND_H__