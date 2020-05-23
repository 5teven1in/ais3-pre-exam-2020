#include <stdio.h>
#include <unistd.h>

#define SCLEN 0x60
#define LEN 16

char nonsense[LEN] = "wubbalubbadubdub";
char sc[SCLEN] = {0};
char name[LEN] = {0};

void init() {
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}

int check() {
    for (int i = 0; i < SCLEN; i++) {
        if (sc[i] < 0x20 || sc[i] > 0x7f) return 0;
        int chk = 1;
        for (int j = 0; j < LEN; j++) {
            if (sc[i + j] != nonsense[j])
                chk = 0;
        }
        if (chk) return 1;
    }
    return 0;
}

int main() {
    init();
    puts("Welcome to Rick and Morty's crazy world.");
    puts("What's your name?");
    read(0, name, LEN);
    puts("Rick's stupid nonsense catchphrase is \"wubba lubba dub dub\".");
    puts("What's yours?");
    read(0, sc, SCLEN);
    if (check()) {
        (*(int(*)()) sc)();
    }
    else {
        puts("Ummm, that's totally nonsense.");
    }
}
