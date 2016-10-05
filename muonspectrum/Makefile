BASENAME    := flux
VERSION     := 1
SUBVERSION  := 0
REVISION    := 1

SONAME      := lib$(BASENAME).so.$(VERSION)
FILENAME    := $(SONAME).$(SUBVERSION).$(REVISION)

.PHONY: clean

all: lib/$(FILENAME)

clean:
	rm -f lib/*.* python/*.pyc python/*.pyo

lib/%.o: src/%.c include/%.h
	gcc -Iinclude -O3 -c -fPIC $< -o $@ 

lib/$(FILENAME): lib/FluxModel.o
	gcc -shared -Wl,-soname,$(SONAME) -o $@ $<
	ln -s $(PWD)/$@ lib/$(SONAME)
	ln -s $(PWD)/lib/$(SONAME) lib/lib$(BASENAME).so
	@ rm $<
