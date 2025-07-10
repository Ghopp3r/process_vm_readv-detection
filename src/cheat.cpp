#define _GNU_SOURCE
#include <sys/uio.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cinttypes>

int main(int argc, char **argv) {
	if (argc != 4) {
		fprintf(stderr, "usage: %s <pid> <hex_addr> <len>\n", argv[0]);
		return 1;
	}
	pid_t pid = static_cast<pid_t>(atoi(argv[1]));
	uintptr_t addr = strtoull(argv[2], nullptr, 16);
	size_t len = static_cast<size_t>(atoi(argv[3]));
	if (len > 256) len = 256;

	char buf[256] = {};
	struct iovec local = { buf, len };
	struct iovec remote = { reinterpret_cast<void *>(addr), len };

	ssize_t n = process_vm_readv(pid, &local, 1, &remote, 1, 0);
	if (n == -1) {
		perror("process_vm_readv");
		return 1;
	}

	printf("read %zd bytes: ", n);
	for (ssize_t i = 0; i < n; ++i)
		printf("%02X ", static_cast<unsigned char>(buf[i]));
	puts("");
	return 0;
}