#define NULL ((void *)0)

struct Node
{
    char pad[0x28];
    struct Node *next;
};

void iosWakeupThread(struct Node *list); /* extern */

void func_40016BD0(struct Node *list)
{
    struct Node *current = list;

    while (current != NULL)
    {
        iosWakeupThread(current);
        current = current->next;
    }
}
