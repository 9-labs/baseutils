BINARIES=cat chmod head mkdir pwd rmdir
OBJECTS=mode.o

all: $(OBJECTS) $(BINARIES)

chmod: mode.o chmod.c
	cc -c chmod.c -o chmod.o
	cc chmod.o mode.o -o chmod

mkdir: mode.o mkdir.c
	cc -c mkdir.c -o mkdir.o
	cc mkdir.o mode.o -o mkdir

clean:
	rm -f $(OBJECTS) $(BINARIES)
