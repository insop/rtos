#
#	makefile of BSP (Board Specific Part) 
#	
#	FEMOS(Fast EMbedded Operating System)
#	for Real Time Operating System project
#
#	bsp.cc	: bsp functions
#

include ../include/util/rules.mk

CP	= cp
OBJS = bsp.o 

compile: $(OBJS)

all: compile
	cp $(OBJS) ../lib
	#mv $(OBJS) ../lib

bsp.o: bsp.cc bsp.h
	$(CC)	$< -c

clean:
	$(RM) bsp *.o core a.out kernel kernel.sys *.sys *~

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
