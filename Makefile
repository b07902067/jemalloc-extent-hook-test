all:
	gcc src/mem_test.c -o mem_test -ljemalloc
	gcc src/mem_test_no_mmap.c -o mem_test_no_mmap -ljemalloc
clean:
	rm *out *png mem_test mem_test_no_mmap