#include <sys/types.h>
#include <unistd.h>
#include <time.h>

unsigned seed;

void
initrandom()
{
	seed = time(NULL);
}

unsigned int
getrandom()
{
	unsigned p1 = 1103515245;
	unsigned p2 = 12345;
	seed = (seed * p1 + p2) % 2147483647;
	return (unsigned) seed / 3;
}
