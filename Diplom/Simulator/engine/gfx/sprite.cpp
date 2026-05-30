/***************************************************************************************\
*   File:                                                                               *
*       sprite.cpp                                                                      *
*                                                                                       *
*   Abstract:                                                                           *
*       sprite renderer                                                                 *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 26-Dec-2013                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Id:  $
#include "stdafx.h"
#include "inc\sprite.h"
#include "inc\texture.h"
#include "inc\surface.h"

BOOL
CSprite::Init()
{
    this->m_pTexture->Reference();
    return TRUE;
}

void
CSprite::Free()
{
    this->m_pTexture->Dereference();
}

ITexture *
CSprite::GetTextureReference()
{
    if (this->m_pTexture->Reference() != 0)
    {
        return this->m_pTexture;
    }
    else
    {
        return 0;
    }
}

void
CSprite::Draw(ISurface *surface, RECT *dimensions, BOOL drawBackground, COLORREF transparency)
{
    COLORREF *pTexture = (COLORREF *)this->m_pTexture->Reference();

    if (pTexture == 0)
        return;

    COLORREF bgColor = this->m_pTexture->BgColor();

    unsigned int textureWidth = this->m_pTexture->Width();
    unsigned int textureHeight = this->m_pTexture->Height();

    LONG drawWidth = 0;
    LONG drawHeight = 0;
    unsigned int left = 0;
    unsigned int top = 0;

    if (dimensions != NULL)
    {
        drawWidth = (LONG)(dimensions->right - dimensions->left);
        drawHeight = (LONG)(dimensions->bottom - dimensions->top);

        top = dimensions->top;
        left = dimensions->left;
    }
    else
    {
        drawWidth = textureWidth;
        drawHeight = textureHeight;
    }

    int yInc = 1;
    int xInc = 1;
    int vInc = textureWidth;
    int uInc = 1;

    if (drawWidth < 0) {
        xInc = -xInc;
        drawWidth = -drawWidth;
        //vInc = -vInc;
    }

    if (drawHeight < 0) {
        yInc = -yInc;
        drawHeight = -drawHeight;
        //uInc = -uInc;
    }

    //float dH = (float)textureHeight / (float)drawHeight;
    //float dW = (float)textureWidth / (float)drawWidth;

    int d2y_i = textureHeight << 1;
    int d2y_d = drawHeight << 1;

    int d2x_i = textureWidth << 1;
    int d2x_d = drawWidth << 1;

    int yError = -d2y_d;

    COLORREF *_yScan = pTexture;

    unsigned int tY = 0;

    for (LONG _y = 0; _y < drawHeight; _y++)
    {

        unsigned int tX = 0;
        int xError = -d2x_d;
        COLORREF *_xScan = _yScan;

        for (LONG _x = 0; _x < drawWidth; _x++)
        {

            COLORREF  color = *_xScan & 0x00FFFFFF;

            BOOL doDraw = TRUE;

            if (drawBackground != TRUE && color == bgColor)
            {
                doDraw = FALSE;
            }

            if (doDraw)
            {
                if (transparency > 0) {
                    surface->BlendPixel((unsigned int)_x + left, (unsigned int)_y + top, color, transparency);
                } else {
                    surface->SetPixel((unsigned int)_x + left, (unsigned int)_y + top, color);
                }
            }

            xError += d2x_i;

            //
            // fixup error value for texture X
            //
            while ((xError >= 0) && (tX <= (textureWidth)))
            {
                tX += xInc;
                _xScan += uInc;
                xError -= d2x_d;
            }
        }

        yError += d2y_i;

        //
        // fixup error value for texture Y
        //
        while ((yError >= 0) && (tY <= (textureHeight)))
        {
            tY += yInc;
            _yScan += vInc;
            yError -= d2y_d;
        }

    }

    this->m_pTexture->Dereference();
}

/*===============================================*/
/*      Sprite 3D implementation                 */
/*===============================================*/

BOOL
CSprite3D::Init()
{
    this->m_pTexture->Reference();
    return TRUE;
}

void
CSprite3D::Free()
{
    this->m_pTexture->Dereference();
}


