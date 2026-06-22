# I completely forget how to make these. But TPAINT MAKEFILE!!!
#

BDIR=build
OBJ=build/main.o build/tpaint_tile.o build/tpaint_files.o build/tpaint_palette.o

tpaint: $(OBJ)
	gcc $(OBJ) -lnyfw -o $@

$(BDIR):
	mkdir -p $(BDIR)

$(BDIR)/main.o: src/main.c | $(BDIR)
	gcc -c $< -o $@

$(BDIR)/tpaint_tile.o: src/tpaint_tile.c | $(BDIR)
	gcc -c $< -o $@

$(BDIR)/tpaint_files.o: src/tpaint_files.c | $(BDIR)
	gcc -c $< -o $@

$(BDIR)/tpaint_palette.o: src/tpaint_palette.c | $(BDIR)
	gcc -c $< -o $@

clean:
	rm -rf build
	rm -f tpaint



