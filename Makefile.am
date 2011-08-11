SUBDIRS = language src help pixmaps gnome

EXTRA_DIST = AUTHORS NEWS ChangeLog COPYING README

uninstall-local:
	rm -rf $(DESTDIR)$(datadir)/$(PACKAGE)
	rm -rf $(DESTDIR)$(datadir)/gnome/help/$(PACKAGE) 
	
# Build ChangeLog from SVN  history
changelog:
	@if test -f $(top_srcdir)/.svn/entries; then \
		echo "" >.$$.ChangeLog; \
		date +"%Y-%m-%d %T" >>.$$.ChangeLog; \
		echo "===================================================================" >> .$$.ChangeLog; \
		svn diff >>.$$.ChangeLog; \
		cat .$$.ChangeLog | cat - ChangeLog > .ChangeLog && mv .ChangeLog ChangeLog; \
		rm -f .$$.ChangeLog; \
	fi
	
.PHONY: ChangeLog	
