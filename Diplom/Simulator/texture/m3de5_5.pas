
{$I-}

uses crt, mouse, dos,
     pak4cntl, graph1,
     m3de5_5p, keyb,
     debug;

const
  eps = 0.0001;
  dkoff  = 4/3;    { Отношение сторон монитора }  {=4/3}
  r_step = 0.1;     { шаг перемещения }
  a_step = 2;       { шаг поворота в плоскости XOY }
  t_step = 1;       { шаг поворота в плоскости ZOX }

const
  timerfreq = 100;

var
  time: longint;
                                        
type
  pblock = ^tblock;

  tpoint = record                   { Точка в 3D-координатах }
    x,y,z: single;
  end;

  pdepthtab = ^tdepthtab;
  tdepthtab = array[0..0] of ppoly; { Таблица глубины }

  pdomaintab = ^tdomaintab;           { Таблица пограничных доменов }
  tdomaintab = array[0..0] of pblock;

  tblock = object                   { Домен }
    xmin,ymin,zmin,
    xmax,ymax,zmax: single;
    nDt,nDm: longint;
    Dt: pdepthtab;
    Dm: pdomaintab;
    function isin(x,y,z: single): boolean;
  end;

  ppolyarr = ^tpolyarr;             { Массив многоугольников }
  tpolyarr = array[0..0] of tpoly;

  pblockarr = ^tblockarr;           { Массив доменов }
  tblockarr = array[0..0] of tblock;

  tmap = object                     { Карта }
    nPoly, nBlk: longint;
    poly: ppolyarr;
    blk: pblockarr;
    procedure load(const fn: string);
    function getblk(x,y,z: single): pblock;
  end;

  tobserver = object                { Наблюдатель }
    x,y,z: single;
    a,t: longint;
    a_sin,a_cos: single;
    t_sin,t_cos: single;
    blk: pblock;
    procedure init(_x,_y,_z: single; _a: longint);
    procedure set_a(_a: longint);
    procedure set_t(_t: longint);
    procedure moveto(_x,_y,_z: single);
    procedure move(dist: single);
  end;
  
var
  { Перехват IRQ0,1 }
  old_int8handler: farpointer;
  enablecontrol: boolean;
  { - Синусы/косинусы 361-ого угла - } 
  sine,cosine: array[0..360] of single;
  { - Текстуры, карта, шрифты, ... - }
  mainpak,texpak: tpak;
  map: tmap;
  mainfont: pointer;
  { - Наблюдатель - }
  obs,obs_cntl: tobserver;
  { - Параметры видео - }
  videomode, _videomode: longint;
  _reqvmchange: boolean;
  hxresmo, hyresmo: longint;
  message: string;
  message_time: longint;
  { - Параметры движка - }
  clipmode: boolean;
  texmode: longint;
  { - Счетчик FPS - }
  fpsenabled: boolean;
  lastfps: string;
  curfps: longint;
  framestart: longint;
        
{ ────────────────────────────────────────────────────────────── }

procedure fatalerror(const message: string); forward;
procedure init_int8; forward;
procedure done_int8; forward;
procedure initvideomode; forward;

{ ────────────────────────────────────────────────────────────── }

procedure debug_writepoly(const poly: tpoly);
var
  i: longint;
begin
  with poly do begin
    writeln('Polygon.');
    writeln('nV = ',nv);
    for i:=0 to nv-1 do with v[i] do begin
      writeln('Vertex ',i);
      write('  X: ',a.x:10:4); writeln('    Xs: ',s.x);
      write('  Y: ',a.y:10:4); writeln('    Ys: ',s.y);
      writeln('  Z: ',a.z:10:4);
    end;
  end;
  repeat until key[1];
end;

procedure debug_writeframe;
var
  f: file;
begin
  assign(f,'frame.raw'); rewrite(f,1);
  blockwrite(f,vscr^,curmode.vmem);
  close(f);
end;

{ ────────────────────────────────────────────────────────────── }

function inttostr(i: longint): string;
var
  r: string;
begin
  str(i,r);
  inttostr:=r;
end;

procedure sendmessage(const _message: string);
begin
  message:=_message;
  message[length(message)+1]:=#0;
  message_time:=time;
end;

procedure setclipmode(cm: boolean);
begin
  clipmode:=cm;
  if clipmode then
    sendmessage('Отсечение: вкл.')
  else
    sendmessage('Отсечение: выкл.');
end;

