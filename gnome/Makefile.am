EXTRA_DIST = lcrt.desktop

define install-menuitem
	test -z $(DESTDIR)${datadir}/$(1) || $(MKDIR_P) $(DESTDIR)/${datadir}/$(1)
	$(INSTALL_DATA) $(2) $(DESTDIR)${datadir}/$(1)
endef

install-data-local:
	$(call install-menuitem,applications,lcrt.desktop)

uninstall-local:
	rm -f $(DESTDIR)${datadir}/applications/lcrt.desktop
