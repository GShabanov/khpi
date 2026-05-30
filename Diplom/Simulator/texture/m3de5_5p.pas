{ Процедуры рисования многоугольников }

unit m3de5_5p;

interface

uses pak4cntl, graph1,
     keyb;

type
  pbyte = ^byte;
  ppoly = ^tpoly;

  tdivobj = object                           { реализация быстрого деления }
    f: longint;
    zn,o,m: single;
    k1,k2: single;
    procedure reset(_b1,_k1,_b2,_k2: single);
    function next_up: longint;
    function next_down: longint;
    function next: longint;
  end;

  ppolydrawproc = procedure (poly: ppoly);

  tmonoslope = record               { Ребро монотонного многоугольника }
    ind,yc: longint;
    x,dx: single;           
  end;

  tafmslope = record     { Ребро аффинно-текстурированного многоугольника }
    ind,yc: longint;
    x,dx: single;           
    xt,yt: single;
    dxt,dyt: single;
  end;

  tpmslope = record  { Ребро перспективно-текстурированного многоугольника }
    ind,yc: longint;
    x,dx: single;           
    wxt,wyt,wx: single;
    d_wxt, d_wyt, d_wx: single;
  end;

  tpmwallslope = record  { Ребро перспективно-текстурированной стены }
    ind,xc: longint;
    y,dy: single;           
    wxt,wyt,wx: single;
    d_wxt, d_wyt, d_wx: single;
  end;

  tpoint2d = record                 { Точка в экранных координатах }
    x,y: longint;
  end;

  tpoint3d = record                 { Точка в пространстве }
    x,y,z: single;
  end;

  tvertex = record                  { Вершина динамического многоугольника }
    a: tpoint3d;
    s: tpoint2d;
    xt,yt: single;
  end;

  tpoly = record                    { Динамический многоугольник }
    v: array[0..31] of tvertex;   
    nv: longint;
    color: longint;
    tex: psprite;
    polydraw: ppolydrawproc;
  end;

procedure DrawMonoPolygon     (poly: ppoly);
procedure DrawAfMappedPolygon (poly: ppoly);
procedure DrawPMappedPolygon  (poly: ppoly);
procedure DrawPMappedPolygon16(poly: ppoly);
procedure DrawFloorPolygon    (poly: ppoly);
procedure DrawPolygonFD       (poly: ppoly);

implementation

uses m3de5_5;

type
  tfpucontext = record          { контекст сопроцессора (для отладки) }
    control: dword;
    state: dword;
    tags: dword;
    d: array[0..3] of dword;
  end;

const
  float_7:  single = 7;
  float_8:  single = 8;
  float_16: single = 16;

var     { перспективное проецирование }
  { рисование scan-line-а }
  xt_b1, yt_b1, xyt_b2: longint;
  { -- кэшируемые данные осн. цикла }
  xt_k1,xt_zn: longint;
  yt_k1,yt_zn: longint;
  xyt_k2: longint;

{ ────────────────────────────────────────────────────────────── }

procedure debug_dumpfpu;
var
  fc: tfpucontext;
  top,i: longint;
begin
  asm
    fstenv fc
  end;
  writeln('FPU:');
  top:=(fc.state shr 11) and 7;
  with fc do
  for i:=0 to 7 do begin
    write('ST(', (i-top+8) mod 8 ,'): ');
    case tags and 3 of
      0: writeln('valid');
      1: writeln('valid (zero)');
      2: writeln('NaN');
      3: writeln('free');
    end;
    tags:=tags shr 2;
  end;
  repeat until key[1];
  while key[1] do;
end;

{ ────────────────────────────────────────────────────────────── }

procedure tdivobj.reset(_b1,_k1,_b2,_k2: single);
begin
  k1:=_k1; k2:=_k2;
  f:=  trunc(_b1/_b2);   
  o:=  _b1-_b2*f;
  m:=  f*_k2;
  zn:= _b2;
end;

function tdivobj.next_up: longint;
begin
  next_up:=f;
  zn:=zn+k2;
  o:=o+k1-m;
  while o>=zn do begin
    inc(f);
    o:=o-zn;
    m:=m+k2;
  end;
end;

function tdivobj.next_down: longint;
begin
  next_down:=f;
  zn:=zn+k2;
  o:=o+k1-m;
  while o<0 do begin
    dec(f);
    o:=o+zn;
    m:=m-k2;
  end;
end;