ITexture *
CSprite3D::GetTextureReference()
{
    if (this->m_pTexture->Reference() != 0)
    {
        return this->m_pTexture;
    }
    else
    {
        return 0;
    }
}

/*VOID
CSprite3D::ConfigureSlopeY(IN VERTEX *point0, IN VERTEX *point1, IN SLOPE_PARAMY *slopeParameters)
{
    slopeParameters->error = 0;
    slopeParameters->x = point0->coord.x;
    slopeParameters->y = point0->coord.y;
    slopeParameters->dx = (LONG)(point1->coord.x - point0->coord.x);
    slopeParameters->dy = (LONG)(point1->coord.y - point0->coord.y);

    slopeParameters->xinc = 1;

    if (slopeParameters->dx < 0) {
        slopeParameters->xinc = -1;
        slopeParameters->dx = -slopeParameters->dx;
    }

    slopeParameters->yinc = 1;
    if (slopeParameters->dy < 0) {
        slopeParameters->yinc = -1;
        slopeParameters->dy = -slopeParameters->dy;
    }
}

void
CSprite3D::IterateSlopeY(IN SLOPE_PARAMY *slopeParameters)
{
    slopeParameters->y += slopeParameters->yinc;
    slopeParameters->error += (slopeParameters->dx << 1);

    while (slopeParameters->error > 0)
    {
        slopeParameters->error -= (slopeParameters->dy << 1);
        slopeParameters->x += slopeParameters->xinc;
    }
}*/

/*void
CSprite3D::ConfigureSlope(IN SLOPEPAIR *point0, IN SLOPEPAIR *point1, IN SLOPE_PARAM *slopeParameters)
{
    slopeParameters->base = point0->base;
    slopeParameters->result = point0->result;

    slopeParameters->goal = point1->result;
    slopeParameters->dbase = (LONG)(point1->base - point0->base);
    slopeParameters->dresult = (LONG)(point1->result - point0->result);

    slopeParameters->binc = 1;

    if (slopeParameters->dbase < 0) {
        slopeParameters->binc = -1;
        slopeParameters->dbase = -slopeParameters->dbase;
    }

    slopeParameters->rinc = 1;
    if (slopeParameters->dresult < 0) {
        slopeParameters->rinc = -1;
        slopeParameters->dresult = -slopeParameters->dresult;
    }

    slopeParameters->error = (slopeParameters->dresult << 1) - slopeParameters->dbase;
}

void
CSprite3D::IterateSlope(IN SLOPE_PARAM *slopeParameters)
{
    slopeParameters->base += slopeParameters->binc;
    slopeParameters->error += (slopeParameters->dresult << 1);

    while ((slopeParameters->error > 0) && (slopeParameters->result != slopeParameters->goal))
    {
        slopeParameters->error -= (slopeParameters->dbase << 1);
        slopeParameters->result += slopeParameters->rinc;
    }
}*/

void
CSprite3D::DrawHLine()
{
}

/*void
CSprite3D::DrawHLine()
{
    
}*/


