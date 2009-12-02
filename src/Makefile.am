## Process this file with automake to produce Makefile.in

## Created by Anjuta

uidir = $(datadir)/matest/ui
ui_DATA = matest.ui

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(MATEST_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = matest

matest_SOURCES = \
	callbacks.c \
	callbacks.h \
	evaluation.c \
	MaTest.c \
	logics.h \
	user.c \
	user_gui.c \
	user_text.c \
	variables.c \
	wffs_pn.c \
	MaTest.h \
	connectives.c

matest_LDFLAGS = \
	-Wl,--export-dynamic

matest_LDADD = $(MATEST_LIBS)

EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(datadir)/matest
