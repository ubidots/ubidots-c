# Ubidots-C Makefile. Copyright 2013, Ubidots.

LDFLAGS=-L/usr/lib/i386-linux-gnu 
LIBS=-lcurl -ljansson


default:
	mkdir -p bin
	gcc -c -fPIC src/ubidots.c $(LDFLAGS) $(LIBS) -o bin/ubidots.o
	@echo Build completed.
test:
	mkdir -p bin
	gcc -g src/ubidots.c $(LDFLAGS) $(LIBS) -o bin/ubidots
	@echo Build completed.
clean:
	rm -rf bin
	@echo Clean completed.