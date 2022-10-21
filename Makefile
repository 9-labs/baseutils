BINARIES = bin/cat

cat: bin/cat
bin/cat: cat/cat.c
	cd cat; $(MAKE)

clean:
	rm -f $(BINARIES)
