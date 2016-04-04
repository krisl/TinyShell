VPATH = ./
CFLAGS = -std=c99
includefiles := $(wildcard ./core/*.h) $(wildcard ./util/*.h)
sourcefiles := $(wildcard ./core/*.c) $(wildcard ./util/*.c)
objectfiles := $(patsubst %.c,%.o,$(wildcard ./core/*.c))

all: $(objectfiles)

$(objectfiles):%.o:%.c $(includefiles)
	$(CC) -c $(CFLAGS) $< -o $@
	ar rcs ./libs/NaturalTinyShell.a $@
	rm $@

