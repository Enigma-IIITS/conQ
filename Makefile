queue: queue.c
	gcc -c $^

test: queue.o test.c
	gcc -o test $^

.PHONY: clean

clean:
	rm -rf *.o queue test

fmt:
	clang-format -i *.c *.h