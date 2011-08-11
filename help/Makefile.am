SUBDIRS = man
DOC_LANGUAGES = en_US zh_CN

DOC_FIGURES = \
	figures/lcrt-connect.png \
	figures/lcrt-global-options.png \
	figures/lcrt-quick-connect.png \
	figures/lcrt-session-options.png

DOC_ENTITIES = \
	lcrt.xml \
	legal.xml

define doc-data-hook
	@for dir in $(DOC_LANGUAGES); do \
		echo "test -z '$(1)/$$dir' || $(MKDIR_P) '$(1)/$$dir'"; \
		test -z "$(1)/$$dir" || $(MKDIR_P) "$(1)/$$dir"; \
		files=`for p in $(DOC_ENTITIES); do echo $$dir/$$p; done`; \
		echo "$(INSTALL_DATA) $$files '$(1)/$$dir'"; \
		$(INSTALL_DATA) $$files "$(1)/$$dir"; \
		echo "test -z '$(1)/$$dir/figures' || $(MKDIR_P) '$(1)/$$dir/figures'"; \
		test -z "$(1)/$$dir/figures" || $(MKDIR_P) "$(1)/$$dir/figures"; \
		files=`for p in $(DOC_FIGURES); do echo $$dir/$$p; done` ; \
		echo "$(INSTALL_DATA) $$files '$(1)/$${dir}/figures'"; \
		$(INSTALL_DATA) $$files "$(1)/$${dir}/figures"; \
	done
endef

dist-hook: 
	$(call doc-data-hook,$(distdir))

install-data-local:
	$(call doc-data-hook,$(DESTDIR)$(datadir)/gnome/help/$(PACKAGE))

uninstall-local:
	@for d in $(DOC_LANGUAGES); do \
		echo " (cd '$(DESTDIR)$(datadir)/gnome/help/$(PACKAGE)/$$d' && rm -f $(DOC_ENTITIES) $(DOC_FIGURES))"; \
		(cd "$(DESTDIR)$(datadir)/gnome/help/$(PACKAGE)/$$d" && rm -f $(DOC_ENTITIES) $(DOC_FIGURES)); \
	done

