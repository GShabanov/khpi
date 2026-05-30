
{ Модуль работы с клавиатурой через перехват IRQ1 }

unit keyb;

interface

uses dos;

var
  old_int9handler: farpointer;
  key: array[0..127] of boolean;

procedure init_int9;
procedure done_int9;

implementation

procedure int9handler; interrupt;
var
  b:byte;
begin
  b:=port[$60];
  key[b and $7F]:=(b and $80=0);
  b:=port[$61]; port[$61]:=b or $80; port[$61]:=b and $7F;
  port[$20]:=$20;
end;

procedure init_int9;
begin
  fillchar(key,sizeof(key),false);
  getintvec(8+1,old_int9handler);
  setintvec(8+1,@int9handler);
end;

procedure done_int9;
begin
  setintvec(8+1,old_int9handler);
end;

end.
