	.file	"kernel.cc"
gcc2_compiled.:
___gnu_compiled_cplusplus:
.data
_Esc:
	.byte 27
LC0:
	.ascii "Warning!!!  No more free task avaiable pNext:%d\12\0"
.text
	.align 4
.globl ___6Kernel
___6Kernel:
	pushl %ebp
	movl %esp,%ebp
	pushl %ebx
	movl 8(%ebp),%ebx
	leal 8(%ebx),%eax
	pushl %eax
	call ___13TaskDescTable
	addl $4,%esp
L82:
	movl %ebx,%eax
	jmp L81
	.align 4,0x90
	jmp L81
	.align 4,0x90
L81:
	movl -4(%ebp),%ebx
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
.globl _initKernel__6Kernelii
_initKernel__6Kernelii:
	pushl %ebp
	movl %esp,%ebp
	pushl %ebx
	movl 8(%ebp),%ebx
	movl 12(%ebp),%eax
	pushl %eax
	pushl %ebx
	call _setKernelDataStart__6Kerneli
	addl $8,%esp
	movl 16(%ebp),%eax
	pushl %eax
	pushl %ebx
	call _setStackTop__6Kerneli
	addl $8,%esp
L83:
	movl -4(%ebp),%ebx
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_setKernelDataStart__6Kerneli:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%edx
	movl 12(%ebp),%ecx
	movl %ecx,4(%edx)
	movl $1,%eax
	jmp L79
	.align 4,0x90
	jmp L79
	.align 4,0x90
L79:
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_setStackTop__6Kerneli:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%edx
	movl 12(%ebp),%ecx
	movl %ecx,(%edx)
	movl $1,%eax
	jmp L77
	.align 4,0x90
	jmp L77
	.align 4,0x90
L77:
	movl %ebp,%esp
	popl %ebp
	ret
.data
_OK:
	.byte 1
_Error:
	.byte -1
_ErrorPriority:
	.byte -13
_ErrorGetHighPriority:
	.byte -15
_ErrorInvalidBindFileNum:
	.byte -17
_ErrorNullBindEntry:
	.byte -19
_ErrorModuleRead:
	.byte -21
_ErrorFindModule:
	.byte -23
_ErrorNullAoutHeader:
	.byte -25
_ErrorAoutHeaderMagic:
	.byte -27
_ErrorSetPriorityRange:
	.byte -29
	.align 2
_MaxTaskNum:
	.long 256
	.align 2
_PriorityRange:
	.long 256
	.align 2
_LowestPriority:
	.long 255
	.align 2
_HighestPriority:
	.long 0
	.align 2
_ClockServerPriority:
	.long 0
	.align 2
_NameServerPriority:
	.long 1
	.align 2
_TrainSerialServerPriority:
	.long 2
	.align 2
_WyseSerialServerPriority:
	.long 3
	.align 2
_KeyboardServerPriority:
	.long 4
	.align 2
_GraphicServerPriority:
	.long 5
	.align 2
_SystemPriority:
	.long 5
	.align 2
_UserPriority:
	.long 6
	.align 2
_IdleTaskPriority:
	.long 255
_Empty:
	.byte -1
