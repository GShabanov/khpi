
unit graph1;

{
  Версия 1.0.
  Автор: Рыжов Александр
  Всем кроме Савинова Николая я разрешаю копирование любых фрагментов этого
  текста, для Савинова сообщаю:
    Копирование фрагментов библиотеки в целях создания больших извращений
    типа graph3 запрещается, а я оставляю за собой право на создание библиотек
    с именами "graphXX".
}

interface

uses zentimer, mouse;

type
  tvideomode = record
    xres, yres: dword;
    vmem: dword;
    vmem_div4: dword;
    mode: word;
  end;

  tpoint = record
    x,y: longint;
  end;

  trect = record
    case integer of
      1: (x1,y1,x2,y2: longint);
      2: (a,b: tpoint);
      3: (p: array[0..1] of tpoint);
  end;

  psprite = ^tsprite;
  tsprite = object
    xsize, ysize: integer;
    r: array[0..0] of byte;
  end;

  pcursor = ^tcursor;
  tcursor = object
    xhot,yhot: integer;
    xsize, ysize: integer;
    r: array[0..0] of byte;
    function sprite: psprite;
    procedure setrange;
  end;

  tmousecursor = object
    buff: psprite;
    buffsize: longint;   
    b,x,y: longint;
    procedure init;
    procedure free;
    procedure showframe;
    procedure setcursor(cur: pcursor);
    private cursor: pcursor;
  end;

  pAnimationProc = procedure(const rect: trect);

const
  maxvideomode = 4;
  videomodes: array[0..maxvideomode] of tvideomode = (
              (mode: $0013; xres:  320; yres: 200; vmem:  320*200; vmem_div4:  320*200 div 4 ),
              (mode: $0100; xres:  640; yres: 400; vmem:  640*400; vmem_div4:  640*400 div 4 ),
              (mode: $0101; xres:  640; yres: 480; vmem:  640*480; vmem_div4:  640*480 div 4 ),
              (mode: $0103; xres:  800; yres: 600; vmem:  800*600; vmem_div4:  800*600 div 4 ),
              (mode: $0105; xres: 1024; yres: 768; vmem: 1024*768; vmem_div4: 1024*768 div 4)  );

  vscr: pointer = nil;
  vscrspr: pointer = nil;

var
  curvideomode, curmode: tvideomode;
  yofstab: array[0..767] of dword;

procedure SetVideoMode(modeindex: integer);
procedure SetTextMode;
procedure SetPalette(pal: pointer);

procedure SetRealMode;
procedure SetSpriteMode(spr: psprite);

function AllocateSprite(xsize,ysize: longint): psprite;
procedure FreeSprite(spr: psprite);

procedure ShowScr;
procedure ClearScr(color: longint);
procedure PutPixel(x,y,color: longint);
function  GetPixel(x,y: longint): byte;
procedure FBox(x1,y1,x2,y2,color: longint);
procedure Line(x1,y1,x2,y2,color: longint);

procedure PutImage(base: pointer; nfile: longint; x,y: longint);
procedure GetImage(buff: pointer; x,y,xsize,ysize: longint);
function ImageSize( xsize,ysize: longint): longint;
procedure TexturedBox( base: pointer; nfile: longint;
                       x1,y1,x2,y2: longint);
procedure PutSprite(base: pointer; nfile: longint; x,y: longint);
procedure PutCursorSprite(base: pointer; nfile: longint; x,y: longint);
procedure ScaleImage( base: pointer; nfile: longint;
                      x1,y1,x2,y2: longint);
procedure ScaleSprite( base: pointer; nfile: longint;
                       x1,y1,x2,y2: longint);
procedure PutString( base: pointer; nfile: longint;
                     s: pchar; x,y,color: longint);
procedure TextExtent( base: pointer; nfile: longint;
                      s: pchar; var xs,ys: longint);

procedure AnimateRect( const rec1,rec2: trect; _time: single;
                       ap: pAnimationProc);

function Coord_to_ptr( x,y: longint ): pointer;

implementation

var
  temp: longint;

{ ───────────── не публичные процедуры ─────────────────────── }

procedure _getmem(var p: pointer; size: dword);
begin
  if memavail<size+256 then begin
    asm mov ax,3; int $10; end;
    writeln('Нехватает памяти!');
    halt;
  end;
  getmem(p,size);
end;

procedure build_yofstab(base: dword);
var
  i: longint;
  ofs: dword;
