ENV	= OUTDIR=../service/share/

all:
	make -B -C ./bof/src $(ENV)
	make -B -C ./nonsense/src $(ENV)
	make -B -C ./portal_gun/src $(ENV)
	make -B -C ./morty_school/src $(ENV)
	make -B -C ./death_crystal/src $(ENV)
	make -B -C ./meeseeks_box/src $(ENV)
