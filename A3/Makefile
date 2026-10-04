.PHONY: all clean

all: mystery

mystery: mystery-main.s libmystery.a
	gcc -g -z noexecstack -no-pie mystery-main.s -L. -lmystery -o mystery

clean:
	rm -f mystery