begin
  ofs:=base;
  for i:=0 to curmode.yres-1 do begin
    yofstab[i]:=ofs;
    inc(ofs,curmode.xres);
  end;
end;

{ ──────────────────────────────────────────────────────────── }

procedure AnimateRect( const rec1,rec2: trect; _time: single;
                       ap: pAnimationProc);
var
  rec: trect;
  mu: double;
  i: longint;
  time0: dword;
begin
  ztimerinit; lztimeron;
  time0:=lztimerlap;
  repeat
    mu:=(lztimerlap-time0)/(1e6*_time);
    if mu>1 then mu:=1;
    for i:=0 to 1 do with rec.p[i] do begin
      x:=round(rec1.p[i].x*(1-mu)+rec2.p[i].x*mu);
      y:=round(rec1.p[i].y*(1-mu)+rec2.p[i].y*mu);
    end;
    ap(rec);
  until mu=1;
  lztimeroff;
end;

{ ──────────────────────────────────────────────────────────── }

procedure setvideomode(modeindex: integer);
begin
  if not (modeindex in [0..maxvideomode]) then exit;
  if vscrspr<>nil then freesprite(vscrspr);
  curvideomode:=videomodes[modeindex];
  vscrspr:=allocatesprite(curvideomode.xres,curvideomode.yres);
  vscr:=pointer(dword(vscrspr)+4);
  asm
    mov ax,$4F02
    mov bx,curvideomode.mode
    int $10
  end;
  setrealmode;
end;

procedure settextmode;
begin
  if vscrspr<>nil then begin
    freesprite(vscrspr);
    vscr:=nil; vscrspr:=nil;
  end;
  asm
    mov ax,3
    int $10
  end;
end;

procedure setrealmode;
begin
  curmode:=curvideomode;
  build_yofstab(dword(vscr));
end;

procedure setspritemode(spr: psprite);
begin
  with curmode do begin
    mode:=-1;
    xres:=spr^.xsize; yres:=spr^.ysize;
    vmem:=dword(xres)*dword(yres);
    vmem_div4:=vmem shr 2;
  end;
  build_yofstab(dword(@spr^.r[0]));
end;

{ ─────────── TCursor; TMouseCursor ────────────────────────── }

function tcursor.sprite: psprite;
begin
  sprite:=pointer(dword(@self)+4);
end;

procedure tcursor.setrange;
begin
  setmouserange(0,0,curmode.xres-1-xhot,curmode.yres-1-yhot);
end;

procedure tmousecursor.init;
begin 
  buffsize:=16;
  getmem(buff,buffsize);
  cursor:=nil;
end;

procedure tmousecursor.free;
begin
  freemem(buff,buffsize);
end;

procedure tmousecursor.showframe;
var
  bm,xm,ym: word;
begin
  getmouseinfo(bm,xm,ym);
  b:=bm; x:=xm+cursor^.xhot; y:=ym+cursor^.yhot;
  getimage(buff,xm,ym,cursor^.xsize,cursor^.ysize);
  putcursorsprite(cursor^.sprite,-1,xm,ym);
  showscr;
  putimage(buff,-1,xm,ym);
end;

procedure tmousecursor.setcursor(cur: pcursor);
var
  l: longint;
begin
  l:=imagesize(cur^.xsize,cur^.ysize);
  if l>buffsize then begin
    freemem(buff,buffsize);
    buffsize:=l;
    getmem(buff,buffsize); 
  end;
  cursor:=cur;
  cur^.setrange;
end;

{ ──────────────────────────────────────────────────────────── }

function AllocateSprite(xsize,ysize: longint): psprite;
var
  spr: psprite;
begin
  getmem(spr,xsize*ysize+4);
  spr^.xsize:=xsize; spr^.ysize:=ysize;
  allocatesprite:=spr;
end;

procedure FreeSprite(spr: psprite);
begin
  freemem(spr,spr^.xsize*spr^.ysize+4);
end;

{ ────────────────────────────────────────────────────────────── }

function coord_to_ptr(x,y: longint): pointer;
asm
  mov edx,y
  mov eax,dword ptr [offset yofstab+edx*4]
  add eax,x 
end;

{ ──────────────────────────────────────────────────────────── }

procedure showscr; assembler;   { пока работает достаточно тормозно } {*}
asm
  mov esi,vscr
  cmp curvideomode.mode,0
  jz @fastcopy
  push ebp
  mov ebp,0A0000h
  mov ebx,curvideomode.vmem_div4
  xor edx,edx
