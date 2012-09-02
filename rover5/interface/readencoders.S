; Written by adam!

    .section    .text.__vector_3,"ax",@progbits
.global __vector_3
    .type __vector_3, @function
__vector_3:
    push r19 ; prepare r19  so we can use them ASAP
    in r19,0x3 ; read the port in FAST

    push r1 ; save the rest of the registers
    push r0
    in r0,__SREG__
    push r0
    clr r0

    push r19
    push r20
    push r21
    push r22
    push r23
    push r24
    push r25

    ;mov r19,r18

    ;; curA
    ;andi r18,0x1 ; Get the low bit
    ;; curB
    swap r19     ; put the 4th byte into the low byte and low byte into 4th
    ;andi r19,0x1 ;
    
    ; Probably will need better name
    lds r25,_ZZ10__vector_3E5lastA

    lds r20,encdists+8+0
    lds r21,encdists+8+1
    lds r22,encdists+8+2
    lds r23,encdists+8+3

    eor r25,r19

    ; If we need to add, skip the jump and continue
    sbrs r25,0
    rjmp .L12

    ; add 1
    subi r20,lo(-1)
    ; The carries can be done later since they're the same
    ;adc r21,0
    ;adc r22,0
    ;adc r23,0
    
    rjmp .L13
.L12
    ; subtract 1
    subi r20,1
    sbic r21,0
    sbic r22,0
    sbic r23,0
.L13
    ; Set lastA to curA (which moved from bit 0 to bit 4 on the swap)
    bst r19,4
    bld r19,0
    sts _ZZ10__vector_3E5lastA,r19

    sts encdists+8+0,r20
    sts encdists+8+1,r21
    sts encdists+8+2,r22
    sts encdists+8+3,r23
    
    pop r25
    pop r24
    pop r23
    pop r22
    pop r21
    pop r20
    pop r19
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_3, .-__vector_3
