#
#
#	makefile of Buffer Server
#	
#	FEMOS(Fast EMbedded Operating System)
#	for Real Time Operating System project
#	
#	from the view pointer of higher server to the client
#
#	distributorServer.cc	: distributor buffer server
#	collectorServer.cc	: collecting buffer server
#
#	followinga are used for distributor and collector server
#	bdBufferInt.cc		: bounded buffer with int
#	bdBufferCommData.h	: bounded buffer with CommData Struct
#	
#	communicateData.cc	: CommData struct wrapper CLASS
#	
#
#	BufferServer.c	:	BufferServer 
#
#

include ../include/util/rules.mk

CP = cp
MV = mv
OBJS = bufferServer distributorServer bdBufferInt.o bdBufferCommData.o communicateData.o
MAIN = bufferServer distributorServer
LIBS = ../lib/sysCall.o ../lib/communicateData.o -lcs452
COMM_DATA = communicateData.o 

compile: $(OBJS)

all : compile
	cp $(MAIN) ../kernel
	cp $(COMM_DATA) ../lib
	$(RM) $(OBJS)

distributorServer: distributorServer.cc distributorServer.h bdBufferInt.o bdBufferCommData.o
	$(CC)	$< $(LIBS) bdBufferInt.o bdBufferCommData.o -o distributorServer

#collectorServer: collectorServer.cc collectorServer.h bdBufferInt.o bdBufferCommData.o
	#$(CC)	$< $(LIBS) bdBufferInt.o bdBufferCommData.o -o collectorServer

bufferServer: bufferServer.cc bufferServer.h bdBufferInt.o bdBufferCommData.o
	$(CC)	$< $(LIBS) bdBufferInt.o bdBufferCommData.o -o bufferServer

bdBufferCommData.o: bdBufferCommData.cc bdBufferCommData.h
	$(CC)	$< -c  

bdBufferInt.o: bdBufferInt.cc bdBufferInt.h
	$(CC)	$< -c 

communicateData.o: communicateData.cc communicateData.h
	$(CC)	$< -c 
	cp $(COMM_DATA) ../lib
clean:
	$(RM) bufferServer *.o core a.out *.sys *~

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
