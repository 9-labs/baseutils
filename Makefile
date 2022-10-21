BINARIES = bin/cat bin/pwd

all: $(BINARIES)

cat: bin/cat
bin/cat: cat/cat.c
	cd cat; $(MAKE)

pwd: bin/pwd
bin/pwd: pwd/pwd.c
	cd pwd; $(MAKE)

clean:
	rm -f $(BINARIES)
