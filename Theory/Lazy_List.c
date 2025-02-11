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

struct ListThunk
{
    struct Thunk *list_thunks; // either that or a function ptr
    struct Node *head;
    int size;
};

int three()
{
    return 3;
}
int four() { return 4; }

int two() { return 2; }
int one() { return 1; }
int compute(struct Thunk t)
{
    printf("%d \n", t.is_computed);
    if (t.is_computed == 1)
    {
        return t.data;
    }
    t.data = t.func_ptr();
    return t.data;
}
int get_elem(struct ListThunk thunk, int n, int index)
{
    if (thunk.head == NULL || index > thunk.size)
    {
        return 0;
    }
    if (n == 0)
    {

        if (thunk.head == NULL)
            printf("NUL \n");
        return compute(thunk.head->data); // Base case: return the value of the current node
    }
    printf("a\n");

    thunk.head->get_next(&thunk.head->next, thunk.list_thunks[index]);
    thunk.head = thunk.head->next;
    index++;
    n--;
    return get_elem(thunk, n, index); // Recursive case: move to the next node
}
int list(struct Node *head, int n, int index, int size)
{
    struct Thunk b = {two, 0, 0};
    struct Thunk c = {three, 0, 0};
    struct Thunk d = {four, 0, 0};

    struct Thunk thunks[] =
        {
            b, c, d};

    // Function to retrieve the nth element (recursively)
    if (head == NULL || index > 3)
    {
        return 0;
    }
    if (n == 0)
    {
        return compute(head->data); // Base case: return the value of the current node
    }
    // printf("%d \n"q, head->value);
    head->get_next(&head->next, thunks[index]);
    head = head->next;
    index++;
    n--;
    return list(head, n, index, size); // Recursive case: move to the next node
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

int main(int argc, char const *argv[])
{
    /* code */

    struct Node *c = NULL;
    // struct Node* c = malloc(sizeof(Node));
    struct Thunk a = {one, 0, 0};
    // appendNode(&c, a);
    // int f = list(c, 1, 0,s 0);
    // printf("f %d \n", f);
    struct ListThunk l = get_list();
    int f = get_elem(l, 2, 0);
    printf("f %d \n", f);
    return 0;
}
