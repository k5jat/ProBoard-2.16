@Echo Off
Cls

If Exist Sdk_Math.Lib Del Sdk_Math.Lib >Nul

rem :: --[ extract the object files ]------------------------------------------

If Exist Sdk_Math.Rsp Del Sdk_Math.Rsp >Nul
    Echo *F_LXMUL *F_PCMP  *F_SCOPY *H_LDIV  & >> Sdk_Math.Rsp
    Echo *H_LLSH  *H_LRSH  *H_LURSH *H_PADA  & >> Sdk_Math.Rsp
    Echo *H_PADD  *H_PINA  *H_PSBH  *H_PSBP  & >> Sdk_Math.Rsp
    Echo *H_SPUSH *N_LXMUL *N_PCMP  *N_SCOPY   >> Sdk_Math.Rsp
\Bc31\Bin\Tlib \Bc31\Lib\Cl.Lib @Sdk_Math.Rsp

rem :: --[ Create the Sdk_Math library [---------------------------------------

If Exist Sdk_Math.Rsp Del Sdk_Math.Rsp >Nul
    Echo +F_LXMUL +F_PCMP  +F_SCOPY +H_LDIV  & >> Sdk_Math.Rsp
    Echo +H_LLSH  +H_LRSH  +H_LURSH +H_PADA  & >> Sdk_Math.Rsp
    Echo +H_PADD  +H_PINA  +H_PSBH  +H_PSBP  & >> Sdk_Math.Rsp
    Echo +H_SPUSH +N_LXMUL +N_PCMP  +N_SCOPY   >> Sdk_Math.Rsp
\Bc31\Bin\Tlib Sdk_Math.Lib @Sdk_Math.Rsp

rem :: --[ Cleanup the directory [---------------------------------------------
rem :: The following line is for 4DOS/NDOS only. Remove it if you use another
rem :: DOS version. You have to delete the OBJ file by yourself...

rem Except (Pb_Sdk.Obj) del *.Obj >Nul


