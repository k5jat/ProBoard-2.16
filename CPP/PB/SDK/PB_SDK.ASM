LOCALS

;----------------------------------------------------------------------------
PBSDK_TEXT      SEGMENT WORD PUBLIC 'CODE'
PBSDK_TEXT      ENDS
;----------------------------------------------------------------------------
_DATA           SEGMENT WORD PUBLIC 'DATA'
_DATA           ENDS
;----------------------------------------------------------------------------
_INIT_          SEGMENT WORD PUBLIC 'INITDATA'
InitStart       label byte
_INIT_          ENDS
;----------------------------------------------------------------------------
_INITEND_       SEGMENT BYTE PUBLIC 'INITDATA'
InitEnd         label byte
_INITEND_       ENDS
;----------------------------------------------------------------------------
_EXIT_          SEGMENT WORD PUBLIC 'EXITDATA'
ExitStart       label byte
_EXIT_          ENDS
;----------------------------------------------------------------------------
_EXITEND_       SEGMENT BYTE PUBLIC 'EXITDATA'
ExitEnd         label byte
_EXITEND_       ENDS
;----------------------------------------------------------------------------
XIFB            SEGMENT WORD PUBLIC 'DATA'
ZInitStart      label byte
XIFB            ENDS
;----------------------------------------------------------------------------
XIF             SEGMENT WORD PUBLIC 'DATA'
XIF             ENDS
;----------------------------------------------------------------------------
XIFE            SEGMENT WORD PUBLIC 'DATA'
ZInitEnd        label byte
XIFE            ENDS
;----------------------------------------------------------------------------
XCFB            SEGMENT WORD PUBLIC 'DATA'
ZExitStart      label byte
XCFB            ENDS
;----------------------------------------------------------------------------
XCF             SEGMENT WORD PUBLIC 'DATA'
XCF             ENDS
;----------------------------------------------------------------------------
XCFE            SEGMENT WORD PUBLIC 'DATA'
ZExitEnd        label byte
XCFE            ENDS
;----------------------------------------------------------------------------
CONST           SEGMENT WORD PUBLIC 'CONST'
CONST           ENDS
;----------------------------------------------------------------------------
_BSS            SEGMENT WORD PUBLIC 'BSS'
_BSS            ENDS
;----------------------------------------------------------------------------
c_common        SEGMENT WORD PUBLIC 'BSS'
c_common        ENDS
;----------------------------------------------------------------------------
FAR_BSS         SEGMENT WORD PUBLIC 'FAR_BSS'
FAR_BSS         ENDS
;----------------------------------------------------------------------------
STACK           SEGMENT PARA STACK 'STACK'
     ORG $+100h
STACK           ENDS
;----------------------------------------------------------------------------


PNEAR           EQU     0
PFAR            EQU     1
NOTUSED         EQU     0ffh

SE              STRUC
calltype        db      ?                       ; 0=near,1=far,ff=not used
priority        db      ?                       ; 0=highest,ff=lowest
addrlow         dw      ?
addrhigh        dw      ?
SE              ENDS


DGROUP GROUP XIFB,XIF,XIFE,XCFB,XCF,XCFE,_DATA,_INIT_,_INITEND_,_EXIT_,_EXITEND_,CONST,_BSS,c_common


COUNTER = 0

FuncHook MACRO funcname
   PUBLIC _&funcname
_&funcname PROC NEAR
           mov  al,COUNTER
           jmp  dispatch
_&funcname ENDP
 COUNTER = COUNTER + 1
ENDM

DataHook MACRO dataname
      PUBLIC _&dataname
_&dataname dd 0ABCDABCDh
ENDM


PUBLIC __acrtused
PUBLIC __setargv__

PUBLIC _ExitTSR
PUBLIC _exit

EXTRN _main : FAR

__acrtused  equ 1234h               ; For Zortech / MickeySoft
__setargv__ equ 1234h               ; For Zortech / MickeySoft

PBSDK_TEXT SEGMENT
      ASSUME CS:PBSDK_TEXT , DS:DGROUP

;     ORG 0

; -------------------------------------------------------------------------
;  Local data section. The first 2 bytes must be set to the host's DS
; -------------------------------------------------------------------------

           dw      offset Destructors    ;
           dw      seg    Destructors    ;
           dw      offset Constructors   ;
           dw      seg    Constructors   ;
