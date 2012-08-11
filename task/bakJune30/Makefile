include rules.mk

# This MakeFile was hacked up by Aaron Foat

CFLAGS = -O6
INITFILES = init.o sysCall.o
CLKNOTIFIER = clockNotifier.o sysCall.o
NAMESERVER = nameserver.o sysCall.o
TESTFILES = test.o sysCall.o
IDLEFILES = idle.o sysCall.o
MAINFILES = main.o sysCall.o
ENDERFILES = ender.o sysCall.o
EATERFILES = eater.o sysCall.o
SERIALFILES = serial.o sysCall.o
SERIALREADGUARDFILES = serialReadGuard.o sysCall.o
SERIALWRITEGUARDFILES = serialWriteGuard.o sysCall.o
SERIALREADNOTIFIERFILES = serialReadNotifier.o sysCall.o
SERIALWRITENOTIFIERFILES = serialWriteNotifier.o sysCall.o
KERNELFILES = TaskDescriptorTable.o GDTfunc.o
KERNELCOMPILE = kernel.c TaskDescriptorTable.o GDTfunc.o queue.o


all: kernel.sys 

kernel.sys: init kernel Bindfile nameserver idle eater ender main serial \
clockNotifier serialReadGuard serialReadNotifier serialWriteNotifier serialWriteGuard
	bind486 -l kernel.sys 

sysCall.o: sysCall.c sysCall.h 
	$(CC)  $< -c 

idle: $(IDLEFILES)
	$(CC) $(IDLEFILES) -o idle -lcs452

# test: $(TESTFILES)
#	$(CC) $(TESTFILES) -o test -lcs452

main: $(MAINFILES)
	$(CC) $(MAINFILES) -o main -lcs452

ender: $(ENDERFILES)
	$(CC) $(ENDERFILES) -o ender -lcs452

eater: $(EATERFILES)
	$(CC) $(EATERFILES) -o eater -lcs452

serialReadGuard: $(SERIALREADGUARDFILES)
	$(CC) $(SERIALREADGUARDFILES) -o serialReadGuard -lcs452

serialWriteGuard: $(SERIALWRITEGUARDFILES)
	$(CC) $(SERIALWRITEGUARDFILES) -o serialWriteGuard -lcs452

serialReadNotifier: $(SERIALREADNOTIFIERFILES)
	$(CC) $(SERIALREADNOTIFIERFILES) -o serialReadNotifier -lcs452

serialWriteNotifier: $(SERIALWRITENOTIFIERFILES)
	$(CC) $(SERIALWRITENOTIFIERFILES) -o serialWriteNotifier -lcs452

serial: $(SERIALFILES)
	$(CC) $(SERIALFILES) -o serial -lcs452

main.o:
	$(CC) -c ../test/main.c -o main.o

ender.o:
	$(CC) -c ../test/ender.c -o ender.o

eater.o:
	$(CC) -c ../test/eater.c -o eater.o

init.o:
	$(CC) -c ../sysfiles/init.c -o init.o

serialReadGuard.o:
	$(CC) -c ../sysfiles/serialReadGuard.c -o serialReadGuard.o

serialWriteGuard.o:
	$(CC) -c ../sysfiles/serialWriteGuard.c -o serialWriteGuard.o

serialReadNotifier.o:
	$(CC) -c ../sysfiles/serialReadNotifier.c -o serialReadNotifier.o

serialWriteNotifier.o:
	$(CC) -c ../sysfiles/serialWriteNotifier.c -o serialWriteNotifier.o

serial.o:
	$(CC) -c ../sysfiles/serial.c -o serial.o

nameserver.o:
	$(CC) -c ../sysfiles/nameserver.c -o nameserver.o
	
clockNotifier.o:
	$(CC) -c clockNotifier.c
	
init: $(INITFILES)
	$(CC) $(INITFILES) -o init -lcs452

