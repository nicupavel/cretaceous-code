#include <stdio.h>
#include <sys/ptrace.h>

int main()
{
	if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0) {
		printf("Acest process nu poate fi urmarit.\n");
		return 1;
	}
	printf("Hello World !\n");
	return 0;
}