function tdivobj.next: longint;
begin
  next:=f;
  zn:=zn+k2;
  o:=o+k1-m;
  while o>=zn do begin
    inc(f);
    o:=o-zn;
    m:=m+k2;
  end;
  while o<0 do begin
    dec(f);
    o:=o+zn;
    m:=m-k2;
  end;
end;

{ ────────────────────────────────────────────────────────────── }

procedure DrawMonoPolygon(poly: ppoly);
var
  sl: array[0..1] of tmonoslope;
  it,ib,i,j: longint;
  y,x1,x2: longint;
  top,bot: longint;
  _color: longint;
  p: pointer;
begin with poly^ do begin
  { Поиск верхней и нижней вершин }
  _color:=(color-16) shl 1;
  top:=maxlongint; bot:=-1;
  for i:=0 to nv-1 do with v[i] do begin
    if s.y<top then begin
      top:=s.y; it:=i;
    end;
    if s.y>bot then begin
      bot:=s.y; ib:=i;
    end;
  end;
  { Подготовка структур TMonoSlope }
  sl[0].yc:=0; sl[0].ind:=it-1;          if sl[0].ind<0 then sl[0].ind:=nv-1;
  sl[1].yc:=0; sl[1].ind:=(it+1) mod nv;
  y:=v[it].s.y;
  { Основной цикл }
  repeat
    { Обрабатываем уклоны }
    with sl[0] do
    while yc = 0 do begin
      ind:=(ind+1) mod nv;
      j:=(ind+1) mod nv;
      if ind = ib then exit;
      yc:=v[ j ].s.y - v[ind].s.y;
      x:=v[ind].s.x;
      if yc<>0 then dx:= ( v[ j ].s.x - x ) / yc;
    end;
    with sl[1] do
    while yc = 0 do begin
      ind:=ind-1; if ind<0 then ind:=nv-1;
      j:=ind-1; if j<0 then j:=nv-1;
      if ind = ib then exit;
      yc:=v[ j ].s.y - v[ind].s.y;
      x:=v[ind].s.x;
      if yc<>0 then dx:= ( v[ j ].s.x - x ) / yc;
    end;
    { Рисуем один scan-line }
    x1:=round(sl[0].x); x2:=round(sl[1].x);
    if x1>x2 then exit;
    p:= pointer ( dword(vscr) + x1 + dword(y*curmode.xres) );
    fillchar( p^, x2-x1+1, _color);
    { Пересчитываем параметры уклонов }
    with sl[0] do begin
      x:=x+dx;
      dec(yc);
    end;
    with sl[1] do begin
      x:=x+dx;
      dec(yc);
    end;
    inc(y);
  until false;
end;end;

{ ────────────────────────────────────────────────────────────── }

procedure DrawAfMappedPolygon(poly: ppoly);
var
  sl: array[0..1] of tafmslope;
  it,ib,i,j: longint;
  y,x1,x2,l: longint;
  top,bot: longint;
  xt,yt,dxt,dyt: single;
  p,pt: pbyte;
begin with poly^ do begin
  { Поиск верхней и нижней вершин }
  top:=maxlongint; bot:=-1;
  for i:=0 to nv-1 do with v[i] do begin
    if s.y<top then begin
      top:=s.y; it:=i;
    end;
    if s.y>bot then begin
      bot:=s.y; ib:=i;
    end;
  end;
  { Подготовка структур TAfMSlope }
  sl[0].yc:=0; sl[0].ind:=it-1;          if sl[0].ind<0 then sl[0].ind:=nv-1;
  sl[1].yc:=0; sl[1].ind:=(it+1) mod nv;   
  y:=v[it].s.y;
  { Основной цикл }
  repeat
    { Обрабатываем уклоны }
    with sl[0] do
    while yc = 0 do begin
      ind:=(ind+1) mod nv;
      j:=(ind+1) mod nv;
      if ind = ib then exit;
      yc:=v[ j ].s.y - v[ind].s.y;
      x:=v[ind].s.x; xt:=v[ind].xt; yt:=v[ind].yt;
      if yc<>0 then begin
        dx:=  ( v[ j ].s.x - x  ) / yc;
        dxt:= ( v[ j ].xt  - xt ) / yc;
        dyt:= ( v[ j ].yt  - yt ) / yc;
      end;
    end;
    with sl[1] do
    while yc = 0 do begin
      ind:=ind-1; if ind<0 then ind:=nv-1;
      j:=ind-1; if j<0 then j:=nv-1;
      if ind = ib then exit;
      yc:=v[ j ].s.y - v[ind].s.y;
      x:=v[ind].s.x; xt:=v[ind].xt; yt:=v[ind].yt;
      if yc<>0 then begin
        dx:=  ( v[ j ].s.x - x  ) / yc;
        dxt:= ( v[ j ].xt  - xt ) / yc;
        dyt:= ( v[ j ].yt  - yt ) / yc;
      end;         
    end;
    { Рисуем один scan-line }
    x1:=round(sl[0].x); x2:=round(sl[1].x); l:=x2-x1+1;
    if x1>x2 then exit; 
    p:= pointer ( dword(vscr) + x1 + dword(y*curmode.xres) );
    xt:=sl[0].xt; dxt:= (sl[1].xt-xt) / l;
    yt:=sl[0].yt; dyt:= (sl[1].yt-yt) / l;
    for i:=0 to l-1 do begin
      p^:=  tex^.r[ round(xt)+round(yt)*tex^.xsize ];
      xt:=xt+dxt; yt:=yt+dyt;
      inc(p);
    end;
    { Пересчитываем параметры уклонов }
    with sl[0] do begin
      x:=x+dx;
      xt:=xt+dxt;
      yt:=yt+dyt;
      dec(yc);
    end;
    with sl[1] do begin
      x:=x+dx;
      xt:=xt+dxt;
      yt:=yt+dyt;
      dec(yc);
    end;
    inc(y);
  until false;  
