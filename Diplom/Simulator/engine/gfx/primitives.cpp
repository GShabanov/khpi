#include "stdafx.h"
#include <math.h>
#include "inc\surface.h"
#include "inc\primitives.h"
#include "inc\vertex.h"

typedef struct _SLOPE_PARAMETERS {
    LONG  dx;
    LONG  dy;
    LONG  xinc;
} SLOPE_PARAMETERS, *PSLOPE_PARAMETERS;

VOID
ConfigureSlope(IN POINT *point0, IN POINT *point1, IN SLOPE_PARAMETERS *slopeParameters)
{
    slopeParameters->dx = (LONG)(point1->x - point0->x);

    slopeParameters->xinc = 1;

    if (slopeParameters->dx < 0) {
        slopeParameters->xinc = -1;
        slopeParameters->dx = -slopeParameters->dx;
    }

    slopeParameters->dx <<= 1;
    slopeParameters->dy = (LONG)((point1->y - point0->y) << 1);
}

VOID
TriangleTextured(ISurface *surface, Face *face)
{
    Vertex  *vertex[3];

    vertex[0] = face->v1;
    vertex[1] = face->v2;
    vertex[2] = face->v3;

    //
    // sort points (high, middle, low)
    //
    if (vertex[0]->point.y > vertex[1]->point.y) { Vertex *temp = vertex[0]; vertex[0] = vertex[1]; vertex[1] = temp; };
    if (vertex[0]->point.y > vertex[2]->point.y) { Vertex *temp = vertex[0]; vertex[0] = vertex[2]; vertex[2] = temp; };
    if (vertex[1]->point.y > vertex[2]->point.y) { Vertex *temp = vertex[1]; vertex[1] = vertex[2]; vertex[2] = temp; };

    // посчитаем u/z, v/z, 1/z во всех вершинах
    vertex[0]->z1 = 1 / ((float)vertex[0]->z + 320);
    vertex[0]->uz = (float)vertex[0]->u * vertex[0]->z1;
    vertex[0]->vz = (float)vertex[0]->v * vertex[0]->z1;
    vertex[1]->z1 = 1 / ((float)vertex[1]->z + 320);
    vertex[1]->uz = (float)vertex[1]->u * vertex[1]->z1;
    vertex[1]->vz = (float)vertex[1]->v * vertex[1]->z1;
    vertex[2]->z1 = 1 / ((float)vertex[2]->z + 320);
    vertex[2]->uz = (float)vertex[2]->u * vertex[2]->z1;
    vertex[2]->vz = (float)vertex[2]->v * vertex[2]->z1;

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
    //LONG  line1dx = (LONG)(vertex[1]->point.x - vertex[0]->point.x);
    //LONG  line1dy = (LONG)(vertex[1]->point.y - vertex[0]->point.y);

    //float   k = (float)(vertex[1]->point.y - vertex[0]->point.y) / (float)(vertex[2]->point.y - vertex[0]->point.y);
    //float   x_start = vertex[0]->point.x + (vertex[2]->point.x - vertex[0]->point.x) * k;
    //float   x_end = (float)vertex[1]->point.x;
    /*float   uz_start = vertex[0]->uz + (vertex[2]->uz - vertex[0]->uz) * k;
    float   vz_start = vertex[0]->vz + (vertex[2]->vz - vertex[0]->vz) * k;
    float   z1_start = vertex[0]->z1 + (vertex[2]->z1 - vertex[0]->z1) * k;
    float   uz_end = vertex[1]->uz;
    float   vz_end = vertex[1]->vz;
    float   z1_end = vertex[1]->z1;*/
    //float   duz = (uz_start - uz_end) / (x_start - x_end);
    //float   dvz = (vz_start - vz_end) / (x_start - x_end);
    //float   dz1 = (z1_start - z1_end) / (x_start - x_end);

    //POINT p1 = vertex[0]->point;
    POINT p2 = vertex[0]->point;
    float   uz_end = 0;
    float   vz_end = 0;
    float   z1_end = 0;
    float   x_start = (float)vertex[0]->point.x;
    float   x_end = (float)vertex[0]->point.x;

    do
    {
        // считаем x, u/z, v/z, 1/z для стороны AC
        float k = (float)(p2.y - vertex[0]->point.y) / (float)(vertex[2]->point.y - vertex[0]->point.y);
        x_start = vertex[0]->point.x + (vertex[2]->point.x - vertex[0]->point.x) * k;
        float uz_start = vertex[0]->uz + (vertex[2]->uz - vertex[0]->uz) * k;
        float vz_start = vertex[0]->vz + (vertex[2]->vz - vertex[0]->vz) * k;
        float z1_start = vertex[0]->z1 + (vertex[2]->z1 - vertex[0]->z1) * k;

        if (p2.y >= vertex[1]->point.y)
        {
            // мы находимся ниже вершины B, считаем x, u/z, v/z, 1/z для стороны BC
            k = (float)(p2.y - vertex[1]->point.y) / (float)(vertex[2]->point.y - vertex[1]->point.y);
            x_end = vertex[1]->point.x + (vertex[2]->point.x - vertex[1]->point.x) * k;
            uz_end = vertex[1]->uz + (vertex[2]->uz - vertex[1]->uz) * k;
            vz_end = vertex[1]->vz + (vertex[2]->vz - vertex[1]->vz) * k;
            z1_end = vertex[1]->z1 + (vertex[2]->z1 - vertex[1]->z1) * k;
        }
        else
        {
            // мы находимся выше вершины B, считаем x, u/z, v/z, 1/z для стороны AB
            k = (float)(p2.y - vertex[0]->point.y) / (float)(vertex[1]->point.y - vertex[0]->point.y);
            x_end = vertex[0]->point.x + (vertex[1]->point.x - vertex[0]->point.x) * k;
            uz_end = vertex[0]->uz + (vertex[1]->uz - vertex[0]->uz) * k;
            vz_end = vertex[0]->vz + (vertex[1]->vz - vertex[0]->vz) * k;
            z1_end = vertex[0]->z1 + (vertex[1]->z1 - vertex[0]->z1) * k;
        }

        // x_start должен находиться левее x_end
        if (x_start > x_end) {
            float tmp = x_start; x_start = x_end; x_end = tmp;
                  tmp = uz_start; uz_start = uz_end; uz_end = tmp;
                  tmp = vz_start; vz_start = vz_end; vz_end = tmp;
                  tmp = z1_start; z1_start = z1_end; z1_end = tmp;
        }

        float   duz = ((uz_start - uz_end) / (x_start - x_end));
        float   dvz = ((vz_start - vz_end) / (x_start - x_end));
        float   dz1 = ((z1_start - z1_end) / (x_start - x_end));

        float uz, vz, z1;
        // готовим u, v к текстурированию
        uz = uz_start;
        vz = vz_start;
        z1 = z1_start;

        // текстурируем строку
        for (float x = x_start; x <= x_end; x++) {
            int u = (int)(uz / z1);
            int v = (int)(vz / z1);

            COLORREF  color = RGB(0, face->texture[v * 256 + u], 0);
			surface->SetPixel((unsigned int)x, (unsigned int)p2.y, color);
            uz += duz;
            vz += dvz;
            z1 += dz1;
        }

        //HLine32(hdc, p2.y, p1.x, p2.x, face->color);

        p2.y++;

    }
    while (p2.y <= vertex[2]->point.y);
}

