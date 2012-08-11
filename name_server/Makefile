#
#
#	makefile of Name Server
#	
#	FEMOS(Fast EMbedded Operating System)
#	for Real Time Operating System project
#	
#	NameServer.c	:	Name server
#
#

include ../include/util/rules.mk

CP = cp
MV = mv
OBJS = nameServer 
LIBS = ../lib/sysCall.o -lcs452

compile: $(OBJS)

all : compile
	cp $(OBJS) ../kernel
	$(RM) $(OBJS)

nameServer: nameServer.c 
	$(CC)	$< $(LIBS) -o nameServer

clean:
	$(RM) nameServer *.o core a.out *.sys *~

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
