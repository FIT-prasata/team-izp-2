#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char const *argv[])
{
    FILE *file;
    char *lines[1000];

    file = fopen (argv[1], "r");
    printf("%s", file[0]);
    return 0;
}
