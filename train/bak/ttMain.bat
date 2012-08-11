gcc -c trainTask.cc

gcc trainTaskMain.cc trainTask.o ../syscall/sysCall.o -o trainTaskMain -lcs452

cp trainTaskMain ../kernel/.
