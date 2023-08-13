#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char* input = (char*) malloc(128 * sizeof(char));
    scanf("%s", input);

    if (!strcmp(input, "abcd")) {
        printf("qwer");
    } else {
        printf("what?");
    }
}