	.file	"kernelMain.cc"
gcc2_compiled.:
___gnu_compiled_cplusplus:
.data
_Esc:
	.byte 27
LC0:
	.ascii "getNextTask Error : no taks\12\0"
LC1:
	.ascii "getNextTask tid[%d] pri %d\12\0"
LC2:
	.ascii "Warning!!!  No more free task avaiable pNext:%d\12\0"
.globl _aTDTable
	.align 2
_aTDTable:
	.space 17452
.globl _aKernel
	.align 2
_aKernel:
	.space 32
LC3:
	.ascii "SysCall Dispatch\12\0"
.text
	.align 4
.globl _sysCallDispatch__Fv
_sysCallDispatch__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl $0,-4(%ebp)
	pushl $LC3
	call _cprintf
	addl $4,%esp
	call _getchar
	movl -4(%ebp),%eax
	jmp L163
	.align 4,0x90
	jmp L163
	.align 4,0x90
L163:
	movl %ebp,%esp
	popl %ebp
	ret
.data
LC4:
	.ascii "clockEvent \12\0"
.text
	.align 4
.globl _clockEvent__Fv
_clockEvent__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl $1,-4(%ebp)
	pushl $LC4
	call _cprintf
	addl $4,%esp
	call _getchar
	movl -4(%ebp),%eax
	jmp L164
	.align 4,0x90
	jmp L164
	.align 4,0x90
L164:
	movl %ebp,%esp
	popl %ebp
	ret
.data
LC5:
	.ascii "serial Event \12\0"
.text
	.align 4
.globl _serialEvent__Fv
_serialEvent__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl $2,-4(%ebp)
	pushl $LC5
	call _cprintf
	addl $4,%esp
	call _getchar
	movl -4(%ebp),%eax
	jmp L165
	.align 4,0x90
	jmp L165
	.align 4,0x90
L165:
	movl %ebp,%esp
	popl %ebp
	ret
.globl _switchFunctions
.data
	.align 2
_switchFunctions:
	.long _sysCallDispatch__Fv
	.long _clockEvent__Fv
	.long _serialEvent__Fv
.text
	.align 4
.globl _initialize__Fv
_initialize__Fv:
	pushl %ebp
	movl %esp,%ebp
	call _initGDT
L166:
	movl %ebp,%esp
	popl %ebp
	ret
.data
LC6:
	.ascii "\12Kernel's code selector 0x%x\12\0"
LC7:
	.ascii "Kernel's data selector 0x%x\12\0"
LC8:
	.ascii "Kernel\0"
LC9:
	.ascii "return val %d\12\0"
LC10:
	.ascii " testing getNextFreeTask\12\0"
LC11:
	.ascii "First free task tid %d\12\0"
LC12:
	.ascii " %d: free task tid %d\12\0"
LC13:
	.ascii " pri queu testing to Prio %d\12\0"
LC14:
	.ascii "%d \0"
LC15:
	.ascii "\12second version of test finished \12\0"
LC16:
	.ascii "EMPTY\12\0"
LC17:
	.ascii "not EMPTY\12\0"
LC18:
	.ascii "\12 test finished \12\0"
.text
	.align 4
