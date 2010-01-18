## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src po

matestdocdir = ${prefix}/doc/matest
matestdoc_DATA = \
	INSTALL\
	ChangeLog.txt \
	Licence.txt \
	History.txt \
	Readme.txt


INTLTOOL_FILES = intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

EXTRA_DIST = $(matestdoc_DATA) \
	$(INTLTOOL_FILES)

DISTCLEANFILES = intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache

AM_CPPFLAGS = 

# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(matestdocdir)
