BINARIES=bin/basename bin/cat bin/chmod bin/dirname bin/echo bin/head \
	 bin/mkdir bin/pwd bin/rmdir
CFLAGS=-std=c99
all: bin $(BINARIES)
bin:
	mkdir bin
clean:
	@rm -f bin/*
	@rmdir bin
	@rm -f *.o

bin/basename: basename.c
	$(CC) $(CFLAGS) basename.c -o bin/basename

bin/cat: cat.c
	$(CC) $(CFLAGS) cat.c -o bin/cat

bin/chmod: bin/util.o bin/chmod.o
	$(CC) $(CFLAGS) bin/chmod.o bin/util.o -o bin/chmod
bin/chmod.o: chmod.c
	$(CC) $(CFLAGS) -c chmod.c -o bin/chmod.o

bin/dirname: dirname.c
	$(CC) $(CFLAGS) dirname.c -o bin/dirname

bin/echo: echo.c
	$(CC) $(CFLAGS) echo.c -o bin/echo

bin/head: head.c
	$(CC) $(CFLAGS) head.c -o bin/head

bin/mkdir: bin/util.o bin/mkdir.o
	$(CC) $(CFLAGS) bin/mkdir.o bin/util.o -o bin/mkdir
bin/mkdir.o: mkdir.c
	$(CC) $(CFLAGS) -c mkdir.c -o bin/mkdir.o

bin/pwd: pwd.c
	$(CC) $(CFLAGS) pwd.c -o bin/pwd

bin/rmdir: rmdir.c
	$(CC) $(CFLAGS) rmdir.c -o bin/rmdir


bin/util.o: util.c
	$(CC) $(CFLAGS) -c util.c -o bin/util.o

