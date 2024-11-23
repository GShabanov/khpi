#pragma once

class CLogCallback
{
public:
    enum LOG_TYPE {
        LOG_GENERAL = 0,
        LOG_GL,
        LOG_SERIAL
    };

    CLogCallback()
        : m_logTypeDefault(CLogCallback::LOG_GENERAL)
    {
    }

    CLogCallback(LOG_TYPE logType)
        : m_logTypeDefault(logType)
    {

    }

    virtual void LogMessage(LOG_TYPE logType, const TCHAR* format, va_list va) = 0;

protected:
    virtual void LogMessage(const TCHAR* format, ...) = 0;
    virtual void LogMessage(LOG_TYPE logType, const TCHAR* format, ...) = 0;

    LOG_TYPE   m_logTypeDefault;

};

