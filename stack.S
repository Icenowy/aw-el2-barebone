.section .bss

.globl stack_generic_end
stack_generic_end:
	.space 512, 0
.globl stack_generic_core3
stack_generic_core3:
	.space 512, 0
.globl stack_generic_core2
stack_generic_core2:
	.space 512, 0
.globl stack_generic_core1
stack_generic_core1:
	.space 512, 0
.globl stack_generic
stack_generic:

.globl stack_exception_start
stack_exception_start:
	.space 512, 0
.globl stack_exception
stack_exception:
