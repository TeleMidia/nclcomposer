REM - Description:
REM - 	This script is responsible to compile and generate the release files 
REM - 	to NCL Composer in Windows environments.
REM -
REM - 	The release files are:
REM -     * A ZIP file
REM -     * An installer file based on NSIS.
REM -
REM -   TODO:
REM -     * Handle Qt release files (.dll)
REM -     * Stop when any error occur.
REM -

REM - GET THE GIT VERSION
cd ../composer-plugins/
git describe --tag | sed "s/v//g" > ..\VERSION

REM - Go to default NCL Composer PATH
cd ..

SET /p CPRVERSION= <VERSION

REM - I CAN FORCE A VERSION IF I UNCOMMENT THE NEXT LINE
SET CPRVERSION=0.1.4

echo Generating NCL Composer %CPRVERSION% Windows Installer and Zip files.

REM - Run qmake with release parameters
IF "%CPRVERSION%"=="" (qmake FORCERELEASE=true RUNSSH_ON=true) ELSE (qmake FORCERELEASE=true RUNSSH_ON=true CPRVERSION=%CPRVERSION%)

REM - Generate translation files
cd composer-gui 
lupdate Composer.pro
lrelease Composer.pro

cd ..\composer-plugins
lupdate plugin-suite.pro
lrelease plugin-suite.pro

cd ..

REM - COMPILE
mingw32-make install
REM - END COMPILE AND INSTALL

REM - Go back to previous directory
cd win32-nsis

REM - Generate .zip release
7z a -tzip nclcomposer-%CPRVERSION%.zip C:\Composer

REM - Generate installer
"C:\Program Files\NSIS\makensis.exe" /DVERSION=%CPRVERSION% composer.nsi

