
{$I-}
{$R-}

unit pak4cntl;

interface

type
  ppakfile = ^tpakfile;
  tpakfile = record
    ver: word;
    nfiles: word;
    offtab: array[0..0] of dword;
  end;

  ppak = ^tpak;
  tpak = object
    pak: ppakfile;
    size: longint;
    function load(const fn: string): boolean;
    function getfhandle(nfile: longint): pointer;
    function getfsize(nfile: longint): longint;
    procedure dispose;
  end;

implementation

procedure _getmem(var p: pointer; size: dword);
begin
  if memavail<size+256 then begin
    asm mov ax,3; int $10; end;
    writeln('Нехватает памяти!');
    halt;
  end;
  getmem(p,size);
end;

function tpak.load(const fn: string): boolean;
var
  f: file;
begin
  load:=true;
  assign(f,fn); filemode:=0; reset(f,1);
  if ioresult<>0 then exit;
  size:=filesize(f);
  _getmem(pak,size);
  blockread(f,pak^,size);
  close(f);
  load:=false;
end;

function tpak.getfhandle(nfile: longint): pointer; assembler;
asm
  mov ebx,self
  mov edx,nfile
  mov ebx,[ebx]
  inc edx
  mov eax,[ebx+edx*4]
  add eax,ebx
end;

function tpak.getfsize(nfile: longint): longint;
var
  l: dword;
begin
  if nfile = pak^.nfiles-1 then
    l:=size
  else
    l:=pak^.offtab[nfile+1];
  getfsize:=l-pak^.offtab[nfile];
end;

procedure tpak.dispose;
begin
  freemem(pak,size);
end;

end.
