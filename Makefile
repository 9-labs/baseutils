BINARIES = bin/cat bin/pwd bin/head bin/mkdir bin/chmod bin/rmdir

OBJECTS = bin/mode.o

all: $(OBJECTS) $(BINARIES)

bin/mode.o: common/mode.c
	cc -c -o bin/mode.o common/mode.c

cat: bin/cat
bin/cat: cat/cat.c
	cd cat; $(MAKE)

pwd: bin/pwd
bin/pwd: pwd/pwd.c
	cd pwd; $(MAKE)

head: bin/head
bin/head: head/head.c
	cd head; $(MAKE)

mkdir: bin/mkdir
bin/mkdir: bin/mode.o mkdir/mkdir.c
	cd mkdir; $(MAKE)

chmod: bin/chmod
bin/chmod: bin/mode.o chmod/chmod.c
	cd chmod; $(MAKE)

rmdir: bin/rmdir
bin/rmdir: rmdir/rmdir.c
	cd rmdir; $(MAKE)

clean:
	rm -f $(BINARIES) $(OBJECTS)
