# Automake file

ACLOCAL_AMFLAGS = -I m4

# Nonstandard package files for distribution
EXTRA_DIST = \
  CHANGES \
  CONTRIBUTORS \
  Makefile

#
# kvf source files that we don't compile directly.  They are
# #included by kvf-all.cc.
#
KVF_SRC = \
  src/kvf.c \
  src/crud.c \
  src/pool.c

EXTRA_DIST += $(KVF_SRC)

# We define the global AM_CFLAGS as everything we compile includes from these
# directories.
AM_CFLAGS = -I$(srcdir) -I$(srcdir)/include

# Modifies compiler and linker flags for pthreads compatibility.
if HAVE_PTHREADS
  AM_CFLAGS += @PTHREAD_CFLAGS@ -DKVF_HAS_PTHREAD=1
  AM_LIBS = @PTHREAD_LIBS@
else
  AM_CFLAGS += -DKVF_HAS_PTHREAD=0
endif

# Build rules for libraries.
lib_LTLIBRARIES = lib/libkvf.la
lib_libkvf_la_SOURCES = src/kvf-all.c

pkginclude_HEADERS = \
  include/kvf/kvf_api.h \
  include/kvf/kvf_list.h \
  include/kvf/kvf_type.h \
  include/kvf/kvf.h
