BINARIES = bin/cat bin/pwd bin/head bin/mkdir

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

clean:
	rm -f $(BINARIES)
