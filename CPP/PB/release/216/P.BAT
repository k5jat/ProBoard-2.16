:again
PROBOARD -S
if errorlevel 99 goto out
if errorlevel 1 goto fatal
goto again
:fatal
echo A fatal error occured.
goto x
:out
echo Normal exit.
:x
