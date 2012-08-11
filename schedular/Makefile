#
#
#	makefile of Path Find 
#	
#	FEMOS(Fast EMbedded Operating System)
#	for Real Time Operating System project
#
#	dijkstra.c
#	graph.c
#	
#

include ../include/util/rules.mk

CP = cp
MV = mv
OBJS = graph.o schedular 
MAIN_FILES = schedular 
LIBS = graph.o ../lib/sysCall.o -lcs452

compile: $(OBJS)

all : compile
	cp $(MAIN_FILES) ../kernel
	#mv $(OBJS) ../lib

graph.o: ../pathfind/graph.c ../pathfind/graph.h ../pathfind/graphtype.h ../train/ironBird.h
	$(CC)	$< -c 
schedular: schedular.c graph.o ../pathfind/graph.h ../pathfind/graphtype.h ../train/ironBird.h	
	$(CC)	$< $(LIBS) -o schedular

clean:
	$(RM) schedular *.o core a.out *.sys *~

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
