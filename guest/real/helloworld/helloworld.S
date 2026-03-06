.code16
.global _start

_start:
    mov $msg, %si

print:
    lodsb              # AL = *SI++
    test %al, %al
    jz halt
    mov $0xE9, %dx
    out %al, %dx
    jmp print

halt:
    jmp halt

msg:
    .ascii "hello from vm!\n"
    .byte 0