procedure settexmode(tm: longint);
begin
  texmode:=tm;
  case texmode of
    0: sendmessage('Текстурирование: выкл.');
    1: sendmessage('Текстурирование: перспективно');
    2: sendmessage('Текстурирование: опт. деление');
    3: sendmessage('Текстурирование: оптимизированно');
  end;
  clearscr(0);
end;

procedure setfpsmode(m: boolean);
begin
  fpsenabled:=m;
  if m then
    begin
      sendmessage('Счетчик FPS: вкл.');
      framestart:=-1; lastfps:='---'#0;
      curfps:=0;
    end
  else
    sendmessage('Счетчик FPS: выкл.');
end;

{ ────────────────────────────────────────────────────────────── }

procedure projection(var poly: tpoly);
var
  i: longint;
begin
  for i:=0 to poly.nv-1 do with poly.v[i] do begin
    s.x:= round( hxresmo*(1-a.y/(dkoff*a.x)) );
    s.y:= round( hyresmo*(1-a.z/a.x        ) );
{
    s.x:= 16+round( (hxresmo-15)*(1-a.y/(dkoff*a.x)) );
    s.y:= 16+round( (hyresmo-15)*(1-a.z/a.x        ) );
}
  end;
end;

procedure drawpoly_noclip(var poly: tpoly);
var
  i: longint;
begin
  for i:=0 to poly.nv-1 do with poly.v[i].a do
    if (y>dkoff*x) or (y<-dkoff*x) or
       (z>x)       or (z<-x)       then exit;
  projection(poly);
  case texmode of
    0: drawmonopolygon(@poly);
    1: DrawPMappedPolygon(@poly);
    2: DrawPolygonFD(@poly);
    3: poly.polydraw(@poly);
  end;
end;

procedure drawpoly_clip(var poly: tpoly);
var
  buff: tpoly;
  vf0,cr: array[0..3] of boolean;  { Видемость вершины-0, флаги отсечений }
  sour,dest,temp: ppoly;           { Источник, приемник отсечения }
  mu: single;
  i,j,k: longint;
  pred: tvertex;                   { Предыдущая вершина }
  predvf,curvf: boolean;           
  nvf,b: boolean;
