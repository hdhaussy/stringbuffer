CFLAGS += -g

all: test benchmark

clean:
	rm -f *.o test benchmark

test: stringbuffer.o

run_test: test
	./test

benchmark: stringbuffer.o

run_benchmark: benchmark
	./benchmark

