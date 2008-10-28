BISONVERSION=bison-2.3
FLEXVERSION=flex-2.5.35
TOOLS=${PWD}/tools

all: tools/bin/flex tools/bin/bison c--

c--: src/c--
	cp $< $@

src/c--:
	cd src && make

tools/bin/bison:
	cd tools && tar zxvf ${BISONVERSION}.tar.gz
	cd tools/${BISONVERSION} && ./configure --prefix=${TOOLS} && make && make install

tools/bin/flex:
	cd tools && tar zxvf ${FLEXVERSION}.tar.gz
	cd tools/${FLEXVERSION} && ./configure --prefix=${TOOLS} && make && make install

clean:
	rm -f c--
	cd src && make clean

clobber: clean
	rm -rf tools/${FLEXVERSION} tools/${BISONVERSION}
	rm -rf tools/include tools/bin tools/usr tools/lib
	rm -rf tools/info tools/man tools/share

test: PROGRAM=phase2test
test: c-- ../pki_machine/pki_run
	./c-- Tests/$(PROGRAM).c--
	../pki_machine/pki_run Tests/$(PROGRAM).s
