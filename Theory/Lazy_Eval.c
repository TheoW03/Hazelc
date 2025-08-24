#include <stdio.h>
#include <stdlib.h>
struct Params
{
    struct ThunkMemoized *a;
    struct ThunkMemoized *b;
    struct ThunkMemoized *n;
};

struct ThunkMemoized
{

    int computation;
    int (*func_ptr)(struct Params *);
    int computed;
    struct Params *params;
};
// sofces evaluation
int forceEvaluation(struct ThunkMemoized *t)
{
    if (t->computed == 0)
    {
        t->computation = t->func_ptr(t->params);
        t->computed = 1;
    }
    return t->computation;
}
int nMinusOne(struct Params *p)
{
    printf("%d \n", p->n->computation);
    printf("%d \n", p->n->computed);

    return forceEvaluation(p->n) - 1;
}

int factorial(struct Params *p)
{
    if (forceEvaluation(p->n) == 1)
    {
        return 1;
    }
    // p->n =
    int n = forceEvaluation(p->n);
    printf("m: %d \n", n);

    // p->n = next_thunk;

    struct ThunkMemoized next_thunk =
        {n, nMinusOne, 0};
    next_thunk.params = malloc(sizeof(struct Params));
    *next_thunk.params = *p;
    p->n = &next_thunk;
    return n * factorial(p);
}
int add(struct Params *p)
{
    return forceEvaluation(p->a) + forceEvaluation(p->b);
}
int functio_for_b(struct Params *params)
{
    struct Params p;
    struct ThunkMemoized next_thunk =
        {5, NULL, 1, &p};
    p.n = &next_thunk;
    return factorial(&p);
}
int main(int argc, char const *argv[])
{

    /*
    struct Params *p1 = malloc(sizeof(struct Params));
    p1->a->computation = 6;
    p1->a->func_ptr = NULL;
    // p1->a = {6, NULL, 1, p1};

    /* code */

    /*
        this is how


        let add (let b : integer, c : integer) => a + b
        let factorial(let n : integer) =>
        conditional : integer
            n = 1 => 1
            $defualt => n * factorial(n - 1)

        let main : integer => add(factorial(5), 1)

        would compile

        parameters every time a function is called, saves itself to
        the thunk, and creates a new copy of itself to pass into the function that contians the actual value.
        it does rely on stack as of right now. but we will see.

        arguably its more efficeient due to the heap being the heap
        I have no gc yet.

    */

    // struct Params *p1 = malloc(sizeof(struct Params));
    // struct ThunkMemoized n =
    //     {
    //         6,
    //         NULL,
    //         1,
    //         p1};
    // p1->n = &n;

    // struct Params *p = malloc(sizeof(struct Params));
    // p.n =
    // int f = factorial(p1);
    // struct ThunkMemoized n2 =
    //     {
    //         0,
    //         functio_for_b,
    //         0,
    //         &p1};
    // p1.b = &n2;

    // int a = add(&p1);
    // printf("%d \n", f);
    int c = 3 & 6 > 1 ^ 2;
    int f = 1 ^ 1 & 2;
    printf("%d \n", c);
    return 0;
}
