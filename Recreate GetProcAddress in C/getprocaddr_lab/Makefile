CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -ldl -lm

TARGET = test_getprocaddr
SOURCES = custom_getprocaddr.c test_program.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
    $(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f $(OBJECTS) $(TARGET)

test: $(TARGET)
    ./$(TARGET)

.PHONY: all clean test
