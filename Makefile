# I completely forget how to make these. But TPAINT MAKEFILE!!!
#

BDIR=build
OBJ=build/main.o build/tile_module.o build/file_module.o

tpaint: $(OBJ)
	gcc $(OBJ) -lnyfw -o $@

$(BDIR):
	mkdir -p $(BDIR)

$(BDIR)/main.o: src/main.c | $(BDIR)
	gcc -c $< -o $@

$(BDIR)/tile_module.o: src/tile_module.c | $(BDIR)
	gcc -c $< -o $@

$(BDIR)/file_module.o: src/file_module.c | $(BDIR)
	gcc -c $< -o $@

clean:
	rm -rf build
	rm -f tpaint



