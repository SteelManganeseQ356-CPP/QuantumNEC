.GLOBAL _asm_thread_switch_to
_asm_thread_switch_to:
    CLI
    /* 栈中此处是RIP */
    PUSHQ %RBP
    PUSHQ %RSI
    PUSHQ %RDI
    PUSHQ %RBX
    PUSHQ %R12
    PUSHQ %R13
    PUSHQ %R14
    PUSHQ %R15
    MOVQ 72(%RSP), %RAX
    MOVQ %RSP, (%RAX)
    /* 保存这个进程数据 */
    /* 切换到下一线程 */
    MOVQ 80(%RSP), %RAX
    MOVQ (%RAX), %RSP
    POPQ %R15
    POPQ %R14
    POPQ %R13
    POPQ %R12
    POPQ %RBX
    POPQ %RDI
    POPQ %RSI
    POPQ %RBP
    STI
    RETQ  /* 返回到上面switch_to下面的那句注释的RIP */