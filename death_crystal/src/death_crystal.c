#include <stdio.h>

#define MAXN 4
#define LEN 0x40

char blocklist[MAXN] = {'$', '\\', '/', '^'};
char format[MAXN] = {'c', 'p', 'n', 'h'};
char message[LEN] = {0};

void init() {
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
    FILE *fin = fopen("/home/death_crystal/flag", "r");
    fread(message, LEN, 1, fin);
    fclose(fin);
}

int check(char* s) {
    int cnt = 0;
    for (int i = 0; s[i] != 0; i++, cnt++) {
        for (int j = 0; j < MAXN; j++) {
            if (s[i] == blocklist[j])
                return 0;
        }
    }
    for (int i = 0; i < cnt - 1; i++) {
        if (s[i] == '%') {
            for (int j = 0; j < MAXN; j++) {
                if (s[i + 1] == format[j])
                    return 0;
            }
        }
    }
    return 1;
}

int main() {
    init();
    puts("Death Crystals are crystals found on Forbodulon Prime that allow those who wield it to foresee their death in multiple possible future scenarios.");
    puts("Before you died, you can see the message I left.");
    while(1) {
        puts("Foresee:");
        char buf[40] = {0};
        scanf("%39s", buf);
        if (check(buf))
            printf(buf);
        puts("");
    }
}