end;end;

{ ────────────────────────────────────────────────────────────── }

procedure DrawPMappedPolygon(poly: ppoly);
var
  sl: array[0..1] of tpmslope;
  it,ib,i,j: longint;
  y,x1,x2,l: longint;
  top,bot: longint;
  wxt,wyt,wx, d_wxt,d_wyt,d_wx: single;
  p: pbyte;
begin with poly^ do begin
  { Поиск верхней и нижней вершин }
  top:=maxlongint; bot:=-1;
  for i:=0 to nv-1 do with v[i] do begin
    if s.y<top then begin
      top:=s.y; it:=i;
    end;
    if s.y>bot then begin
      bot:=s.y; ib:=i;
    end;
  end;
  { Подготовка структур TPMSlope }
  sl[0].yc:=0; sl[0].ind:=it-1;          if sl[0].ind<0 then sl[0].ind:=nv-1;
  sl[1].yc:=0; sl[1].ind:=(it+1) mod nv;   
  y:=v[it].s.y;
  { Основной цикл }
  repeat
    { Обрабатываем уклоны }
    with sl[0] do
    while yc = 0 do begin
      ind:=(ind+1) mod nv;
      j:=(ind+1) mod nv;
      if ind = ib then exit;
      yc:=v[ j ].s.y - v[ind].s.y;
      x:=v[ind].s.x;
      wxt:=v[ind].xt/v[ind].a.x;
      wyt:=v[ind].yt/v[ind].a.x;
      wx:=         1/v[ind].a.x;
      if yc<>0 then begin
        dx:=  ( v[ j ].s.x - x  ) / yc;
        d_wxt:= ( v[ j ].xt/v[j].a.x  - wxt ) / yc;
        d_wyt:= ( v[ j ].yt/v[j].a.x  - wyt ) / yc;
        d_wx:=  (         1/v[j].a.x  - wx  ) / yc;
      end;
    end;
    with sl[1] do
    while yc = 0 do begin
      ind:=ind-1; if ind<0 then ind:=nv-1;
      j:=ind-1; if j<0 then j:=nv-1;
      if ind = ib then exit;
      yc:=v[ j ].s.y - v[ind].s.y;
      x:=v[ind].s.x;
      wxt:=v[ind].xt/v[ind].a.x;
      wyt:=v[ind].yt/v[ind].a.x;
      wx:=         1/v[ind].a.x;
      if yc<>0 then begin
        dx:=  ( v[ j ].s.x - x  ) / yc;
        d_wxt:= ( v[ j ].xt/v[j].a.x  - wxt ) / yc;
        d_wyt:= ( v[ j ].yt/v[j].a.x  - wyt ) / yc;
        d_wx:=  (         1/v[j].a.x  - wx  ) / yc;
      end;         
    end;
    { Рисуем один scan-line }
    x1:=round(sl[0].x); x2:=round(sl[1].x); l:=x2-x1+1;
    if x1>x2 then exit;
    p:= pointer ( dword(vscr) + x1 + dword(y*curmode.xres) );
    wxt:=sl[0].wxt;
    wyt:=sl[0].wyt;
    wx:= sl[0].wx;
    d_wxt:= (sl[1].wxt - wxt) / l;
    d_wyt:= (sl[1].wyt - wyt) / l;
    d_wx:=  (sl[1].wx  - wx ) / l;
    for i:=0 to l-1 do begin
      p^:=  tex^.r[ round(wxt/wx)+round(wyt/wx)*tex^.xsize ];
      wxt:=wxt+d_wxt;
      wyt:=wyt+d_wyt;
      wx:= wx +d_wx;
      inc(p);
    end;
    { Пересчитываем параметры уклонов }
    with sl[0] do begin
      x:=x+dx;
      wxt:=wxt+d_wxt;
      wyt:=wyt+d_wyt;
      wx:=wx+d_wx;
      dec(yc);
    end;
    with sl[1] do begin
      x:=x+dx;
      wxt:=wxt+d_wxt;
      wyt:=wyt+d_wyt;
      wx:=wx+d_wx;
      dec(yc);
    end;
    inc(y);
  until false;  
