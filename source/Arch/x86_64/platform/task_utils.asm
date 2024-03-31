[SECTION .s64]
[BITS 64]
GLOBAL _asm_switch_to
_asm_switch_to:
    PUSH RSI
    PUSH RDI
    PUSH RBX
    PUSH RBP
    PUSH R12
    PUSH R13
    PUSH R14
    PUSH R15
    MOV RAX, [RSP + 72]
    MOV [RAX], RSP
    MOV RAX, [RSP + 80]
    MOV RSP, [RAX]
    POP R15
    POP R14
    POP R13
    POP R12
    POP RBP
    POP RBX
    POP RDI
    POP RSI
    RET