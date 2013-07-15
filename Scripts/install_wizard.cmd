@goto start

Copyright 2012 Ness Technologies

Script:     install_wizard.cmd

Synopsis:   This script demonstrate how to install the Scs Installation wizard without
            being administrtor, but registering it on the user portion of the COM registry

Author:     Uri London

:start
    @echo off

:run
    regsvr32 /s /n /i:user SiaCore.dll


:test
    if errorlevel 1 goto error
    echo Installation of the 'Scs Installation Wizard' succeeded
    goto end


:error
    echo error installing the Scs Installation Wizard
    goto end


:end


