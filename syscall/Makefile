#
#	makefile of syscall ( system call of RTOS ) 
#	
#	FEMOS(Fast EMbedded Operating System)
#	for Real Time Operating System project
#
#		
#	sysCall.c	: system call function
#			: get the paramenter from 
#			: user task and signal(interrupt) to kernel
#	
#

include ../include/util/rules.mk

CP = cp
MV = mv
OBJS = sysCall.o 

compile: $(OBJS) 

all : compile
	cp $(OBJS) ../lib
	#mv $(OBJS) ../lib

sysCall.o: sysCall.c sysCall.h
	$(CC)	$< -c

clean:
	$(RM) sysCall *.o core a.out *.sys *~

# List of file dependencies.
depend:
	$(CC) -M *.c *.S > makedep
	echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
	echo '$$r makedep' >>eddep
	echo 'w' >>eddep
	ed - Makefile < eddep
	rm eddep makedep
	echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
	echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
	echo '# see make depend above' >> Makefile
