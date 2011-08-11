SUBDIRS = language src help pixmaps gnome

EXTRA_DIST = AUTHORS NEWS ChangeLog COPYING README

uninstall-local:
	rm -rf $(DESTDIR)$(datadir)/$(PACKAGE)
	rm -rf $(DESTDIR)$(datadir)/gnome/help/$(PACKAGE) 
	
# Build ChangeLog from SVN  history
changelog:
	@if test -f $(top_srcdir)/.git/HEAD; then \
		git log --stat > $@; \
	fi
.PHONY: ChangeLog	
