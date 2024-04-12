;*******************************************************************************
;   File:                                                                      *
;       utils.asm                                                              *
;                                                                              *
;   Abstract:                                                                  *
;       utilites in assembler                                                  *
;                                                                              *
;   Author:                                                                    *
;       Gennady Shabanov (asterisk@foxportal.com) 1-Oct-2009                   *
;                                                                              *
;   Revision History:                                                          *
;******************************************************************************/
; $Id:  $
.686
.XMM
.MODEL  FLAT

_data   segment
_data   ENDS


_text   SEGMENT PARA 'CODE'
;_TEXT   SEGMENT BYTE USE32 PUBLIC 'CODE'
public _FillPathern
public _ConvertForDiBits

_FillPathern  proc
      push    edi
      mov     eax, [esp+10h]
      mov     ecx, [esp+0Ch]
      mov     edi, [esp+08h]
;      movd      mm0, eax
;      punpckldq mm0, mm0
;      shr       ecx, 6
;$memset_amd_ic_1:
;      movntq      [edi], mm0      ; write 64 bits
;      movntq      [edi+8], mm0
;      movntq      [edi+16], mm0
;      movntq      [edi+24], mm0
;      movntq      [edi+32], mm0
;      movntq      [edi+40], mm0
;      movntq      [edi+48], mm0
;      movntq      [edi+56], mm0
;
;      add         edi, 64         ; update destination pointer
;      dec         ecx             ; count down
;      jnz         $memset_amd_ic_1    ; last 64-byte block?

      rep     stosb
;      sfence
;      emms
      pop     edi
      retn
_FillPathern  endp

;-------------------------------
; arg0 - bufferIn
; arg4 - bufferOut
; arg8 - count
_ConvertForDiBits  proc
      push    edi
      push    esi
      mov     esi, [esp+0Ch]
      mov     ecx, [esp+10h]
      test    ecx, ecx
      jz      @@end
      mov     edi, esi
      ;mov     eax, ecx
      ;shl     eax, 2
      ;add     edi, eax
      ;prefetchnta [esi + 64]
;      shr     ecx, 4
;align 16
@@next:
      ; 0
      lodsd                  ; XXRRGGBB
      bswap    eax           ; BBGGRRXX
      shr      eax, 8        ; 00BBGGRR
      mov      ebx, eax
      stosd

      loop    @@next
;      lodsd                  ; XXRRGGBB
;      bswap    eax           ; BBGGRRXX
;      shr      eax, 8        ; 00BBGGRR
;      push     eax
;      lodsd                  ; XXRRGGBB
;      bswap    eax           ; BBGGRRXX
;      shr      eax, 8        ; 00BBGGRR
;      push     eax
;      lodsd                  ; XXRRGGBB
;      bswap    eax           ; BBGGRRXX
;      shr      eax, 8        ; 00BBGGRR
;      push     eax
;      lodsd                  ; XXRRGGBB
;      bswap    eax           ; BBGGRRXX
;      shr      eax, 8        ; 00BBGGRR
;      push     eax
;      movups  xmm0, [esp]
;      add     esp, 16
;      movntps [edi], xmm0
;      add     edi, 16
;      dec     ecx
;      jnz     @@next
;align 16     
@@end:
      ;sfence
      ;emms
      pop     esi
      pop     edi
      retn
_ConvertForDiBits  endp

;setpixel    PROC NEAR    ; x, y, color
;            mov  edi, [esp + 4]
;            mov  edx,edi
;            shl  edx, 6
;            shl  edi, 8
;            add  edi, edx
;            add  edi, [esp + 8]
;            add  edi,00A0000h
;            mov  eax, [esp + 12]
;            mov  [edi], al
;            retn
;setpixel    ENDP

_memsetEx proc
     retn
_memsetEx endp

_SetPixel640xNx32@16  proc
      mov     eax, [esp + 0Ch] ; Y
      push    edi
      mov     edx, eax
      shl     eax, 9
      shl     edx, 7
      mov     edi, [esp + 8h]   ; buffer
      add     edx, eax
      add     edx, [esp + 0Ch] ; X
      mov     eax, [esp + 14h]
      shl     edx, 2           ; offset * 4
      add     edi, edx
      mov     [edi], eax
      pop     edi
      ;DB      0F3h
      retn    10h
_SetPixel640xNx32@16  endp

_SetPixel800xNx32@16  proc
      retn    10h
_SetPixel800xNx32@16  endp


_TEXT   ENDS

END