CC=gcc
CFLAGS=-Wall -Wextra -std=c99
SRCDIR=src
OBJDIR=output

# Basic parser
basic: $(SRCDIR)/pe_parser.c $(SRCDIR)/main.c
    $(CC) $(CFLAGS) -o $(OBJDIR)/pe_parser $(SRCDIR)/pe_parser.c $(SRCDIR)/main.c

# Enhanced parser with imports
enhanced: $(SRCDIR)/pe_parser.c $(SRCDIR)/import_resolver.c $(SRCDIR)/main_enhanced.c
    $(CC) $(CFLAGS) -o $(OBJDIR)/pe_parser_enhanced $(SRCDIR)/pe_parser.c $(SRCDIR)/import_resolver.c $(SRCDIR)/main_enhanced.c

all: basic enhanced

clean:
    rm -f $(OBJDIR)/*

.PHONY: all clean basic enhanced
