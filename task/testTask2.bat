rm -f a.out
rm -f task.o
gcc -c task.cc
gcc -c taskDescTable.cc
gcc -c priQueue.cc
gcc -c dbLinkQueue.cc
gcc testTask2.cc priQueue.o dbLinkQueue.o task.o taskDescTable.o -o testTask2

