/***************************************************************************************\
*   File:                                                                               *
*       riffFile.h                                                                      *
*                                                                                       *
*   Abstract:                                                                           *
*       RIFF file format                                                                *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 11-Feb-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 43 $
#ifndef __REFFFILE_H__
#define __REFFFILE_H__

class CRiffWaveSource : public IWaveSource
{
private:
    //int    iResetFile();

    //DWORD  iRead(BYTE* pBuffer, DWORD dwSizeToRead, DWORD *pdwSizeRead);

    int SafeGetData(BYTE *ptr, PVOID pData, SIZE_T size);
    int ReadMMIO(void);


    unsigned char *   m_pPcmData;
    SIZE_T            m_tPcmDataSize;

public:
    CRiffWaveSource(struct _waveformatextensible* pwfx)
        : IWaveSource(pwfx)
    {
        //m_ResetFile     = (RESET_FILE_PFN)&CRiffWaveSource::iResetFile;
        //m_ReadFile      = (READ_FILE_PFN)&CRiffWaveSource::iRead;
    };

    virtual BOOL   Init(class IDataStream* pDataStream);
    virtual VOID   Free();
};

#endif // __REFFFILE_H__
