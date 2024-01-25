#include "../server.h"

int main(int argc, char *argv[])
{
    loadJson(argv[1]);
    printf("%d",max_user);
    return 0;
}