begin with poly do begin
  { Определяем какими гранями вообще должен отсекаеться многоугольник }
  with v[0].a do begin
    vf0[0]:= y <=  x*dkoff;
    vf0[1]:= y >= -x*dkoff;
    vf0[2]:= z <=  x;
    vf0[3]:= z >= -x;
    nvf:= (x<0);
  end;
  fillchar(cr,sizeof(cr),false);
  for i:=1 to nv-1 do with v[i].a do begin
    cr[0]:= cr[0] or ( vf0[0] <> ( y <  x*dkoff ) );
    cr[1]:= cr[1] or ( vf0[1] <> ( y > -x*dkoff ) );
    cr[2]:= cr[2] or ( vf0[2] <> ( z <  x       ) );
    cr[3]:= cr[3] or ( vf0[3] <> ( z > -x       ) );
    nvf:=nvf and (x<0);
  end;
  { Полная видемость? }
  if (not (cr[0] or cr[1] or cr[2] or cr[3])) and 
     vf0[0] and vf0[1] and vf0[2] and vf0[3] then begin
    projection(poly);
    case texmode of
      0: drawmonopolygon(@poly);
      1: DrawPMappedPolygon(@poly);
      2: DrawPolygonFD(@poly);
      3: poly.polydraw(@poly);
    end;
    exit;
  end;
  { Полная невидемость? }
  if nvf then exit;
  { Частичная видемость }
  buff.color:=color; buff.tex:=tex; buff.polydraw:=polydraw;
  sour:=@poly; dest:=@buff;
  for j:=0 to 3 do if cr[j] then begin
    dest^.nv:=0;
    with sour^.v[sour^.nv-1].a do
      case j of
        0: predvf:= (y <=  x*dkoff);
        1: predvf:= (y >= -x*dkoff);
        2: predvf:= (z <=  x);
        3: predvf:= (z >= -x);
      end;
    pred:=sour^.v[sour^.nv-1];
    for i:=0 to sour^.nv-1 do begin
      with sour^.v[i].a do
        case j of
          0: curvf:= (y <=  x*dkoff);
          1: curvf:= (y >= -x*dkoff);
          2: curvf:= (z <=  x);
          3: curvf:= (z >= -x);
        end;
      if predvf xor curvf then begin
        with dest^.v[dest^.nv] do
          case j of
            0: { Y=dX }
              begin
                mu:= ( pred.a.y - dkoff*pred.a.x ) /
                     ( dkoff*(sour^.v[i].a.x - pred.a.x) -
                       sour^.v[i].a.y + pred.a.y );
                a.x:= pred.a.x + mu * ( sour^.v[i].a.x - pred.a.x );
                a.y:= dkoff*a.x;
                a.z:= pred.a.z + mu * ( sour^.v[i].a.z - pred.a.z );
                xt:=  pred.xt + mu * ( sour^.v[i].xt - pred.xt );
                yt:=  pred.yt + mu * ( sour^.v[i].yt - pred.yt );
              end;
            1: { Y=-dX }
              begin
                mu:= ( dkoff*pred.a.x + pred.a.y ) /
                     ( dkoff*(pred.a.x - sour^.v[i].a.x) +
                       pred.a.y - sour^.v[i].a.y);
                a.x:= pred.a.x + mu * ( sour^.v[i].a.x - pred.a.x );
                a.y:= -dkoff*a.x;
                a.z:= pred.a.z + mu * ( sour^.v[i].a.z - pred.a.z );
                xt:=  pred.xt + mu * ( sour^.v[i].xt - pred.xt );
                yt:=  pred.yt + mu * ( sour^.v[i].yt - pred.yt );
              end;
            2: { Z=X } 
              begin
                mu:= ( pred.a.z - pred.a.x ) /
                     ( sour^.v[i].a.x - pred.a.x -
                       sour^.v[i].a.z + pred.a.z );
                a.x:= pred.a.x + mu * ( sour^.v[i].a.x - pred.a.x );
                a.y:= pred.a.y + mu * ( sour^.v[i].a.y - pred.a.y );
                a.z:= a.x;
                xt:=  pred.xt + mu * ( sour^.v[i].xt - pred.xt );
                yt:=  pred.yt + mu * ( sour^.v[i].yt - pred.yt );
              end;
            3: { Z=-X }
              begin
                mu:= ( pred.a.x + pred.a.z ) /
                     ( pred.a.x - sour^.v[i].a.x +
                       pred.a.z - sour^.v[i].a.z);
                a.x:= pred.a.x + mu * ( sour^.v[i].a.x - pred.a.x );
                a.y:= pred.a.y + mu * ( sour^.v[i].a.y - pred.a.y );
                a.z:= -a.x;
                xt:=  pred.xt + mu * ( sour^.v[i].xt - pred.xt );
                yt:=  pred.yt + mu * ( sour^.v[i].yt - pred.yt );
              end;
          end;
        inc(dest^.nv);
      end;
      if curvf then begin
        dest^.v[dest^.nv]:=sour^.v[i];
        inc(dest^.nv);
      end;
      pred:=sour^.v[i];
      predvf:=curvf;                   
    end;
    if dest^.nv = 0 then exit;
    temp:=sour;
    sour:=dest;
    dest:=temp;
  end;
  with sour^.v[0].a do
    if (y<=dkoff*x+eps) and (y>=-dkoff*x-eps) and
       (z<=x+eps)       and (z>=-x-eps)       then begin
      projection(sour^);
      case texmode of
        0: drawmonopolygon     (sour);
        1: DrawPMappedPolygon  (sour);
        2: DrawPolygonFD       (sour);
        3: sour^.polydraw      (sour);
      end;                  
    end;
end; end;

procedure processfps;
begin
  inc(curfps);
  if time>=framestart+100 then begin
    framestart:=time;
    lastfps:='FPS: '+inttostr(curfps);
    lastfps[length(lastfps)+1]:=#0;
    curfps:=0;
  end;
  putstring(mainfont,-1,pchar(@lastfps[1]),curmode.xres-64,4,12);
end;

procedure drawframe;
var
  i,j: longint;
  poly: tpoly;
  x,y,z,x1,y1,z1: single;
begin
  obs:=obs_cntl;
  if not clipmode then clearscr(240);
  { Рисуем все многоугольники, видимые отсюда }
  for i:=0 to obs.blk^.ndt-1 do with obs.blk^.dt^[i]^ do begin
    { Переводим многоугольник в систему координат наблюдателя }
    poly:=obs.blk^.dt^[i]^;
    for j:=0 to nv-1 do begin
      { Перенос осей }
      x:= v[j].a.x - obs.x;
      y:= v[j].a.y - obs.y;
      z:= v[j].a.z - obs.z;
      { Поворот в плоскости XOY }
      x1:= x*obs.a_cos - y*obs.a_sin;
      y1:= x*obs.a_sin + y*obs.a_cos;
      z1:= z;
      { Поворот в плоскости ZOX }
      poly.v[j].a.x:= x1*obs.t_cos - z1*obs.t_sin;
      poly.v[j].a.y:= y1;
      poly.v[j].a.z:= x1*obs.t_sin + z1*obs.t_cos;
    end;      
    { Прорисовка многоугольника }
    if clipmode then
      drawpoly_clip(poly)
    else
      drawpoly_noclip(poly);
  end;
  { Информационая строка }
  if time<message_time+400 then
    putstring(mainfont,-1,@message[1],4,4,15);
  if fpsenabled then processfps;
  showscr;
