#include <stdio.h>
#include <string.h>

void main()
{
    // char original[] = "Hello, World!";
    char *original = "hello, world";
    char substring[10];
    int start = 7;
    int length = 13 - start;
    printf("%d \n", length);
    strncpy(substring, original + start, length);
    substring[length] = '\0'; // Null-terminate the substring

    printf("%s\n", substring);
}