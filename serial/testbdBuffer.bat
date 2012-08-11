rm -f a.out
rm -f testbdBuffer
gcc -c bdBuffer.cc
gcc testbdBuffer.cc bdBuffer.o -o testbdBuffer

