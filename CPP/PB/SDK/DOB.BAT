bcc -c -ml -DPEX -O1 %1.c*
tlink pb_sdk.obj %1,%1.pex,%1.map/s/c
copy %1.pex e:\bbs\pex
