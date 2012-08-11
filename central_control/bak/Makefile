#
#
#	makefile of Central Control 
#	
#	FEMOS(Fast EMbedded Operating System)
#	for Real Time Operating System project
#
#	centralControl.cc	: central control class
#	centralControlMain.cc	: central control main function
#	
#

include ../include/util/rules.mk

CP = cp
MV = mv
OBJS = centralControl.o centralControlMain 
MAIN_FILES = centralControlMain
LIBS = centralControl.o ../lib/sysCall.o -lcs452

compile: $(OBJS)

all : compile
	cp $(MAIN_FILES) ../kernel
	$(RM) $(OBJS)

centralControl.o: centralControl.cc centralControl.h ../train/ironBird.h
	$(CC)	$< -c 
centralControlMain: centralControlMain.cc centralControl.o ../train/ironBird.h	
	$(CC)	$< $(LIBS) -o centralControlMain

clean:
	$(RM) centralControlMain centralContrl *.o core a.out *.sys *~

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
