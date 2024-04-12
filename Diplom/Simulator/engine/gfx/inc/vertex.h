// ��������� "�������"
typedef struct _Vertex {
  LONG  x, y, z;       // ���������� �������
  //LONG  rx, ry, rz;  // ��������������� (eg, ����������) ���������� �������
  POINT  point;        // �������� (���������������) ���������� �������
  LONG  u, v;          // ��������������� �� ���������� ��������
  float uz, vz, z1;    // u/z, v/z, 1/z (��� ������������-������������������)
  float nx, ny, nz;    // ���������� ������� ������� � �������
  //float rnx, rny, rnz; // ��������������� (eg, ����������) ���������� �������
  float cc;            // ������������ ������� (��� ��������� �� ����)
} Vertex;

// ��������� "�����"
typedef struct _Face {
  Vertex    *v1, *v2, *v3; // ��������� �� ������� �����
  UCHAR     *texture;      // ��������� �� ��������
  float     nx, ny, nz;    // ���������� ������� ������� � �����
  COLORREF  color;
} Face;

// ������� ������������� �������
extern void
projectVertex(Vertex *v);

extern void
calcNormals(Face *faces, int numFaces, Vertex *verts, int numVerts);

extern void
sortFaces(Face *faces, int numFaces);

// ������� ������� ������������ ��� Ox �� ���� angle
extern void
xRotateVertex(Vertex *v, float angle);

// ������� ������� ������������ ��� Oy �� ���� angle
extern void
yRotateVertex(Vertex *v, float angle);

// ������� ������� ������������ ��� Oz �� ���� angle
extern void
zRotateVertex(Vertex *v, float angle);

// ������� ������� ������������ ��� Ox �� ���� angle
extern void
RotateTexure(Vertex *v, float angle);