VOID
Triangle32(HDC hdc, Face *face)
/*++

   Routine:
        Triangle()

   Description:
        Drawns triangle using high performance algorithm

   Comment:
        This function uses array of points. First point and last used in main loop,
        so points can be sorted for more optimal view
--*/
{
    POINT  point[3];

    point[0] = face->v1->point;
    point[1] = face->v2->point;
    point[2] = face->v3->point;

    //
    // sort points (high, middle, low)
    //
    if (point[0].y > point[1].y) { POINT temp = point[0]; point[0] = point[1]; point[1] = temp; };
    if (point[0].y > point[2].y) { POINT temp = point[0]; point[0] = point[2]; point[2] = temp; };
    if (point[1].y > point[2].y) { POINT temp = point[1]; point[1] = point[2]; point[2] = temp; };

    if (point[0].y == point[2].y) return;


    SLOPE_PARAMETERS  line1_slop;
    SLOPE_PARAMETERS  line2_slop;

    ConfigureSlope(&point[0], &point[1], &line1_slop);
    ConfigureSlope(&point[0], &point[2], &line2_slop);

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
    LONG line1e = 0;
    LONG line2e = 0;     // errors

    POINT p1 = point[0];
    POINT p2 = point[0];

    //BOOL    bSlope1 = TRUE;

    do
    {
        if (p2.y == point[1].y)
        {
            if (point[2].y == point[1].y) return;

            ConfigureSlope(&point[1], &point[2], &line1_slop);

            p1 = point[1];
            line1e = 0;
        }

        HLine32(hdc, p2.y, p1.x, p2.x, face->color);

        p2.y++;

        line1e += line1_slop.dx;

        while (line1e > 0)
        {
            line1e -= line1_slop.dy;
            p1.x += line1_slop.xinc;

        }

        line2e += line2_slop.dx;

        while (line2e > 0)
        {
            line2e -= line2_slop.dy;
            p2.x += line2_slop.xinc;
        }

    }
    while (p2.y <= point[2].y);

}

