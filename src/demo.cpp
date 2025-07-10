#include "detector.h"
#include <cstdio>
#include <unistd.h>

struct Vector3 {
	float x, y, z;
};

struct Player {
	int health;
	Vector3 pos;
};

struct Engine {
	Player **playerList;
} gEngine;

int main() {
	void *trapPage = trap::allocate();
	if (!trapPage) {
		fprintf(stderr, "Failed to allocate trap page\n");
		return 1;
	}

	/* Place a trap instead of the real list of players */
	gEngine.playerList = reinterpret_cast<Player **>(trapPage);

	printf("pid = %d\n", getpid());
	printf("trap addr = %p\n", trapPage);
    printf("page backed? %d\n", trap::touched(trapPage));

	puts("press <Enter> after running cheat...");
	getchar();

	printf("page backed? %d\n", trap::touched(trapPage));
	return 0;
}
