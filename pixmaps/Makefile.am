lcrt_LOGO = lcrt-logo.xpm
pkgdata_DATA = \
	lcrt-connect-in-tab.png \
    lcrt-cut.png \
    lcrt-find.png \
    lcrt-info.png \
    lcrt-paste.png \
    lcrt-rename.png \
	lcrt-connect.png \
    lcrt-delete.png \
    lcrt-help.png \
    lcrt-quick-connect.png \
    lcrt-settings.png \
	lcrt-copy.png \
    lcrt-disconnect.png \
    lcrt-home.png \
    lcrt-new-session.png \
    lcrt-reconnect.png \
    lcrt-terminal.png \
	$(lcrt_LOGO)

EXTRA_DIST = $(pkgdata_DATA)

pkgdatadir = ${datadir}/${PACKAGE}/pixmaps

install-data-local:
	test -z $(DESTDIR)${datadir}/pixmaps || $(MKDIR_P) $(DESTDIR)${datadir}/pixmaps
	$(INSTALL_DATA) $(lcrt_LOGO) $(DESTDIR)${datadir}/pixmaps

uninstall-local:
	rm -f $(DESTDIR)${datadir}/pixmaps/$(lcrt_LOGO)
