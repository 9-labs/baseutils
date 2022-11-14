BINARIES=basename cat chmod dirname echo head mkdir pwd rmdir
CFLAGS=-std=c99
all: $(BINARIES)

chmod: util.o chmod.c
	$(CC) $(CFLAGS) -c chmod.c -o chmod.o
	$(CC) $(CFLAGS) chmod.o util.o -o chmod
	rm -f chmod.o
mkdir: util.o mkdir.c
	$(CC) $(CFLAGS) -c mkdir.c -o mkdir.o
	$(CC) $(CFLAGS)  mkdir.o util.o -o mkdir
	rm -f mkdir.o

clean:
	rm -f $(BINARIES) util.o