@lab_1:
  push ebx
  mov eax,$4F05
  xor ebx,ebx
  int $10
  pop ebx
  inc edx
  mov ecx,16384
  cmp ebx,ecx
  jg @lab_2
  mov ecx,ebx
@lab_2:
  mov edi,ebp
  sub ebx,ecx
  rep movsd
  test ebx,ebx
  jnz @lab_1
  pop ebp
  jmp @lab_e
@fastcopy:
  mov edi,0A0000h
  mov ecx,16000
  rep movsd
@lab_e:
end;

procedure clearscr(color: longint); assembler;
asm
  mov al,byte ptr color
  mov ah,al
  push ax
  mov edi,vscr
  push ax
  mov ecx,curmode.vmem_div4
  pop eax
  rep stosd
end;

procedure putpixel(x,y,color: longint); assembler;
asm
  mov edx,y
  mov ebx,dword ptr [offset yofstab+edx*4]
  mov al,byte ptr color
  add ebx,x
  mov [ebx],al
end;

function getpixel(x,y: longint): byte; assembler;
asm
  mov edx,y
  mov ebx,dword ptr [offset yofstab+edx*4]
  add ebx,x
  mov al,[ebx] 
end;

procedure FBox(x1,y1,x2,y2,color: longint); assembler;
asm
   push ebp
   mov eax,y1
   mov ebx,y2
   mov edi,dword ptr [offset yofstab+eax*4]
   sub ebx,eax
   mov edx,x2
   mov esi,x1
   inc ebx
   sub edx,esi
   mov al,byte ptr color
   add edi,esi
   mov ah,al
   inc edx
   push ax
   mov esi,edx
   push ax
   shr edx,2
   mov ebp,curmode.xres
   pop eax
   sub ebp,esi
   and esi,11b
@lab_1:
   mov ecx,edx
   rep stosd
   mov ecx,esi
   rep stosb
   add edi,ebp
   dec ebx
   jnz @lab_1
   pop ebp
end;

procedure PutImage(base: pointer; nfile: longint; x,y: longint); assembler;
asm
  push ebp
  mov edx,y
  mov edi,dword ptr [offset yofstab+edx*4]
  mov ebx,base
  add edi,x
  mov edx,nfile
  cmp edx,-1
  jz @lab_1
  mov ebx,[ebx]
  inc edx
  mov eax,[ebx+edx*4]
  add ebx,eax
@lab_1:
  mov esi,ebx
  movzx ebx,word ptr [esi+2]
  movzx ebp,word ptr [esi]
  add esi,4
  mov edx,ebp
  shr edx,2
@lab_2:
  mov ecx,edx
  rep movsd
  mov ecx,ebp
  and ecx,3
  rep movsb
  add edi,curmode.xres
  sub edi,ebp
  dec ebx
  jnz @lab_2
  pop ebp
end;

procedure Line(x1,y1,x2,y2,color: longint); assembler;
asm
  push ebp
  mov al,byte ptr color
  mov ecx,x1
  mov [offset @post_color - 1],al
  mov ebx,y2
  mov eax,y1
  mov esi,curmode.xres
  mov edi,dword ptr [yofstab+eax*4]
  add edi,ecx
  sub ebx,eax
  jz @lab_equ_y
  jnc @lab1
  neg esi
  neg ebx
@lab1:
  sub ecx,x2
  mov ebp,-1
  jnc @lab2
  neg ecx
  neg ebp
@lab2:
  cmp ecx,ebx
  jnc @lab3
  xchg ecx,ebx
  xchg ebp,esi
@lab3:
  mov eax,ecx
  mov edx,ecx
  shr eax,1
  inc ecx
@lab_loop:
  mov byte ptr [edi],$23
@post_color:
  add edi,ebp
  sub eax,ebx
  jnc @lab4
  add eax,edx
  add edi,esi
@lab4:
  dec ecx
  jnz @lab_loop
  pop ebp
  jmp @lab_exit
@lab_equ_y:
  cmp ecx,x2
  clc
  jnz @lab1
  mov al,byte ptr color
  mov [edi],al
@lab_exit:
end;

procedure PutSprite(base: pointer; nfile: longint; x,y: longint); assembler;
asm
  push ebp
  mov edx,y
  mov edi,dword ptr [offset yofstab+edx*4]
  mov ebx,base
  add edi,x
  mov edx,nfile
  cmp edx,-1
  jz @lab_1
  mov ebx,[ebx]
  inc edx
  mov eax,[ebx+edx*4]
  add ebx,eax
