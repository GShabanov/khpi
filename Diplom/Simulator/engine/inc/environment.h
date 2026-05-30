/***************************************************************************************\
*   File:                                                                               *
*       environment.h                                                                   *
*                                                                                       *
*   Abstract:                                                                           *
*       shared globals environment values                                               *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 13-Jan-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 68 $
#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

class CGfxData
{
private:
    LONG                     m_lMouseX;
    LONG                     m_lMouseY;
    BOOL                     m_bMouseVisible;

	RECT                     m_Rect;

public:
	LARGE_INTEGER             m_DrawPerfomance;

    DWORD                     m_dwPerfomanceSec;
    DWORD                     m_dwPerfomanceMsec;

    float                     m_fi;

public:
    CGfxData()
    {
        m_lMouseX = 0;
        m_lMouseY = 0;
        m_bMouseVisible = FALSE;

        m_fi = 0.0;

		m_DrawPerfomance.HighPart = 0;
		m_DrawPerfomance.LowPart = 0;

        m_dwPerfomanceSec = 0;
        m_dwPerfomanceMsec = 0;
    }

    void SetMousePos(LONG mouseX, LONG mouseY, BOOL mouseVisiable);

    __inline void SetRect(RECT *lpRect)
    {
        this->m_Rect = *lpRect;
    }

    LONG GetMaxX() { return this->m_Rect.right - this->m_Rect.left; };
    LONG GetMaxY() { return this->m_Rect.bottom - this->m_Rect.top; };
    LONG GetMinX() { return this->m_Rect.left; };
    LONG GetMinY() { return this->m_Rect.top; };

    __inline LONG GetMouseX() { return this->m_lMouseX; };
    __inline LONG GetMouseY() { return this->m_lMouseY; };
    __inline LONG IsMouseVisible() { return this->m_bMouseVisible; };

};

class CSfxData
{
public:
    BOOL    n_bReverse;

    CSfxData()
    {
        n_bReverse = FALSE;
    }
};

class CSharedEnvironment 
{
    class IScene            *m_pCurrentScene;
    BOOL                     m_bLogsActive;
    BOOL                     m_bMouseLDown;
    BOOL                     m_bMouseLClick;

public:

    CGfxData                 m_gfxInfo;
    CSfxData                 m_sfxInfo;
    CSharedEnvironment()
        : m_gfxInfo()
        , m_sfxInfo()
        , m_bLogsActive(TRUE)
    {
        m_bMouseLDown = FALSE;
        m_bMouseLClick = FALSE;
    }

    BOOL Initialize();
    void Finalize();

    BOOL LoadScene(int nNumber);
    BOOL LockExclusive();
    BOOL LockShared();

    void Update();
    void Unlock();

    void SetMouseLDown(BOOL bDown);

    __inline class IScene *GetCurrentScene()
    {
        return this->m_pCurrentScene;
    }

    __inline BOOL GetMouseLClick() { return this->m_bMouseLClick; };

    __inline BOOL GetMouseLDown() { return this->m_bMouseLDown; };

    __inline BOOL IsLogsActive() { return m_bLogsActive; };
};

#endif // __ENVIRONMENT_H__
