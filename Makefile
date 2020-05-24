all:
	make -C ./death_crystal/src OUTDIR=../service/share/
	make -C ./meeseeks_box/src OUTDIR=../service/share/
	make -C ./morty_school/src OUTDIR=../service/share/
	make -C ./nonsense/src OUTDIR=../service/share/
	make -C ./portal_gun/src OUTDIR=../service/share/

