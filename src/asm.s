.section .text
	.global check_type

check_type:
	imull $20, %esi              # 20*2 = 40 Bytes (Struct size).
	movw 20(%rdi, %rsi, 2), %bx  # Moves the containerType to %bx.

	cmpw $0, %bx
	jne return_one

return_zero:
	movl $0, %eax  # Returns 0 if containerType is zero.
	jmp end

return_one:
	movl $1, %eax  # Returns 1 if containerType is not zero.

end:
	ret
