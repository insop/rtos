	.file	"testTask.cc"
gcc2_compiled.:
___gnu_compiled_cplusplus:
.data
_Esc:
	.byte 27
LC0:
	.ascii "Warning!!!  No more free task avaiable pNext:%d\12\0"
LC1:
	.ascii " testing getNextFreeTask\12\0"
LC2:
	.ascii "First free task tid %d\12\0"
LC3:
	.ascii " %d: free task tid %d\12\0"
LC4:
	.ascii " priqueu testing \12\0"
LC5:
	.ascii "%d \0"
LC6:
	.ascii "\12second version of test finished \12\0"
.text
	.align 4
.globl _main
_main:
	pushl %ebp
	movl %esp,%ebp
	subl $17464,%esp
	pushl %ebx
	call ___main
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	pushl %eax
	call ___13TaskDescTable
	addl $4,%esp
	pushl $LC1
	call _printf
	addl $4,%esp
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	pushl %eax
	call _getNextFreeTask__13TaskDescTable
	addl $4,%esp
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	movl %eax,-17460(%ebp)
	movl -17460(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	pushl %eax
	call _getTid__4Task
	addl $4,%esp
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	pushl %eax
	pushl $LC2
	call _printf
	addl $8,%esp
	movl -17460(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	pushl %eax
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%edx
	addl $15360,%edx
	pushl %edx
	call _addPriQueue__8PriQueueP4Task
	addl $8,%esp
	movl $1,-4(%ebp)
L70:
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	pushl %eax
	call _getMaxNumTasks__13TaskDescTable
	addl $4,%esp
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%edx
	addl $2,%edx
	cmpl %edx,-4(%ebp)
	setl %al
	movb %al,-17464(%ebp)
	cmpb $0,-17464(%ebp)
	jne L73
	jmp L71
	.align 4,0x90
L73:
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	pushl %eax
	call _getNextFreeTask__13TaskDescTable
	addl $4,%esp
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	movl %eax,-17460(%ebp)
	cmpl $-1,-17460(%ebp)
	setne %al
	movb %al,-17464(%ebp)
	cmpb $0,-17464(%ebp)
	je L74
	movl -17460(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	pushl %eax
	call _getTid__4Task
	addl $4,%esp
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	pushl %eax
	movl -4(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	pushl %eax
	pushl $LC3
	call _printf
	addl $12,%esp
	movl -4(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl $1717986919,%ecx
	movl -17464(%ebp),%eax
	imull %ecx
	movl %edx,%ecx
	sarl $2,%ecx
	movl -17464(%ebp),%ebx
	sarl $31,%ebx
	movl %ecx,%edx
	subl %ebx,%edx
	movl %edx,%ebx
	leal 0(,%ebx,4),%ecx
	addl %edx,%ecx
	movl %ecx,%edx
	addl %ecx,%edx
	movl -17464(%ebp),%eax
	subl %edx,%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	pushl %eax
	movl -17460(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	pushl %eax
	call _setPriority__4Taski
	addl $8,%esp
	pushl $2
	movl -17460(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	pushl %eax
	call _setState__4Task9TaskState
	addl $8,%esp
	movl -17460(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	pushl %eax
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%edx
	addl $15360,%edx
	pushl %edx
	call _addPriQueue__8PriQueueP4Task
	addl $8,%esp
L74:
L72:
	incl -4(%ebp)
	jmp L70
	.align 4,0x90
L71:
	pushl $LC4
	call _printf
	addl $4,%esp
	call _getchar
	pushl $255
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%edx
	addl $15360,%edx
	pushl %edx
	call _printQueue__8PriQueuei
	addl $8,%esp
	pushl $9
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%edx
	addl $15360,%edx
	pushl %edx
	call _printQueue__8PriQueuei
	addl $8,%esp
	pushl $8
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%edx
	addl $15360,%edx
	pushl %edx
	call _printQueue__8PriQueuei
	addl $8,%esp
	pushl $7
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%edx
	addl $15360,%edx
	pushl %edx
	call _printQueue__8PriQueuei
	addl $8,%esp
	pushl $6
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%edx
	addl $15360,%edx
	pushl %edx
	call _printQueue__8PriQueuei
	addl $8,%esp
	pushl $5
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%edx
	addl $15360,%edx
	pushl %edx
	call _printQueue__8PriQueuei
	addl $8,%esp
	pushl $4
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%edx
	addl $15360,%edx
	pushl %edx
	call _printQueue__8PriQueuei
	addl $8,%esp
	pushl $3
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%edx
	addl $15360,%edx
	pushl %edx
	call _printQueue__8PriQueuei
	addl $8,%esp
	pushl $2
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%edx
	addl $15360,%edx
	pushl %edx
	call _printQueue__8PriQueuei
	addl $8,%esp
	pushl $1
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%edx
	addl $15360,%edx
	pushl %edx
	call _printQueue__8PriQueuei
	addl $8,%esp
	pushl $0
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%edx
	addl $15360,%edx
	pushl %edx
	call _printQueue__8PriQueuei
	addl $8,%esp
	call _getchar
	movl $0,-4(%ebp)
L75:
	movl -4(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	pushl %eax
	pushl $LC5
	call _printf
	addl $8,%esp
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%edx
	addl $15360,%edx
	pushl %edx
	call _getNextTask__8PriQueue
	addl $4,%esp
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	movl %eax,-17460(%ebp)
	incl -4(%ebp)
L77:
	cmpl $-1,-17460(%ebp)
	setne %al
	movb %al,-17464(%ebp)
	cmpb $0,-17464(%ebp)
	jne L78
	jmp L76
	.align 4,0x90
L78:
	jmp L75
	.align 4,0x90
L76:
	pushl $LC6
	call _printf
	addl $4,%esp
	call _getchar
	pushl $2
	leal -17456(%ebp),%eax
	movl %eax,-17464(%ebp)
	movl -17464(%ebp),%eax
	pushl %eax
	call __$_13TaskDescTable
	addl $8,%esp
	xorl %eax,%eax
	jmp L69
	.align 4,0x90
	jmp L69
	.align 4,0x90
L69:
	movl -17468(%ebp),%ebx
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_getMaxNumTasks__13TaskDescTable:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%edx
	movl $256,%eax
	jmp L68
	.align 4,0x90
	jmp L68
	.align 4,0x90
L68:
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_getNextFreeTask__13TaskDescTable:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	pushl %ebx
	movl 8(%ebp),%ebx
	pushl %ebx
	call _getMaxNumTasks__13TaskDescTable
	addl $4,%esp
	movl %eax,%eax
	cmpl %eax,__13TaskDescTable$nextTask
	setge %al
	testb %al,%al
	je L64
	movl __13TaskDescTable$freeTaskList,%eax
	movl 16(%eax),%edx
	pushl %edx
	pushl $LC0
	call _printf
	addl $8,%esp
	call _getchar
	movl $-1,%eax
	jmp L63
	.align 4,0x90
	jmp L63
	.align 4,0x90
	jmp L65
	.align 4,0x90
L64:
	movl __13TaskDescTable$freeTaskList,%eax
	cmpl $-1,16(%eax)
	sete %al
	testb %al,%al
	je L66
	movl __13TaskDescTable$freeTaskList,%eax
	movl %eax,-4(%ebp)
	jmp L67
	.align 4,0x90
L66:
	movl __13TaskDescTable$freeTaskList,%eax
	movl %eax,-4(%ebp)
	movl __13TaskDescTable$freeTaskList,%eax
	movl 16(%eax),%edx
	movl %edx,__13TaskDescTable$freeTaskList
L67:
	incl __13TaskDescTable$nextTask
	movl -4(%ebp),%eax
	jmp L63
	.align 4,0x90
	jmp L63
	.align 4,0x90
L65:
L63:
	movl -8(%ebp),%ebx
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
__$_13TaskDescTable:
	pushl %ebp
	movl %esp,%ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 8(%ebp),%esi
	movl 12(%ebp),%ebx
L47:
	testl %esi,%esi
	je L48
	leal 15360(%esi),%edi
L50:
	cmpl %edi,%esi
	jne L52
	jmp L51
	.align 4,0x90
L52:
	addl $-60,%edi
	pushl $2
	pushl %edi
	call __$_4Task
	addl $8,%esp
	jmp L50
	.align 4,0x90
L51:
	jmp L49
	.align 4,0x90
L48:
L49:
	movl %ebx,%eax
	andl $1,%eax
	testl %eax,%eax
	je L53
	pushl %esi
	call ___builtin_delete
	addl $4,%esp
	jmp L54
	.align 4,0x90
L53:
L54:
L46:
	leal -12(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_setPriority__4Taski:
	pushl %ebp
	movl %esp,%ebp
	pushl %ebx
	movl 8(%ebp),%ecx
	movl 12(%ebp),%edx
	xorb %al,%al
	cmpl $254,8(%ecx)
	setg %bl
	testb %bl,%bl
	jne L32
	cmpl $0,8(%ecx)
	setle %bl
	testb %bl,%bl
	jne L32
	jmp L31
	.align 4,0x90
L32:
	movb $1,%al
L31:
	testb %al,%al
	je L30
	movl %edx,8(%ecx)
	movl $1,%eax
	jmp L29
	.align 4,0x90
	jmp L29
	.align 4,0x90
	jmp L33
	.align 4,0x90
L30:
	movl $-29,%eax
	jmp L29
	.align 4,0x90
	jmp L29
	.align 4,0x90
L33:
L29:
	movl -4(%ebp),%ebx
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_setState__4Task9TaskState:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%eax
	movl 12(%ebp),%edx
	movl %edx,4(%eax)
L27:
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_getTid__4Task:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%edx
	movl (%edx),%eax
	jmp L24
	.align 4,0x90
	jmp L24
	.align 4,0x90
L24:
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