@lab_1:
  mov esi,ebx
  movzx ebx,word ptr [esi+2]
  movzx edx,word ptr [esi]
  mov ebp,curmode.xres
  add esi,4
  sub ebp,edx
@lab_2:
  mov ecx,edx
@lab_3:
  mov al,[esi]
  inc esi
  cmp al,13
  jz @lab_4
  mov [edi],al
@lab_4:
  inc edi
  dec ecx
  jnz @lab_3
  add edi,ebp
  dec ebx
  jnz @lab_2
  pop ebp
end;

procedure ScaleImage( base: pointer; nfile: longint;              {*}
                      x1,y1,x2,y2: longint);          assembler;
asm
  push ebp
  mov edx,y1
  mov edi,dword ptr [offset yofstab+edx*4]
  mov ebx,base
  add edi,x1
  mov edx,nfile
  cmp edx,-1
  jz @lab1
  mov ebx,[ebx]
  inc edx
  mov eax,[ebx+edx*4]
  add ebx,eax
@lab1:
  mov esi,ebx
  movzx eax,word ptr [esi+2]
  mov ebx,y2
  xor edx,edx
  sub ebx,y1    
  inc ebx
  mov dword ptr [offset @l_dy1- 4],ebx
  mov dword ptr [offset @l_dy2- 4],ebx
  div ebx           { edx;eax - y_add }
  mov dword ptr [offset @l_ymodadd - 4],edx
  movzx edx,word ptr [esi]
  push edx
  mov dword ptr [offset @l_xsprsize - 4],edx 
  mul edx
  mov dword ptr [offset @l_ydivadd - 4],eax 
  mov ecx,x2
  xor edx,edx
  mov eax,curmode.xres
  sub ecx,x1
  inc ecx
  sub eax,ecx
  mov dword ptr [offset @l_yscradd - 4],eax 
  pop eax
  push ecx
  shl eax,16
  div ecx           { eax - x_add*$10000 }
  mov dword ptr [offset @l_xadd - 4],eax 
  pop ecx
  jmp @labres
@labres:
  mov edx,ecx
  add esi,4
  xor eax,eax
@lab2:
  mov ecx,edx
  xor ebp,ebp
@lab3:
  push eax
  push ebp
  shr ebp,16
  mov al,[esi+ebp]
  pop ebp    
  mov [edi],al
  pop eax
  add ebp,$12345678
@l_xadd: 
  inc edi
  dec ecx
  jnz @lab3
  add esi,$12345678
@l_ydivadd:
  add eax,$12345678
@l_ymodadd:
  cmp eax,$12345678
@l_dy1:
  jl @lab4
  sub eax,$12345678
@l_dy2:
  add esi,$12345678
@l_xsprsize:
@lab4:
  add edi,$12345678
@l_yscradd:
  dec ebx
  jnz @lab2
  pop ebp
end;
 
procedure ScaleSprite( base: pointer; nfile: longint;              {*}
                      x1,y1,x2,y2: longint);          assembler;
asm
  push ebp
  mov edx,y1
  mov edi,dword ptr [offset yofstab+edx*4]
  mov ebx,base
  add edi,x1
  mov edx,nfile
  cmp edx,-1
  jz @lab1
  mov ebx,[ebx]
  inc edx
  mov eax,[ebx+edx*4]
  add ebx,eax
@lab1:
  mov esi,ebx
  movzx eax,word ptr [esi+2]
  mov ebx,y2
  xor edx,edx
  sub ebx,y1    
  inc ebx
  mov dword ptr [offset @l_dy1- 4],ebx
  mov dword ptr [offset @l_dy2- 4],ebx
  div ebx           { edx;eax - y_add }
  mov dword ptr [offset @l_ymodadd - 4],edx
  movzx edx,word ptr [esi]
  push edx
  mov dword ptr [offset @l_xsprsize - 4],edx 
  mul edx
  mov dword ptr [offset @l_ydivadd - 4],eax 
  mov ecx,x2
  xor edx,edx
  mov eax,curmode.xres
  sub ecx,x1
  inc ecx
  sub eax,ecx
  mov dword ptr [offset @l_yscradd - 4],eax 
  pop eax
  push ecx
  shl eax,16
  div ecx           { eax - x_add*$10000 }
  mov dword ptr [offset @l_xadd - 4],eax 
  pop ecx
  jmp @labres
