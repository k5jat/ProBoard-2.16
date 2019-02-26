@echo off


bcc -c -ml -P-C %1.c

tlink ..\sdk\pb_sdk.lib %1,%1.pex /n /c

