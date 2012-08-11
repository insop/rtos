#
#
#	makefile of Sensor tasks()
#	
#	FEMOS(Fast EMbedded Operating System)
#	for Real Time Operating System project
#
#	commandSrv.c	: main function of data send and receive block	
#			: sending sensor command
#			: getting sensor data
#			: sending speed, switch command
#	sensorSched.c	: sensor command sending scheduller
#	snesorSrv.c	: sensor data dispatch server
#			: provide sensor data to the other tasks
#			: read sensor data form sensor.c using 
#			: shared memory
#	
#

include ../include/util/rules.mk

CP = cp
MV = mv
OBJS = commandSrv sensorSched sensorSrv sensorReporter
LIBS = ../lib/sysCall.o -lcs452

compile: $(OBJS)

all : compile
	cp $(OBJS) ../kernel
	$(RM) $(OBJS)

commandSrv: commandSrv.c ../train/ironBird.h
	$(CC)	$< $(LIBS) -o commandSrv
sensorSrv: sensorSrv.c ../train/ironBird.h	
	$(CC)	$< $(LIBS) -o sensorSrv
sensorSched: sensorSched.c ../train/ironBird.h	
	$(CC)	$< $(LIBS) -o sensorSched
sensorReporter: sensorReporter.c ../train/ironBird.h	
	$(CC)	$< $(LIBS) -o sensorReporter

clean:
	$(RM) sensorSched sensorSrv commandSrv *.o core a.out *.sys *~

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
