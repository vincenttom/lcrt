SUBDIRS = language src help pixmaps gnome

EXTRA_DIST = AUTHORS NEWS ChangeLog COPYING README Doxyfile

uninstall-local:
	rm -rf $(DESTDIR)$(datadir)/$(PACKAGE)
	rm -rf $(DESTDIR)$(datadir)/gnome/help/$(PACKAGE) 
	
# Build ChangeLog from GIT  history
changelog:
	@if test -f $(top_srcdir)/.git/HEAD; then \
		git log --stat > $@; \
	fi

dist: ChangeLog	

.PHONY: ChangeLog doc

DOC_TOOLS = doxygen dot
doc:
	@for i in $(DOC_TOOLS);\
	do \
		which $$i >/dev/null; \
		if [ $? -ne 0 ]; then \
			echo "Doc tool '$$i' isn't found,please install it and try again."; \
			exit 1; \
		fi \
	done
	@doxygen Doxyfile



