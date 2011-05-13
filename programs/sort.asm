main :

    ; allocate 8 words for items to be sorted
    add rsp, -32
    
    ; place 8 words in the first 8 locations of this
    ; stack frame
    mov r0  , rbp
    mov r1, 4
    mov [r0], r1
    
    add r0, -4
    mov r1, 7
    mov [r0], r1
    
    add r0, -4
    mov r1, 1
    mov [r0], r1
    
    add r0, -4
    mov r1, 3
    mov [r0], r1
    
    add r0, -4
    mov r1, 8
    mov [r0], r1
    
    add r0, -4
    mov r1, 5
    mov [r0], r1
    
    add r0, -4
    mov r1, 2
    mov [r0], r1
    
    add r0, -4
    mov r1, 6
    mov [r0], r1
    
    mov r1, 8
    push r1
    push r0
    call bubble_sort
    add rsp, -8
    hlt

; bubble_sort (word * items, int items_n)
bubble_sort :
    push rbp
    mov rbp, rsp
    
    mov r7, rbp
    add r7, 12
    mov r7, [r7] ; items_n
    
    mov r6, rbp
    add r6, 8
    mov r6, [r6] ; items
    
    mov r0, 4
    mul r7, r0
    add r7, r6 ; items + (items_n * 4)
    
    mov r5, 0 ; this gets set to 1 if we swap

    mov r0, r6

bubble_sort_loop :    
    mov r1, r0
    add r1, 4
    
    mov r2, [r0]
    mov r3, [r1]
    
    cmp r2, r3
    jg bubble_sort_swap
    
bubble_sort_loop_1 :
    add r0, 4
    cmp r0, r7
    je bubble_sort_loop_end
    jmp bubble_sort_loop
    
bubble_sort_loop_end :
    cmp r5, 0
    je pop_return
    
    mov r5, 0
    mov r0, r6
    jmp bubble_sort_loop
    
bubble_sort_swap :
    mov [r0], r3
    mov [r1], r2
    mov r5, 1
    jmp bubble_sort_loop_1

pop_return :
    pop rbp
    ret
