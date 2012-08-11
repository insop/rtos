#
#	makefile of serial communication 
#	
#	FEMOS(Fast EMbedded Operating System)
#	for Real Time Operating System project
#
#		
#	
#	bdBuffer.cc	: bounded buffer (char type)
#			: for serial communication
#	serial.cc	: serial class
#			: it wraps bounded buffer and other stuffs
#
#

include ../include/util/rules.mk

CP = cp
MV = mv
OBJS = bdBuffer.o serial.o 

compile: $(OBJS) 

all : compile
	cp bdBuffer.o ../lib
	#mv $(OBJS) ../lib

bdBuffer.o: bdBuffer.cc bdBuffer.h
	$(CC)	$< -c
serial.o: serial.cc serial.h
	$(CC)	$< -c

clean:
	$(RM) bdbuffer serial *.o core a.out testbdBuffer kernel.sys *.sys *~

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
