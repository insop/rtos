#
#	makefile of TASK related stuff 
#	
#	FEMOS(Fast EMbedded Operating System)
#	for Real Time Operating System project
#
#	dbLinkQueue.cc	: double linked list class for timer syscall
#			: like Delay() DelayUntil()
#	priQueue.cc	: priority queue class with constant search
#			: time using bit operation and look up table
#	queue.cc	: simple queue class
#	taskDescTable.cc: task descriptor table, warapper class
#			: it has a Task,two PriQueue(ready queue, message queue)
#			: a DbLinkQueue(timer queue) Objects
#	
#

include ../include/util/rules.mk

CP	= cp
OBJS = dbLinkQueue.o priQueue.o queue.o task.o taskDescTable.o 

compile: $(OBJS) 

all: compile
	cp $(OBJS) ../lib
	#mv $(OBJS) ../lib

dbLinkQueue.o: dbLinkQueue.cc dbLinkQueue.h
	$(CC)	$< -c
priQueue.o: priQueue.cc priQueue.h
	$(CC)	$< -c
queue.o: queue.cc queue.h
	$(CC)	$< -c
task.o:	task.cc task.h
	$(CC)	$< -c
taskDescTable.o: taskDescTable.cc taskDescTable.h
	$(CC)	$< -c

clean:
	$(RM) dbLinkQueue priQueue queue task taskDescTable *.o core a.out kernel kernel.sys testTask testTask1 testTask2 *.sys *~

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
