#
#	makefile of hal(Hardware Abstract Layer)
#	
#	FEMOS(Fast EMbedded Operating System)
#	for Real Time Operating System project
#
#		
#	
#	bindfile.cc	: bindfile
#	aout.cc		: a.out format reader
#	gdt.c		: General Descriptor Table setup
#			: GDT(kind of a segment register block)
#	clock.o		: timer(clock, 8254) chip setup
#	idt.o		: interrupt descriptor table setup
#			: idt(sort of a interrupt vector)
#	serial.o	: serial port set up for communication
#

include ../include/util/rules.mk

CP = cp
MV = mv
OBJS = bindfile.o aout.o gdt.o clock.o idt.o serial.o

compile: bindfile.o aout.o gdt.o clock.o idt.o serial.o 

all : compile
	cp $(OBJS) ../lib
	#mv $(OBJS) ../lib

bindfile.o: bindfile.cc bindfile.h
	$(CC)	$< -c
aout.o:	aout.cc aout.h
	$(CC)	$< -c
gdt.o:	gdt.c gdt.h
	$(CC)	$< -c
idt.o:	idt.c idt.h
	$(CC)	$< -c
serial.o: serial.c serial.h
	$(CC)	$< -c
clock.o: clock.c clock.h
	$(CC)	$< -c

clean:
	$(RM) gdt aout *.o core a.out bindfile kernel kernel.sys testBind testBind.sys clock serial *~

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
