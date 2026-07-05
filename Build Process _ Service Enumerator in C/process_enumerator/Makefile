CC=gcc
CFLAGS=-Wall -Wextra -std=c99
TARGETS=process_enum service_enum process_monitor

all: $(TARGETS)

process_enum: process_enum.c
    $(CC) $(CFLAGS) -o $@ $<

service_enum: service_enum.c
    $(CC) $(CFLAGS) -o $@ $<

process_monitor: process_monitor.c
    $(CC) $(CFLAGS) -o $@ $<

clean:
    rm -f $(TARGETS)

install: all
    sudo cp $(TARGETS) /usr/local/bin/

uninstall:
    sudo rm -f /usr/local/bin/process_enum
    sudo rm -f /usr/local/bin/service_enum
    sudo rm -f /usr/local/bin/process_monitor

.PHONY: all clean install uninstall