end;end;

{ ────────────────────────────────────────────────────────────── }

procedure DrawPMappedPolygon16(poly: ppoly);
var
  sl: array[0..1] of tpmslope;
  it,ib,i,j: longint;
  y,x1,x2,l: longint;
  top,bot: longint;
  wxt,wyt,wx, d_wxt,d_wyt,d_wx: single;
  xt,yt,d_xt,d_yt: single;
  p: pbyte;
begin with poly^ do begin
  { Поиск верхней и нижней вершин }
  top:=maxlongint; bot:=-1;
  for i:=0 to nv-1 do with v[i] do begin
    if s.y<top then begin
      top:=s.y; it:=i;
    end;
    if s.y>bot then begin
      bot:=s.y; ib:=i;
    end;
  end;
  { Подготовка структур TPMSlope }
  sl[0].yc:=0; sl[0].ind:=it-1;          if sl[0].ind<0 then sl[0].ind:=nv-1;
  sl[1].yc:=0; sl[1].ind:=(it+1) mod nv;   
  y:=v[it].s.y;
  { Основной цикл }
  repeat
    { Обрабатываем уклоны }
    with sl[0] do
    while yc = 0 do begin
      ind:=(ind+1) mod nv;
      j:=(ind+1) mod nv;
      if ind = ib then exit;
      yc:=v[ j ].s.y - v[ind].s.y;
      x:=v[ind].s.x;
      wxt:=v[ind].xt/v[ind].a.x;
      wyt:=v[ind].yt/v[ind].a.x;
      wx:=         1/v[ind].a.x;
      if yc<>0 then begin
        dx:=  ( v[ j ].s.x - x  ) / yc;
        d_wxt:= ( v[ j ].xt/v[j].a.x  - wxt ) / yc;
        d_wyt:= ( v[ j ].yt/v[j].a.x  - wyt ) / yc;
        d_wx:=  (         1/v[j].a.x  - wx  ) / yc;
      end;
    end;
    with sl[1] do
    while yc = 0 do begin
      ind:=ind-1; if ind<0 then ind:=nv-1;
      j:=ind-1; if j<0 then j:=nv-1;
      if ind = ib then exit;
      yc:=v[ j ].s.y - v[ind].s.y;
      x:=v[ind].s.x;
      wxt:=v[ind].xt/v[ind].a.x;
      wyt:=v[ind].yt/v[ind].a.x;
      wx:=         1/v[ind].a.x;
      if yc<>0 then begin
        dx:=  ( v[ j ].s.x - x  ) / yc;
        d_wxt:= ( v[ j ].xt/v[j].a.x  - wxt ) / yc;
        d_wyt:= ( v[ j ].yt/v[j].a.x  - wyt ) / yc;
        d_wx:=  (         1/v[j].a.x  - wx  ) / yc;
      end;         
    end;
    { Рисуем один scan-line }
    x1:=round(sl[0].x); x2:=round(sl[1].x); l:=x2-x1+1;
    if x1>x2 then exit;
    p:= pointer ( dword(vscr) + x1 + dword(y*curmode.xres) );
    wxt:=sl[0].wxt;
    wyt:=sl[0].wyt;
    wx:= sl[0].wx;
    d_wxt:= (sl[1].wxt - wxt) / l *16;
    d_wyt:= (sl[1].wyt - wyt) / l *16;
    d_wx:=  (sl[1].wx  - wx ) / l *16;
    xt:=wxt/wx;
    yt:=wyt/wx;
    for i:=0 to l-1 do begin
      if i and 15 = 0 then begin
        if i < l - 17 then
          begin
            wxt:=wxt+d_wxt;
            wyt:=wyt+d_wyt;
            wx:= wx +d_wx;
          end
        else
          begin
            wxt:=wxt+d_wxt /16*(l-i);
            wyt:=wyt+d_wyt /16*(l-i);
            wx:= wx +d_wx  /16*(l-i);
          end;
        if i < l-17 then
          begin
            d_xt:= ( (wxt/wx) - xt ) / 16;
            d_yt:= ( (wyt/wx) - yt ) / 16;
          end
        else
          begin
            d_xt:= ( (wxt/wx) - xt ) / (l-i);
            d_yt:= ( (wyt/wx) - yt ) / (l-i);
          end;
      end;
      p^:=  tex^.r[ round(xt)+round(yt)*tex^.xsize ];
      xt:=xt+d_xt;
      yt:=yt+d_yt;
      inc(p);
    end;
    { Пересчитываем параметры уклонов }
    with sl[0] do begin
      x:=x+dx;
      wxt:=wxt+d_wxt;
      wyt:=wyt+d_wyt;
      wx:=wx+d_wx;
      dec(yc);
    end;
    with sl[1] do begin
      x:=x+dx;
      wxt:=wxt+d_wxt;
      wyt:=wyt+d_wyt;
      wx:=wx+d_wx;
      dec(yc);
    end;
    inc(y);
  until false;  
