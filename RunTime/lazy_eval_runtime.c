// its going to use a stack data structure
// function pointers are pushed onto it and poped onto it
#define STACK_SIZE 10000
void *stack[STACK_SIZE];
int sp = 0;

/* Push a continuation `cont` */
void pushContinuation(void *cont)
{
    stack[sp] = cont;
    sp++;
}

/* Pop a continuation frame. */
void *popContinuation()
{
    sp--;
    void *cont = stack[sp];
    return cont;
}

/* bottom = bottom */
void bottom()
{
    bottom();
}
