#include <stdio.h>
#include <stdlib.h>

void init() {
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}

void haha() {
    system("sh");
}

int main() {
    char s[100];
    init();
    puts("The Portal Gun is a gadget that allows the user(s) to travel between different universes/dimensions/realities.");
    puts("Where do you want to go?");
    gets(s);
}
