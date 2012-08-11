
gcc dijkstra.c ../syscall/sysCall.o graph.o -o pathFind -lcs452

cp pathFind ../kernel/.
