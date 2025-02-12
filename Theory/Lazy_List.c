#include <stdlib.h>
#include <stdio.h>

struct Thunk
{
    int (*func_ptr)();
    int is_computed;
    int data;
};
struct Node
{
    struct Thunk data;
    struct Node *next;
    void (*get_next)(struct Node *, struct Thunk);
};
struct RangedListThunk
{
    int start;
    int end;
    int size;
    struct Node *head;
};
struct ListThunk
{
    struct Thunk *list_thunks; // either that or a function ptr
    struct Node *head;
    int size;
};
int one() { return 1; }
int two() { return 2; }
int three()
{
    return 3;
}
int four() { return 4; }

int compute(struct Thunk t)
{
    if (t.is_computed == 1)
    {
        return t.data;
    }
    t.data = t.func_ptr();
    t.is_computed = 1;
    return t.data;
}
int get_iterated_elem(struct RangedListThunk thunk, int n, int index)
{
    if (thunk.head == NULL || index > thunk.size)
    {
        return 0;
    }
    if (n == 0)
    {

        return compute(thunk.head->data); // Base case: return the value of the current node
    }
    struct Thunk new_elem = {
        NULL,
        1,
        (thunk.start + index) + 1,
    };
    thunk.head->get_next(&thunk.head->next, new_elem);
    thunk.head = thunk.head->next;
    index++;
    n--;
    return get_iterated_elem(thunk, n, index); // Recursive case: move to the next node
}
/**
 * this traverses the list and allocates as it traverses the list
 *
 */
int get_elem(struct ListThunk thunk, int n, int index)
{
    if (thunk.head == NULL || index > thunk.size)
    {
        return 0;
    }
    if (n == 0)
    {

        return compute(thunk.head->data); // Base case: return the value of the current node
    }

    thunk.head->get_next(&thunk.head->next, thunk.list_thunks[index]);
    thunk.head = thunk.head->next;
    index++;
    n--;
    return get_elem(thunk, n, index); // Recursive case: move to the next node
}
// Helper function to create a new node

struct Node *createNode(struct Thunk value)
{
    void appendNode(struct Node * *head, struct Thunk value);
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->get_next = appendNode;
    newNode->next = NULL;
    return newNode;
}

// Helper function to append a node to the linked list
void appendNode(struct Node **head, struct Thunk value)
{
    struct Node *newNode = createNode(value);
    if (*head == NULL)
    {
        *head = newNode;
        return;
    }
    struct Node *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newNode;
}
struct ListThunk get_list()
{
    struct Thunk a = {one, 0, 0};
    struct Thunk b = {two, 0, 0};
    struct Thunk c = {three, 0, 0};
    struct Thunk d = {four, 0, 0};
    struct Node *list = NULL;
    struct Thunk *thunks = malloc(sizeof(struct Thunk) * 3);
    // struct Thunk thunks[3];
    thunks[0] = b;
    thunks[1] = c;
    thunks[2] = d;
    appendNode(&list, a);
    struct ListThunk t = {thunks, list, 4};
    return t;
}
struct RangedListThunk get_ranged_list(int start, int end)
{
    struct Thunk first =
        {
            NULL,
            1,
            start};
    struct Node *list = NULL;

    appendNode(&list, first);
    struct RangedListThunk a =
        {
            start,
            end - start,
            end,
            list};
    return a;
}

int main(int argc, char const *argv[])
{
    /* code */

    struct ListThunk l = get_list();
    int f = get_elem(l, 2, 0);
    struct RangedListThunk ranged_list = get_ranged_list(0, 20);
    int f1 = get_iterated_elem(ranged_list, 1, 0);
    printf("f %d \n", f1);
    ranged_list = get_ranged_list(0, 20);
    int f2 = get_iterated_elem(ranged_list, 19, 0);
    printf("f %d \n", f2);

    return 0;
}
