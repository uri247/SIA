@goto start

Copyright (c) 2007 Ness Technologies

Script: mkrop.cmd

Description:    After build, this script will collect all deliverable into a drop folder

Author:         Uri London

:start
    @echo on
    setlocal


:set_var
    set drop=c:\drop
    set variant=Release
    set rcs=c:\IntelSCS\Resources\IntelSCS\RCS
    

:clean
    rmdir /s /q %drop%
    if exist %drop% goto cant_clean
    mkdir %drop%


:copy
    xcopy html %drop%\. /efi
    copy %variant%\SiaCore.dll %drop%\.
    copy %variant%\ScsIA.exe %drop%\.
    copy %variant%\SiaCon.exe %drop%\.
    copy %variant%\SiaCore.pdb %drop%\.
    copy %variant%\ScsIA.pdb %drop%\.
    copy %variant%\SiaCon.pdb %drop%\.
    copy scripts\start.cmd %drop%\.
    copy scripts\fixreg.cmd %drop%\.
    xcopy scripts %drop%\scripts /efi
    xcopy %rcs% %drop%\RCS /efi
    
    goto end


:cant_clean
    echo can't clean %drop%. DROP DID NOT HAPPEN
    goto end

:end
    endlocal


