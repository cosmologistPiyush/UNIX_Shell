# set compiler
CC = gcc
CFLAGS = -Wall -Werror

ch-dir.o: ch-dir.c helper.h
	$(CC) -c ch-dir.c

path.o: path.c helper.h
	$(CC) -c path.c

masterip.o: masterip.c helper.h
	$(CC) -c masterip.c

pshell.o: pshell.c helper.h
	$(CC) -c pshell.c

pshell: pshell.o ch-dir.o path.o masterip.o
	$(CC) $(CFALGS) -o pshell pshell.o ch-dir.o path.o masterip.o

pshellg: pshell.o ch-dir.o path.o masterip.o
	$(CC) $(CFLAGS) -g -o pshell pshell.o ch-dir.o path.o masterip.o