host_ds    dw      4250h                 ; Host Data Segment
           dw      offset Entry          ; Address of entry point
           dw      seg    Entry          ;
           dw      offset FuncTable      ; Address of function table
           dw      seg    FuncTable      ;
           dw      offset DataTable      ; Address of data table
           dw      seg    DataTable      ;
           dw      215                   ; Version number

; -------------------------------------------------------------------------
;  Ordinary DOS execution code
; -------------------------------------------------------------------------

EVEN

start:     push    cs
           pop     ds
           mov     dx,offset pb_msg
           mov     ah,9
           int     21h
           mov     ax,4C01h
           int     21h

pb_msg     db    'ProBoard SDK 2.15',13,10,'$',26

; -----------------------------------------------------------------------
;  Entry point. The host application should make a far call to this point
; -----------------------------------------------------------------------

     EVEN

entry_sp   dw 0

Entry:
        push    bp
        mov     bp,sp

        mov     ax,DGROUP
        mov     ds,ax

        mov     cs:[entry_sp],sp

        push    [bp+10]
        push    [bp+8]         ;
        push    [bp+6]         ; Push main() parameters on stack
        call    _main          ; call main()
        add     sp,6           ; Restore stack
        xor     ax,ax

back_to_pb:
        push    cs:[host_ds]   ; Restore original DS
        pop     ds             ;
        pop     bp
        retf

; -------------------------------------------------------------------------
;  Ordinary & TSR exit code
; -------------------------------------------------------------------------

_exit:
        xor     ax,ax
__exit:
        mov     sp,cs:[entry_sp]
        jmp     back_to_pb

_ExitTSR:
        mov     ax,-1
        jmp     __exit

Constructors:
        push    bp
        mov     bp,sp
        push    si
        push    di

        mov     ax,DGROUP
        mov     ds,ax

        mov     es, ax
        mov     si,offset DGROUP:InitStart      ;si = start of table
        mov     di,offset DGROUP:InitEnd        ;di = end of table

        call    Initialize
        call    Doctors

        pop     di
        pop     si

        jmp     back_to_pb


Destructors:
        push    bp
        mov     bp,sp
        push    si
        push    di

        mov     ax,DGROUP
        mov     ds,ax
        mov     es, ax

        mov     si,offset DGROUP:ExitStart      ;si = start of table
        mov     di,offset DGROUP:ExitEnd        ;di = end of table

        call    Cleanup
        call    Dodtors

        pop     di
        pop     si

        jmp     back_to_pb

Initialize      proc near
@@Start:        mov     ax,100h                 ;start with lowest priority
                mov     dx,di                   ;set sentinel to end of table
                mov     bx,si                   ;bx = start of table

@@TopOfTable:   cmp     bx,di                   ;and the end of the table?
                je      @@EndOfTable            ;yes, exit the loop
                cmp     es:[bx.calltype],NOTUSED;check the call type
                je      @@Next
                mov     cl, es:[bx.priority]    ;move priority to CX
                xor     ch, ch
                cmp     cx,ax                   ;check the priority
                jae     @@Next                  ;too high?  skip
                mov     ax,cx                   ;keep priority
                mov     dx,bx                   ;keep index in dx
@@Next:         add     bx,SIZE SE              ;bx = next item in table
                jmp     @@TopOfTable

@@EndOfTable:   cmp     dx,di                   ;did we exhaust the table?
                je      @@Done                  ;yes, quit
                mov     bx,dx                   ;bx = highest priority item
                cmp     es:[bx.calltype],PNEAR  ;is it near or far?
                mov     es:[bx.calltype],NOTUSED;wipe the call type
                push    es                      ;save es
                je      @@NearCall

@@FarCall:      call    DWORD PTR es:[bx.addrlow]
                pop     es                      ;restore es
                jmp     short @@Start

@@NearCall:     call    WORD PTR es:[bx.addrlow]
                pop     es                      ;restore es
                jmp     short @@Start

@@Done:         ret
                endp

Cleanup         proc near
@@Start:        mov     ah,0                    ;start with highest priority
                mov     dx,di                   ;set sentinel to end of table
                mov     bx,si                   ;bx = start of table

