
SUBDIRS = bflang bflib templates

EXTRA_DIST = $(nobase_bfdata_DATA) $(bfdata_SCRIPTS)

MAINTAINERCLEANFILES = Makefile.in

bfdatadir = $(pkgdatadir)
nobase_bfdata_DATA = \
	default_accelmap \
	encodings \
	snippets \
	colorprofiles/Light_theme \
	colorprofiles/Dark_theme

bfdata_SCRIPTS = \
	lorem-ipsum-generator \
	jsmin.py \
	json_prettyprint.py \
	css_decompressor \
	cssmin.py
