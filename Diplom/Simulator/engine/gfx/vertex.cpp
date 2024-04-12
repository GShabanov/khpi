#include "stdafx.h"
#include <math.h>
#include "inc\vertex.h"

#define XSIZE 640  // размер экрана по горизонтали
#define YSIZE 480  // размер экрана по вертикали
#define DIST  320  // расстояние от камеры до экрана

void
projectVertex(Vertex *v)
{
    v->point.x = XSIZE/2 + v->x * DIST / (DIST - v->z);
    v->point.y = YSIZE/2 - v->y * DIST / (DIST - v->z);
}

void
calcNormals(Face *faces, int numFaces, Vertex *verts, int numVerts)
{
    LONG  ax, ay, az, bx, by, bz;
    float nx, ny, nz, l;
    int i;

    for (i = 0; i < numFaces; i++) {
        ax = faces[i].v3->x - faces[i].v1->x;
        ay = faces[i].v3->y - faces[i].v1->y;
        az = faces[i].v3->z - faces[i].v1->z;
        bx = faces[i].v2->x - faces[i].v1->x;
        by = faces[i].v2->y - faces[i].v1->y;
        bz = faces[i].v2->z - faces[i].v1->z;

        nx = faces[i].nx = (float)(ay * bz - az * by);
        ny = faces[i].ny = (float)(az * bx - ax * bz);
        nz = faces[i].nz = (float)(ax * by - ay * bx);
        l = (float)sqrt(nx * nx + ny * ny + nz * nz);
        faces[i].nx /= l;
        faces[i].ny /= l;
        faces[i].nz /= l;
    }

    for (i = 0; i < numVerts; i++) {
        verts[i].nx = 0;
        verts[i].ny = 0;
        verts[i].nz = 0;
    }

    for (i = 0; i < numFaces; i++) {
        faces[i].v1->nx += faces[i].nx;
        faces[i].v1->ny += faces[i].ny;
        faces[i].v1->nz += faces[i].nz;
        faces[i].v2->nx += faces[i].nx;
        faces[i].v2->ny += faces[i].ny;
        faces[i].v2->nz += faces[i].nz;
        faces[i].v3->nx += faces[i].nx;
        faces[i].v3->ny += faces[i].ny;
        faces[i].v3->nz += faces[i].nz;
    }

    for (i = 0; i < numVerts; i++) {
        l = (float)sqrt(verts[i].nx * verts[i].nx + verts[i].ny * verts[i].ny +
            verts[i].nz * verts[i].nz);
        verts[i].nx /= l;
        verts[i].ny /= l;
        verts[i].nz /= l;
    }
}

void
sortFaces(Face *faces, int numFaces)
{
    // sort by Z (bubble sort :-) )
    for (register int i = 0; i < numFaces; i++)
    {
        for (register int j = i; j < numFaces; j++)
        {
            LONG z1 = (faces[i].v1->z + faces[i].v2->z + faces[i].v3->z) / 3;
            LONG z2 = (faces[j].v1->z + faces[j].v2->z + faces[j].v3->z) / 3;
            if (z1 > z2)
            {
                Face temp = faces[i];
                faces[i] = faces[j];
                faces[j] = temp;
            }
        }
    }
}

void
xRotateVertex(Vertex *v, float angle) {

    if (angle == 0.0)
    {
        /*v->rx = (LONG)(v->x);
        v->ry = (LONG)(v->y);
        v->rz = (LONG)(v->z);*/
        return;
    }


    register float mysin = (float)sin(angle);
    register float mycos = (float)cos(angle);

    register LONG y = v->y;
    register LONG z = v->z;

    // поворачиваем саму точку
    v->x = (LONG)(v->x);
    v->y = (LONG)(y * mycos - z * mysin);
    v->z = (LONG)(y * mysin + z * mycos);

    register float ny = v->ny;
    register float nz = v->nz;

    // поворачиваем нормаль к объекту в этой точке
    v->nx = v->nx;
    v->ny = ny * mycos - nz * mysin;
    v->nz = ny * mysin + nz * mycos;

    // поворачиваем саму точку
    //v->x = (LONG)(v->x);
    //v->y = (LONG)(v->y * mycos - v->z * mysin);
    //v->z = (LONG)(v->y * mysin + v->z * mycos);

    // поворачиваем нормаль к объекту в этой точке
    //v->nx = v->nx;
    //v->ny = v->ny * mycos - v->nz * mysin;
    //v->nz = v->ny * mysin + v->nz * mycos;
}

void
yRotateVertex(Vertex *v, float angle) {

    if (angle == 0.0)
    {
        return;
    }

    register float mysin = (float)sin(angle);
    register float mycos = (float)cos(angle);

    register LONG x = v->x;
    register LONG z = v->z;

    // поворачиваем саму точку
    v->x = (LONG)(x * mycos + z * mysin);
    v->y = (LONG)(v->y);
    v->z = (LONG)(z * mycos - x * mysin);

    register float nx = v->nx;
    register float nz = v->nz;

    // поворачиваем нормаль к объекту в этой точке
    v->nx = nx * mycos + nz * mysin;
    v->ny = v->ny;
    v->nz = nz * mycos - nx * mysin;
}

void
zRotateVertex(Vertex *v, float angle) {

    if (angle == 0.0)
    {
        return;
    }

    register float mysin = (float)sin(angle);
    register float mycos = (float)cos(angle);

    register LONG x = v->x;
    register LONG y = v->y;

    // поворачиваем саму точку
    v->x = (LONG)(x * mycos - y * mysin);
    v->y = (LONG)(x * mysin + y * mycos);
    v->z = (LONG)(v->z);

    register float nx = v->nx;
    register float ny = v->ny;

    // поворачиваем нормаль к объекту в этой точке
    v->nx = nx * mycos - ny * mysin;
    v->ny = nx * mysin + ny * mycos;
    v->nz = v->nz;
}


// поворот текстуры относительно центра на угол angle
void
RotateTexure(Vertex *v, float angle)
{
    register float mysin = (float)sin(angle);
    register float mycos = (float)cos(angle);
    register LONG  mu = v->u - 127;
    register LONG  mv = v->v - 127;

    v->u = (LONG)(mu * mycos - mv * mysin) + 127;
    /*if (v->u < 0)
        v->u += 255;

    if (v->u > 255)
        v->u -= 255;*/

    v->v = (LONG)(mu * mysin + mv * mycos) + 127;

    /*if (v->v < 0)
        v->v += 255;

    if (v->v > 255)
        v->v -= 255;*/
}
