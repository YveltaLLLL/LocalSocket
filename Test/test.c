#include <stdio.h>

int main() {
	int a = 8;
	char buffer[1024] = { 0 };
	*buffer = "%dţ��", a;
	printf("%s", buffer);
	return 0;
}