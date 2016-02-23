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
REM -   DEPENDENCIES (must be in path)
REM -     * qmake
REM -     * mingw32-make
REM -     * lupdate/lrelease
REM -     * 7z
REM -     * makensis
REM -     * ssc/scp (if we want to publish)
echo off

REM - GET THE GIT VERSION
REM - cd ../
REM - git describe --tag | sed "s/v//g" > ..\VERSION

REM - Go to default NCL Composer PATH
cd ..

SET /p CPRVERSION= <VERSION

REM - I CAN FORCE A VERSION IF I UNCOMMENT THE NEXT LINE
SET CPRVERSION=0.2.1

REM - makensis path
SET MAKENSIS="makensis.exe"

SET PUBLISH_SERVER="robertogerson@xserve1"
SET PUBLISH_SERVER_PATH="/Library/WebServer/Documents/composer/downloads/nightly/"
SET PUBLISH_URL="%PUBLISH_SERVER%:%PUBLISH_SERVER_PATH%"

echo Generating NCL Composer %CPRVERSION% Windows Installer and Zip files.

REM - Run qmake with release parameters
IF "%CPRVERSION%"=="" (qmake FORCERELEASE=true RUNSSHON=true) ELSE (qmake FORCERELEASE=true RUNSSHON=true CPRVERSION=%CPRVERSION%)

REM - Generate translation files
cd src\gui 
lupdate gui.pro
lrelease gui.pro

cd ..\plugins
lupdate plugin-suite.pro
lrelease plugin-suite.pro

cd ..\..\

REM - COMPILE
mingw32-make install
REM - END COMPILE AND INSTALL

REM - Go back to previous directory
cd win32-nsis

REM - Generate .zip release
7z a -tzip nclcomposer-%CPRVERSION%.zip C:\Composer

REM - Generate installer
%MAKENSIS% /DVERSION=%CPRVERSION% composer.nsi
IF "%1"=="publish" (scp nclcomposer-installer-%CPRVERSION%.exe %PUBLISH_URL% && ssh %PUBLISH_SERVER% chmod 644 "%PUBLISH_SERVER_PATH%nclcomposer-installer-%CPRVERSION%.exe")

