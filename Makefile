ALLSRC  := $(wildcard src/*.c)
SRC     := $(filter-out src/test.c, $(ALLSRC))
TESTSRC := $(filter-out src/main.c, $(ALLSRC))
PKGS    := gtk+-3.0
CC      := clang
CFLAGS  := -Iheaders
CFLAGS  += -O2 -std=gnu17 -rdynamic -rtlib=libgcc $(shell pkg-config --cflags $(PKGS))
LDFLAGS := -lm -lGL $(shell pkg-config --libs $(PKGS))

.PHONY: dev clean compile cnr
	
all: cnr

dev:
	echo $(CFLAGS) | tr " " "\n" > compile_flags.txt
	echo $(LDFLAGS) | tr " " "\n" >> compile_flags.txt

clean:
	$(RM) *.o a.out

compile: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o a.out

test-compile: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o a.out

cnr: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o a.out
	./a.out
	$(RM) *.o a.out

tnr: $(TESTSRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o a.out
	./a.out
	$(RM) *.o a.out
