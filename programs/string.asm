main:

    ; test test_string
    mov r0, test_string
    push r0
    call strlen
    add rsp, 4
    nop
    nop
    nop
    
    ; test strcpy
    mov r0, test_string
    mov r1, copy_string
    push r1
    push r0
    call strcpy
    add rsp, 8
    nop
    nop
    nop
    
    ; test strcmp
    mov r0, test_string
    mov r1, copy_string
    push r0
    push r1
    call strcmp
    add rsp, 8
    
    hlt

; int strlen (char * string)
; returns number of characters in string until null byte is found
strlen :
    push rbp
    mov  rbp, rsp
    
    push r1
    mov  r0, rbp
    add  r0, 8
    mov  r0, [r0]
    
strlen_loop :
    mov r1, byte [r0]
    ;and r1, 0xff
    cmp r1, 0
    je strlen_ret
    
    add r0, 1
    jmp strlen_loop

strlen_ret :
    mov r1, rbp
    add r1, 8
    mov r1, [r1]
    sub r0, r1
    add r0, -1
    pop r1
    pop rbp
    ret

; void strcpy (char * dest, char * src)
; copies string from source into destination
strcpy :
    push rbp
    mov  rbp, rsp
    
    ; r1 = dest
    ; r2 = src
    push r1
    push r2
    mov r1, rbp
    mov r2, rbp
    add r1, 8
    add r2, 12
    mov r1, [r1]
    mov r2, [r2]
    
strcpy_loop :
    mov r0, byte [r2]
    cmp r0, 0
    je strcpy_ret
    
    mov byte [r1], r0
    
    add r1, 1
    add r2, 1
    jmp strcpy_loop

strcpy_ret :
    pop r2
    pop r1
    pop rbp
    ret

; int strcmp (char * a, char * b)
; returns 0 if strings are identical
; returns -1 if string a less than string b
; returns 1 if string a greater than string b
strcmp :
    push rbp
    mov rbp, rsp
    
    push r1
    push r2
    push r3
    
    mov r2, rbp
    add r2, 8 
    mov r2, [r2]
    mov r3, rbp
    add r3, 12
    mov r3, [r3]
    
strcmp_loop :
    mov r1, byte [r3]
    cmp r1, 0
    je strcmp_ret_0
    
    mov r0, byte [r2]
    cmp r0, r1
    je strcmp_loop_add
    jg strcmp_ret_neg1
    jmp strcmp_ret_1

strcmp_loop_add :
    add r2, 1
    add r3, 1
    jmp strcmp_loop

strcmp_ret_0 :
    mov r0, 0
    jmp strcmp_ret

strcmp_ret_neg1 :
    mov r0, -1
    jmp strcmp_ret

strcmp_ret_1 :
    mov r0, 1
    jmp strcmp_ret

strcmp_ret :
    pop r3
    pop r2
    pop r1
    pop rbp
    ret
    
test_string : db "this is a test"

copy_string : db "ABCDEFGHIJKLMN"