@labres:
  mov edx,ecx
  add esi,4
  xor eax,eax
@lab2:
  mov ecx,edx
  xor ebp,ebp
@lab3:
  push eax
  push ebp
  shr ebp,16
  mov al,[esi+ebp]
  pop ebp    
  cmp al,13
  jz @lab3a
  mov [edi],al
@lab3a:
  pop eax
  add ebp,$12345678
@l_xadd: 
  inc edi
  dec ecx
  jnz @lab3
  add esi,$12345678
@l_ydivadd:
  add eax,$12345678
@l_ymodadd:
  cmp eax,$12345678
@l_dy1:
  jl @lab4
  sub eax,$12345678
@l_dy2:
  add esi,$12345678
@l_xsprsize:
@lab4:
  add edi,$12345678
@l_yscradd:
  dec ebx
  jnz @lab2
  pop ebp
end;

procedure TextExtent( base: pointer; nfile: longint;
                      s: pchar; var xs,ys: longint); assembler;
asm
  mov ebx,base
  mov edx,nfile
  mov esi,s
  cmp edx,-1
  jz @lab1
  mov ebx,[ebx]
  inc edx
  mov eax,[ebx+edx*4]
  add ebx,eax
@lab1:          { [ebx] - шрифт }
  xor eax,eax
@lab2:
  movzx edx,byte ptr [esi]
  test edx,edx
  jz @lab_exit
  movzx ecx,word ptr [ebx+edx*2]
  movzx edx,byte ptr [ebx+ecx]
  add eax,edx
  inc esi
  jmp @lab2
@lab_exit:
  movzx edx,word ptr [ebx+255*2]
  movzx ecx,byte ptr [edx+ebx+1]
  mov ebx,xs
  mov edx,ys
  mov [ebx],eax
  mov [edx],ecx
end;

procedure PutCursorSprite(base: pointer; nfile: longint; x,y: longint); assembler; {*}
asm
  push ebp
  mov edx,y
  mov edi,dword ptr [offset yofstab+edx*4]
  mov ecx,x
  mov ebx,base
  add edi,ecx
  mov edx,nfile
  cmp edx,-1
  jz @lab_1
  mov ebx,[ebx]
  inc edx
  mov eax,[ebx+edx*4]
  add ebx,eax
@lab_1:
  mov eax,y
  mov esi,ebx
  movzx ebx,word ptr [esi+2]
  add eax,ebx
  sub eax,curmode.yres
  jl @y_ok
  sub ebx,eax    
@y_ok:
  movzx edx,word ptr [esi]
  add ecx,edx
  xor ebp,ebp
  sub ecx,curmode.xres
  jl @x_ok
  sub edx,ecx
  mov ebp,ecx
@x_ok:
  mov ah,13
  add esi,4
@lab1:
  mov ecx,edx
@lab2:
  mov al,[esi]
  inc esi
  cmp al,13
  jz @lab3
  mov [edi],al
@lab3:
  inc edi
  dec ecx
  jnz @lab2
  add esi,ebp
  add edi,curmode.xres
  sub edi,edx
  dec ebx
  jnz @lab1
  pop ebp
end;

function imagesize(xsize,ysize: longint): longint; assembler;
asm
  mov eax,xsize
  mul ysize
  add eax,4
end;

procedure GetImage(buff: pointer; x,y,xsize,ysize: longint); assembler;
asm
  push ebp
  mov edx,y
  mov ebx,ysize
  mov eax,x
  mov esi,dword ptr [offset yofstab+edx*4]
  mov edi,buff
  add edx,ebx
  add esi,eax
  sub edx,curmode.yres
  jl @y_ok
  sub ebx,edx
@y_ok:
  mov edx,xsize
  mov [edi+2],bx
  add eax,edx
  sub eax,curmode.xres
  jl @x_ok
  sub edx,eax
@x_ok:
  mov [edi],dx
  mov eax,curmode.xres
  add edi,4
  sub eax,edx
  mov dword ptr [offset @l_yadd - 4],eax
  jmp @labres
@labres:
  mov ebp,edx
  shr edx,2
  and ebp,3
@lab1:
  mov ecx,edx
  rep movsd
  mov ecx,ebp
  rep movsb
  add esi,$12345678
@l_yadd:
  dec ebx
  jnz @lab1
  pop ebp
end;