end;end;

{ ────────────────────────────────────────────────────────────── }

procedure DrawFloorPolygon(poly: ppoly);
var
  sl0,sl1: tpmslope;
  it,ib,i,j: longint;
  y,x1,x2,l: longint;
  top,bot: longint;
  tx1,ty1,tx2,ty2: longint;
  p: pbyte;
  ge: boolean;
begin with poly^ do begin

                                drawpolygonfd(poly); exit;

  ge:=false;
  { Поиск верхней и нижней вершин }
  top:=maxlongint; bot:=-1;
  for i:=0 to nv-1 do with v[i] do begin
    if s.y<top then begin
      top:=s.y; it:=i;
    end;
    if s.y>bot then begin
      bot:=s.y; ib:=i;
    end;
  end;
  { Подготовка структур TPMSlope }
  sl0.yc:=0; sl0.ind:=it-1;          if sl0.ind<0 then sl0.ind:=nv-1;
  sl1.yc:=0; sl1.ind:=(it+1) mod nv;   
  y:=v[it].s.y;
  { Основной цикл }
  repeat
    { Обрабатываем уклоны }
    with sl0 do
    while yc = 0 do begin
      ind:=(ind+1) mod nv;
      j:=(ind+1) mod nv;
      if ind = ib then exit;
      yc:=v[ j ].s.y - v[ind].s.y;
      x:=v[ind].s.x;
      wxt:=v[ind].xt/v[ind].a.x;
      wyt:=v[ind].yt/v[ind].a.x;
      wx:=         1/v[ind].a.x;
      if yc<>0 then begin
        dx:=  ( v[ j ].s.x - x  ) / yc;
        d_wxt:= ( v[ j ].xt/v[j].a.x  - wxt ) / yc;
        d_wyt:= ( v[ j ].yt/v[j].a.x  - wyt ) / yc;
        d_wx:=  (         1/v[j].a.x  - wx  ) / yc;
      end;
    end;
    with sl1 do
    while yc = 0 do begin
      ind:=ind-1; if ind<0 then ind:=nv-1;
      j:=ind-1; if j<0 then j:=nv-1;
      if ind = ib then exit;
      yc:=v[ j ].s.y - v[ind].s.y;
      x:=v[ind].s.x;
      wxt:=v[ind].xt/v[ind].a.x;
      wyt:=v[ind].yt/v[ind].a.x;
      wx:=         1/v[ind].a.x;
      if yc<>0 then begin
        dx:=  ( v[ j ].s.x - x  ) / yc;
        d_wxt:= ( v[ j ].xt/v[j].a.x  - wxt ) / yc;
        d_wyt:= ( v[ j ].yt/v[j].a.x  - wyt ) / yc;
        d_wx:=  (         1/v[j].a.x  - wx  ) / yc;
      end;         
    end;
    { Рисуем один scan-line }
    asm
                fld sl0.tpmslope.x
                fistp x1
                mov eax,y
                fld sl1.tpmslope.x
                fistp x2
                mov ebx,x1
                mov edi,dword ptr [offset yofstab + eax*4]
                mov ecx,x2
                sub ecx,ebx                             
                jns @lab_0
                mov ge,1
                jmp @lab_exit
        @lab_0:
                inc ecx                                 -- ecx = length
                add edi,ebx                             -- [edi] = (x1,y)
                                -- FPU: free
                fld float_7     -- FPU: 7
                fld sl0.wyt     -- FPU: wyt  7
                fdiv sl0.wx     -- FPU: yt 7
                fscale          -- FPU: yt(:7) 7
                fistp ty1                             -- ty1(fixed25:7)
                                -- FPU: 7
                fld sl1.wyt     -- FPU: wyt  7
                fdiv sl1.wx     -- FPU: yt 7
                fscale          -- FPU: yt(:7) 7
                fistp ty2                             -- ty2(fixed25:7)
                ffree st(0)     -- FPU: free
                fld float_8     -- FPU: 8
                mov eax,ty2
                mov ebx,ty1                             -- ebx = ty1
                fld sl0.wxt     -- FPU: wxt  8
                sub eax,ebx
                fdiv sl0.wx     -- FPU: xt 8
                cdq
                fscale          -- FPU: xt(:8) 8
                idiv ecx                              
                fistp tx1                             -- tx1(fixed24:8)
                                -- FPU: 8
                fld sl1.wxt     -- FPU: wxt  8
                mov esi,eax                           -- esi = dy
                fdiv sl1.wx     -- FPU: xt 8
                fscale          -- FPU: xt(:8) 8
                fistp tx2                             -- tx2(fixed24:8)
                ffree st(0)     -- FPU: free
                                ------------------------------
                mov eax,tx2
                sub eax,tx1             
                cdq                     
                idiv ecx                                                                                  
                mov edx,tx1                           
                push ebp
                mov ebp,poly
                mov ebp,ebp[tpoly.tex]
                {       Регистры:       eax  =  dx
                                        ebx  =  yt
                                        ecx  =  счетчик
                                        edx  =  xt
                                        esi  =  dy
                                        edi  =  указатель на (Xs;Ys)
                                        ebp  =  указатель на текстуру
                                                                        }
        @lab_1:
                push eax
                xor eax,eax
                push ebx
                mov al,dh
                and ebx,255*128
                add ebx,eax
                mov al,[ebx+ebp+4]
                mov [edi],al
                pop ebx
                inc edi
                pop eax
                add ebx,esi
                add edx,eax
                dec ecx
                jnz @lab_1
                pop ebp
        @lab_exit:
    end;
    if ge then exit;                                
    { Пересчитываем параметры уклонов }
    with sl0 do begin
      x:=x+dx;
      wxt:=wxt+d_wxt;
      wyt:=wyt+d_wyt;
      wx:=wx+d_wx;
      dec(yc);
    end;
    with sl1 do begin
      x:=x+dx;
      wxt:=wxt+d_wxt;
      wyt:=wyt+d_wyt;
      wx:=wx+d_wx;
      dec(yc);
    end;
    inc(y);
  until false;  
