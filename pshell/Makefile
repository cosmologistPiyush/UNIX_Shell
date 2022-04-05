CC = clang

CFLAGS = -Wall
CFLAGS += -Werror

SOURCES = ch-dir.c
SOURCES += path.c
SOURCES += pshell.c

OBJECTS = $(SOURCES:.c=.o)

TARGET = pshell

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	@rm $(TARGET) $(OBJECTS)

#ch-dir.o: ch-dir.c helper.h
	#clang -c ch-dir.c

#path.o: path.c helper.h
	#clang -c path.c

#pshell.o: pshell.c helper.h
	#clang -c pshell.c

#pshell: pshell.o ch-dir.o path.o
	#clang -o pshell pshell.o ch-dir.o path.o

