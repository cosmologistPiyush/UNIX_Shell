ch-dir.o: ch-dir.c helper.h
	clang -c ch-dir.c

path.o: path.c helper.h
	clang -c path.c

masterip.o: masterip.c helper.h
	clang -c masterip.c

pshell.o: pshell.c helper.h
	clang -c pshell.c

pshell: pshell.o ch-dir.o path.o masterip.o
	clang -o pshell pshell.o ch-dir.o path.o masterip.o

