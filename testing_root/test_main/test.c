#include <stdio.h>
#ifdef TEST
#else
#include "../pushpush_server.c"
#endif

int main(){
	printf("Happy happy happy\n");
	write_byte(2, "HI\n", 3);
	return 0;
}
