;----------------------------------------------
; Bluefish Windows NSIS Install Script
; [bluefish.nsi]
; 
;  Copyright (C) 2009-2025 The Bluefish Developers
;   Shawn Novak <Kernel86@gmail.com>
;   Daniel Leidert <daniel.leidert@wgdd.de>
;   Olivier Sessink <olivier@bluefish.openoffice.nl>
;----------------------------------------------

 
; Includes
;----------------------------------------------
!include "MUI2.nsh"
!include "logiclib.nsh"
!include "includes\fileassoc.nsh"

; External Defines
;----------------------------------------------
!ifndef PACKAGE
	!define PACKAGE "bluefish"
!endif
;!define LOCALE
!ifndef VERSION
	!define VERSION "2.2-nodef"
!endif

; Defines
;----------------------------------------------
!define PRODUCT		"Bluefish"
!define PUBLISHER	"The Bluefish Developers"
!define HOMEPAGE	"https://bluefish.openoffice.nl/"
!define HELPURL		"https://bluefish.openoffice.nl/manual/"
!define APPFILE	"${PACKAGE}.exe"
!define UNINSTALL_EXE	"bluefish-uninst.exe"
!define NAME "${PRODUCT} ${VERSION}"


; Variables
;----------------------------------------------


; Installer configuration settings
;----------------------------------------------
Name		"${PRODUCT} v${VERSION}"
OutFile "${NAME} Setup.exe"
InstallDir	"$PROGRAMFILES\${PRODUCT}"
InstallDirRegKey HKCU "Software\${NAME}" ""

; Tell Windows Vista and Windows 7 that we want admin rights to install
RequestExecutionLevel admin

SetCompressor /SOLID lzma
ShowInstDetails show
ShowUninstDetails show

;--------------------------------
; UI
  
!define MUI_ICON "pixmaps\bluefish-icon.ico"
;!define MUI_HEADERIMAGE
;!define MUI_WELCOMEFINISHPAGE_BITMAP "assets\welcome.bmp"
;!define MUI_HEADERIMAGE_BITMAP "assets\head.bmp"
;!define MUI_ABORTWARNING
;!define MUI_WELCOMEPAGE_TITLE "${SLUG} Setup"

;--------------------------------
; Pages
  
  ; Installer pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\COPYING"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
Var StartMenuFolder
!insertmacro MUI_PAGE_STARTMENU "Application" $StartMenuFolder
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
  
; Set UI language
!insertmacro MUI_LANGUAGE "English"


;--------------------------------
; Section - Install App

  Section "-hidden app"
    SectionIn RO
    SetOutPath "$INSTDIR"
    File /r "build\*.*" 
    WriteRegStr HKCU "Software\${NAME}" "" $INSTDIR
	!insertmacro APP_ASSOCIATE "bfproject" "bluefish.bfproject" "Bluefish project" "$INSTDIR\${APPFILE},0" "Open with Bluefish" "$INSTDIR\${APPFILE} $\"%1$\""
    WriteUninstaller "$INSTDIR\Uninstall.exe"
  SectionEnd

;--------------------------------
; Section - Shortcut

  Section "Desktop Shortcut" DeskShort
    CreateShortCut "$DESKTOP\${NAME}.lnk" "$INSTDIR\${APPFILE}"
!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
	CreateShortCut "$SMPROGRAMS\Bluefish.lnk" "$INSTDIR\${APPFILE}"
!insertmacro MUI_STARTMENU_WRITE_END

  SectionEnd

;--------------------------------
; Descriptions

  ;Language strings
  LangString DESC_DeskShort ${LANG_ENGLISH} "Create Shortcut on Dekstop."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${DeskShort} $(DESC_DeskShort)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
  


;--------------------------------
; Remove empty parent directories

  Function un.RMDirUP
    !define RMDirUP '!insertmacro RMDirUPCall'

    !macro RMDirUPCall _PATH
          push '${_PATH}'
          Call un.RMDirUP
    !macroend

    ; $0 - current folder
    ClearErrors

    Exch $0
    ;DetailPrint "ASDF - $0\.."
    RMDir "$0\.."

    IfErrors Skip
    ${RMDirUP} "$0\.."
    Skip:

    Pop $0

  FunctionEnd
  
;--------------------------------
; Section - Uninstaller

Section "Uninstall"

  ;Delete Shortcut
  Delete "$DESKTOP\${NAME}.lnk"
  Delete "$SMPROGRAMS\Bluefish.lnk"
  !insertmacro APP_UNASSOCIATE "bfproject" "bluefish.bfproject"
  ;Delete Uninstall
  Delete "$INSTDIR\Uninstall.exe"

  ;Delete Folder
  RMDir /r "$INSTDIR"
  ${RMDirUP} "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\${NAME}"

SectionEnd










