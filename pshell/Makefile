ch-dir.o: ch-dir.c helper.h
	clang -c ch-dir.c

path.o: path.c helper.h
	clang -c path.c

pshell.o: pshell.c helper.h
	clang -c pshell.c

pshell: pshell.o ch-dir.o path.o
	clang -o pshell pshell.o ch-dir.o path.o

