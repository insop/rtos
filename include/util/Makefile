#
#	makefile of UTILITIES (mostly error funcion for debugging) 
#	
#	FEMOS(Fast EMbedded Operating System)
#	for Real Time Operating System project
#
#	bsp.cc	: bsp functions
#

include rules.mk

CP	= cp
OBJS = error.o 
LIB_DIR = ../../lib

compile: $(OBJS)

all: compile 
	#cp $(OBJS) $(LIB_DIR) 
	#mv $(OBJS) ../../lib

error.o: error.cc error.h
	$(CC)	$< -c

clean:
	$(RM) error *.o core a.out kernel *.sys *~

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