@@TopOfTable:   cmp     bx,di                   ;and the end of the table?
                je      @@EndOfTable            ;yes, exit the loop
                cmp     es:[bx.calltype],NOTUSED;check the call type
                je      @@Next
                cmp     es:[bx.priority],ah     ;check the priority
                jb      @@Next                  ;too low?  skip
                mov     ah,es:[bx.priority]     ;keep priority
                mov     dx,bx                   ;keep index in dx
@@Next:         add     bx,SIZE SE              ;bx = next item in table
                jmp     @@TopOfTable

@@EndOfTable:   cmp     dx,di                   ;did we exhaust the table?
                je      @@Done                  ;yes, quit
                mov     bx,dx                   ;bx = highest priority item
                cmp     es:[bx.calltype],PNEAR  ;is it near or far?
                mov     es:[bx.calltype],NOTUSED;wipe the call type
                push    es                      ;save es
                je      @@NearCall

@@FarCall:      call    DWORD PTR es:[bx.addrlow]
                pop     es                      ;restore es
                jmp     short @@Start

@@NearCall:     call    WORD PTR es:[bx.addrlow]
                pop     es                      ;restore es
                jmp     short @@Start

@@Done:         ret
                endp


Doctors proc    near
	;Call far ctors
        mov     DI,offset DGROUP:ZInitEnd
@@CT2:
        cmp     DI,offset DGROUP:ZInitStart
        jbe     @@CT4
	sub	DI,4
        mov     AX,[DI]
        or      AX,2[DI]
        jz      @@CT3                            ;skip null pointers
        call    dword ptr [DI]
@@CT3:
        jmp     @@CT2

@@CT4:    ret
Doctors endp

Dodtors proc near

@@DT1:
        mov     DI,offset DGROUP:ZExitStart
@@DT2:
        cmp     DI,offset DGROUP:ZExitEnd
        jae     @@DT4
        mov     AX,[DI]
        or      AX,2[DI]
        jz      @@DT3                             ;skip null pointers
        call    dword ptr [DI]
@@DT3:
	add	DI,4
        jmp     @@DT2

@@DT4:
	ret
Dodtors endp


; -----------------------------------------------------------------------
;  Function table. Must be filled by the host application
; -----------------------------------------------------------------------

EVEN

FuncTable  dd 256 DUP (87654321h)

; -----------------------------------------------------------------------
;  Function dispatch code. Used to direct PEX-calls to host application
; -----------------------------------------------------------------------

