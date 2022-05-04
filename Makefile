SRC     := $(wildcard src/*.c)
PKGS    := gtk+-3.0
CC      := clang
CFLAGS  := -Iheaders
CFLAGS  += -Wall -O2 -std=c17 -rdynamic $(shell pkg-config --cflags $(PKGS))
LDFLAGS := -lGL $(shell pkg-config --libs $(PKGS))

.PHONY: dev clean compile cnr
	
all: cnr

dev:
	echo $(CFLAGS) | tr " " "\n" > compile_flags.txt
	echo $(LDFLAGS) | tr " " "\n" >> compile_flags.txt

clean:
	$(RM) *.o a.out

compile: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o a.out

cnr: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o a.out
	./a.out
	$(RM) *.o a.out
