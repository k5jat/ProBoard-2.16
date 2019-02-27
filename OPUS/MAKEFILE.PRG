# This is a sample PROGRAM template file for OPUS MKMF 3.0

# MKMF automatically maintains the HDRS, EXTHDRS, OBJS and SRCS macros.
# The PROGRAM macro is the name of the executable to be constructed.
#
# The EXTHDRS, HDRS, and SRCS macro are not explicitly used in this makefile
# but you may have a need for them elsewhere (e.g. for revision control).
#
CFLAGS		=		# optional - macro used by the .c.obj rule and
				# used to pass compiler flags to the C compiler.
DEST		= .		# optional - the directory where the PROGRAM
				# will be installed.
EXTHDRS		=
HDRS		=
LDFLAGS		=		# optional - names the linker flags
LDMAP		=	 	# optional - to name the linker map file
LIBS		=		# optional - library files (full pathnames)
LINKER		= link		# optional - the name of the linker
MODEL		= S		# optional - a compiler model letter
OBJS		=
PROGRAM		= test.exe
SRCS		=


# This is the only required target. It states that the PROGRAM depends
# on all files listed in the OBJS and LIBS macros. The shell lines for
# updating the library come from the %.exe rule, which is built into Make
# and is also reproduced below.
#
$(PROGRAM):     $(OBJS) $(LIBS)


# %.exe :
# 	%if %null(.SOURCES)
# 	% error %.exe rule can't update $(.TARGET)
# 	%endif
# 	$(LINK) $(.SOURCES,M${.REGEX_DOT}obj),$(.TARGET),$(LINKMAP), \
#		$(.SOURCES,M${.REGEX_DOT}lib) $(LINKLIBS) $(LINKFLAGS);


# The following targets are optional, and are included here as examples.
#
clean:;		@erase *.obj

depend:;	@mkmf

install:	$(PROGRAM)
		@echo Installing $(PROGRAM) in $(DEST)
		@if not $(DEST)x==.x copy $(PROGRAM) $(DEST)

