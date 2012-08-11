#
#
#	makefile of Train tasks()
#	
#	FEMOS(Fast EMbedded Operating System)
#	for Real Time Operating System project
#
#	trainTask.cc	: train task class
#	trainTaskMain.cc: train task main funciton
#	ironBird.h	: train application definition header
#	
#

include ../include/util/rules.mk

CP = cp
MV = mv
OBJS = trainTask.o trainTaskMain 
MAIN_FILES=trainTaskMain
LIBS = trainTask.o ../lib/sysCall.o ../lib/communicateData.o -lcs452

compile: $(OBJS)

all : compile
	cp $(MAIN_FILES) ../kernel
	$(RM) $(OBJS)

trainTask.o: trainTask.cc trainTask.h ironBird.h
	$(CC)	$< -c
trainTaskMain: trainTaskMain.cc trainTask.o trainTask.h ironBird.h	
	$(CC)	$< $(LIBS) -o trainTaskMain

clean:
	$(RM) trainTaskMain trainTask *.o core a.out *.sys *~

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