end; end;

{ ────────────────────────────────────────────────────────────── }

procedure DrawPolygonFD(poly: ppoly);
var
  sl0,sl1: tpmslope;
  it,ib,i,j: longint;
  y,x1,x2,l: longint;
  top,bot: longint;
  wxt,wyt,wx, d_wxt,d_wyt,d_wx: single;
  p: pbyte;
--  xt_div,yt_div: tdivobj;
  ge: boolean;
begin with poly^ do begin
  ge:=false;
  { Поиск верхней и нижней вершин }
  top:=maxlongint; bot:=-1;
  for i:=0 to nv-1 do with v[i] do begin
    if s.y<top then begin
      top:=s.y; it:=i;
    end;
    if s.y>bot then begin
      bot:=s.y; ib:=i;
    end;
  end;
  { Подготовка структур TPMSlope }
  sl0.yc:=0; sl0.ind:=it-1;          if sl0.ind<0 then sl0.ind:=nv-1;
  sl1.yc:=0; sl1.ind:=(it+1) mod nv;   
  y:=v[it].s.y;
  { Основной цикл }
  repeat
    { Обрабатываем уклоны }
    with sl0 do
    while yc = 0 do begin
      ind:=(ind+1) mod nv;
      j:=(ind+1) mod nv;
      if ind = ib then exit;
      yc:=v[ j ].s.y - v[ind].s.y;
      x:=v[ind].s.x;
      wxt:=v[ind].xt/v[ind].a.x;
      wyt:=v[ind].yt/v[ind].a.x;
      wx:=         1/v[ind].a.x;
      if yc<>0 then begin
        dx:=  ( v[ j ].s.x - x  ) / yc;
        d_wxt:= ( v[ j ].xt/v[j].a.x  - wxt ) / yc;
        d_wyt:= ( v[ j ].yt/v[j].a.x  - wyt ) / yc;
        d_wx:=  (         1/v[j].a.x  - wx  ) / yc;
      end;
    end;
    with sl1 do
    while yc = 0 do begin
      ind:=ind-1; if ind<0 then ind:=nv-1;
      j:=ind-1; if j<0 then j:=nv-1;
      if ind = ib then exit;
      yc:=v[ j ].s.y - v[ind].s.y;
      x:=v[ind].s.x;
      wxt:=v[ind].xt/v[ind].a.x;
      wyt:=v[ind].yt/v[ind].a.x;
      wx:=         1/v[ind].a.x;
      if yc<>0 then begin
        dx:=  ( v[ j ].s.x - x  ) / yc;
        d_wxt:= ( v[ j ].xt/v[j].a.x  - wxt ) / yc;
        d_wyt:= ( v[ j ].yt/v[j].a.x  - wyt ) / yc;
        d_wx:=  (         1/v[j].a.x  - wx  ) / yc;
      end;         
    end;
    { Рисуем один scan-line }
    asm
                fld sl0.tpmslope.x
                fistp x1
                mov eax,y
                fld sl1.tpmslope.x
                mov ebx,x1
                fistp x2
                mov edi,dword ptr [offset yofstab + eax*4]
                mov ecx,x2
                sub ecx,ebx                             
                jns @lab_0
                mov ge,1
                jmp @lab_exit
        @lab_0:
                inc ecx                                 -- ecx = length
                add edi,ebx                             -- [edi] = (x1,y)
                                      ---- готовимся к вычислению Xt(x)
                                -- FPU: free
                fld float_16    -- FPU: 16
                                      -- вычисляем (wxt2-wxt1):16
                fld sl1.wxt     -- FPU: wxt2 16
                fld sl0.wxt     -- FPU: wxt1 wxt2 16
                fld st(2)       -- FPU: 16 wxt1 wxt2 16
                fld st(1)       -- FPU: wxt1 16 wxt1 wxt2 16
                fscale          -- FPU: wxt1:16 16 wxt1 wxt2 16
                fistp xt_b1     -- FPU: 16 wxt1 wxt2 16
                                                                -- xt_b1
                fstp st(0)      -- FPU: wxt1 wxt2 16
                fsubp           -- FPU: (wxt2-wxt1) 16
                fscale          -- FPU: (wxt2-wxt1):16 16
                fistp xt_k1     -- FPU: 16
                                      -- / вычисляем (wx2-wx1):16
                                      -- \ вычисляем xt_k1 = (wxt2-wxt1)/l :16
                fld sl1.wx      -- FPU: wx2 16
                mov eax,xt_k1
                fld sl0.wx      -- FPU: wx1 wx2 16
                cdq
                fld st(2)       -- FPU: 16 wx1 wx2 16
                fld st(1)       -- FPU: wx1 16 wx1 wx2 16
                idiv ecx
                fscale          -- FPU: wx1:16 16 wx1 wx2 16
                fistp xyt_b2    -- FPU: 16 wx1 wx2 16
                                                                -- xyt_b2
                mov xt_k1,eax                                   -- xt_k1
                fstp st(0)      -- FPU: wx1 wx2 16
                fsubp           -- FPU: (wx2-wx1) 16
                mov eax,xyt_b2
                fscale          -- FPU: (wx2-wx1):16 16
                mov xt_zn,eax
                mov yt_zn,eax
                fistp xyt_k2    -- FPU: 16                      -- xyt_k2'
                                      -- / вычисляем (wyt2-wyt1):16
                                      -- \ вычисляем xyt_k2 = (wx2-wx1)/l :16
                fld sl1.wyt     -- FPU: wyt2 16
                mov eax,xyt_k2
                fld sl0.wyt     -- FPU: wyt1 wyt2 16
                cdq
                fld st(2)       -- FPU: 16 wyt1 wyt2 16
                fld st(1)       -- FPU: wyt1 16 wyt1 wyt2 16
                idiv ecx
                fscale          -- FPU: wyt1:16 16 wyt1 wyt2 16
                fistp yt_b1     -- FPU: 16 wyt1 wyt2 16
                mov xyt_k2,eax
                fstp st(0)      -- FPU: wyt1 wyt2 16
                fsubp           -- FPU: (wyt2-wyt1) 16
                fscale          -- FPU: (wyt2-wyt1):16 16
                fistp yt_k1     -- FPU: 16
                                                                  -- yt_k1'
                mov esi,edi
                fstp st(0)      -- FPU: free
                add esi,ecx
                                      -- / вычисляем xt_f, xt_o, xt_m
                                      -- \ вычисляем yt_k1 = (wyt2-wyt1)/l :16
                mov eax,xt_b1
                mov ebx,xyt_b2
                cdq
                mov dword ptr [offset @lab_pend - 4],esi
                idiv ebx
                                                -- edx = xt_o   
                mov ebx,edx
                mov esi,eax                     -- esi = xt0
                imul xyt_k2
                                                -- eax = xt_m   
                mov edx,poly
                push ebp
                add esi,edx[tpoly.tex]          -- [esi] = tex[0,xt0]
                push eax
                mov eax,yt_k1
                cdq
                add esi,4
                idiv ecx
                mov yt_k1,eax                
                mov eax,yt_b1
                pop ecx
                                        -- вычисляем yt_f, yt_o, yt_m
                cdq
                mov ebp,xyt_b2
                idiv ebp
                                        -- eax = yt_f
                                        -- edx = yt_o                
                push edx
                push eax
                imul xyt_k2             -- eax = yt_m
                mov ebp,eax
                pop eax
                shl eax,7
                pop edx
                add esi,eax             -- [esi] = tex[yt0,xt0]
                                -- основной цикл
        {       Регистры:    eax - резерв под мат. операции и передачу текселя 
                             ebx - xt_o
                             ecx - xt_m
                             edx - yt_o
                             esi - указатель в текстуре
                             edi - указатель на экране
                             ebp - yt_m                                  
                                                                       }
        @lab_main:                      
                mov al,[esi]
                add ebx,xt_k1                   -- xt_o += xt_k1
                mov [edi],al                 
                sub ebx,ecx                     -- xt_o -= xt_m
                mov eax,xyt_k2
                add edx,yt_k1                   -- yt_o += yt_k1
                add xt_zn,eax                   -- xt_zn += xyt_k2
                inc edi
                add yt_zn,eax                   -- yt_zn += xyt_k2
                sub edx,ebp                     -- yt_o -= yt_m
                                -- сдвиг в текстуре
                                        -- X++
                cmp ebx,xt_zn
                jl @lab_skip_incx
        @lab_incx:
                sub ebx,xt_zn
                inc esi
                add ecx,xyt_k2
                cmp ebx,xt_zn
                jge @lab_incx
        @lab_skip_incx:
                                        -- X--
                test ebx,ebx
                jns @lab_skip_decx
        @lab_decx:
                dec esi
                sub ecx,xyt_k2
                add ebx,xt_zn
                js @lab_decx
        @lab_skip_decx:
                                        -- Y++
                cmp edx,yt_zn
                jl @lab_skip_incy
        @lab_incy:
                sub edx,yt_zn
                add esi,128
                add ebp,xyt_k2
                cmp edx,yt_zn
                jge @lab_incy
        @lab_skip_incy:
                                        -- Y--
                test edx,edx
                jns @lab_skip_decy
        @lab_decy:
                sub esi,128
                sub ebp,xyt_k2
                add edx,yt_zn
                js @lab_decy
        @lab_skip_decy:
        @lab_skipmove:
                cmp edi,$12345678
        @lab_pend:
                jnz @lab_main
                pop ebp
       @lab_exit:
    end;
    if ge then exit;
    { Пересчитываем параметры уклонов }
    with sl0 do begin
      x:=x+dx;
      wxt:=wxt+d_wxt;
      wyt:=wyt+d_wyt;
      wx:=wx+d_wx;
      dec(yc);
    end;
    with sl1 do begin
      x:=x+dx;
      wxt:=wxt+d_wxt;
      wyt:=wyt+d_wyt;
      wx:=wx+d_wx;
      dec(yc);
    end;
    inc(y);
  until false;  
end;end;

end.
