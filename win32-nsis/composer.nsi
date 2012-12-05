; composer.nsi
;
; This script is used to create the NCL Composer package installer for
; Windows.
;  

!include "FileAssociation.nsh"

;--------------------------------

!ifndef VERSION
!define VERSION "0.1.3"
!endif

Name "NCL Composer ${VERSION}"
Caption "NCL Composer Installer"
OutFile "nclcomposer-installer-${VERSION}.exe"
Icon "../composer-gui/images/icon.ico"

; The default installation directory
; InstallDir "$PROGRAMFILES\NCL Composer"
InstallDir "C:\Composer"

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
; The stuffs to install

Section "NCL Composer Core (required)" ; No components page, name is not important
  SectionIn RO
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
      
  ; Put file there
  File "C:\Composer\*.dll"
  File "C:\Composer\*.exe"

  WriteUninstaller "uninstall.exe"

  ; include Files
  SetoutPath $INSTDIR\include
  File /r "C:\Composer\include\*"

  ; data Files
  SetOutPath $INSTDIR\data
  File "C:\Composer\data\*"

  ; install NCL Language Profile
  SetOutPath $INSTDIR\extensions
  File "C:\Composer\extensions\NCLLanguageProfile.dll"

  ; translation files
  SetOutPath $INSTDIR\extensions
  File "C:\Composer\extensions\*.qm"

  ; Associate .cpr files with NCL Composer
  ${registerExtension} $INSTDIR\composer.exe ".cpr" "NCL Composer project"
SectionEnd ; end the section

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"
  SectionIn 1
  CreateDirectory "$SMPROGRAMS\NCL Composer"
  CreateShortCut "$SMPROGRAMS\NCL Composer\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\NCL Composer\NCL Composer.lnk" "$INSTDIR\composer.exe" "" "$INSTDIR\composer.exe" 0      
  CreateShortCut "$SMPROGRAMS\NCL Composer\ (MakeNSISW).lnk" "$INSTDIR\nclcomposer.nsi" "" "$INSTDIR\nclcomposer.nsi" 0      
SectionEnd

; Plugins optional section (can be disabled by the user)
SectionGroup /e "Install Default Plugins"
  Section "Textual View"
    SectionIn 1
    SetOutPath $INSTDIR\extensions
    ; File "C:\Composer\extensions\qscintilla2.dll"
    File "C:\Composer\extensions\ncl_textual_plugin.dll"
  SectionEnd

  Section "Layout View"
    SectionIn 1
    SetOutPath $INSTDIR\extensions
    File "C:\Composer\\extensions\Qnly0.dll"
  SectionEnd

  Section "Properties View"
    SectionIn 1
    SetOutPath $INSTDIR\extensions
    File "C:\Composer\extensions\properties_view.dll"
  SectionEnd

  Section "Structural View"
    SectionIn 1
    SetOutPath $INSTDIR\extensions
    File "C:\Composer\extensions\Qnst0.dll"
  SectionEnd

  Section "Outline View"
    SectionIn 1
    SetOutPath $INSTDIR\extensions
    File "C:\Composer\extensions\outline_view.dll"
  SectionEnd

  Section "Validator Plugin"
    SectionIn 1
    SetOutPath $INSTDIR\extensions
    File "C:\Composer\extensions\ValidatorPlugin.dll"
  SectionEnd
SectionGroupEnd

;--------------------------------
; Uninstaller
UninstallText "This will uninstall NCL Composer. Hit next to continue"
Section "Uninstall"
  Delete "$INSTDIR\*"
  Delete "$INSTDIR\extensions\*"
  RMDir "$INSTDIR\extensions"
  RMDir "$INSTDIR"

  ;Shortcuts
  Delete "$SMPROGRAMS\NCL Composer\*"
  RMDir "$SMPROGRAMS\NCL Composer"

  ;Remove file association
  ${unregisterExtension} ".cpr" "NCL Composer project"
SectionEnd
