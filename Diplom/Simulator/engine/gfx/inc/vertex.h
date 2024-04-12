// стркутура "вершина"
typedef struct _Vertex {
  LONG  x, y, z;       // координаты вершины
  //LONG  rx, ry, rz;  // преобразованные (eg, повернутые) координаты вершины
  POINT  point;        // экранные (спроецированные) координаты вершины
  LONG  u, v;          // соответствующие ей координаты текстуры
  float uz, vz, z1;    // u/z, v/z, 1/z (для перспективно-скорректированного)
  float nx, ny, nz;    // координаты вектора нормали к вершине
  //float rnx, rny, rnz; // преобразованные (eg, повернутые) координаты нормали
  float cc;            // освещенность вершины (для освещения по Гуро)
} Vertex;

// структура "грань"
typedef struct _Face {
  Vertex    *v1, *v2, *v3; // указатели на вершины грани
  UCHAR     *texture;      // указатель на текстуру
  float     nx, ny, nz;    // координаты вектора нормали к грани
  COLORREF  color;
} Face;

// функция проецирования вершины
extern void
projectVertex(Vertex *v);

extern void
calcNormals(Face *faces, int numFaces, Vertex *verts, int numVerts);

extern void
sortFaces(Face *faces, int numFaces);

// поворот вершины относительно оси Ox на угол angle
extern void
xRotateVertex(Vertex *v, float angle);

// поворот вершины относительно оси Oy на угол angle
extern void
yRotateVertex(Vertex *v, float angle);

// поворот вершины относительно оси Oz на угол angle
extern void
zRotateVertex(Vertex *v, float angle);

// поворот вершины относительно оси Ox на угол angle
extern void
RotateTexure(Vertex *v, float angle);
