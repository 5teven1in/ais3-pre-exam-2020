#include <stdio.h>

int system(const char *command) {
    puts("** system function hook **");
    return 0;
}
