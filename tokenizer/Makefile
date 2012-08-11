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
OBJS = Stringlib.o tokenizer 
MAIN_FILES = tokenizer 
LIBS = Stringlib.o ../lib/sysCall.o -lcs452

compile: $(OBJS)

all : compile
	cp $(MAIN_FILES) ../kernel
	#mv $(OBJS) ../lib

Stringlib.o: Stringlib.c 
	$(CC)	$< -c 

tokenizer: tokenmain.c Stringlib.o ../train/ironBird.h	
	$(CC)	$< $(LIBS) -o tokenizer

clean:
	$(RM) tokenizer *.o core a.out *.sys *~

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