procedure TexturedBox( base: pointer; nfile: longint;         {**}
                       x1,y1,x2,y2: longint); assembler;
asm
  push ebp
  mov edx,y1
  mov edi,dword ptr [offset yofstab+edx*4]
  mov ebx,base
  add edi,x1
  mov edx,nfile
  cmp edx,-1
  jz @lab_1
  mov ebx,[ebx]
  inc edx
  mov eax,[ebx+edx*4]
  add ebx,eax
@lab_1:
  mov esi,ebx       { [esi] - спрайт-текстура  }
  movzx eax,word ptr [ebx]
  mov word ptr [offset @lw_sprxsize - 2],ax
  mov dword ptr [offset @l_sprxsize1 - 4],eax
  mov dword ptr [offset @l_sprxsize2 - 4],eax
  movzx eax,word ptr [ebx+2]
  mov word ptr [offset @lw_sprysize - 2],ax
  mov edx,x2
  mov eax,y2
  sub edx,x1
  sub eax,y1
  inc edx
  inc eax
  mov temp,eax
  xor ebx,ebx
  mov ebp,4
  jmp @lab_2
@lab_2:
  rol ebx,16
  mov ecx,edx
  xor bx,bx
  push ebp
@lab_3:           { bx  -  X в спрайте }
  mov al,[esi+ebp]
  mov [edi],al
  inc ebp
  inc edi
  inc bx
  cmp bx,$1234
@lw_sprxsize:
  jl @lab_4
  sub ebp,$12345678
@l_sprxsize1:
  xor bx,bx
@lab_4:
  dec ecx
  jnz @lab_3
  add edi,curmode.xres
  pop ebp
  sub edi,edx
  rol ebx,16    { bx  -  Y в спрайте }
  inc bx
  cmp bx,$1234
@lw_sprysize:
  jl @lab_5
  mov ebp,4
  xor bx,bx
  jmp @lab_6
@lab_5:
  add ebp,$12345678
@l_sprxsize2:
@lab_6:
  dec temp
  jnz @lab_2
  pop ebp
end;

procedure PutString( base: pointer; nfile: longint;
                     s: pchar; x,y,color: longint); assembler;
asm
  push ebp
  mov al,byte ptr color
  mov edx,y
  mov edi,dword ptr [offset yofstab+edx*4]
  mov byte ptr [offset @l_color - 1],al
  mov ebx,base
  add edi,x
  mov edx,nfile
  mov temp,edi
  cmp edx,-1
  jz @lab1
  mov ebx,[ebx]
  inc edx
  mov eax,[ebx+edx*4]
  add ebx,eax
@lab1:          { [ebx] - шрифт }
  mov esi,s
@lab2:
  movzx eax,byte ptr [esi]
  test eax,eax
  jz @lab_exit
  cmp eax,$0A
  jz @lab_0A
  cmp eax,$0D
  jz @lab_0D
  push ebx
  push esi
  movzx esi,word ptr [ebx+eax*2]
  add esi,ebx      { [esi] - символ из шрифта }
  movzx edx,byte ptr [esi]
  movzx ebx,byte ptr [esi+1]
  mov ebp,curmode.xres
  sub ebp,edx
  add esi,2        { [esi] - растр символа }
  xor eax,eax
  push edi
@lab3:
  mov ecx,edx
@lab4:
  test ah,ah
  jnz @lab5
  mov al,[esi]
  mov ah,8
  inc esi
@lab5:
  test al,$80
  jz @lab6
  mov byte [edi],$12
@l_color:
@lab6:
  inc edi
  shl al,1
  dec ah
  dec ecx
  jnz @lab4
  add edi,ebp
  dec ebx
  jnz @lab3
  pop edi
  add edi,edx
  pop esi
  pop ebx
  inc esi
  jmp @lab2
@lab_0A:
  movzx eax,word ptr [ebx+255*2]
  movzx eax,byte ptr [ebx+eax+1]
  mov edx,dword ptr [offset yofstab + eax*4]
  sub edx,vscr
  inc esi
  add edi,edx
  add temp,edx
  jmp @lab2
@lab_0D:
  mov edi,temp
  inc esi
  jmp @lab2
@lab_exit:
  pop ebp
end;

procedure SetPalette(pal: pointer); assembler;
asm
  mov esi,pal
  mov edx,$3C8
  xor eax,eax
  out dx,al
  inc edx
  mov ecx,768
  rep outsb
end;

end.
