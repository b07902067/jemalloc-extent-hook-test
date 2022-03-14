#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jemalloc/jemalloc.h>
// #include "jemalloc_util.h"
#include <sys/mman.h>
#include <unistd.h>

void *pre_alloc_backup;
void *pre_alloc;
void *pre_alloc_end;


void print_heap_state(){
	size_t sz;
	uint64_t epoch = 1;
	sz = sizeof(epoch);
	mallctl("epoch", &epoch, &sz, &epoch, sz);

	size_t allocated, active, metadata, resident, mapped;
	sz = sizeof(size_t);
	if (mallctl("stats.allocated", &allocated, &sz, NULL, 0) == 0
		&& mallctl("stats.active", &active, &sz, NULL, 0) == 0
		&& mallctl("stats.metadata", &metadata, &sz, NULL, 0) == 0
		&& mallctl("stats.resident", &resident, &sz, NULL, 0) == 0
		&& mallctl("stats.mapped", &mapped, &sz, NULL, 0) == 0) {
			fprintf(stdout,
				"[Heap State] Current allocated pages/active pages/metadata/resident/mapped: %zu/%zu/%zu/%zu/%zu\n", 
				allocated/4096, active/4096, metadata/4096, resident/4096, mapped/4096);
	}
}

static void *
my_hooks_alloc(extent_hooks_t *extent_hooks, void *new_addr, size_t size,
		size_t alignment, bool *zero, bool *commit, unsigned arena_ind)
{
	printf("\n[New Hook Info] In wrapper alloc_hook: new_addr:%p "
		"size:%lu(%lu pages) alignment:%lu "
		"zero:%s commit:%s arena_ind:%u\n",
		new_addr, size, size / 4096, alignment,
		(*zero) ? "true" : "false",
		(*commit) ? "true" : "false",
		arena_ind);
	void *ret = pre_alloc;
	if (ret >= pre_alloc_end) {
		printf("[hook] Not enough memory\n\n");
		return NULL;
	}
	pre_alloc = (char *)pre_alloc + size;

	if (*zero)
		memset(ret, size, 0);
 
	printf("[hook] hook returning range: [%p,%p)\n", ret, (char *)ret + size);
	return ret;
}

extent_hooks_t hooks = {
	my_hooks_alloc,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

int main(int argc, char *argv[])
{
	/* argv[1] : mmap size (MB)
	   argv[2] : malloc request size (MB) */
	size_t ret, sz;
	unsigned arena_ind = -1;
	extent_hooks_t *new_hooks;
	
	size_t hooks_len, memsize = 256 * 4096 * atoi(argv[1]);
	print_heap_state();
	pre_alloc = mmap(NULL, memsize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (!pre_alloc) {
		perror("Could not pre-allocate memory");
		exit(1);
	}
	pre_alloc_backup = pre_alloc;
	pre_alloc_end = pre_alloc + memsize;

	new_hooks = &hooks;

	sz = sizeof(arena_ind);
	ret = mallctl("arenas.create", (void *)&arena_ind, &sz, (void *)&new_hooks, sizeof(extent_hooks_t *));
	if (ret) {
		// dbg_print(ret, "mallctl error creating arena with new hooks");
        printf("mallctl error creating arena with new hook\n");
		exit(1);
	}

	size_t iter = atoi(argv[2]);
	for(size_t i = 0 ; i < 8 * iter ; i ++) {
		void *p = mallocx(4096 * 32, MALLOCX_ARENA(arena_ind) | MALLOCX_TCACHE_NONE);
	}
	system("cat /proc/$PPID/status");
    return 0;
}