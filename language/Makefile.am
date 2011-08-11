lcrt_LANGUAGES = en_US.db zh_CN.db de_DE.db

noinst_PROGRAMS = make_language
make_language_SOURCES = \
	en_US.c en_US.h \
	zh_CN.c zh_CN.h \
	de_DE.c de_DE.h \
	language.c language.h \
	$(top_srcdir)/src/mkconfig.c $(top_srcdir)/src/mkconfig.h \
	$(top_srcdir)/src/foperate.c $(top_srcdir)/src/foperate.h

make_language_LDADD = \
	$(LCRT_LIB_DEPS_LIBS)

make_language_CFLAGS = \
	-DLCRT_INSTALL_DIR="\"$(datadir)/$(PACKAGE)\"" \
	-DLCRT_VERSION="\"$(VERSION)\""

INCLUDES = \
	-I$(top_builddir) \
	-I$(top_srcdir)/src \
	$(LCRT_LIB_DEPS_CFLAGS)

LANGUAGE_CONFIG_PATH = $(top_srcdir)/etc/language/

lcrt_DATABASES = $(addprefix $(LANGUAGE_CONFIG_PATH), $(lcrt_LANGUAGES))

all: $(lcrt_DATABASES)

$(lcrt_DATABASES): make_language
	@ rm -rf $(top_srcdir)/etc
	@ mkdir -p $(LANGUAGE_CONFIG_PATH)
	@ echo "make language database:"
	@ echo "============================================================================="
	@ ./$<

install-data-local:
	echo "$(MKDIR_P) $(DESTDIR)${datadir}/${PACKAGE}/etc/language"
	$(MKDIR_P) $(DESTDIR)${datadir}/${PACKAGE}/etc/language
	echo "$(INSTALL_DATA) $(lcrt_DATABASES) '$(DESTDIR)${datadir}/${PACKAGE}/etc/language'"
	$(INSTALL_DATA) $(lcrt_DATABASES) $(DESTDIR)${datadir}/${PACKAGE}/etc/language

uninstall-local:
	echo " (cd '$(DESTDIR)${datadir}/${PACKAGE}/etc/language' && rm -f $(lcrt_LANGUAGES))"
	(cd $(DESTDIR)${datadir}/${PACKAGE}/etc/language && rm -f $(lcrt_LANGUAGES))

clean-local:
	@if test -d $(LANGUAGE_CONFIG_PATH) ;then \
		echo "(cd '$(LANGUAGE_CONFIG_PATH)' && rm -f $(lcrt_LANGUAGES))"; \
		(cd $(LANGUAGE_CONFIG_PATH) && rm -f $(lcrt_LANGUAGES)); \
	fi
	@if test -d $(top_srcdir)/etc ; then \
		rm -rf $(top_srcdir)/etc; \
	fi