void
CSprite3D::Draw(ISurface *surface, VERTEX *vertexes, BOOL drawBackground, COLORREF transparency)
{
    //SIZE_T  clSize = ObjSize();
    //SIZE_T  cSize = sizeof(*this);
    COLORREF *pTexture = (COLORREF *)this->m_pTexture->Reference();

    if (pTexture == 0)
        return;

    COLORREF bgColor = this->m_pTexture->BgColor();

    unsigned int textureWidth = this->m_pTexture->Width();
    unsigned int textureHeight = this->m_pTexture->Height();

    VERTEX vertex[3];

    vertex[0] = vertexes[0];
    vertex[1] = vertexes[1];
    vertex[2] = vertexes[2];

    //
    // sort points (high, middle, low)
    //
    if (vertex[0].coord.y > vertex[1].coord.y) { VERTEX temp = vertex[0]; vertex[0] = vertex[1]; vertex[1] = temp; };
    if (vertex[0].coord.y > vertex[2].coord.y) { VERTEX temp = vertex[0]; vertex[0] = vertex[2]; vertex[2] = temp; };
    if (vertex[1].coord.y > vertex[2].coord.y) { VERTEX temp = vertex[1]; vertex[1] = vertex[2]; vertex[2] = temp; };

    if (vertex[0].coord.y == vertex[2].coord.y) return;

    //----------------------------------
    //  drawing high and low part
    //----------------------------------
    //
    //     point[0]
    //      /---|
    //     /----| <---- high part of triangle
    //    /-----|
    // point[1] | <--- normale
    //    \*****|
    //     \****| <---- low part of triangle
    //      \***|
    //      point[2]
    //
    //
    // gain DX, DY delta and normalize value
    //
    //  vertex[0].z1 = 1 / (float)(vertex[0].coord.z + 320);
    //  vertex[0].uz = vertex[0].u / (vertex[0].coord.z + 320);
    //  vertex[0].vz = vertex[0].v / (vertex[0].coord.z + 320);
    //  k1 = (float)(y1 - vertex[0].coord.y) / (float)(vertex[2].coord.y - vertex[0].coord.y);
    //  k2 = (float)(y2 - vertex[1].coord.y) / (float)(vertex[2].coord.y - vertex[1].coord.y);
    //  z1_start = vertex[0].z1 + (vertex[2].z1 - vertex[0].z1) * k1;
    //  z1_end = vertex[1].z1 + (vertex[2].z1 - vertex[1].z1) * k2;
    //  z1 = z1_start
    //  uz = (float)(vertex[1].z1 + (vertex[2].z1 - vertex[1].z1) * k2 - vertex[0].z1 + (vertex[2].z1 - vertex[0].z1) * k1) / (float)(x_end - x_start);
    //  (LONG)(uz / z1)

    // посчитаем u/z, v/z, 1/z во всех вершинах
    vertex[0].z1 = 1 / (float)(vertex[0].coord.z + 320);
    vertex[1].z1 = 1 / (float)(vertex[1].coord.z + 320);
    vertex[2].z1 = 1 / (float)(vertex[2].coord.z + 320);

    vertex[0].uz = vertex[0].u; // * vertex[0].z1;
    vertex[0].vz = vertex[0].v; // * vertex[0].z1;
    vertex[1].uz = vertex[1].u; // * vertex[1].z1;
    vertex[1].vz = vertex[1].v; // * vertex[1].z1;
    vertex[2].uz = vertex[2].u; // * vertex[2].z1;
    vertex[2].vz = vertex[2].v; // * vertex[2].z1;

    // YX slope
    CBresenhame<LONG>::POINT  vertex0_YX = {vertex[0].coord.y, vertex[0].coord.x};
    CBresenhame<LONG>::POINT  vertex1_YX = {vertex[1].coord.y, vertex[1].coord.x};
    CBresenhame<LONG>::POINT  vertex2_YX = {vertex[2].coord.y, vertex[2].coord.x};
    // YZ slope
    CBresenhame<LONG>::POINT  vertex0_YZ = {vertex[0].coord.y, vertex[0].coord.z};
    CBresenhame<LONG>::POINT  vertex1_YZ = {vertex[1].coord.y, vertex[1].coord.z};
    CBresenhame<LONG>::POINT  vertex2_YZ = {vertex[2].coord.y, vertex[2].coord.z};
    // configure vectors slope with base Y

    CBresenhame<LONG> vector01_YX(&vertex0_YX, &vertex1_YX);
    CBresenhame<LONG> vector02_YX(&vertex0_YX, &vertex2_YX);
    CBresenhame<LONG> vector01_YZ(&vertex0_YZ, &vertex1_YZ); 
    CBresenhame<LONG> vector02_YZ(&vertex0_YZ, &vertex2_YZ);

    // UZ slope
    CBresenhame<LONG>::POINT  vertex0_YU = {vertex[0].coord.y, (LONG)vertex[0].u};
    CBresenhame<LONG>::POINT  vertex1_YU = {vertex[1].coord.y, (LONG)vertex[1].u};
    CBresenhame<LONG>::POINT  vertex2_YU = {vertex[2].coord.y, (LONG)vertex[2].u};
    // VZ slope
    CBresenhame<LONG>::POINT  vertex0_YV = {vertex[0].coord.y, (LONG)vertex[0].v};
    CBresenhame<LONG>::POINT  vertex1_YV = {vertex[1].coord.y, (LONG)vertex[1].v};
    CBresenhame<LONG>::POINT  vertex2_YV = {vertex[2].coord.y, (LONG)vertex[2].v};

    //
    // slope configuration 
    //
    CBresenhame<LONG> vector01_YU(&vertex0_YU, &vertex1_YU);
    CBresenhame<LONG> vector02_YU(&vertex0_YU, &vertex2_YU);
    CBresenhame<LONG> vector01_YV(&vertex0_YV, &vertex1_YV);
    CBresenhame<LONG> vector02_YV(&vertex0_YV, &vertex2_YV);

    LONG iterationsY = vector02_YX.getCount();

    while (iterationsY > 0)
    {
        //
        // recalculate coordinates for next slope
        //
        if (vector01_YX.base() == vertex[1].coord.y)
        {
            // reconfigure slopes for line 1-2
            vector01_YX(&vertex1_YX, &vertex2_YX);
            vector01_YZ(&vertex1_YZ, &vertex2_YZ);

            vector01_YU(&vertex1_YU, &vertex2_YU);
            vector01_YV(&vertex1_YV, &vertex2_YV);
        }

        LONG x_start = vector02_YX.result();
        LONG x_end = vector01_YX.result();

        LONG uz_start = vector02_YU.result();
        LONG uz_end = vector01_YU.result();

        LONG vz_start = vector02_YV.result();
        LONG vz_end = vector01_YV.result();

        //float k = (float)(line01_slopYX.base - vertex[0].coord.y) / (float)(vertex[2].coord.y - vertex[0].coord.y);
        //float uz_start = vertex[0].uz + (vertex[2].uz - vertex[0].uz) * k;
        //float vz_start = vertex[0].vz + (vertex[2].vz - vertex[0].vz) * k;
        //float z1_start = vertex[0].z1 + (vertex[2].z1 - vertex[0].z1) * k;
        //float uz_end;
        //float vz_end;
        //float z1_end;

        //if (line01_slopYX.base >= vertex[1].coord.y) {
            // мы находимся ниже вершины B, считаем x, u/z, v/z, 1/z для стороны BC
            //k = (float)(line01_slopYX.base - vertex[1].coord.y) / (float)(vertex[2].coord.y - vertex[1].coord.y);
            //uz_end = vertex[1].uz + (vertex[2].uz - vertex[1].uz) * k;
            //vz_end = vertex[1].vz + (vertex[2].vz - vertex[1].vz) * k;
            //z1_end = vertex[1].z1 + (vertex[2].z1 - vertex[1].z1) * k;
        //} else {
            // мы находимся выше вершины B, считаем x, u/z, v/z, 1/z для стороны AB
            //k = (float)(line01_slopYX.base - vertex[0].coord.y) / (float)(vertex[1].coord.y - vertex[0].coord.y);
            //uz_end = vertex[0].uz + (vertex[1].uz - vertex[0].uz) * k;
            //vz_end = vertex[0].vz + (vertex[1].vz - vertex[0].vz) * k;
            //z1_end = vertex[0].z1 + (vertex[1].z1 - vertex[0].z1) * k;
        //}

        CBresenhame<LONG>::POINT  lineX1_XU = {x_start, uz_start};
        CBresenhame<LONG>::POINT  lineX2_XU = {x_end, uz_end};
        CBresenhame<LONG>::POINT  lineX1_XV = {x_start, vz_start};
        CBresenhame<LONG>::POINT  lineX2_XV = {x_end, vz_end};

        CBresenhame<LONG> vector_XU(&lineX1_XU, &lineX2_XU);
        CBresenhame<LONG> vector_XV(&lineX1_XV, &lineX2_XV);

        LONG iterationsX = vector_XU.getCount();  //x_start > x_end ? x_start : x_end;

        while (iterationsX > 0)
        {

            LONG  textureV = vector_XV.result(); // (LONG)(vz /* / z1*/);
            LONG  textureU = vector_XU.result(); //(LONG)(uz /* / z1*/);

            if (textureV < 0) {
                textureV = textureHeight - textureV;
            }
            if (textureU < 0) {
                textureU = textureWidth - textureU;
            }

            COLORREF  color = *(pTexture + textureV * textureWidth + textureU) & 0x00FFFFFF;

            BOOL doDraw = (drawBackground != TRUE && color == bgColor) ? FALSE : TRUE;

            if (doDraw) {
                if (transparency > 0) {
                    surface->BlendPixel(vector_XU.base(), vector01_YX.base(), color, transparency);
                } else {
                    surface->SetPixel(vector_XU.base(), vector01_YX.base(), color);
                }
            }

            vector_XU.interate();
            vector_XV.interate();

            iterationsX--;
        }

        surface->SetPixel(vector01_YX.result(), vector01_YX.base(), RGB(255, 0, 0));
        surface->SetPixel(vector02_YX.result(), vector02_YX.base(), RGB(255, 0, 0));

        vector01_YX.interate();
        vector02_YX.interate();

        vector01_YU.interate();
        vector02_YU.interate();

        vector01_YV.interate();
        vector02_YV.interate();

        iterationsY--;
    }

    this->m_pTexture->Dereference();
}


