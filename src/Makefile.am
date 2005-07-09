bin_PROGRAMS=mptestsuite
mptestsuite_SOURCES=mptestsuite.c mpread.c mpwrite.c mpscan.c mptestsuite.h spod.c spod.h
EXTRA_DIST=*.mp3
INCLUDES=-I@BUILDDIR@
AM_CFLAGS=-pg
mptestsuite_LDFLAGS=-pg -a -L@BUILDDIR@/.libs
mptestsuite_LDADD=-lmp

