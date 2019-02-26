			OPUS Software README
			--------------------

This README file contains the most recent information about the OPUS
MAKE package. This README is for Opus Make v6.02 and MKMF v3.21. There
are 5 sections to this README:

	1) Additional information for upgrading from Make v5.2x.
	2) Documentation Clarifications and Error Corrections
	3) Unresolved problems in Make and MKMF known to us.
	4) Details of feature changes and enhancements since v6.00
	5) Details of bug fixes since v6.00


==============================================================================
		   1) Upgrading from Opus Make v5.2x
==============================================================================


For Opus Make v6.0 our goal has been to add capability to Make while
reducing "behind the back" operations that cause the usage of Make to be
just plain confusing at times. This second goal has resulted in the
introduction of several incompatibilities with Make v5.2x. The most
important incompatibility is the way inference rules are used.

In v5.2x, Make *always* used inference rules to check for an inferred
prerequisite (now called an inferred source).

In v6.0, Make uses inference rules *only* for targets without shell lines! 
What this means is that if you have something like:

	foo.obj : bar.h
		shell lines that update target.obj

where foo.obj also implicitly depends on foo.c. Make v6.0 will *not* use
inference rules to find the dependence of foo.obj on foo.c. Make v5.2x would
find this dependence.


For Make v6.0 you can get the prior behavior of always using inference
rules by adding the following directive to the makefile or make.ini

	.OPUS_52X : infer

You can also modify the dependency line shown above to make the dependence
of foo.obj on foo.c explicit:

	foo.obj : foo.c bar.h
		shell lines that update target.obj

or to force Make to use its inference rules to infer the dependence of
foo.obj on foo.c:

	foo.obj .INFER : bar.h
		shell lines that update target.obj


We apologize for this change but have introduced it to eliminate the
confusion that resulted when Make did an inference check even though
the user had seemingly specified everything necessary to build a target.


==============================================================================
		2) Documentation Clarifications and Error Corrections
==============================================================================


The Manual
==========

1. Page 6 - clarification
-------------------------

The following should better clarify the major incompatibility between
Make v6.0 and v5.2x.

		    Upgrading from Opus Make v5.2x
		    ------------------------------

For Opus Make v6.0 our goal has been to add capability to Make while
reducing behind-the-back operations that cause the usage of Make to be
just plain confusing at times. This second goal has resulted in the
introduction of several incompatibilities with Make v5.2x. The most
important incompatibility is the way inference rules are used.

In v5.2x, Make *always* used inference rules to check for an inferred
prerequisite (now called an inferred source).

In v6.0, Make uses inference rules *only* for targets without shell lines! 
What this means is that if you have something like:

	foo.obj : bar.h
		echo What is $(.SOURCE)?

where foo.obj depends implicitly on foo.c. Because of the shell lines,
v6.0 does *not* use inference rules to find foo.c. v5.2x would look
for foo.c. Note that the $(.SOURCE) macro (which is also $<) has the
value "bar.h" (the first explicit source) for v6.0 and the value "foo.c"
(the inferred source) for v5.2x. If you see Make trying to compile .h
files almost certainly you are being bitten by this incompatibility.


For Make v6.0 you can get the v5.2x behavior of always using inference
rules by adding the following directive to the makefile or make.ini.

	.OPUS_52X : infer

You can also modify the dependency line shown above to make the dependence
of foo.obj on foo.c explicit:

	foo.obj : foo.c bar.h
		shell lines that update target.obj

You can also force Make to use its inference rules to infer the dependence of
foo.obj on foo.c by using the .INFER attribute:

	foo.obj .INFER : bar.h
		shell lines that update target.obj


We have introduced this change to eliminate the confusion that resulted
when Make did an inference check even though the user had seemingly
specified everything necessary to build a target.


2. Page 18 - correction
-----------------------

In the example, the rule:

	io.obj : io.c
		$(CC) $(CFLAGS) -v -c io.c

should be:

	io.obj : io.c
		$(CC) $(CFLAGS) -c io.c

4. Page 99 - clarification
--------------------------