end;

procedure debug_sendposinfo;
var
  s,s1: string;
begin
  with obs_cntl do begin
    str(x:8:2,s);
    str(y:8:2,s1);
    s:='(X;Y;Z;A) = ('+s+';'+s1+';';
    str(z:8:2,s1);
    s:=s+s1+';'+inttostr(a)+')';
  end;
  sendmessage(s);
end;

procedure walk;
begin
  enablecontrol:=true;
  repeat
    if _reqvmchange then begin
      videomode:=_videomode;
      initvideomode;
    end;
    drawframe;
    if key[59] then begin
      setclipmode(not clipmode);
      while key[59] do;
    end;
    if key[60] then begin
      setfpsmode(not fpsenabled);
      while key[60] do;
    end;
    if key[67] then debug_sendposinfo;
    if key[68] then debug_writeframe;
  until key[1];
  enablecontrol:=false;
end;

{ ────────────────────────────────────────────────────────────── }

procedure tmap.load(const fn: string);
var
  f: file;
  s: string;
  ob: tpoint3d;
  oba: longint;
  i,j,k,iclass: longint;
begin
  assign(f,fn); reset(f,1);
  if ioresult<>0 then fatalerror('Нет файла карты: '+fn);
  s[0]:=#4; blockread(f,s[1],4);
  if s<>'3DE4' then fatalerror('Неверный формат карты: '+fn);
  blockread(f,ob,sizeof(tpoint3d)); blockread(f,oba,4);
  blockread(f,npoly,4); blockread(f,nblk,4);
  getmem(poly,npoly*sizeof(tpoly)); fillchar(poly^,npoly*sizeof(tpoly),0);
  getmem(blk,nblk*sizeof(tblock));  fillchar(blk^, nblk*sizeof(tblock),0);
  { Читаем многоугольники }
  for i:=0 to npoly-1 do with poly^[i] do begin
    blockread(f,nv,4);
    blockread(f,color,4);
    blockread(f,j,4);
    tex:=texpak.getfhandle(j);
    blockread(f,iclass,4);
    case iclass of
      0: polydraw:=drawpolygonfd;
      1: polydraw:=drawpolygonfd;
      2: polydraw:=drawfloorpolygon;
      else fatalerror('Недопустимый класс многоугольника: '+inttostr(i));
    end;
    for j:=0 to nv-1 do begin
      blockread(f,v[j].a,sizeof(tpoint3d));
      blockread(f,v[j].xt,4);
      blockread(f,v[j].yt,4);
    end;
  end;         
  { Читаем домены }
  for i:=0 to nblk-1 do with blk^[i] do begin
    blockread(f,xmin,(6+2)*4);
    if ndt<>0 then getmem(dt,4*ndt);
    if ndm<>0 then getmem(dm,4*ndm);
    for j:=0 to nDt-1 do begin
      blockread(f,k,4);
      dt^[j]:=@poly^[k];
    end;
    for j:=0 to nDm-1 do begin
      blockread(f,k,4);
      dm^[j]:=@blk^[k];
    end;
  end;       
  close(f);
  obs.init(ob.x,ob.y,ob.z,oba);
  obs_cntl:=obs;
end;

function tmap.getblk(x,y,z: single): pblock;
var
  i: longint;
begin
  for i:=0 to nblk-1 do
    if blk^[i].isin(x,y,z) then
      begin
        getblk:=@blk^[i]; exit;
      end;
  getblk:=nil;
end;

{ ────────────────────────────────────────────────────────────── }

function tblock.isin(x,y,z: single): boolean;
begin
  if (x>xmin) and (y>ymin) and (z>zmin) and
     (x<xmax) and (y<ymax) and (z<zmax) then
    begin
      isin:=true; exit;
    end;
  isin:=false;
end;

procedure tobserver.init(_x,_y,_z: single; _a: longint);
begin
  x:=_x; y:=_y; z:=_z;
  set_a(_a); set_t(0);
  blk:=map.getblk(_x,_y,_z);
