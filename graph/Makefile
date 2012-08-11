#
#	makefile of Graph
#	
#	FEMOS(Fast EMbedded Operating System)
#	for Real Time Operating System project
#
#	graph.cc	: graph class
#	graphMain.cc	: main graph handling file
#	
#

include ../include/util/rules.mk

CP	= cp
OBJS = graph.o DFS.o BFS.o bdBuffer.o dijkstra.o stack.o maxPQueue.o graphMain 
MAIN_FILES = graphMain
#LIBS = ../lib/sysCall.o -lcs452
LIBS = graph.o DFS.o BFS.o bdBuffer.o dijkstra.o stack.o maxPQueue.o

compile: $(OBJS) 

all: compile
	cp $(MAIN_FILES) ../kernel
	#mv $(OBJS) ../lib

graph.o: graph.cc graph.h
	$(CC)	$< -c
maxPQueue.o: maxPQueue.cc maxPQueue.h
	$(CC)	$< -c
DFS.o: DFS.cc DFS.h
	$(CC)	$< -c
BFS.o: BFS.cc BFS.h
	$(CC)	$< -c
dijkstra.o: dijkstra.cc dijkstra.h
	$(CC)	$< -c
bdBuffer.o: bdBuffer.cc bdBuffer.h
	$(CC)	$< -c
stack.o: stack.cc stack.h
	$(CC)	$< -c
graphMain: graphMain.cc DFS.o DFS.cc DFS.h graph.o graph.h graph.cc bdBuffer.o bdBuffer.cc bdBuffer.h BFS.o BFS.cc BFS.h dijkstra.o dijkstra.cc dijkstra.h stack.o stack.cc stack.h maxPQueue.o maxPQueue.cc maxPQueue.h
	$(CC)	$< $(LIBS) -o graphMain 

clean:
	$(RM) graph  *.o core a.out *.sys *~

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
