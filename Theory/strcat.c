#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct String
{
    char *str;
    int size;
};

int main(int argc, char const *argv[])
{

    struct String s1 = {"hello world", 5};
    size_t start = 2;
    size_t end = 10;
    size_t len = end - start;
    char *sub = malloc(len);
    memcpy(sub, s1.str + start, len);
    printf("%s \n", sub);
    return 0;
}
