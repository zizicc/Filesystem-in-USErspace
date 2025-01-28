# Makefile for FUSE projects in C
all: hello hellotime

clean:
	rm -f hello hellotime

# Compile hello.c into an executable
hello: hello.c
	gcc -Wall -D_FILE_OFFSET_BITS=64 $^ `pkg-config fuse --cflags --libs` -o $@

# Compile hellotime.c into an executable
hellotime: hellotime.c
	gcc -Wall -D_FILE_OFFSET_BITS=64 $^ `pkg-config fuse --cflags --libs` -o $@