gcc main.c  ../syscall/sysCall.o -lcs452 -o main
gcc ender.c  ../syscall/sysCall.o -lcs452 -o ender
gcc eater.c  ../syscall/sysCall.o -lcs452 -o eater

cp main ../kernel/
cp ender ../kernel/
cp eater ../kernel/