clockNotifier: $(CLKNOTIFIER)
	$(CC) $(CLKNOTIFIER) -o clockNotifier -lcs452

nameserver: $(NAMESERVER)
	$(CC) $(NAMESERVER) -o nameserver -lcs452

idle.o:
	$(CC) -c idle.c

kernel: $(KERNELCOMPILE) 
	$(CC) $(KERNELCOMPILE) -o kernel -lcs452

GDTfunc.o: GDTfunc.c GDTfunc.h
	$(CC) $< -c

TaskDescriptorTable.o: TaskDescriptorTable.c TaskDescriptorTable.h TaskDescriptor.h
	$(CC) $< -c

# test: $(TESTFILES)
#	$(CC) $(TESTFILES) -o test -lcs452

#readbind.sys: readbind.bind readbind
#	bind486 -f readbind.bind readbind.sys

clean:
	$(RM) init kernel.sys *.o core a.out main eater ender test idle \
nameserver kernel clockNotifier serialguard serialnotifier serial
#	$(RM) readbind readbind.sys

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

#----------------------------------------------------------------- 
# DO NOT DELETE THIS LINE -- make depend uses it
# DEPENDENCIES MUST END AT END OF FILE
init.o: ../sysfiles/init.c /u/cs452/i486/i486-unknown-cs452/include/cs452.h \
  /u/cs452/i486/i486-unknown-cs452/include/sys/types.h \
  /u/cs452/i486/i486-unknown-cs452/include/machine/endian.h \
  /u/cs452/i486/i486-unknown-cs452/include/sys/cdefs.h \
  /u/cs452/i486/i486-unknown-cs452/include/machine/ansi.h \
  /u/cs452/i486/i486-unknown-cs452/include/machine/types.h sysCall.h
test.o: test.c /u/cs452/i486/i486-unknown-cs452/include/cs452.h \
  /u/cs452/i486/i486-unknown-cs452/include/sys/types.h \
  /u/cs452/i486/i486-unknown-cs452/include/machine/endian.h \
  /u/cs452/i486/i486-unknown-cs452/include/sys/cdefs.h \
  /u/cs452/i486/i486-unknown-cs452/include/machine/ansi.h \
  /u/cs452/i486/i486-unknown-cs452/include/machine/types.h sysCall.h
eater.o: ../test/eater.c /u/cs452/i486/i486-unknown-cs452/include/cs452.h \
  /u/cs452/i486/i486-unknown-cs452/include/sys/types.h \
  /u/cs452/i486/i486-unknown-cs452/include/machine/endian.h \
  /u/cs452/i486/i486-unknown-cs452/include/sys/cdefs.h \
  /u/cs452/i486/i486-unknown-cs452/include/machine/ansi.h \
  /u/cs452/i486/i486-unknown-cs452/include/machine/types.h sysCall.h
ender.o: ../test/ender.c /u/cs452/i486/i486-unknown-cs452/include/cs452.h \
  /u/cs452/i486/i486-unknown-cs452/include/sys/types.h \
  /u/cs452/i486/i486-unknown-cs452/include/machine/endian.h \
  /u/cs452/i486/i486-unknown-cs452/include/sys/cdefs.h \
  /u/cs452/i486/i486-unknown-cs452/include/machine/ansi.h \
  /u/cs452/i486/i486-unknown-cs452/include/machine/types.h sysCall.h
main.o: ../test/main.c /u/cs452/i486/i486-unknown-cs452/include/cs452.h \
  /u/cs452/i486/i486-unknown-cs452/include/sys/types.h \
  /u/cs452/i486/i486-unknown-cs452/include/machine/endian.h \
  /u/cs452/i486/i486-unknown-cs452/include/sys/cdefs.h \
  /u/cs452/i486/i486-unknown-cs452/include/machine/ansi.h \
  /u/cs452/i486/i486-unknown-cs452/include/machine/types.h sysCall.h
sysCall.o: sysCall.c
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above

