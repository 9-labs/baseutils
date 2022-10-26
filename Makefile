BINARIES = bin/cat bin/pwd bin/head bin/mkdir bin/chmod bin/rmdir

all: $(BINARIES)

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
bin/mkdir: mkdir/mkdir.c
	cd mkdir; $(MAKE)

chmod: bin/chmod
bin/chmod: chmod/chmod.c
	cd chmod; $(MAKE)

rmdir: bin/rmdir
bin/rmdir: rmdir/rmdir.c
	cd rmdir; $(MAKE)

clean:
	rm -f $(BINARIES)
