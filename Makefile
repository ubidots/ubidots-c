# Ubidots-C Makefile. Copyright 2013, Ubidots.

CFLAGS=$(curl-config --cflags)
LDFLAGS=$(curl-config --libs)

default:
	mkdir -p bin
	gcc -c -fPIC $(CFLAGS) $(LDFLAGS) src/ubidots.c -o bin/ubidots.o


