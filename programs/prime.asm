main :
    mov rbp, rsp
    add rsp, -40
    mov r1, 2

main_loop :
    push r1
    call check_for_prime
    
    mov r2, rbp
    mov r3, r1
    mov r4, 4
    mul r3, r4
    sub r2, r3
    mov [r2], r0
    
    add r1, 1
    cmp r1, 10
    je main_hlt
    jmp main_loop


main_hlt :
    hlt

check_for_prime :
    push rbp
    mov rbp, rsp
    
    ; we are going to use r0, r1 and r2
    ; however, r0 is the return register, so we do not need to save its value
    push r1
    push r2
    
    mov r0, rbp
    add r0, 8
    mov r0, [r0]
    
    mov r1, 2 ; r1 is the number we check against r0
    
check_for_prime_loop :
    mov r2, r1
    mul r2, r2
    cmp r2, r0
    jg check_for_prime_return_true
    
    mov r2, r0
    mod r2, r1
    cmp r2, 0
    jz check_for_prime_return_false
    
    add r1, 1
    jmp check_for_prime_loop
    
check_for_prime_return_false :
    mov r0, -1
    jmp check_for_prime_return

check_for_prime_return_true :
    mov r0, 1
    jmp check_for_prime_return
    
check_for_prime_return :
    pop r2
    pop r1
    pop rbp
    ret
