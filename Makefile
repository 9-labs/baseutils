BINARIES=cat chmod head mkdir pwd rmdir

all: $(BINARIES)

chmod: util.o chmod.c
	cc -c chmod.c -o chmod.o
	cc chmod.o util.o -o chmod
	rm -f chmod.o
mkdir: util.o mkdir.c
	cc -c mkdir.c -o mkdir.o
	cc mkdir.o util.o -o mkdir
	rm -f mkdir.o

clean:
	rm -f $(BINARIES) util.o
