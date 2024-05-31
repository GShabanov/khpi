#pragma once
#ifndef __OBJECT_H__
#define __OBJECT_H__


class CBaseObject
{
public:
    virtual BOOL Init() = 0;
    virtual void Draw(DWORD *canvas, CRect& canvasDimensions, CRect& drawRect) = 0;
    virtual void Update(double speedFactor) = 0;

};


class CMath
{
private:
    double* m_sinTable;

public:

    CMath() {
        m_sinTable = NULL;
    }

    BOOL Init()
    {

        m_sinTable = (double*)malloc(6283 * sizeof(double));

        if (m_sinTable)
        {
            for (DWORD i = 0; i < 6283; i++)
            {
                m_sinTable[i] = ::sin((double)i / 1000.0);
            }

            return TRUE;
        }

        return FALSE;

    }

    ~CMath()
    {
        free(m_sinTable);
    }

    double  sin(double rad)
    {
        bool changeSign = false;

        if (rad < 0)
        {
            rad = -rad;
            changeSign = true;
        }

        while (rad > 6.283)
        {
            rad -= 6.283;
        }

        rad *= 1000;

        return (changeSign ? -(m_sinTable[(DWORD)rad]) : m_sinTable[(DWORD)rad]);

    }

    double  cos(double rad)
    {

        return sin(rad + 1.5707);
    }

};

extern CMath    m_math;


#endif // __OBJECT_H__