end;

procedure tobserver.set_a(_a: longint);
begin
  a:=_a;
  a_sin:=sine[_a]; a_cos:=cosine[_a];
end;

procedure tobserver.set_t(_t: longint);
begin
  t:=_t;
  t_sin:=sine[_t]; t_cos:=cosine[_t];
end;

procedure tobserver.moveto(_x,_y,_z: single);
var
  i: longint;
begin
  if blk^.isin(_x,_y,_z) then begin
    x:=_x; y:=_y; z:=_z;
    exit;
  end;
  for i:=0 to blk^.ndm-1 do
    if blk^.dm^[i]^.isin(_x,_y,_z) then
      begin
        x:=_x; y:=_y; z:=_z;
        blk:=blk^.dm^[i];
        exit;
      end;
end;

procedure tobserver.move(dist: single);
begin
  if key[54] then dist:=dist/16;
  moveto( x+dist*a_cos, y-dist*a_sin, z );
end;

{ ────────────────────────────────────────────────────────────── }

procedure initvideomode;
begin
  _reqvmchange:=false;
  setvideomode(videomode);
  setpalette(mainpak.getfhandle(1));
  clearscr(0);
  initmouse; hidemouse;
  hxresmo:= (curmode.xres-1) div 2;
  hyresmo:= (curmode.yres-1) div 2;
  sendmessage( 'Видеорежим: '+
               inttostr(curmode.xres)+'x'+inttostr(curmode.yres) );
end;

procedure init_engine;
begin
  setclipmode(true);
  settexmode(0);
  setfpsmode(false);
end;

procedure init;
var
  i: longint;
  a: single;
begin
  { табл. синусов }
  for i:=0 to 360 do begin
    a:= i*pi/180;
    sine[i]:=sin(a);
    cosine[i]:=cos(a);
  end;
  { пакеты }
  if texpak.load('textures.pak') then
    fatalerror('Нет файла текстур: Textures.pak');
  if mainpak.load('main.pak') then
    fatalerror('Нет файла: Main.pak');
  mainfont:=mainpak.getfhandle(0);
  { видео }
  directvideo:=false;
  videomode:=0;
  initvideomode;
  { Движок }
  init_engine;
  { IRQ0,1 }
  init_int9;
  init_int8;
end;

procedure done;
begin
  { IRQ0,1 }
  done_int8;
  done_int9;
  { видео }
  settextmode;
end;

procedure fatalerror(const message: string);
begin
  settextmode;
  writeln(message);
  halt;
end;

{ ────────────────────────────────────────────────────────────────────── }

procedure changevm(vm: longint);
begin
  _reqvmchange:=true;
  _videomode:=vm;
end;

procedure int8handler; interrupt;
begin
  if enablecontrol then begin
    { Управление наблюдателем }
    with obs_cntl do begin
      if key[72] then move(r_step);
      if key[80] then move(-r_step); 
      if key[75] then set_a( (a-a_step+360) mod 360 );
      if key[77] then set_a( (a+a_step    ) mod 360 );
      if key[30] then set_t( (t-t_step+360) mod 360 );
      if key[44] then set_t( (t+t_step    ) mod 360 );
      if key[32] then moveto( x,y,z+r_step );
      if key[46] then moveto( x,y,z-r_step );
    end;
    { Режимы текстурирования }
    if key[63] then settexmode(0);
    if key[64] then settexmode(1);   
    if key[65] then settexmode(2);   
    if key[66] then settexmode(3);   
    { Видеорежимы }
    if key[2] then changevm(0);
    if key[3] then changevm(1);
    if key[4] then changevm(2);
    if key[5] then changevm(3);
    if key[6] then changevm(4);
  end;
  inc(time);
  port[$20]:=$20;
end;

procedure init_int8;
var
  f: longint;
begin
  enablecontrol:=false;
  time:=0;
  getintvec(8+0,old_int8handler);
  setintvec(8+0,@int8handler);
  f:=round(1193180/timerfreq);
  port[$43]:=$36;
  port[$40]:=f and $FF;
  port[$40]:=(f shr 8) and $FF;
  while time=0 do;
end;

procedure done_int8;
begin
  setintvec(8+0,old_int8handler);
end;

{ ────────────────────────────────────────────────────────────────────── }

begin
  init;
  if paramcount = 0 then
    map.load('level1.map')
  else
    map.load(paramstr(1));
  sendmessage('M3DE5. Автор: Рыжов Александр.');
  walk;
  done;
end.