The second paragraph starting with "Several tricks ..." is sketchy. In
detail, the tricks used are:

	a) the .RULE attribute is used so a "makefile : makefile.__v" rule
	is created. If we didn't use the .RULE attribute the "makefile :
	makefile.__v" would be a dependency line that stated that "makefile
	depends on makefile.__v". The difference between these two is that
	with a rule, makefile.__v does not need to exist. If it does exist,
	Make will compare the timestamp of makefile to makefile.__v. If it
	doesn't exist, no comparison occurs. With a simple dependency,
	makefile.__v *must* exist or be makable.

	Essentially the same affect occurs by writing the rule as:

		makefile% : makefile.__v%

	b) the .MAKE attribute is used so the makefile gets browsed out of
	makefile.__v even if we used the "-n" (no execute) command-line flag.

	c) the .NOCHAIN attribute is used for a tiny increase in run-time
	speed. It results in makefile.__v not being checked with inference
	rules (since presumably it is a storage file that exists and Make
	doesn't know how to build it anyway).


3. Page 207 - correction
------------------------

To browse out files from Sourcerer's Apprentice the shell line:

	$(SAGET) $(SAGETFLAGS) $(.SOURCE) $(.VERSION)

should have been:

	$(SAGET) $(SAGETFLAGS) $(.TARGET) $(.VERSION)



The Quick Reference Guide
=========================

1.
--

Due to a font substitution error an "M" was printed instead of three vertical
dots in the forms for response files, object libraries and inference rules.
The three vertical dots symbol means the preceding makefile line can be
repeated zero or more times.


2.
--

Under Target Attributes, the description for the .COMPARE attribute is wrong.

If a target, S, has the .COMPARE attribute then when S is the source
for another target, T, the timestamp of S is compared to the timestamp 
of T to determine if T will be updated.

If S has the .NOCOMPARE attribute it means its timestamp will
not be compared. The normal behavior is for all targets to have the
.COMPARE attribute, but you can explicitly use .NOCOMPARE to say "I want
a target that is used to execute shell lines, but I don't want the fact
that the shell lines were executed to affect whether this target is updated."

For example:

	project.exe : import $(OBJS)
		<link OBJ files>

	import .NOCOMPARE :
		<get files into the project>





==============================================================================
		3) Unresolved Problems known to Opus Software
==============================================================================

Bug)	There is a sporadic problem with PROTECTED-MODE APPLICATIONS run
	with Make's memory miser. This bug is environment specific -- it
	will either bite you right away or (most likely) not at all. The
	symptoms are that Windows will report that your memory is corrupt
	and you should reboot. The only work-around is to *not* use the
	memory miser for the problematic application.
	 
	(Of course, there is no need to run a protected-mode app. with Make's
	memory miser, but it does happen...)

Bug)	For Make's SourceSafe support - although SourceSafe can get the
	username from Novell Netware, Make cannot. To use our SourceSafe
	support you must either define the SSUSER and SSPWD environment
	variables before using Make, or use the SSINIT program.


==============================================================================
		4) Feature Changes (Chg) and Enhancements (Enh)
==============================================================================

Chg)	EMS is no longer supported for the memory miser.

Chg)	The value of the MAKEVERSION macro is now 602.

Enh)	The .LOGO directive has been added

	The .LOGO makefile directive causes Make and MKMF to display their
	banners if they haven't displayed their banners yet. This directive
	is in the sample make.ini file in place of the MFLAGS=-V
	macro definition.

Enh)	The "Extension options" (printed with the "-v" command-line flag)
	now shows where the relevant macros are defined and what their
	expanded values are.

Enh)	MKMF now puts dependency information either in the makefile specified
	with the "-f" command-line flag or, if that makefile "%include"s a
	file that contains dependency information, then the new dependency
	information goes into the included file. This is useful if you keep
	the dependencies separate from the main makefile.

Enh)	Make 6.02 is supplied with an Microsoft NT executable. The only things
	not working are the TLIB or object library support.



==============================================================================
		5) Bug Fixes (Fix)
==============================================================================

Fixed in version 6.01
---------------------

