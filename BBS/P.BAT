:again
PROBOARD -S %1 %2 %3 %4 %5 %6 %7 %8 %9
if errorlevel 99 goto out
if errorlevel 1 goto fatal
goto again
:fatal
echo A fatal error occured.
goto x
:out
echo Normal exit.
:x
