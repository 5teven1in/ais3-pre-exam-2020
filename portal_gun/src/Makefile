CC		= gcc
CCFLAGS		= -fno-stack-protector -no-pie -s

PROGS	= portal_gun
PREFIX	= AIS3

all: $(PROGS)

%: %.c
	$(CC) -o $(OUTDIR)$@ $(CCFLAGS) $<
	$(CC) -o $(OUTDIR)hook.so -fPIC -shared -ldl hook.c

.PHONY: clean sig

clean:
	rm -f *~ $(PROGS) hook.so

sig: $(PROGS)
	mv $(PROGS) $(PROGS)-`sha256sum $(PROGS) | awk '{print $$1}'`