VOID
HLine32(HDC dc, LONG y, LONG x1, LONG x2, COLORREF color)
{
    if (x1 > x2)
    {
        LONG temp = x2;
        x2 = x1;
        x1 = temp;
    }

    HPEN  hpen = CreatePen(PS_SOLID, 0, color);

    if (hpen != NULL)
    {
        HPEN hpen_old = (HPEN)SelectObject(dc, hpen);

        MoveToEx(dc, x1, y, NULL);
        LineTo(dc, x2, y);

        SelectObject(dc, hpen_old);

        DeleteObject(hpen);
    }

    /*for (LONG i = x1; i < x2; i++) {
        SetPixel(dc, i, y, color);
    }
    SetPixel(dc, x2, y, color);*/
}

VOID 
Line32(HDC dc, POINT &point1, POINT &point2, COLORREF color)
{
    /*register PULONG   buffer = 
        reinterpret_cast<PULONG>(this->m_lpSurface) + point1.x + point1.y * this->m_lLineWidth;*/

    POINT   current = point1;
    //
    // gain DX delta and normalize value
    //
    LONG  dx = (LONG)(point2.x - point1.x);
    LONG  noerror_inc = 1;

    if (dx < 0)
    {
        noerror_inc = -noerror_inc;
        dx = -dx;
    }

    //
    // gain DY delta and normalize value
    //
    LONG dy = (LONG)(point2.y - point1.y);
    LONG error_inc = 1; //this->m_lLineWidth;

    if (dy < 0)
    {
        error_inc = -error_inc;
        dy = -dy;
    }

    //
    // here we change leading variable
    //
    BOOL    bX = TRUE;

    if (dx < dy)
    {
        noerror_inc = InterlockedExchange(&error_inc, noerror_inc);
        dy = InterlockedExchange(&dx, dy);
        bX = FALSE;
    }

    LONG inc_error = dy << 2;
    LONG dec_error = -(dx << 2);
    register LONG error = 0;

    register LONG i = 0;

    do
    {
        //*buffer = color.rgb;
        SetPixel(dc, current.x, current.y, color);

        i++;

        error += inc_error;

        if (error > 0)
        {
            error += dec_error;
            //buffer = buffer + error_inc;
            bX ? current.y += error_inc : current.x += error_inc;
        }

        //buffer = buffer + noerror_inc;
        bX ? current.x += noerror_inc : current.y += noerror_inc;

    }
    while (i <= dx);

}
