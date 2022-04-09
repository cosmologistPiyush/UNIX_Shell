CC = gcc

CFLAGS = -Wall
CFLAGS += -Werror
CFLAGS += -g

SOURCES = ch-dir.c
SOURCES += path.c
SOURCES += pshell.c
SOURCES += masterip.c
SOURCES += helper.c

OBJECTS = $(SOURCES:.c=.o)

TARGET = pshell

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	@rm $(TARGET) $(OBJECTS)
