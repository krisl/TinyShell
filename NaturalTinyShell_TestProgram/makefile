VPATH = ../
CFLAGS = -std=c99 -I ../core -I ../util
includefiles := $(wildcard ../core/*.h) $(wildcard ../util/*.h) cmd.h
sourcefiles = main.c cmd.c
libfiles = ../libs/NaturalTinyShell.a 

main.exe: $(includefiles) $(sourcefiles) $(libfiles)
	$(CC) $(CFLAGS) $(sourcefiles) $(libfiles) -o $@

