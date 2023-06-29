#pragma once


class CMFCNotify
{
public:
    virtual void ControlCallback(CWnd *from, void *data) = 0;
};
