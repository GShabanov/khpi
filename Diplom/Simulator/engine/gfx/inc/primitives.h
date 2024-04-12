
extern VOID
Line32(HDC dc, POINT &point1, POINT &point2, COLORREF color);

extern VOID
HLine32(HDC dc, LONG y, LONG x1, LONG x2, COLORREF color);

extern VOID
Triangle32(HDC hdc, struct _Face *face);

extern VOID
TriangleTextured(class ISurface *surface, struct _Face *face);
