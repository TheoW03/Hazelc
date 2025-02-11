
struct Params
{
    struct Thunk0 *a;
    struct Thunk0 *b;

    /* data */
};

struct Thunk0
{

    int calc; // Cached value
    struct Params p;
    int (*func_ptr)(struct Params);
    int computed; // Flag to check if value is already computed
};

struct Node
{
    struct Thunk0 data;
    struct Node *next;
};
// Function to evaluate and cache the thunk's result
int forceEvaluation0(struct Thunk0 t)
{
    if (t.computed == 0)
    {                             // If not already computed
        t.calc = t.func_ptr(t.p); // Compute and store the result
        t.computed = 1;           // Mark as computed
    }
    return t.calc; // Return stored value
}
// to ompile this

// 1. in compilerHigh level it goes through the functions and sees what "thunks types" it needs
// it generates the thunk types as well as param type for eevry thunk it needs

int add(struct Thunk0 a, struct Thunk0 b)
{
    return forceEvaluation0(a) + forceEvaluation0(b);
}
int a(struct Params p)
{
    return 1;
}
int b(struct Params p)
{
    return 1;
}

int one(struct Params p)
{
    one(p);
}
int main()
{

    struct Thunk0 athunk;
    athunk.func_ptr = a;
    athunk.computed = 0;

    struct Thunk0 bthunk;
    bthunk.func_ptr = b;
    bthunk.computed = 0;
    struct Params p = {&athunk, &bthunk};

    athunk.p = p;
    bthunk.p = p;
    printf("%d \n", add(athunk, bthunk));

    //[1,2,3,4,5]
    // struct Thunk0 thubk1;
    // thubk1.func_ptr = a;
    // thubk1.computed = 0;

    // struct Thunk0 thubk2;
    // thubk2.func_ptr = b;
    // thubk2.computed = 0;

    return 0;
}
