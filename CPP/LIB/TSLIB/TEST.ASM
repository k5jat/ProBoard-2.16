	ifndef	??version
?debug	macro
	endm
publicdll macro	name
	public	name
	endm
$comm	macro	name,dist,size,count
	comm	dist name:BYTE:count*size
	endm
	else
$comm	macro	name,dist,size,count
	comm	dist name[size]:BYTE:count
	endm
	endif
	?debug	V 300h
	?debug	S "test.cpp"
	?debug	C E90E95651908746573742E637070
	?debug	C E94019CA1817433A5C424333315C494E434C5544455C737464696F+
	?debug	C 2E68
	?debug	C E94019CA1817433A5C424333315C494E434C5544455C5F64656673+
	?debug	C 2E68
	?debug	C E94019CA1818433A5C424333315C494E434C5544455C5F6E66696C+
	?debug	C 652E68
	?debug	C E94019CA1817433A5C424333315C494E434C5544455C5F6E756C6C+
	?debug	C 2E68
	?debug	C E94019CA1818433A5C424333315C494E434C5544455C7374646C69+
	?debug	C 622E68
	?debug	C E94019CA1818433A5C424333315C494E434C5544455C737472696E+
	?debug	C 672E68
	?debug	C E91AAA621914433A5C4350505C4C49425C746F6F6C732E687070
_TEXT	segment byte public 'CODE'
_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
_TEXT	segment byte public 'CODE'
   ;	
   ;	main()
   ;	
	assume	cs:_TEXT
_main	proc	near
	push	bp
	mov	bp,sp
	sub	sp,22
	push	si
   ;	
   ;	{
   ;	    ObjFile<long> f;
   ;	
	lea	ax,word ptr [bp-22]
	push	ax
	call	near ptr @File@init$qv
	pop	cx
   ;	
   ;	
   ;	    f.open("TEST.FIL",fmode_create);
   ;	
	xor	ax,ax
	push	ax
	mov	ax,4
	push	ax
	mov	ax,offset DGROUP:s@
	push	ax
	lea	ax,word ptr [bp-22]
	push	ax
	call	near ptr @File@open$qpzcuiui
	add	sp,8
   ;	
   ;	
   ;	    long x = 5;
   ;	
	mov	word ptr [bp-2],0
	mov	word ptr [bp-4],5
   ;	
   ;	
   ;	    for(int i=0;i<10;i++,x++) 
   ;	
	xor	si,si
	jmp	short @1@114
@1@58:
   ;	
   ;	    {
   ;	        f.write(x);
   ;	
	lea	ax,word ptr [bp-4]
	push	ax
	lea	ax,word ptr [bp-22]
	push	ax
	call	near ptr @@%ObjFile$tl%@write$qrl
	pop	cx
	pop	cx
   ;	
   ;	        f << x;
   ;	
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	lea	ax,word ptr [bp-22]
	push	ax
	call	near ptr @File@$blsh$ql
	add	sp,6
	inc	si
	add	word ptr [bp-4],1
	adc	word ptr [bp-2],0
@1@114:
	cmp	si,10
	jl	short @1@58
	lea	ax,word ptr [bp-22]
	push	ax
	call	near ptr @File@close$qv
	pop	cx
   ;	
   ;	    }
   ;	
   ;	    return 0;
   ;	
	xor	ax,ax
	jmp	short @1@170
@1@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_main	endp
_TEXT	ends
_TEXT	segment byte public 'CODE'
@%ObjFile$tl%@seek$ql	segment	virtual
	assume	cs:_TEXT
@@%ObjFile$tl%@seek$ql	proc	near
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	mov	si,word ptr [bp+4]
	mov	dx,word ptr [bp+8]
	mov	ax,word ptr [bp+6]
	mov	cl,2
	call	near ptr N_LXLSH@
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
	xor	ax,ax
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	push	si
	call	near ptr @File@seek$qli
	add	sp,8
	cmp	dx,word ptr [bp-2]
	jne	short @2@114
	cmp	ax,word ptr [bp-4]
	jne	short @2@114
	mov	al,1
	jmp	short @2@142
@2@114:
	mov	al,0
@2@142:
	jmp	short @2@170
@2@170:
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@@%ObjFile$tl%@seek$ql	endp
@%ObjFile$tl%@seek$ql	ends
_TEXT	ends
_TEXT	segment byte public 'CODE'
@%ObjFile$tl%@$brsh$qrl	segment	virtual
	assume	cs:_TEXT
@@%ObjFile$tl%@$brsh$qrl	proc	near
	push	bp
	mov	bp,sp
	push	si
	mov	si,word ptr [bp+4]
	push	word ptr [bp+6]
	push	si
	call	near ptr @@%ObjFile$tl%@write$qrl
	pop	cx
	pop	cx
	mov	ax,si
	jmp	short @3@58
@3@58:
	pop	si
	pop	bp
	ret	
@@%ObjFile$tl%@$brsh$qrl	endp
@%ObjFile$tl%@$brsh$qrl	ends
_TEXT	ends
_TEXT	segment byte public 'CODE'
@%ObjFile$tl%@write$qrl	segment	virtual
	assume	cs:_TEXT
@@%ObjFile$tl%@write$qrl	proc	near
	push	bp
	mov	bp,sp
	push	si
	mov	si,word ptr [bp+4]
	mov	ax,4
	push	ax
	push	word ptr [bp+6]
	push	si
	call	near ptr @File@write$qpvui
	add	sp,6
	cmp	ax,4
	je	short @4@114
	mov	al,0
@4@86:
	jmp	short @4@142
@4@114:
	mov	al,1
	jmp	short @4@86
@4@142:
	pop	si
	pop	bp
	ret	
@@%ObjFile$tl%@write$qrl	endp
@%ObjFile$tl%@write$qrl	ends
_TEXT	ends
_TEXT	segment byte public 'CODE'
@%ObjFile$tl%@read$qrl	segment	virtual
	assume	cs:_TEXT
@@%ObjFile$tl%@read$qrl	proc	near
	push	bp
	mov	bp,sp
	push	si
	mov	si,word ptr [bp+4]
	mov	ax,4
	push	ax
	push	word ptr [bp+6]
	push	si
	call	near ptr @File@read$qpvui
	add	sp,6
	cmp	ax,4
	je	short @5@114
	mov	al,0
@5@86:
	jmp	short @5@142
@5@114:
	mov	al,1
	jmp	short @5@86
@5@142:
	pop	si
	pop	bp
	ret	
@@%ObjFile$tl%@read$qrl	endp
	?debug	C E9
	?debug	C FA00000000
@%ObjFile$tl%@read$qrl	ends
_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	'TEST.FIL'
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	extrn	N_LXLSH@:far
	public	_main
	extrn	@File@$blsh$ql:near
	extrn	@File@seek$qli:near
	extrn	@File@write$qpvui:near
	extrn	@File@read$qpvui:near
	extrn	@File@close$qv:near
	extrn	@File@open$qpzcuiui:near
	extrn	@File@init$qv:near
_atoi	equ	atoi
_abs	equ	abs
	extrn	@$bdele$qpv:near
_s@	equ	s@
	end