void
CSprite3D::Draw4(ISurface *surface, VERTEX *vertexes, BOOL drawBackground, COLORREF transparency)
{
    UNREFERENCED_PARAMETER(drawBackground);
    UNREFERENCED_PARAMETER(transparency);
    COLORREF *pTexture = (COLORREF *)this->m_pTexture->Reference();

    if (pTexture == 0)
        return;

    unsigned int textureWidth = this->m_pTexture->Width();
    unsigned int textureHeight = this->m_pTexture->Height();

    VERTEX vertex[3];

    vertex[0] = vertexes[0];
    vertex[1] = vertexes[1];
    vertex[2] = vertexes[2];

    //
    // sort points (high, middle, low)
    //
    if (vertex[0].coord.y > vertex[1].coord.y) { VERTEX temp = vertex[0]; vertex[0] = vertex[1]; vertex[1] = temp; };
    if (vertex[0].coord.y > vertex[2].coord.y) { VERTEX temp = vertex[0]; vertex[0] = vertex[2]; vertex[2] = temp; };
    if (vertex[1].coord.y > vertex[2].coord.y) { VERTEX temp = vertex[1]; vertex[1] = vertex[2]; vertex[2] = temp; };

    if (vertex[0].coord.y == vertex[2].coord.y) return;

    //----------------------------------
    //  drawing high and low part
    //----------------------------------
    //
    //     point[0]
    //      /---|
    //     /----| <---- high part of triangle
    //    /-----|
    // point[1] | <--- normale
    //    \*****|
    //     \****| <---- low part of triangle
    //      \***|
    //      point[2]
    //
    //
    // gain DX, DY delta and normalize value
    //

    // посчитаем u/z, v/z, 1/z во всех вершинах
    vertex[0].z1 = 1 / (float)(vertex[0].coord.z + 320);
    vertex[0].uz = (vertex[0].u * vertex[0].z1);
    vertex[0].vz = (vertex[0].v * vertex[0].z1);

    vertex[1].z1 = 1 / (float)(vertex[1].coord.z + 320);
    vertex[1].uz = (vertex[1].u * vertex[1].z1);
    vertex[1].vz = (vertex[1].v * vertex[1].z1);

    vertex[2].z1 = 1 / (float)(vertex[2].coord.z + 320);
    vertex[2].uz = ((float)vertex[2].u * vertex[2].z1);
    vertex[2].vz = ((float)vertex[2].v * vertex[2].z1);

    for (LONG current_sy = vertex[0].coord.y; current_sy <= vertex[2].coord.y; current_sy++)
    {
        float k = (float)(current_sy - vertex[0].coord.y) / (float)(vertex[2].coord.y - vertex[0].coord.y);
        float x_start = vertex[0].coord.x + (float)(vertex[2].coord.x - vertex[0].coord.x) * k;
        float uz_start = vertex[0].uz + (vertex[2].uz - vertex[0].uz) * k;
        float vz_start = vertex[0].vz + (vertex[2].vz - vertex[0].vz) * k;
        float z1_start = vertex[0].z1 + (vertex[2].z1 - vertex[0].z1) * k;
        float x_end;
        float uz_end;
        float vz_end;
        float z1_end;

        if (current_sy >= vertex[1].coord.y) {
            // мы находимся ниже вершины B, считаем x, u/z, v/z, 1/z для стороны BC
            k = (float)(current_sy - vertex[1].coord.y) / (float)(vertex[2].coord.y - vertex[1].coord.y);
            x_end = vertex[1].coord.x + (float)(vertex[2].coord.x - vertex[1].coord.x) * k;
            uz_end = vertex[1].uz + (vertex[2].uz - vertex[1].uz) * k;
            vz_end = vertex[1].vz + (vertex[2].vz - vertex[1].vz) * k;
            z1_end = vertex[1].z1 + (vertex[2].z1 - vertex[1].z1) * k;
        } else {
            // мы находимся выше вершины B, считаем x, u/z, v/z, 1/z для стороны AB
            k = (float)(current_sy - vertex[0].coord.y) / (float)(vertex[1].coord.y - vertex[0].coord.y);
            x_end = vertex[0].coord.x + (float)(vertex[1].coord.x - vertex[0].coord.x) * k;
            uz_end = vertex[0].uz + (vertex[1].uz - vertex[0].uz) * k;
            vz_end = vertex[0].vz + (vertex[1].vz - vertex[0].vz) * k;
            z1_end = vertex[0].z1 + (vertex[1].z1 - vertex[0].z1) * k;
        }


        // x_start должен находиться левее x_end
        if (x_start > x_end) {
            float tmp;
            tmp = x_start; x_start = x_end; x_end = tmp;
            tmp = uz_start; uz_start = uz_end; uz_end = tmp;
            tmp = vz_start; vz_start = vz_end; vz_end = tmp;
            tmp = z1_start; z1_start = z1_end; z1_end = tmp;
        }

        float   duz = (float)(uz_end - uz_start) / (float)(x_end - x_start); // колличество вхождений UZ по оси X
        float   dvz = (float)(vz_end - vz_start) / (float)(x_end - x_start);
        float   dz1 = (float)(z1_end - z1_start) / (float)(x_end - x_start);

        float uz, vz, z1;
        // готовим u, v к текстурированию
        uz = uz_start;
        vz = vz_start;
        z1 = z1_start;

        for (float current_sx = x_start; current_sx <= x_end; current_sx++)
        {
            LONG  textureV = (LONG)(vz / z1);
            LONG  textureU = (LONG)(uz / z1);

            if (textureV < 0) {
                textureV = textureHeight - textureV;
            }
            if (textureU < 0) {
                textureU = textureWidth - textureU;
            }

            COLORREF  color = *(pTexture + textureV * textureWidth + textureU) & 0x00FFFFFF;

            surface->SetPixel((unsigned int)current_sx, (LONG)current_sy, color);

            uz += duz;
            vz += dvz;
            z1 += dz1;

        }

        surface->SetPixel((unsigned int)x_start, current_sy, RGB(255, 0, 0));
        surface->SetPixel((unsigned int)x_end, current_sy, RGB(255, 0, 0));
    }

    /*do
    {
        if (y == y1)
        {
            //if (y2 == vertex[1].coord.y) break;

            ConfigureSlope(&vertex[1], &vertex[2], &line01_slop);

            p1 = vertex[1].coord;
            line1e = 0;
        }

        surface->SetPixel(p1.x, y, RGB(255, 0, 0));
        surface->SetPixel(p2.x, y, RGB(255, 0, 0));
        //HLine32(hdc, p2.y, p1.x, p2.x, face->color);

        y++;

        line1e += line01_slop.dx;

        while (line1e > 0)
        {
            line1e -= line01_slop.dy;
            p1.x += line01_slop.xinc;
        }

        line2e += line02_slop.dx;

        while (line2e > 0)
        {
            line2e -= line02_slop.dy;
            p2.x += line02_slop.xinc;
        }

    }
    while (y <= y2);*/


    this->m_pTexture->Dereference();
}