Fix)	Inline response files and named response files.

	A macro reference could not be used to specify the name of the
	response file, so something like the following did not work:

	x:
		command <<$(RESPONSE)

		< ...


Fix)	Double-colon dependency lines simply did not work.


Fix)	The compiled-in .SA_GET and .PVCS_GET special targets were wrong.

	The .PVCS_GET special target should have been as defined on page 137.
	The .SA_GET special target should have been as defined on page 144.


Fix)	The "-" shell line prefix on commands executed by the shell.

	On a shell line, the "-" prefix before a command executed by the
	shell (and, in particular, for MS-DOS internal commands) inhibited
	the creation of the batch file "wrapper" around the command.
	This optimization was thought acceptable because the "-" prefix
	tells Make to ignore the exit status of the command anyway.
	However, hitting ^C during execution of the command went undetected
	by Make. Therefore, the optimization has been removed and a batch
	file wrapper is placed around all commands executed by the shell.


Fix)	Making the makefile before reading it (.MAKEMAKEFILE).

	By default, Make tries to read makefile, then makefile.mak. In
	fact, in general, for any <file> without extension, Make tries to
	read <file> then <file>.mak.
	
	To get the same behavior with making of <file> before reading it, Make
	now sees if <file> exists or if <file> can be made. If neither are
	true, Make tries making <file>.mak. This means that either makefile
	or makefile.mak will be made.


Fix)	A NULL-pointer dereference in the TLIB support caused a segmentation
	violation under OS/2.


Fix)	SourceSafe support.

 	Using the SSUSER environment variable to name the SourceSafe user
	caused a crash. Using SourceSafe's ssinit.com program without using
	SSUSER worked fine.


Fix)	Multiple-step inference rules

	MKMF could not follow multiple-step inference rules, so MKMF
	could not figure out the path from a source file to a built object
	file if the path involved multiple inference rules.


Fix) 	$^ at end of line

	In a makefile, the $^\n sequence was interpreted as "dollar",
	literal newline character (because ^\n is a literal newline in
	Microsoft NMAKE). The sequence is now properly interpreted as $^
	followed by end of line.


Fix) 	The BUILTINS macro

	The BUILTINS macro is now set before the builtins file is read, so can
	be used from with the builtins file.


Fix)	The %exists(name) functional operator

	%exists(/), %exists(\), %exists(<drive>:\) and %exists(<drive>:/) all
	returned 0 inappropriately.


Fix)	The %time(name) functional operator

	This is documented as returning the on-disk timestamp but was
	returning the on-disk timestamp for normal files and the VCS timestamp
	for VCS storage files. It now returns only the on-disk timestamp.

	Also, %if %time(file1)  comparison-operator  %time(file2) did not
	always work because "comparison-operator" was a 16-bit integer
	comparison and the timestamp difference could be larger than that.


Fixed in version 6.02
---------------------

Fix)	Mixing targets with queued shell lines and targets with conditional
	directives (but without queued shell lines) caused an "unterminated
	%if" error.

	As part of the fix, there is no longer the "exactly one shell line"
	restriction on targets that can be queued. So, this will work:

	%.obj : %.c
		%if defined(NOQUEUE)
			$(CC) $(CFLAGS) -c $<
		%else
			$(CC) $(CFLAGS) -c {$< }
		%endif

	For the purposes of queuing, non-conditional directives (i.e. internal
	commands) are treated like regular shell lines, so the following
	will execute the first "%setenv" line, queue the $(CC) line, then
	flush the queued line, then execute the second "%setenv" line.

	%.obj : %.c
		%setenv CL=/Zi/Od
		$(CC) $(CFLAGS) -c {$< }
		%setenv CL=


Fix)	Syntax error in %if with filenames allowed by DOS.

	DOS allows '&' in filenames, but '&' would cause a syntax error in the
	%if code. For example:

		%if %exists(R&D)

	would give a syntax error. This has been fixed and '&' is now allowed.


