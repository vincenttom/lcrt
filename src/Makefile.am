bin_PROGRAMS = lcrt

lcrt_SOURCES = lcrt.c \
	iwindow.c iwindow.h \
	cwindow.c cwindow.h \
	imenubar.c imenubar.h \
	cmenubar.c cmenubar.h \
	ifile.c ifile.h \
	cfile.c cfile.h \
	iedit.c iedit.h \
	cedit.c cedit.h \
	iview.c iview.h \
	cview.c cview.h \
	ioptions.c ioptions.h \
	coptions.c coptions.h \
	ihelp.c ihelp.h \
	chelp.c chelp.h \
	itoolbar.c itoolbar.h \
	ctoolbar.c ctoolbar.h \
	inotebook.c inotebook.h \
	cnotebook.c cnotebook.h \
	istatusbar.c istatusbar.h \
	cstatusbar.c cstatusbar.h \
	iterminal.c iterminal.h \
	cterminal.c cterminal.h \
	istatus.c istatus.h \
	cstatus.c cstatus.h \
	ipopup.c ipopup.h \
	cpopup.c cpopup.h \
	ilogin.c ilogin.h \
	clogin.c clogin.h \
	iqconnect.c iqconnect.h \
	cqconnect.c cqconnect.h \
	iconnect.c iconnect.h \
	cconnect.c cconnect.h \
	ilock.c ilock.h \
	clock.c clock.h \
	irename.c irename.h \
	crename.c crename.h \
	ifind.c ifind.h \
	cfind.c cfind.h \
	isettings.c isettings.h \
	csettings.c csettings.h \
	iabout.c iabout.h \
	cabout.c cabout.h \
	iaccels.c iaccels.h \
	caccels.c caccels.h \
	foperate.c foperate.h \
	mkconfig.c mkconfig.h \
	message.c message.h \
	support.c support.h \
	user.c user.h \
	serial.c serial.h \
	ssh.c ssh.h \
	telnet.c telnet.h \
	rlogin.c rlogin.h \
	shell.c shell.h \
	lang.c \
	list.h \
	debug.h

INCLUDES = \
	-I$(top_builddir) \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/language \
	$(LCRT_LIB_DEPS_CFLAGS)

lcrt_LDADD = \
	$(LCRT_LIB_DEPS_LIBS)

lcrt_CFLAGS = \
	-DLCRT_INSTALL_DIR="\"$(datadir)/$(PACKAGE)\"" \
	-DLCRT_HELP_DIR="\"$(datadir)/gnome/help/$(PACKAGE)\""

