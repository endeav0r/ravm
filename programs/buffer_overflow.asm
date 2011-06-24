; must be assembled with string.asm

buffer_string : db 0x20fffffc, 0x3e414243, 0x41414141, 0x41414141, 0x41414141, 0x41414141, 0xdc030000
password      : db "passw0rd"
target_value  : db 0xdeadbeef

main :
    mov r0, buffer_string
    push r0
    
    call check_string
    add rsp, 4
    
    cmp r0, 1
    je set_target

    hlt
    
set_target :
    mov r0, target_value
    mov r1, 0
    mov [r0], r1
    hlt

; int check_string (char * s)
; returns 1 if string matches password
; returns 0 otherwise
check_string :
    push rbp
    mov rbp, rsp
    
    add rsp, -20 ; add room for 20 bit string
    
    ; prepare strcpy call
    mov r0, rbp
    add r0, 8
    mov r0, [r0]
    push r0
    
    mov r0, rbp
    add r0, -20
    push r0
    
    call strcpy
    
    add rsp, 8
    
    ; check password
    
    mov r0, rbp
    add r0, -20
    push r0
    
    mov r0, password
    push r0
    
    call strcmp
    add rsp, 8

    cmp r0, 0
    je check_string_ret_1
    jmp check_string_ret_0

check_string_ret_1 :
    mov r0, 1
    jmp check_string_ret

check_string_ret_0 :
    mov r0, 0
    jmp check_string_ret

check_string_ret :
    add rsp, 20
    pop rbp
    ret
