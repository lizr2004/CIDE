##### Makefile #####
# Make file for bait on Linux or compatible OS
# Released to the public domain 2000 by Neil Hodgson neilh@scintilla.org
# This makefile tested with GCC 3.2 and GNOME 2.0

.SUFFIXES: .c .o .h .a

INCLUDEDIRS=-I../scintilla/include
CXXFLAGS= -DGTK -DSCI_LEXER -W -Wall -lm
LEXEROBJS=$(wildcard ../scintilla/gtk/Lex*.o)

all: bait

.c.o:
	gcc `pkg-config --cflags gtk+-2.0` $(INCLUDEDIRS) $(CXXFLAGS) -c $< -o $@ -g 
bait: bait.o $(LEXEROBJS) ../scintilla/bin/scintilla.a
	gcc -lm -DGTK $^ -o $@ -lstdc++ `pkg-config --libs gtk+-2.0 gthread-2.0` -lgmodule-2.0 -g
clean:
	rm -rf bait *.o
