#include "detector.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdio>

namespace trap {

static constexpr size_t kPageSize = 4096;

void *allocate() {
	void *page = mmap(nullptr, kPageSize, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	return (page == MAP_FAILED) ? nullptr : page;
}

bool touched(void *addr, uint64_t *entry_out) {
	if (!addr) return false;

	int fd = open("/proc/self/pagemap", O_RDONLY | O_CLOEXEC);
	if (fd == -1) {
		perror("open(pagemap)");
		return false;
	}

	uint64_t entry = 0;
	off_t off = (reinterpret_cast<uintptr_t>(addr) / kPageSize) * sizeof(entry);
	ssize_t rd = pread64(fd, &entry, sizeof(entry), off);
	close(fd);

	if (rd != sizeof(entry)) {
		fprintf(stderr, "pread64 failed (%zd)\n", rd);
		return false;
	}
	if (entry_out) *entry_out = entry;
	return entry & (1ULL << 63); /* bit 63 — present */
}

}   // namespace trap