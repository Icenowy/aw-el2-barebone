.section .bss

.globl stack_generic_end
stack_generic_end:
	.space 512, 0
.globl stack_generic
stack_generic:

.globl stack_exception_start
stack_exception_start:
	.space 512, 0
.globl stack_exception
stack_exception:
