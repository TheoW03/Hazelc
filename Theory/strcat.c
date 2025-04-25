#include <stdio.h>

struct String
{
    char *str;
    int size;
    /* data */
};

int main(int argc, char const *argv[])
{
    /* code */
    // struct Thunk b[] = {{(void *)1, NULL, NULL}};
    // struct Thunk a[] = {{NULL, b, one}, {NULL, NULL, two}};

    // printf("%f \n", *(double *)add(a));
    // char buff[100];
    // char *a = "hello";
    // char *b = "world";
    // strcat(a, b);
    // snprintf();
    char *str1 = "Hello";
    char *str2 = " World";
    struct String a = {"hello", 6};
    struct String b = {" world", 7};
    char *buffer = (char *)malloc(b.size + a.size);
    snprintf(buffer, b.size + a.size, "%s%s", a.str, b.str);
    struct String c = {buffer, a.size + b.size};
    printf("%s \n", c.str);
    return 0;
}
