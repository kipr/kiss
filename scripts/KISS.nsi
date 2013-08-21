; Header for Modern UI
!include "MUI.nsh"

; Define KISS application name and version number
!define APP_NAME "KISS Platform"
!define APP_MAJOR_VERSION "4"
!define APP_MINOR_VERSION "1"
!define BUILD_NUMBER "1"

; Standard Release app name and version
!define VERSION "${APP_MAJOR_VERSION}.${APP_MINOR_VERSION}.${BUILD_NUMBER}"
!define INSTALLER_FILENAME "KISS-Platform-${VERSION}"
!define APP_NAME_AND_VERSION "${APP_NAME} ${VERSION}"

; Paths to files to build into the installer
!define KISS_DIR "C:\Users\Nafis\Documents\Development\kiss"
!define COMPUTER_DIR "C:\Users\Nafis\Documents\Development\computer"
!define MINGW_DIR "C:\Users\Nafis\Documents\Development\MinGW"
!define LIBKOVAN_DOCS_DIR "C:\Users\Nafis\Documents\Development\libkovan\doc"
!define LINK_DOCS_DIR "C:\Users\Nafis\Documents\Development\link-docs\KIPR Link C Standard Library"

; Name of the installer
Name "${APP_NAME_AND_VERSION}"

; Path to final install directory
InstallDir "$PROGRAMFILES\${APP_NAME_AND_VERSION}"

; Path to the installer itself
OutFile "${KISS_DIR}\releases\${INSTALLER_FILENAME}.exe"

; Modern interface settings
!define MUI_ABORTWARNING
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "License.txt"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; Set languages (first is default language)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_RESERVEFILE_LANGDLL

Section "KISS IDE" KISSIDE
	; Set Section properties
	SetOverwrite on  ; overwrite existing files
	SectionIn RO     ; cannot be unchecked  
  	
	; Set KISS Files	
	SetOutPath "$INSTDIR\KISS\"
	File /r "${KISS_DIR}\deploy\*.*"
	File /r "${KISS_DIR}\dlls\*.*"
		
	; Set up start menu entry
	CreateDirectory  "$SMPROGRAMS\${APP_NAME_AND_VERSION}"
	CreateShortCut "$SMPROGRAMS\${APP_NAME_AND_VERSION}\KISS IDE ${VERSION}.lnk" "$INSTDIR\kiss\kiss.exe"

	; Set up desktop shortcut
	CreateShortCut "$DESKTOP\KISS IDE ${VERSION}.lnk" "$INSTDIR\KISS\kiss.exe"
SectionEnd

Section "Computer Target" ComputerTarget
	; Set Section properties
	SetOverwrite on  ; overwrite existing files
	SectionIn RO     ; cannot be unchecked
  	
	; Set Computer Files	
	SetOutPath "$INSTDIR\computer\"
	File /r "${COMPUTER_DIR}\deploy\*.*"
	File /r "${COMPUTER_DIR}\dlls\*.*"
		
	; Set up start menu entry
	CreateShortCut "$SMPROGRAMS\${APP_NAME_AND_VERSION}\KISS IDE ${VERSION} Computer Target.lnk" "$INSTDIR\computer\computer.exe"

	; Set up desktop shortcut
	CreateShortCut "$DESKTOP\KISS IDE ${VERSION} Computer Target.lnk" "$INSTDIR\computer\computer.exe"
SectionEnd

Section "MinGW" MinGW
	; Set Section properties
	SetOverwrite on  ; overwrite existing files
	SectionIn RO     ; cannot be unchecked  
  	
	; Set MinGW Files	
	SetOutPath "$INSTDIR\MinGW\"
	File /r "${MINGW_DIR}\*.*"
SectionEnd

Section "libkovan Documentation" libkovan_doc
	; Set Section properties
	SetOverwrite on  ; overwrite existing files
  	
	; Set libkovan Documentation Files	
	SetOutPath "$INSTDIR\KISS\docs\libkovan\"
	File /r "${LIBKOVAN_DOCS_DIR}\*.*"
SectionEnd

Section "Link Documentation" link_doc
	; Set Section properties
	SetOverwrite on  ; overwrite existing files
  	
	; Set Link Documentation Files	
	SetOutPath "$INSTDIR\KISS\docs\KIPR Link C Standard Library\"
	File /r "${LINK_DOCS_DIR}\*.*"
SectionEnd

Section -FinishSection
	WriteRegStr HKLM "Software\${APP_NAME_AND_VERSION}" "" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME_AND_VERSION}" "DisplayName" "${APP_NAME_AND_VERSION}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME_AND_VERSION}" "UninstallString" "$INSTDIR\uninstall.exe"
	WriteUninstaller "$INSTDIR\uninstall.exe"
SectionEnd

; Modern install component descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${KISSIDE} "KIPR's Instructional Software System IDE"
!insertmacro MUI_DESCRIPTION_TEXT ${ComputerTarget} "A target for KISS IDE used to run programs locally"
!insertmacro MUI_DESCRIPTION_TEXT ${MinGW} "Minimalist GNU for Windows"
!insertmacro MUI_DESCRIPTION_TEXT ${link_doc} "Documentation for the KIPR Link"
!insertmacro MUI_DESCRIPTION_TEXT ${libkovan_doc} "Documentation for the libkovan standard library"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

; Uninstall section
Section Uninstall
	; Remove keys from registry
	DeleteRegKey HKLM "SOFTWARE\${APP_NAME_AND_VERSION}"
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME_AND_VERSION}"

	; Delete the uninstaller itself
	Delete "$INSTDIR\uninstall.exe"

	; Delete start menu entires and desktop shortcuts
	Delete "$SMPROGRAMS\${APP_NAME_AND_VERSION}\${APP_NAME_AND_VERSION}.lnk"
	Delete "$DESKTOP\${APP_NAME_AND_VERSION}.lnk"
	Delete "$SMPROGRAMS\${APP_NAME_AND_VERSION}\${APP_NAME_AND_VERSION} Computer Target.lnk"
	Delete "$DESKTOP\${APP_NAME_AND_VERSION} Computer Target.lnk"
	RMDir  "$SMPROGRAMS\${APP_NAME_AND_VERSION}"

	; Delete the entire install directory
	RMDir /r "$INSTDIR\"
SectionEnd

BrandingText "KISS Institute For Practical Robotics"