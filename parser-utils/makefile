OUTPATH=/home/linux/programs/parser-utils/out
CFLAGS=-Wall -fpic
CC=cc
IFLAGS=-I/home/linux/programs/parser-utils/inc
SRCPATH=/home/linux/programs/parser-utils/src
LDFLAGS=-L/home/linux/programs/parser-utils/out

all: main

main: libparser.so
	$(CC) $(IFLAGS) $(LDFLAGS) -o $(OUTPATH)/main main.c -lparser
libparser.so: parser.o
	$(CC) -shared -o $(OUTPATH)/libparser.so $(OUTPATH)/parser.o
parser.o:
	$(CC) -c $(CFLAGS) $(IFLAGS) $(SRCPATH)/parser.c
	mv *.o $(OUTPATH)
clean:
	rm $(OUTPATH)/main $(OUTPATH)/parser.o $(OUTPATH)/libparser.so 
