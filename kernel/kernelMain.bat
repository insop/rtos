rm -f kernelMain

gcc init.c ../syscall/sysCall.o -o init -lcs452
gcc idle.c ../syscall/sysCall.o -o idle -lcs452
gcc stat.c ../syscall/sysCall.o -o stat -lcs452
gcc matrix.c ../syscall/sysCall.o -o matrix -lcs452

gcc ../sensor/sensor.c ../syscall/sysCall.o -o sensor -lcs452
gcc ../sensor/sensorSrv.c ../syscall/sysCall.o -o sensorSrv -lcs452
gcc ../sensor/sensorSched.c ../syscall/sysCall.o -o sensorSched -lcs452
gcc ../train/command.c ../syscall/sysCall.o -o command -lcs452
gcc ../train/input.c ../syscall/sysCall.o -o input -lcs452
gcc ../train/trainTaskMain.cc  ../train/trainTask.o ../syscall/sysCall.o -o trainTaskMain -lcs452

gcc -c kernel.cc
gcc kernelMain.cc  ../task/queue.o ../hal/serial.o ../serial/bdBuffer.o ../serial/serial.o kernel.o ../task/taskDescTable.o ../task/priQueue.o ../task/dbLinkQueue.o ../task/task.o ../hal/bindfile.o ../hal/aout.o ../hal/idt.o ../hal/gdt.o ../hal/clock.o ../bsp/bsp.o ../include/util/error.o -lcs452 -o kernelMain

bind486 -l kernelMain.sys
