CC=gcc
CFLAGS=-Wall -Wextra -std=c99
TARGETS=file_walker enhanced_walker fs_stats

all: $(TARGETS)

file_walker: file_walker.c
    $(CC) $(CFLAGS) -o $@ $<

enhanced_walker: enhanced_walker.c
    $(CC) $(CFLAGS) -o $@ $<

fs_stats: fs_stats.c
    $(CC) $(CFLAGS) -o $@ $<

clean:
    rm -f $(TARGETS)

test: all
    @echo "Testing basic walker..."
    ./file_walker test_dir 2
    @echo "Testing enhanced walker..."
    ./enhanced_walker test_dir -d 2 -p "*.txt"
    @echo "Testing statistics tool..."
    ./fs_stats test_dir 2

.PHONY: all clean test
