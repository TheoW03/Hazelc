#include <stdio.h>
#include <string.h>

struct String
{
    char *str;
    int len;
};

struct String get_str()
{
    struct String c1 = {"hello world", 6};
    return c1;
}
int main(int argc, char const *argv[])
{
    struct String c2 = {"hello ", 6};
    struct String c1 = get_str();
    int eq = strcmp(c1.str, c2.str);
    printf("%d\n", eq);
    return 0;
}
