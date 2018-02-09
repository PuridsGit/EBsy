 AREA    Text, CODE, READONLY, ALIGN=2
 EXPORT ctxsw
 EXPORT ctxswi
 EXPORT firstContext
 EXPORT firstContexti
 THUMB
		
ctxsw\
	push { lr }
	push { r4 - r7 }
	mov r6, r12
	mov r5, r11
	mov r4, r10
	mov r3, r9
	mov r2, r8
	push { r2 - r6 }

	str sp, [r0]
	ldr sp, [r1]

	pop { r2 - r6 }
	mov r12, r6
	mov r11, r5
	mov r10, r4
	mov r9, r3
	mov r8, r2
	pop { r4 - r7}
	pop { pc }
	
ctxswi\

	mov r2,r0
	mrs r0, psp
	; subs r0, #32
	stmdb r0!, {r4-r11}

	str r0, [r2]
	ldr r0, [r1]

	ldmia r0!, { r4 - r11 }
	msr psp, r0
	mov lr, #0xFFFFFFFD
	bx lr

firstContext\
	mov sp, r0
	pop { r2-r6 }
	mov r12, r6
	mov r11, r5
	mov r10, r4
	mov r9, r3
	mov r8, r2
	pop { r4-r7, pc }
	
firstContexti\
	mrs r1, CONTROL
	mov r2, #2
	orr r1, r2
	msr CONTROL, r1
	add r0, #20
	mov sp, r0
	pop { r4-r11 }
	pop { lr }
	pop	{ pc }
	
	END