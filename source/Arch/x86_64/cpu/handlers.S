.section .text
.GLOBAL interrupt_entry
.GLOBAL interrupt_exit
interrupt_entry:
    PUSHQ %R15
    PUSHQ %R14
    PUSHQ %R13
    PUSHQ %R12
    PUSHQ %R11
    PUSHQ %R10
    PUSHQ %R9
    PUSHQ %R8
    PUSHQ %RDI
    PUSHQ %RSI
    PUSHQ %RBP
    PUSHQ %RDX
    PUSHQ %RCX
    PUSHQ %RBX
    PUSHQ %RAX 
    MOVQ  %GS, %RAX
    PUSHQ %RAX
    MOVQ  %FS, %RAX
    PUSHQ %RAX
    MOVQ  %ES, %RAX
    PUSHQ %RAX
    MOVQ  %DS, %RAX
    PUSHQ %RAX
    MOVQ  %RSP, %RSI
    MOVW  %SS, %DX
    MOVW  %DX, %DS
    MOVW  %DX, %ES
    MOVQ %RSI, %RDI
    LEAQ general_interrupt_handler(%RIP), %RAX
    CALLQ *%RAX
interrupt_exit: /* 可调用 */
    MOVQ %RAX, %RSP
    POPQ %RAX
    MOVQ %RAX, %DS
    POPQ %RAX
    MOVQ %RAX, %ES
    POPQ %RAX
    MOVQ %RAX, %FS
    POPQ %RAX
    MOVQ %RAX, %GS
    POPQ %RAX
    POPQ %RBX
    POPQ %RCX
    POPQ %RDX
    POPQ %RBP
    POPQ %RSI
    POPQ %RDI
    POPQ %R8
    POPQ %R9
    POPQ %R10
    POPQ %R11
    POPQ %R12
    POPQ %R13
    POPQ %R14
    POPQ %R15
    ADD $24, %RSP
    STI
    IRETQ

.section .data
.GLOBAL stack_space
stack_space: 
    .long 0x10000 // 64kb栈空间
stack_top:
