#
#
#	makefile of one step command generation 
#	
#	FEMOS(Fast EMbedded Operating System)
#	for Real Time Operating System project
#
#	main.c
#	stepgraph.c
#	onestepmain.c
#	stepgraph.h
#	
#

include ../include/util/rules.mk

CP = cp
MV = mv
OBJS = stepgraph.o onestep
MAIN_FILES = onestep
LIBS = ../lib/sysCall.o -lcs452

compile: $(OBJS)

all : compile
	cp $(MAIN_FILES) ../kernel
	#mv $(OBJS) ../lib

stepgraph.o: stepgraph.c 
	$(CC) -c stepgraph.c -o stepgraph.o

onestep: onestepmain.c stepgraph.o ../train/ironBird.h
	$(CC) onestepmain.c stepgraph.o $(LIBS) -o onestep

clean:
	$(RM) onestep *.o core a.out *.sys *~

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