dispatch:  mov   bx,offset FuncTable
           mov   cl,2
           xor   ah,ah
           shl   ax,cl
           add   bx,ax
           push  cs:[host_ds]
           pop   ds
           call  dword ptr cs:[bx]
           mov    cx,DGROUP
           mov    ds,cx
           retf

   EVEN

        FuncHook l_mul
        FuncHook l_div
        FuncHook l_mod
        FuncHook ul_div
        FuncHook ul_mod
        FuncHook l_shl
        FuncHook l_shr
        FuncHook ul_shl
        FuncHook ul_shr
        FuncHook fopen
        FuncHook freopen
        FuncHook fseek
        FuncHook ftell
        FuncHook fgets
        FuncHook fgetc
        FuncHook fflush
        FuncHook fclose
        FuncHook fputs
        FuncHook getc
        FuncHook getchar
        FuncHook gets
        FuncHook fputc
        FuncHook putc
        FuncHook putchar
        FuncHook puts
        FuncHook fread
        FuncHook fwrite
        FuncHook printf
        FuncHook fprintf
        FuncHook vfprintf
        FuncHook vprintf
        FuncHook sprintf
        FuncHook vsprintf
        FuncHook setbuf
        FuncHook setvbuf
        FuncHook rename
        FuncHook rewind
        FuncHook clearerr
        FuncHook feof
        FuncHook isalnum
        FuncHook isalpha
        FuncHook iscntrl
        FuncHook isdigit
        FuncHook isgraph
        FuncHook islower
        FuncHook isprint
        FuncHook ispunct
        FuncHook isspace
        FuncHook isupper
        FuncHook isxdigit
        FuncHook toupper
        FuncHook tolower
        FuncHook int86
        FuncHook int86x
        FuncHook intdos
        FuncHook intdosx
        FuncHook dos_findfirst
        FuncHook dos_findnext
        FuncHook read
        FuncHook write
        FuncHook open
        FuncHook creat
        FuncHook close
        FuncHook unlink
        FuncHook remove
        FuncHook chsize
        FuncHook dup
        FuncHook dup2
        FuncHook lseek
        FuncHook access
        FuncHook filesize
        FuncHook filelength
        FuncHook isatty
        FuncHook atoi
        FuncHook atol
        FuncHook strtol
        FuncHook rand
        FuncHook srand
        FuncHook calloc
        FuncHook free
        FuncHook malloc
        FuncHook realloc
        FuncHook getenv
        FuncHook ___bsearch
        FuncHook ___qsort
        FuncHook abs
        FuncHook labs
        FuncHook memcpy
        FuncHook memmove
        FuncHook strcpy
        FuncHook strncpy
        FuncHook strcat
        FuncHook strncat
        FuncHook memcmp
        FuncHook strcmp
        FuncHook strncmp
        FuncHook memchr
        FuncHook strchr
        FuncHook strcspn
        FuncHook strpbrk
        FuncHook strrchr
        FuncHook strspn
        FuncHook strstr
        FuncHook strtok
        FuncHook memset
        FuncHook strlen
        FuncHook memicmp
        FuncHook stpcpy
        FuncHook strcmpl
        FuncHook strnicmp
        FuncHook strdup
        FuncHook strlwr
        FuncHook strupr
        FuncHook strnset
        FuncHook strrev
        FuncHook strset
        FuncHook swab
        FuncHook clock
        FuncHook time
        FuncHook mktime
        FuncHook asctime
        FuncHook ctime
        FuncHook localtime
        FuncHook gmtime
        FuncHook strftime
        FuncHook sleep
        FuncHook usleep
        FuncHook msleep
        FuncHook form
        FuncHook MemAvail
        FuncHook SetColor
        FuncHook MenuFunction
        FuncHook WaitKey
        FuncHook WaitKeys
        FuncHook Input
        FuncHook Log
        FuncHook HangUp
        FuncHook PostMessage
        FuncHook ReadMsgArea
        FuncHook MsgEd
        FuncHook AddTime
        FuncHook TimeLeft
        FuncHook TimeOnline
        FuncHook CheckAccess
        FuncHook SuspendTimer
        FuncHook RestartTimer
        FuncHook _InstallHandler
        FuncHook EnableStop
        FuncHook DisableStop
        FuncHook Stopped
        FuncHook IO_SendByte
        FuncHook IO_ReadByte
        FuncHook IO_Carrier
        FuncHook IO_ByteReady
        FuncHook IO_BufEmpty
        FuncHook IO_StartBreak
        FuncHook IO_StopBreak
        FuncHook IO_DTR
        FuncHook PostInfo
        FuncHook ShowHotkeyFile
        FuncHook ShowHotkeyANSIFile
        FuncHook PlayMusic
        FuncHook AdjustTime
        FuncHook ReadUser
        FuncHook WriteUser
        FuncHook ReadMessage
        FuncHook WriteMSGTMP
        FuncHook AppendMSGTMP
        FuncHook ShowMessage
        FuncHook CreateMessageText
        FuncHook FirstMessage
        FuncHook NextMessage
        FuncHook DeleteMessage
        FuncHook PostNetmail
        FuncHook MarkMessage
        FuncHook ReadMarkedMessages
        FuncHook ListMarkedMessages
        FuncHook UnMarkAllMessages
        FuncHook Ask
        FuncHook PeekChar
        FuncHook ScanKey
        FuncHook InitLineCounter
        FuncHook LineCounter
        FuncHook ReadFileArea
        FuncHook CreateMessageTextString
        FuncHook FuzzySearch
        FuncHook _RemoveHandler
        FuncHook ExternalInput
        FuncHook GotoXY
        FuncHook SetFullColor
        FuncHook putenv
        FuncHook ErrorLevel
        FuncHook ClrEol
        FuncHook GetIniVar
        FuncHook SetIniVar
        FuncHook NumMsgAreas
        FuncHook NumFileAreas
        FuncHook GetLastRead
        FuncHook SetLastRead
        FuncHook ReadHandler
        FuncHook RestoreHandler
        FuncHook ParseStringVars
        FuncHook LocalDisplay
        FuncHook ShowRIPscrip
        FuncHook RIP
        FuncHook RemoteDisplay
        FuncHook MsgNum
        FuncHook MsgId
        FuncHook HighMsg
        FuncHook NumMsgs
        FuncHook TimeUntilEvent
        FuncHook ResetInactivity
        FuncHook mkdir
        FuncHook rmdir
        FuncHook chdir
        FuncHook GetTaggedFiles
        FuncHook PutTaggedFiles
        FuncHook AddTaggedFile
        FuncHook RemoveTaggedFile
        FuncHook ClearTaggedFiles
        FuncHook IsTagged
        FuncHook SetUser
        FuncHook IO_SendBlock
        FuncHook IO_ReadBlock
        FuncHook getdisk                     ; Added in v2.2
        FuncHook getcurdir                   ; Added in v2.2
        FuncHook _dos_getftime               ; Added in v2.2
        FuncHook _dos_setftime               ; Added in v2.2
        FuncHook fileno                      ; Added in v2.2

        FuncHook _fut_func_20                ; Unused (available for use)
        FuncHook _fut_func_21                ; Unused (available for use)
        FuncHook _fut_func_22                ; Unused (available for use)
        FuncHook _fut_func_23                ; Unused (available for use)
        FuncHook _fut_func_24                ; Unused (available for use)
        FuncHook _fut_func_25                ; Unused (available for use)
        FuncHook _fut_func_26                ; Unused (available for use)
        FuncHook _fut_func_27                ; Unused (available for use)
        FuncHook _fut_func_28                ; Unused (available for use)
        FuncHook _fut_func_29                ; Unused (available for use)
        FuncHook _fut_func_30                ; Unused (available for use)
        FuncHook _fut_func_31                ; Unused (available for use)
        FuncHook _fut_func_32                ; Unused (available for use)
        FuncHook _fut_func_33                ; Unused (available for use)
        FuncHook _fut_func_34                ; Unused (available for use)
        FuncHook _fut_func_35                ; Unused (available for use)
        FuncHook _fut_func_36                ; Unused (available for use)
        FuncHook _fut_func_37                ; Unused (available for use)
        FuncHook _fut_func_38                ; Unused (available for use)
        FuncHook _fut_func_39                ; Unused (available for use)
        FuncHook _fut_func_40                ; Unused (available for use)
        FuncHook _fut_func_41                ; Unused (available for use)
        FuncHook _fut_func_42                ; Unused (available for use)
        FuncHook _fut_func_43                ; Unused (available for use)
        FuncHook _fut_func_44                ; Unused (available for use)
        FuncHook _fut_func_45                ; Unused (available for use)
        FuncHook _fut_func_46                ; Unused (available for use)


