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
    return forceEvaluation(p->n) - 1;
}

int factorial(struct Params *p)
{
    if (forceEvaluation(p->n) == 1)
    {
        return 1;
    }
    int n = forceEvaluation(p->n);
    struct ThunkMemoized next_thunk =
        {0, nMinusOne, 0, p};
    struct Params new_params;
    new_params.n = &next_thunk;
    return n * factorial(&new_params);
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

    /* code */

    // int f = factorial(&p);
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

    struct Params p1;
    struct ThunkMemoized n =
        {
            1,
            NULL,
            1,
            &p1};
    p1.a = &n;
    struct ThunkMemoized n2 =
        {
            0,
            functio_for_b,
            0,
            &p1};
    p1.b = &n2;

    int a = add(&p1);
    printf("%d \n", a);
    return 0;
}
