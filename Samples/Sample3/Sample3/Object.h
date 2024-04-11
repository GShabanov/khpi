#pragma once
#ifndef __OBJECT_H__
#define __OBJECT_H__


class CBaseObject
{
public:
    virtual BOOL Init() = 0;
    virtual void Draw(CDC& dc, CRect& drawRect, CRect& boundingRect) = 0;
    virtual void Update(double speedFactor) = 0;

};

#endif // __OBJECT_H__