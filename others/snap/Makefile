#
# Makefile for non-Microsoft compilers
#

tri_snap: snap-core tri.cc
	g++ tri.cc -O3 -o tri_snap -fopenmp -Lsnap-core -lsnap -Isnap-core -Iglib-core

clean:
	rm tri_snap
	$(MAKE) clean -C snap-core
	$(MAKE) clean -C examples
	$(MAKE) clean -C test
	$(MAKE) clean -C tutorials
