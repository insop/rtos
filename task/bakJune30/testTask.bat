rm -f a.out
rm -f task.o
gcc -c task.cc
gcc -c taskDescTable.cc
gcc -c priQueue.cc
gcc testTask.cc priQueue.o task.o taskDescTable.o -o testTask

