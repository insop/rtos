gcc -c centralControl.cc

gcc centralControlMain.cc centralControl.o ../syscall/sysCall.o -o centralCtrlMain -lcs452

cp centralCtrlMain ../kernel/.
