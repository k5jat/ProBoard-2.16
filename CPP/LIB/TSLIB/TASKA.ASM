
% MODEL MEM_MOD

STRUC state
      task_ss dw 0
      task_sp dw 0
ENDS state

PUBLIC __switch_task
PUBLIC __set_stack

CODESEG

PROC __switch_task
       ARG  task1 : PTR , task2 : PTR
       push     bp
       mov      bp,sp
       push     si
       push     di

if @DataSize
       les      bx,[task1]
else
       push     ds
       pop      es
       mov      bx,[task1]
endif

       mov      [es:bx+0],ss
       mov      [es:bx+2],sp

if @DataSize
       les      bx,[task2]
else
       mov      bx,[task2]
endif

       cli
       mov      ss,[es:bx+0]
       mov      sp,[es:bx+2]
       sti

       pop      di
       pop      si
       pop      bp
       ret
ENDP __switch_task


PROC __set_stack
       ARG nexttask : PTR

       push     bp
       mov      bp,sp

if @DataSize
       les      bx,[nexttask]
else
       push     ds
       pop      es
       mov      bx,[nexttask]
endif
       pop      bp

       mov      bp,sp

       mov      ax,[bp]         ; ax = ip
if @CodeSize
       mov      dx,[bp+2]       ; dx = cs
endif

       cli
       mov      ss,[es:bx+0]
       mov      sp,[es:bx+2]
       sub      sp,2            ; needed for argument
       sti

if @CodeSize
       push     dx              ; push cs back
endif
       push     ax              ; push ip back

       ret
ENDP __set_stack

end
