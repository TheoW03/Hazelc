#include <stdio.h>

struct Thunk
{
    void *value;
    struct Thunk *args;
    void *(*computation)(struct Thunk *);
};
void *generate_func(struct Thunk t)
{

    if (t.value != NULL)
    {
        return t.value;
    }
    t.value = t.computation(t.args);
    return t.value;
}
void *add(struct Thunk args[])
{
    void *pd = generate_func(args[0]);
    void *plhs = generate_func(args[1]);
    double pans = *(double *)plhs + *(double *)pd;
    void *a = malloc(sizeof(double));
    a = &pans;
    return a;
}
void *two(struct Thunk args[])
{
    double f = 3.1415;
    void *pd = malloc(sizeof(double));
    pd = &f;
    return pd;
}
void *one(struct Thunk args[])
{
    // int a = (int)generate_func(args[0]);
    // printf("%d \n", a);
    double f = 3.1415;
    void *pd = malloc(sizeof(double));
    pd = &f;
    return pd;
    // return (void *)3.1415;
}
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
    char buffer[a.size + b.size]; // Allocate space for 20 chars
    snprintf(buffer, b.size + a.size, "%s%s", a.str, b.str);

    struct String c = {buffer, a.size + b.size};
    printf("%s \n", c.str);
    return 0;
}