; -----------------------------------------------------------------------
;  Data table. Must be filled by the host application
; -----------------------------------------------------------------------

PBSDK_TEXT ENDS

_DATA SEGMENT

DataTable LABEL WORD

         DataHook CurUser
         DataHook LoginDate
         DataHook LoginTime
         DataHook UserRecNr
         DataHook Limits
         DataHook NumLimits
         DataHook NetEntered
         DataHook EchoEntered
         DataHook NumUsers
         DataHook NodeNumber
         DataHook CurMenu
         DataHook UserFirstName
         DataHook PrevUser
         DataHook StartupPath
         DataHook SysPath
         DataHook Config
         DataHook PBVersion
         DataHook BaudRate
         DataHook Beta
         DataHook PageReason
         DataHook PageCount
         DataHook _fut_dat_06
         DataHook _fut_dat_07
         DataHook _fut_dat_08
         DataHook _fut_dat_09
         DataHook _fut_dat_10
         DataHook _fut_dat_11
         DataHook _fut_dat_12
         DataHook _fut_dat_13
         DataHook _fut_dat_14
         DataHook _fut_dat_15
         DataHook _fut_dat_16
         DataHook _fut_dat_17
         DataHook _fut_dat_18
         DataHook _fut_dat_19
         DataHook _fut_dat_20

_DATA ENDS

END start

