
;* ------------------------------------------------------------
;* Filename ............... TVidCard.Asm
;*
;* General Purpose ........ VideoCard functions
;* ------------------------------------------------------------
;* First date ............. 12-04-94 09:00pm
;*
;* First in version ....... 2.00
;*
;* Written by ............. Alain Schellinck
;*
;* Copyright .............. Nephilim software development
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

;*--] defines [------------------------------------------------------------*

HERCULES_CARD           EQU 1
EGA_CARD                EQU 2
CGAMDA_CARD             EQU 3

;*--] public [-------------------------------------------------------------*

EXTRN   _vrVideoCard    : BYTE
EXTRN   _vrHorSize      : WORD
EXTRN   _vrVerSize      : WORD
EXTRN   _vrVideoAddr    : FAR PTR
EXTRN   _vrScanTop      : BYTE
EXTRN   _vrScanBottom   : BYTE

;*-------------------------------------------------------------------------*

PUBLIC  _vrInit

;*-------------------------------------------------------------------------*

        CODESEG

;*---] local functions [---------------------------------------------------*

;*
;* Routine   : vrCheck6845()
;* Purpose   : check if we  have a card with a 6845 controller
;* ------------------------------------------------------------------------
;* Parameters: None
;* Return    : None
;*

PROC    vrCheck6845
;-------------------------------------]
        mov      al, 0fh
        out      dx, al
        inc      dx
        in       al, dx
        mov      ah, al
        mov      al, 66h
        out      dx, al
        mov      cx, 300h
;-------------------------------------]
@@loop:
        loop     @@loop
        in       al,dx
        xchg     ah,al
        out      dx,al
        cmp      ah,66h
        je       @@is6845
        stc
;-------------------------------------]
@@is6845:
        ret
;-------------------------------------]
ENDP    vrCheck6845


;*
;* Routine   : vrCheckHercules()
;* Purpose   : check if we have a hercules card
;* ------------------------------------------------------------------------
;* Parameters: None
;* Return    : carry flag set: we got a hercules card...
;*

PROC    vrCheckHercules
;-------------------------------------]
        mov       dx, 3b4h
        call      vrCheck6845
        jc        @@noHERC
;-------------------------------------]
        mov       dl, 0bah
        in        al, dx
        and       al, 80h
        mov       ah, al
        mov       cx, 8000h
;-------------------------------------]
@@loop:
        in        al, dx
        and       al, 80h
        cmp       ah, al
        loope     @@loop
        je        @@noHERC
;-------------------------------------]
@@isHERC:
        stc
        ret
;-------------------------------------]
@@noHERC:
        clc
        ret
;-------------------------------------]
ENDP    vrCheckHercules


;*
;* Routine   : vrCheckVideoCard
;* Purpose   : check the video card type
;* ------------------------------------------------------------------------
;* Parameters: None
;* Return    : return value in the vrVideoCard variable
;*

PROC    vrCheckVideoCard
;-------------------------------------]
        push    bp
        mov     bp,sp
;-------------------------------------]
@@checkEGA:
        mov     ax, 1200h
        mov     bl, 10h
        int     10h
        cmp     bl, 10h
        je      @@checkHERC
;-------------------------------------]
@@isEGA:
        mov     [BYTE _vrVideoCard], EGA_CARD
        pop     bp
        ret
;-------------------------------------]
@@checkHERC:
        call    vrCheckHercules
        jnc     @@isCGA
;-------------------------------------]
@@isHERC:
        mov     [BYTE _vrVideoCard], HERCULES_CARD
        pop     bp
        ret
;-------------------------------------]
@@isCGA:
        mov     [BYTE _vrVideoCard], CGAMDA_CARD
        pop     bp
        ret
;-------------------------------------]
ENDP    vrCheckVideoCard

;*
;* Routine   : vrInitCursor()
;* Purpose   : initialize the cursor (save scan lines)
;* ------------------------------------------------------------------------
;* Parameters: None
;* Return    : None
;*

PROC    vrInitCursor
;-------------------------------------]
        push    bp
        mov     bp,sp
;-------------------------------------]
        xor     ax, ax
        mov     es, ax
        mov     bx, 460h
        mov     ah, [es:bx]
        mov     [_vrScanBottom], ah
        inc     bx
        mov     ah, [es:bx]
        mov     [_vrScanTop], ah
;-------------------------------------]
        pop     bp
        ret
;-------------------------------------]
ENDP    vrInitCursor

;*
;* Routine   : vrCheckDesqView()
;* Purpose   : check if desqView is available
;* ------------------------------------------------------------------------
;* Parameters: None
;* Return    : AL: 1 if desqView is detected
;*

PROC    vrCheckDesqView
;-------------------------------------]
        push    bp
        mov     bp, sp
;-------------------------------------] ; Ralf Brown: page 15-54
        mov     ax, 2B00h
        mov     cx, 4445h               ; "DE"
        mov     dx, 5351h               ; "SQ"
        int     21h
        inc     al
        xor     ah, ah
;-------------------------------------]
        pop     bp
        ret
;-------------------------------------]
ENDP    vrCheckDesqView


;*
;* Routine   : vrGetDVVideoSeg
;* Purpose   : Get the desqView video segment
;* ------------------------------------------------------------------------
;* Parameters: original video segment (in AX)
;* Return    : DesqView video segment (or original if no DV available)
;*

PROC    vrGetDVVideoSeg
;-------------------------------------]
        push    bp
        mov     bp,sp
        push    di
;-------------------------------------] ; Ralf Brown: page 15-1
        mov     es, ax
        xor     di, di
        mov     ah, 0FEh
        int     10h
        mov     ax, es
;-------------------------------------]
        pop     di
        pop     bp
        ret
;-------------------------------------]
ENDP    vrGetDVVideoSeg

;*---] Global functions [--------------------------------------------------*

;*
;* Routine   : vrInit()
;* Purpose   : Initialize the video ram code
;* ------------------------------------------------------------------------
;* Parameters: None
;* Return    : None
;*

PROC    _vrInit
;-------------------------------------]
        push    bp
        mov     bp,sp
;-------------------------------------]
        xor     ax, ax
        mov     es, ax
        mov     bp, 449h
        mov     bl, [es:bp]
        mov     ax, 0B800h
        cmp     bl, 7
        jne     @@noHerc
        mov     ax, 0B000h
;-------------------------------------]
@@noHerc:
        call    vrGetDVVideoSeg
        mov     [WORD PTR _vrVideoAddr + 2], ax
;-------------------------------------]
        xor     ax, ax
        mov     es, ax
        mov     bp, 44Eh
        mov     ax, [es:bp]
        mov     [WORD PTR _vrVideoAddr], ax
;-------------------------------------]
        call    vrInitCursor
        call    vrCheckVideoCard
        mov     ax, 25
        cmp     [_vrVideoCard], EGA_CARD
        jne     @@noEga
        xor     ax, ax
        mov     es, ax
        mov     bp, 484h
        mov     al, [es:bp]
        inc     al
;-------------------------------------]
@@noEga:
        mov     [_vrVerSize], ax
        xor     ax, ax
        mov     es, ax
        mov     bp, 44Ah
        mov     al, [es:bp]
        mov     [_vrHorSize], ax
;-------------------------------------]
        pop     bp
        ret
;-------------------------------------]
ENDP    _vrInit

;*-------------------------------------------------------------------------*

        ENDS
        END

;*---------------------------] END OF THE CODE [---------------------------*




