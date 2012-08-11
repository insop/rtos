rm -f a.out
rm -f bindfile.o
gcc  -c bindfile.cc
gcc  -c aout.cc
gcc -c gdt.c
gcc  testBind.cc gdt.o aout.o bindfile.o ../include/util/error.o -o testBind -lcs452
bind486 -l testBind.sys
