@echo off

title Resetting ACLs...

cd /d "%ProgramFiles%\Windows Resource Kits\Tools"

echo. 

echo Resetting ACLs...

echo (this may take several minutes to complete)

echo. 

echo ==========================================================================

echo. 

echo. 

subinacl /subkeyreg HKEY_LOCAL_MACHINE /grant=administrators=f /grant=system=f

echo. 

echo. 

subinacl /subkeyreg HKEY_CURRENT_USER /grant=administrators=f /grant=system=f

echo. 

echo. 

subinacl /subkeyreg HKEY_CLASSES_ROOT /grant=administrators=f /grant=system=f

echo. 

echo. 

echo System Drive...

subinacl /subdirectories %SystemDrive% /grant=administrators=f /grant=system=f

echo. 

echo. 

echo Windows Directory...

subinacl /subdirectories %windir%\*.* /grant=administrators=f /grant=system=f

echo. 

echo. 

echo ==========================================================================

echo. 

echo FINISHED.

echo. 

echo Press any key to exit . . .

pause >NUL