Fix)	Several fixes to queued shell lines:

	a) queued shell lines and "-k" did not interact properly when
	   an error occurred on a queued shell line.

   	b) a file-system lookup would not occur for queued targets when
	   the targets had not existed. The only way to get a lookup was
	   to give the targets the .REREAD attribute. This was in contrast
	   to non-queued targets where the lookup properly occurred for
	   *either* non-existing or .REREAD targets.


Fix)	The "-<N>" shell line prefix did not work properly.

	This prefix was to ignore shell lines that returned an error
	status up to and including '<N>'.


Fix)	Long shell lines could cause a system hang.

	Shell lines that are longer than DOS allows and are executed with
	the memory miser on could cause a system hang.


Fix)	Make would complain about "dummy" targets that could not be made.

	Dummy (or phony) targets are used to group other targets. For example:

		all : prog1.exe prog2.exe

	The "all" target is used for the affect that "make all" causes first
	"prog1.exe" then "prog2.exe" to be made.

	For a dummy target whose name had a file extension (e.g. "all.any"),
	Make would complain that it didn't know how to make the target. Using
	the .PHONY attribute (e.g. "all.any .PHONY :") would not help. This
	has been fixed. Now the .PHONY (or .ALWAYS) attribute tells Make not
	to complain about dummy targets that have a file extension.

	In PolyMake emulation mode, Make now never complains about not
	being able to make dummy targets, irrespective of whether the target
	has an extension or not.


Fix)	Two changes to PolyMake support:

	a) $[<any>,<arg>...] transformation macros now ignore whitespace
	   between "," and "<arg>".

    	b) %if <function>(<arg>) now ignore whitespace between "<function>"
	   and "(" and between "(" and "<arg>".


Fix)	Three changes to the library support:

	a) The shell line for the %.lib rule had the incorrect order of the
	   target name and LIBFLAGS. The correct shell line should have been
	   as printed in the manual on page 130, and repeated here:

		%.lib :
			%if %null(.NEWSOURCES)
			% error OPUS MAKE: %.lib can't update $(.TARGET)
			%endif
			$(STAMPOBJ) $(.NEWSOURCES)
			$(LIBEXE) $(.TARGET) $(LIBFLAGS) -+ $(.NEWSOURCES);

	b) The shell lines for alternative librarians were printed wrong on
	   page 130. The rules should be:

	   For SLR optlib:

		%.lib :
			<Optionally check that $(.NEWSOURCES) is not empty.>
			optlib $(.TARGET) $(LIBFLAGS) ~ $(.NEWSOURCES);

	   For IDC LibTools:
		%.lib :
			<Optionally check that $(.NEWSOURCES) is not empty.>
			libman $(.TARGET) $(LIBFLAGS) ~ $(.NEWSOURCES);

	c) The "%.lib : %.obj" rule has been dropped. This rule was necessary
	   (and appeared as the .obj.lib rule) for Opus Make v5.2x to get its
	   library support to work more quickly. This rule is completely
	   unnecessary for Opus Make v6.0


Fix)	In make.ini it was not possible to change the shell lines of the
	.PVCS_GET and other special targets. Make produced a warning that
	it was ignoring the new shell lines.  This has been fixed.


Fix)	For OS/2 Opus Make the rules were case sensitive rather than being
	dependent on the .IG/.NOIG directives.


Fix)	.SUFFIXES did not work properly if an extension showed up more than
 	once.


Fix)	MKMF RC file support did not work properly for locating ICON, BITMAP,
	CURSOR and FONT resources.


Fix)	PVCS support did not always find the proper trunk version when
	when there was also a branch off the trunk.

Fix)	%include "name" did not search for file "name" as described
	in the manual -- it only looked in the current directory and not
	in the directory of make.ini.


Fix)	MKMF Relative paths output did not always produce the correct output.


Fix)	The SourceSafe/Make API did not allow user names with "_" in them,
	while SourceSafe itself allowed "_".


Fix)	MKMF now supports:
		#if defined SYMBOL
	as well as:
		#if defined(SYMBOL)

Fix)	MKMF now supports #elif


Fix) 	Make's TLIB project tracking file support was broken.


Fix)	TOUCH.EXE was supposed to take a response file from the command
	line, but didn't.
