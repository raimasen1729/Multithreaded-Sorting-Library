CC=gcc
CFLAGS=-Wall -pthread

build: cpubench

test: cpubench
	bash runbench.sh

cpubench: cpubench.c
	$(CC) $(CFLAGS) -o cpubench $<

clean:
	rm -rf cpubench
