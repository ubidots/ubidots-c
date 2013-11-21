# Ubidots-C Makefile. Copyright 2013, Ubidots.

LDFLAGS=-L/usr/lib/i386-linux-gnu 
LIBS=-lcurl -ljson


default:
	mkdir -p bin
	gcc -c -fPIC src/ubidots.c -o bin/ubidots.o

test:
	mkdir -p bin
	gcc src/ubidots.c $(LDFLAGS) $(LIBS) -o bin/ubidots

