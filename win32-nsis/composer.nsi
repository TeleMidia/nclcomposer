; composer.nsi
;
; This script is used to create the NCL Composer package installer for
; Windows.
;  

;--------------------------------

!define VERSION "0.1.0"
Name "NCL Composer ${VERSION}"
Caption "NCL Composer Installer"
OutFile "nclcomposer-installer-${VERSION}.exe"
Icon "../composer-gui/images/icon.ico"

; The default installation directory
InstallDir $PROGRAMFILES\Composer

; License informations
LicenseText "Please review the license terms before installing NCL Composer."
LicenseData "../composer-core/LICENSE.EPL"	

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages
Page license
Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------
; Install Types

InstType "Full" 
InstType "Minimal"

;--------------------------------
; The stuff to install

Section "NCL Composer Core (required)" ; No components page, name is not important
  SectionIn RO
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
      
  ; Put file there
  File "composer.nsi"
  WriteUninstaller "uninstall.exe"
SectionEnd ; end the section

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"
  SectionIn 1
  CreateDirectory "$SMPROGRAMS\NCL Composer"
  CreateShortCut "$SMPROGRAMS\NCL Composer\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\NCL Composer\ (MakeNSISW).lnk" "$INSTDIR\nclcomposer.nsi" "" "$INSTDIR\nclcomposer.nsi" 0      
SectionEnd

; Plugins optional section (can be disabled by the user)
SectionGroup /e "Install Default Plugins"
  Section "Textual View"
    SectionIn 1
  SectionEnd
  Section "Layout View"
    SectionIn 1
  SectionEnd
  Section "Properties View"
    SectionIn 1
  SectionEnd
  Section "Structural View"
    SectionIn 1
  SectionEnd
  Section "Outline View"
    SectionIn 1
  SectionEnd
SectionGroupEnd

;--------------------------------
; Uninstaller
UninstallText "This will uninstall NCL Composer. Hit next to continue"
Section "Uninstall"
  Delete "$INSTDIR\*"
  Delete "$SMPROGRAMS\NCL Composer\*"
  RMDir "$SMPROGRAMS\NCL Composer"
SectionEnd