.globl _main
_main:
	pushl %ebp
	movl %esp,%ebp
	subl $60,%esp
	pushl %ebx
	call ___main
	movl 12(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	movl 8(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	leal -8(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	call ___8BindfileUii
	addl $12,%esp
	leal -40(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	call ___4Aout
	addl $4,%esp
	call _initialize__Fv
	pushl $1
	call _initDebug__Fi
	addl $4,%esp
	pushl $0
	call _getCodeSelector
	addl $4,%esp
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	pushl $LC6
	call _cprintf
	addl $8,%esp
	pushl $0
	call _getDataSelector
	addl $4,%esp
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	pushl $LC7
	call _cprintf
	addl $8,%esp
	pushl $LC8
	leal -8(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	call _findModule__8BindfilePc
	addl $8,%esp
	leal -8(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	call _printBindfile__8Bindfile
	addl $4,%esp
	leal -8(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	leal -40(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	call _printAoutHeader__4AoutP8Bindfile
	addl $8,%esp
	call _getchar
	pushl $0
	leal -8(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	call _getStackSize__8Bindfilei
	addl $8,%esp
	movl %eax,-60(%ebp)
	movl _baseDS,%edx
	addl _dataTop,%edx
	movl -60(%ebp),%eax
	addl %edx,%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	pushl $_aKernel
	call _setStackTop__6Kerneli
	addl $8,%esp
	movl _baseDS,%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	pushl $_aKernel
	call _setKernelDataStart__6Kerneli
	addl $8,%esp
	movl -48(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	movl _switchFunctions(,%eax,4),%ebx
	call *%ebx
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	movl %eax,-52(%ebp)
	movl -52(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	pushl $LC9
	call _printf
	addl $8,%esp
	pushl $LC10
	call _printf
	addl $4,%esp
	pushl $_aTDTable
	call _getNextFreeTask__13TaskDescTable
	addl $4,%esp
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	movl %eax,-56(%ebp)
	movl -56(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	call _getTid__4Task
	addl $4,%esp
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	pushl $LC11
	call _printf
	addl $8,%esp
	movl -56(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	pushl $_aTDTable+15360
	call _addPriQueue__8PriQueueP4Task
	addl $8,%esp
	movl $1,-44(%ebp)
L168:
	pushl $_aTDTable
	call _getMaxNumTasks__13TaskDescTable
	addl $4,%esp
	movl %eax,-60(%ebp)
	movl -60(%ebp),%edx
	addl $2,%edx
	cmpl %edx,-44(%ebp)
	setl %al
	movb %al,-60(%ebp)
	cmpb $0,-60(%ebp)
	jne L171
	jmp L169
	.align 4,0x90
L171:
	pushl $_aTDTable
	call _getNextFreeTask__13TaskDescTable
	addl $4,%esp
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	movl %eax,-56(%ebp)
	cmpl $-1,-56(%ebp)
	setne %al
	movb %al,-60(%ebp)
	cmpb $0,-60(%ebp)
	je L172
	movl -56(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	call _getTid__4Task
	addl $4,%esp
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	movl -44(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	pushl $LC12
	call _printf
	addl $12,%esp
	movl -44(%ebp),%eax
	movl %eax,-60(%ebp)
	movl $1374389535,%ecx
	movl -60(%ebp),%eax
	imull %ecx
	movl %edx,%ecx
	sarl $6,%ecx
	movl -60(%ebp),%ebx
	sarl $31,%ebx
	movl %ecx,%edx
	subl %ebx,%edx
	movl %edx,%ebx
	movl %ebx,%ecx
	addl %ebx,%ecx
	addl %edx,%ecx
	leal 0(,%ecx,8),%ebx
	addl %edx,%ebx
	leal 0(,%ebx,8),%edx
	movl -60(%ebp),%eax
	subl %edx,%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	movl -56(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	call _setPriority__4Taski
	addl $8,%esp
	pushl $2
	movl -56(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	call _setState__4Task9TaskState
	addl $8,%esp
	movl -56(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	pushl $_aTDTable+15360
	call _addPriQueue__8PriQueueP4Task
	addl $8,%esp
L172:
L170:
	incl -44(%ebp)
	jmp L168
	.align 4,0x90
L169:
	pushl $_aTDTable
	call _getMaxNumTasks__13TaskDescTable
	addl $4,%esp
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	pushl $LC13
	call _printf
	addl $8,%esp
	movl $0,-44(%ebp)
L173:
	pushl $_aTDTable
	call _getMaxNumTasks__13TaskDescTable
	addl $4,%esp
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	cmpl %eax,-44(%ebp)
	setl %al
	movb %al,-60(%ebp)
	cmpb $0,-60(%ebp)
	jne L176
	jmp L174
	.align 4,0x90
L176:
	movl -44(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	pushl $_aTDTable+15360
	call _printQueue__8PriQueuei
	addl $8,%esp
L175:
	incl -44(%ebp)
	jmp L173
	.align 4,0x90
L174:
	pushl $15
	pushl $_aTDTable+15360
	call _printQueue__8PriQueuei
	addl $8,%esp
	call _getchar
	movl $0,-44(%ebp)
L177:
	movl -44(%ebp),%eax
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	pushl %eax
	pushl $LC14
	call _printf
	addl $8,%esp
	pushl $_aTDTable+15360
	call _getNextTask__8PriQueue
	addl $4,%esp
	movl %eax,-60(%ebp)
	movl -60(%ebp),%eax
	movl %eax,-56(%ebp)
	incl -44(%ebp)
L179:
	cmpl $-1,-56(%ebp)
	setne %al
	movb %al,-60(%ebp)
	cmpb $0,-60(%ebp)
	jne L180
	jmp L178
	.align 4,0x90
L180:
	jmp L177
	.align 4,0x90
L178:
	pushl $LC15
	call _printf
	addl $4,%esp
	call _getchar
	pushl $_aTDTable+15360
	call _isEmpty__8PriQueue
	addl $4,%esp
	movl %eax,-60(%ebp)
	cmpl $0,-60(%ebp)
	setne %al
	movb %al,-60(%ebp)
	cmpb $0,-60(%ebp)
	je L181
	pushl $LC16
	call _printf
	addl $4,%esp
	jmp L182
	.align 4,0x90
L181:
	pushl $LC17
	call _printf
	addl $4,%esp
L182:
	pushl $LC18
	call _printf
	addl $4,%esp
	call _getchar
	xorl %eax,%eax
	jmp L167
	.align 4,0x90
	jmp L167
	.align 4,0x90
L167:
	movl -64(%ebp),%ebx
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
.globl __GLOBAL_$D$aTDTable
__GLOBAL_$D$aTDTable:
	pushl %ebp
	movl %esp,%ebp
	pushl $2
	pushl $_aTDTable
	call __$_13TaskDescTable
	addl $8,%esp
L183:
	movl %ebp,%esp
	popl %ebp
	ret
.stabs "___DTOR_LIST__",22,0,0,__GLOBAL_$D$aTDTable
	.align 4
.globl __GLOBAL_$I$aTDTable
__GLOBAL_$I$aTDTable:
	pushl %ebp
	movl %esp,%ebp
	pushl $_aTDTable
	call ___13TaskDescTable
	addl $4,%esp
	pushl $_aKernel
	call ___6Kernel
	addl $4,%esp
L184:
	movl %ebp,%esp
	popl %ebp
	ret
.stabs "___CTOR_LIST__",22,0,0,__GLOBAL_$I$aTDTable
	.align 4
_setKernelDataStart__6Kerneli:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%edx
	movl 12(%ebp),%ecx
	movl %ecx,4(%edx)
	movl $1,%eax
	jmp L113
	.align 4,0x90
	jmp L113
	.align 4,0x90
L113:
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
	jmp L111
	.align 4,0x90
	jmp L111
	.align 4,0x90
L111:
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_getMaxNumTasks__13TaskDescTable:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%edx
	movl $256,%eax
	jmp L106
	.align 4,0x90
	jmp L106
	.align 4,0x90
L106:
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
	je L102
	movl __13TaskDescTable$freeTaskList,%eax
	movl 16(%eax),%edx
	pushl %edx
	pushl $LC2
	call _printf
	addl $8,%esp
	call _getchar
	movl $-1,%eax
	jmp L101
	.align 4,0x90
	jmp L101
	.align 4,0x90
	jmp L103
	.align 4,0x90
L102:
	movl __13TaskDescTable$freeTaskList,%eax
	cmpl $-1,16(%eax)
	sete %al
	testb %al,%al
	je L104
	movl __13TaskDescTable$freeTaskList,%eax
	movl %eax,-4(%ebp)
	jmp L105
	.align 4,0x90
L104:
	movl __13TaskDescTable$freeTaskList,%eax
	movl %eax,-4(%ebp)
	movl __13TaskDescTable$freeTaskList,%eax
	movl 16(%eax),%edx
	movl %edx,__13TaskDescTable$freeTaskList
L105:
	incl __13TaskDescTable$nextTask
	movl -4(%ebp),%eax
	jmp L101
	.align 4,0x90
	jmp L101
	.align 4,0x90
L103:
L101:
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
L85:
	testl %esi,%esi
	je L86
	leal 15360(%esi),%edi
L88:
	cmpl %edi,%esi
	jne L90
	jmp L89
	.align 4,0x90
L90:
	addl $-60,%edi
	pushl $2
	pushl %edi
	call __$_4Task
	addl $8,%esp
	jmp L88
	.align 4,0x90
L89:
	jmp L87
	.align 4,0x90
L86:
L87:
	movl %ebx,%eax
	andl $1,%eax
	testl %eax,%eax
	je L91
	pushl %esi
	call ___builtin_delete
	addl $4,%esp
	jmp L92
	.align 4,0x90
L91:
L92:
L84:
	leal -12(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_addPriQueue__8PriQueueP4Task:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 8(%ebp),%ebx
	movl 12(%ebp),%esi
	pushl %esi
	call _getPriority__4Task
	addl $4,%esp
	movl %eax,%edi
	pushl %esi
	call _getTid__4Task
	addl $4,%esp
	movl %eax,-4(%ebp)
	pushl %edi
	pushl %esi
	pushl %ebx
	call _addQueue__8PriQueueP4Taski
	addl $12,%esp
	movb %al,%al
	movsbl %al,%edx
	movl %edx,%eax
	jmp L80
	.align 4,0x90
	jmp L80
	.align 4,0x90
L80:
	leal -16(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_getNextTask__8PriQueue:
	pushl %ebp
	movl %esp,%ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 8(%ebp),%ebx
	cmpl $0,40(%ebx)
	setg %al
	testb %al,%al
	je L77
	pushl %ebx
	call _getHighestPriority__8PriQueue
	addl $4,%esp
	movl %eax,%esi
	movl %esi,%eax
	shrl $31,%eax
	testb %al,%al
	je L78
	pushl $LC0
	call _printf
	addl $4,%esp
	movl $-1,%eax
	jmp L76
	.align 4,0x90
	jmp L76
	.align 4,0x90
L78:
	pushl %esi
	pushl %ebx
	call _removeQueue__8PriQueuei
	addl $8,%esp
	movl %eax,%edi
	pushl %edi
	call _getPriority__4Task
	addl $4,%esp
	movl %eax,%eax
	pushl %eax
	pushl %edi
	call _getTid__4Task
	addl $4,%esp
	movl %eax,%eax
	pushl %eax
	pushl $LC1
	call _printf
	addl $12,%esp
	movl %edi,%eax
	jmp L76
	.align 4,0x90
	jmp L76
	.align 4,0x90
	jmp L79
	.align 4,0x90
L77:
	movl $-1,%eax
	jmp L76
	.align 4,0x90
	jmp L76
	.align 4,0x90
L79:
L76:
	leal -12(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_removeQueue__8PriQueuei:
	pushl %ebp
	movl %esp,%ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 8(%ebp),%esi
	movl 12(%ebp),%ebx
	movl %ebx,%eax
	leal 0(,%eax,8),%edx
	movl 44(%edx,%esi),%edi
	cmpl $-1,16(%edi)
	setne %al
	testb %al,%al
	je L82
	movl %ebx,%eax
	leal 0(,%eax,8),%edx
	movl 16(%edi),%eax
	movl %eax,44(%edx,%esi)
	jmp L83
	.align 4,0x90
L82:
	pushl %ebx
	pushl %esi
	call _clearPriorityMask__8PriQueuei
	addl $8,%esp
	movl %ebx,%eax
	leal 0(,%eax,8),%edx
	movl $-1,44(%edx,%esi)
	movl %ebx,%eax
	leal 0(,%eax,8),%edx
	movl $-1,48(%edx,%esi)
L83:
	decl 40(%esi)
	movl %edi,%eax
	jmp L81
	.align 4,0x90
	jmp L81
	.align 4,0x90
L81:
	leal -12(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_getHighestPriority__8PriQueue:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 8(%ebp),%ebx
	movzbl (%ebx),%eax
	movzbl _priorityMap(%eax),%esi
	movzbl 1(%esi,%ebx),%eax
	movzbl _priorityMap(%eax),%edi
	movl %esi,%eax
	leal 0(,%eax,8),%edx
	leal (%ebx,%edx),%eax
	leal (%edi,%eax),%edx
	leal 5(%edx),%eax
	movl %eax,%edx
	movzbl (%edx),%eax
	movzbl _priorityMap(%eax),%ecx
	movl %ecx,-4(%ebp)
	movl %esi,%ecx
	sall $6,%ecx
	movl %ecx,-8(%ebp)
	leal 0(,%edi,8),%eax
	addl %eax,-8(%ebp)
	movl -4(%ebp),%ecx
	addl %ecx,-8(%ebp)
	movl -8(%ebp),%ecx
	pushl %ecx
	call _priChange__Fi
	addl $4,%esp
	movl %eax,%edx
	movl %edx,%eax
	jmp L75
	.align 4,0x90
	jmp L75
	.align 4,0x90
L75:
	leal -20(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_clearPriorityMask__8PriQueuei:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 8(%ebp),%esi
	movl 12(%ebp),%ebx
	pushl %ebx
	call _priChange__Fi
	addl $4,%esp
	movl %eax,-12(%ebp)
	movl -12(%ebp),%edi
	sarl $6,%edi
	andl $7,%edi
	movl -12(%ebp),%ecx
	sarl $3,%ecx
	movl %ecx,-4(%ebp)
	andl $7,-4(%ebp)
	movl -12(%ebp),%ebx
	andl $7,%ebx
	movl %ebx,-8(%ebp)
	movl %edi,%eax
	leal 0(,%eax,8),%edx
	leal (%esi,%edx),%eax
	movl -4(%ebp),%edx
	addl %eax,%edx
	leal 5(%edx),%ecx
	movl %ecx,-24(%ebp)
	movl -24(%ebp),%eax
	movl %edi,%edx
	leal 0(,%edx,8),%ebx
	movl %ebx,-24(%ebp)
	movl -24(%ebp),%edx
	addl %esi,%edx
	movl -4(%ebp),%ecx
	addl %edx,%ecx
	movl %ecx,-24(%ebp)
	movl -24(%ebp),%ebx
	addl $5,%ebx
	movl %ebx,-16(%ebp)
	movl -16(%ebp),%edx
	movl $1,%ebx
	movl -8(%ebp),%ecx
	sall %cl,%ebx
	movl %ebx,-24(%ebp)
	movb -24(%ebp),%cl
	notb %cl
	movb %cl,-20(%ebp)
	movb -20(%ebp),%bl
	andb (%edx),%bl
	movb %bl,(%eax)
	movl %edi,%eax
	leal 0(,%eax,8),%edx
	leal (%esi,%edx),%eax
	movl -4(%ebp),%edx
	addl %eax,%edx
	leal 5(%edx),%eax
	movl %eax,%edx
	cmpb $0,(%edx)
	sete %al
	testb %al,%al
	je L73
	movl $1,%eax
	movl -4(%ebp),%ecx
	sall %cl,%eax
	movb %al,%dl
	notb %dl
	andb %dl,1(%edi,%esi)
	cmpb $0,1(%edi,%esi)
	sete %al
	testb %al,%al
	je L74
	movl $1,%eax
	movl %edi,%ecx
	sall %cl,%eax
	movb %al,%dl
	notb %dl
	andb %dl,(%esi)
L74:
L73:
L72:
	leal -36(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_priChange__Fi:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%edx
	movl $255,%ecx
	subl %edx,%ecx
	movl %ecx,%eax
	jmp L64
	.align 4,0x90
	jmp L64
	.align 4,0x90
L64:
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_addQueue__8PriQueueP4Taski:
	pushl %ebp
	movl %esp,%ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 8(%ebp),%edi
	movl 12(%ebp),%ebx
	movl 16(%ebp),%esi
	movl %esi,%eax
	leal 0(,%eax,8),%edx
	cmpl $-1,44(%edx,%edi)
	sete %al
	testb %al,%al
	je L69
	pushl %esi
	pushl %edi
	call _setPriorityMask__8PriQueuei
	addl $8,%esp
	movl %esi,%eax
	leal 0(,%eax,8),%edx
	movl %ebx,44(%edx,%edi)
	movl %esi,%eax
	leal 0(,%eax,8),%edx
	movl %ebx,48(%edx,%edi)
	movl $-1,16(%ebx)
	jmp L70
	.align 4,0x90
L69:
	movl %esi,%eax
	leal 0(,%eax,8),%edx
	movl 48(%edx,%edi),%eax
	movl %ebx,16(%eax)
	movl %esi,%eax
	leal 0(,%eax,8),%edx
	movl %ebx,48(%edx,%edi)
	movl $-1,16(%ebx)
L70:
	incl 40(%edi)
	movl $1,%eax
	jmp L68
	.align 4,0x90
	jmp L68
	.align 4,0x90
L68:
	leal -12(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_setPriorityMask__8PriQueuei:
	pushl %ebp
	movl %esp,%ebp
	subl $16,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 8(%ebp),%esi
	movl 12(%ebp),%ebx
	pushl %ebx
	call _priChange__Fi
	addl $4,%esp
	movl %eax,-12(%ebp)
	movl -12(%ebp),%edi
	sarl $6,%edi
	andl $7,%edi
	movl -12(%ebp),%ecx
	sarl $3,%ecx
	movl %ecx,-4(%ebp)
	andl $7,-4(%ebp)
	movl -12(%ebp),%ebx
	andl $7,%ebx
	movl %ebx,-8(%ebp)
	movl $1,%eax
	movl %edi,%ecx
	sall %cl,%eax
	orb %al,(%esi)
	movl $1,%eax
	movl -4(%ebp),%ecx
	sall %cl,%eax
	orb %al,1(%edi,%esi)
	movl %edi,%eax
	leal 0(,%eax,8),%edx
	leal (%esi,%edx),%eax
	movl -4(%ebp),%edx
	addl %eax,%edx
	leal 5(%edx),%ebx
	movl %ebx,-16(%ebp)
	movl -16(%ebp),%eax
	movl %edi,%edx
	leal 0(,%edx,8),%ecx
	movl %ecx,-16(%ebp)
	movl -16(%ebp),%edx
	addl %esi,%edx
	movl -4(%ebp),%ebx
	addl %edx,%ebx
	movl %ebx,-16(%ebp)
	movl -16(%ebp),%edx
	addl $5,%edx
	movl %edx,-16(%ebp)
	movl $1,%edx
	movl -8(%ebp),%ecx
	sall %cl,%edx
	movl -16(%ebp),%ebx
	movb (%ebx),%bl
	orb %dl,%bl
	movb %bl,(%eax)
L71:
	leal -28(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_isEmpty__8PriQueue:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%edx
	cmpl $0,40(%edx)
	sete %al
	testb %al,%al
	je L66
	movl $1,%eax
	jmp L67
	.align 4,0x90
L66:
	xorl %eax,%eax
L67:
	movl %eax,%eax
	jmp L65
	.align 4,0x90
	jmp L65
	.align 4,0x90
L65:
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_getPriority__4Task:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%edx
	movl 8(%edx),%eax
	jmp L55
	.align 4,0x90
	jmp L55
	.align 4,0x90
L55:
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
	jne L53
	cmpl $0,8(%ecx)
	setle %bl
	testb %bl,%bl
	jne L53
	jmp L52
	.align 4,0x90
L53:
	movb $1,%al
L52:
	testb %al,%al
	je L51
	movl %edx,8(%ecx)
	movl $1,%eax
	jmp L50
	.align 4,0x90
	jmp L50
	.align 4,0x90
	jmp L54
	.align 4,0x90
L51:
	movl $-29,%eax
	jmp L50
	.align 4,0x90
	jmp L50
	.align 4,0x90
L54:
L50:
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
L48:
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
_getTid__4Task:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%edx
	movl (%edx),%eax
	jmp L45
	.align 4,0x90
	jmp L45
	.align 4,0x90
L45:
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
_priorityMap:
	.byte 0
	.byte 0
	.byte 1
	.byte 1
	.byte 2
	.byte 2
	.byte 2
	.byte 2
	.byte 3
	.byte 3
	.byte 3
	.byte 3
	.byte 3
	.byte 3
	.byte 3
	.byte 3
	.byte 4
	.byte 4
	.byte 4
	.byte 4
	.byte 4
	.byte 4
	.byte 4
	.byte 4
	.byte 4
	.byte 4
	.byte 4
	.byte 4
	.byte 4
	.byte 4
	.byte 4
	.byte 4
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
	.byte 7
