#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXN 10

struct student {
    int id;
    int visit;
    int key;
    unsigned char *message;
};

struct student classroom[MAXN] = {0};

void init() {
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}

char* xor(unsigned char *m, int key) {
    unsigned char *message = strdup(m);
    for (int i = 0; i < strlen(message); i++) {
        message[i] ^= key;
    }
    return message;
}

void add_student(int id, int visit, int key, unsigned char* message) {
    int pos = -1;
    for (int i = 0; i < MAXN; i++) {
        if (!classroom[i].message) {
            pos = i;
            break;
        }
    }
    struct student* s = &classroom[pos];
    s->id = id;
    s->visit = visit;
    s->key = key & 0xff;
    s->message = message;
}

void show_classroom() {
    for (int i = 0; i < MAXN; i++) {
        if (!classroom[i].message) continue;
        if (classroom[i].visit) {
            printf("==== Student ID %d ====\n", classroom[i].id);
            printf("message: %s\n\n", xor(classroom[i].message, classroom[i].key));
        }
    }
}

void init_classroom() {
    add_student(0x0, 1, 0xaa, "\xc2\xc3\xc2\xc3\x86\x8a\xcb\xc4\xd3\xc8\xc5\xce\xd3\x8a\xc2\xcf\xd8\xcf\x95");
    add_student(0x1, 1, 0xca, "\xa3\xea\xab\xa7\xea\xa3\xa4\xea\xbe\xa2\xaf\xea\xa9\xa6\xab\xb9\xb9\xb8\xa5\xa5\xa7");
    add_student(0x2, 1, 0xdb, "\xb2\xfb\xba\xb6\xfb\xb2\xb5\xfb\xaf\xb3\xbe\xfb\xb8\xb7\xba\xa8\xa8\xa9\xb4\xb4\xb6\xf7\xfb\xaf\xb4\xb4");
    add_student(0x3, 1, 0x98, "\xf1\xb8\xf9\xf5\xb8\xf1\xf6\xb8\xec\xf0\xfd\xb8\xfb\xf4\xf9\xeb\xeb\xea\xf7\xf7\xf5\xb4\xb8\xec\xf0\xea\xfd\xfd");
    add_student(0x4, 1, 0x87, "\xee\xa7\xe6\xea\xa7\xee\xe9\xa7\xf3\xef\xe2\xa7\xe4\xeb\xe6\xf4\xf4\xf5\xe8\xe8\xea\xab\xa7\xe1\xe8\xf5\xb8");
    add_student(0x5, 1, 0x66, "\x11\xe\x9\x46\xf\x15\x46\x8\x9\x12\x46\xf\x8\x46\x12\xe\x3\x46\x5\xa\x7\x15\x15\x14\x9\x9\xb\x59");
    add_student(0xdead, 0, 0xde, "\xaa\xb6\xbf\xaa\xf9\xad\xfe\xbf\xfe\xb9\xb1\xb1\xba\xfe\xb7\xba\xbb\xbf\xf2\xfe\x8c\xb7\xbd\xb5");
    add_student(0x1337, 0, 0xff, "\x8b\x97\x96\x8c\xdf\x96\x8c\xdf\x9e\xdf\x98\x8d\x9a\x9e\x8b\xdf\x9e\x9b\x89\x9a\x91\x8b\x8a\x8d\x9a");
    add_student(0xcafe, 0, 0xef, "\x86\xcf\x83\x80\x99\x8a\xcf\x9b\x80\xcf\x8d\x8a\xcf\x96\x80\x9a\x9d\xcf\x81\x8a\x98\xcf\xa2\x80\x9d\x9b\x96");
}

void join_classroom() {
    int pos;
    puts("Which Morty you want to teach?");
    scanf("%d", &pos);
    void* ptr = &classroom;
    pos *= sizeof(struct student);
    struct student* stu = (ptr + pos);
    if (!stu->message) return;
    puts("Talk him the correct message:");
    int n = read(0, stu->message, 0x100);
    char s[100];
    puts("Confirm again:");
    read(0, s, 0x100);
}

void menu() {
    puts("Welcome to Morty school ^_^");
    puts("We need you to teach Morty.");
    printf("\nUseful information: %p\n\n", puts);
}

int main() {
    init();
    menu();
    init_classroom();
    show_classroom();
    join_classroom();
}
