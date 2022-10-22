BINARIES = bin/cat bin/pwd bin/head

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

clean:
	rm -f $(BINARIES)
