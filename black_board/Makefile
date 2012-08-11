#
#
#	makefile of Buffer Server
#	
#	FEMOS(Fast EMbedded Operating System)
#	for Real Time Operating System project
#	
#	blackBoard.cc		: blackBoard class
#
#	blackBoardMain.cc	: blackBoardMain server
#
#

include ../include/util/rules.mk

CP = cp
MV = mv
OBJS = blackBoardMain blackBoard.o 
MAIN = blackBoardMain 
LIBS = ../lib/sysCall.o ../lib/communicateData.o -lcs452

compile: $(OBJS)

all : compile
	cp $(MAIN) ../kernel
	$(RM) $(OBJS)

blackBoardMain: blackBoardMain.cc blackBoardMain.h blackBoard.o blackBoard.h
	$(CC)	$< $(LIBS) blackBoard.o -o blackBoardMain

blackBoard.o: blackBoard.cc blackBoard.h
	$(CC)	$< -c  

clean:
	$(RM) blackBoardMain *.o core a.out *.sys *~

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
