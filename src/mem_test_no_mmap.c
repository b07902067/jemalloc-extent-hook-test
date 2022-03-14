#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jemalloc/jemalloc.h>
// #include "jemalloc_util.h"
#include <sys/mman.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	size_t ret, sz;
	size_t iter = atoi(argv[1]);
	for(size_t i = 0 ; i < 8 * iter ; i ++) {
		void *p = mallocx(4096 * 32, MALLOCX_TCACHE_NONE);
	}
	system("cat /proc/$PPID/status");
    return 0;
}