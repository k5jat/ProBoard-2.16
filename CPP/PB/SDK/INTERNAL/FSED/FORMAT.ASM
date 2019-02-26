
;* ------------------------------------------------------------
;* Filename ............... TFormat.Asm
;*
;* General Purpose ........ Format (wordwrap) text
;* ------------------------------------------------------------
;* First date ............. 11-02-94 10:14pm
;*
;* First in version ....... 1.00 rev. A
;*
;* Written by ............. Alain Schellinck
;* ------------------------------------------------------------
;* Revisions:
;* ----------
;*
;*   Date   | By |                  Purpose                   |
;* ---------+----+--------------------------------------------+
;*          |    |                                            |
;*          |    |                                            |
;*          |    |                                            |
;*/

;*-------------------------------------------------------------------------*

        IDEAL
%       MODEL   LARGE
        LOCALS  @@

;*--] public [-------------------------------------------------------------*

public _FormatText

;*-------------------------------------------------------------------------*

        CODESEG

;*-------------------------------------------------------------------------*

breakPos        dw      0               ; last line separator
lineCnt         dw      0               ; current line number
colCnt          dw      0               ; current column number
width           dw      0               ; width of the screen
limit           dw      0               ; maximum characters to check
lineStart       dw      0               ; offset to the start of current line
textOffset      dw      0               ; to convert to real offset (in RAM)

;*-------------------------------------------------------------------------*

;*
;* Routine   : FormatText()
;* Purpose   : format (wordwrap) the text
;* ------------------------------------------------------------------------
;* Parameters: text and rows pointer, startline, width and limit
;* Return    : last line used in the rows buffer
;*

PROC    _FormatText
;-------------------------------------]
        ARG     _text:PTR, _rows:PTR, _start:WORD, _width:WORD, _limit:WORD
;-------------------------------------]
        push    bp
        mov     bp,sp
        push    ds
        push    es
        push    si
        push    di
;-------------------------------------]
        mov     ax, [_width]            ; save width
        dec     ax
        mov     [cs:width], ax
;-------------------------------------]
        mov     ax, [_limit]            ; save limit
        mov     [cs:limit], ax
;-------------------------------------]
        mov     ax, [_start]            ; save startline
        mov     [cs:lineCnt], ax
;-------------------------------------]
        les     di, [_rows]             ; load offset to rows buffer
        mov     ax, [cs:lineCnt]        ; pick line number to start from
        shl     ax, 1                   ; word pointers: line number * 2
        add     di, ax                  ; DI points now to the start line
;-------------------------------------]
        lds     si, [_text]             ; go to the first character
        mov     [cs:textOffset], si     ; start of the text offset
        mov     ax, [es:di]             ; pick offset from the startline
        add     di, 2                   ; move pointer to next offset
        add     si, ax                  ; add offset to start of buffer
        mov     [cs:lineStart], ax      ; save relative offset
;-------------------------------------]
clearCol:
        mov     [cs:colCnt], 0          ; clear column counter...
        mov     [cs:breakPos], 0FFFFh   ; ... and break position
;-------------------------------------]
charLoop:
        mov     al, [ds:si]             ; load character
        cmp     al, 0                   ; terminating 0 ?
        jne     checkChar
        jmp     endFormat               ; this is the end...
;-------------------------------------]
checkChar:
        cmp     al, ' '                 ; >= space (32)
        jae     checkWidth              ; ok, accept it
;-------------------------------------]
        cmp     al, 0Ah                 ; line feed ?
        je      saveOffset              ; Yes, save the offset
;-------------------------------------]
        mov     al, ' '                 ; illegal character
        mov     [ds:si], al             ; translate it to a space
        jmp     checkWidth              ;
;-------------------------------------]
saveOffset:
        inc     [cs:lineCnt]            ; line feed! Increment line counter
        mov     ax, si                  ; take the current offset
        inc     ax                      ; go to the next character
        sub     ax, [cs:textOffset]     ; convert to relative offset
        stosw                           ; save the offset
        mov     [cs:lineStart], ax      ; save relative offset
        mov     [cs:colCnt], 0          ; restart at column 0
        mov     [cs:breakPos], 0FFFFh   ; and no break position
        jmp     checkLimit              ; did we reached the limit ?
;-------------------------------------]
checkWidth:
        mov     bx, [cs:colCnt]         ; load current column width
        cmp     bx, [cs:width]          ; did we reached the width limit ?
        jb      checkBreak              ; no, so continue
        mov     bx, [cs:breakPos]       ; Yes!
        cmp     bx, 0FFFFh              ; do we have a word separator ?
        je      skipRewind              ; no, so break up the line here!
;-------------------------------------]
Rewind:
        mov     bx, [cs:lineStart]      ; yes, so rewind the buffer
        add     bx, [cs:breakPos]       ; go to the break position
        mov     [cs:lineStart], bx      ; save this as a relative offset
        add     bx, [cs:textOffset]     ; convert to real offset
        sub     si, bx                  ; how many characters rewinded ?
        add     [cs:limit], si          ; add them to the limit counter
        mov     si, bx                  ; and point to the char a/t break pos
;-------------------------------------]
skipRewind:
        jmp     saveOffset              ; now, save the offset
;-------------------------------------]
checkBreak:
        cmp     al, ' '                 ; space is a word separator
        je      setBreak
        cmp     al, '-'                 ; and '-' is a word separator
        jne     incrCol
;-------------------------------------]
setBreak:
        mov     bx, [cs:colCnt]         ; we got a word separator
        mov     [cs:breakPos], bx       ; so save the position!
;-------------------------------------]
incrCol:
        inc     [cs:colCnt]
;-------------------------------------]
checkLimit:
        inc     si                      ; did we reached the end of the text?
        dec     [cs:limit]
        jz      endFormat               ; yes, this is the end...
        jmp     charLoop
;-------------------------------------]
endFormat:
        xor     ax, ax
        stosw
        stosw
        mov     ax, [cs:lineCnt]        ; return the number of lines.
;-------------------------------------]
        pop     di
        pop     si
        pop     es
        pop     ds
        pop     bp
        ret
;-------------------------------------]
ENDP    _FormatText

;*-------------------------------------------------------------------------*

        ENDS
        END

;*---------------------------] END OF THE CODE [---------------------------*




