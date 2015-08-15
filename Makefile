
debug:
	@(cd core && make -j debug)
	@(cd input && make -j debug)
	@(cd gfx && make -j debug)
	@(cd net && make -j debug)
	@(cd audio && make -j debug)
	@(cd map && make -j debug)
	@(cd gui && make -j debug)
	@(cd game && make -j debug)

release:
	@(cd core && make -j release)
	@(cd input && make -j release)
	@(cd gfx && make -j release)
	@(cd net && make -j release)
	@(cd audio && make -j release)
	@(cd map && make -j release)
	@(cd gui && make -j release)
	@(cd game && make -j release)

clean:
	@(cd core && make -j clean)
	@(cd input && make -j clean)
	@(cd gfx && make -j clean)
	@(cd net && make -j clean)
	@(cd audio && make -j clean)
	@(cd map && make -j clean)
	@(cd gui && make -j clean)
	@(cd game && make -j clean)
	@(make -f Makefile.test clean)

test: debug
	@(make -f Makefile.test debug)

run: test
	./test_d

test_release: release
	@(make -f Makefile.test release)


