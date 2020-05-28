CC		= gcc
CCFLAGS		= -no-pie -s

PROGS	= morty_school
PREFIX	= AIS3

all: $(PROGS)

%: %.c
	$(CC) -o $(OUTDIR)$@ $(CCFLAGS) $<

.PHONY: clean sig

clean:
	rm -f *~ $(PROGS)

sig: $(PROGS)
	mv $(PROGS) $(PROGS)-`sha256sum $(PROGS) | awk '{print $$1}'`
