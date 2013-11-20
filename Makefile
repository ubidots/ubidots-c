# Ubidots-C Makefile. Copyright 2013, Ubidots.

default:
	mkdir -p bin
	gcc -c -fPIC src/ubidots.c -o bin/ubidots.o


