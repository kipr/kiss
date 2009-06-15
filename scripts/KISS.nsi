; Script generated with the Venis Install Wizard

; Define your application name
!define APPNAME "KISS-C"
!define APPMAJORVERSION "1"
!define APPMINORVERSION "4"
!define BUILDNUMBER "7"

!define VERSION "${APPMAJORVERSION}.${APPMINORVERSION}.${BUILDNUMBER}"
!define INSTALL_FILENAME "KISS-${APPMAJORVERSION}.${APPMINORVERSION}.${BUILDNUMBER}"
;!define VERSION "${APPMAJORVERSION}.${APPMINORVERSION}-BETA"
;!define INSTALL_FILENAME "KISS-C-${APPMAJORVERSION}-${APPMINORVERSION}-SIM-BETA"
!define APPNAMEANDVERSION "KISS-C ${VERSION}"


; Main Install settings
Name "${APPNAMEANDVERSION}"
InstallDir "$PROGRAMFILES\${APPNAMEANDVERSION}"
InstallDirRegKey HKLM "Software\KISS" ""
OutFile "${INSTALL_FILENAME}.exe"

; Modern interface settings
!include "MUI.nsh"

; Section constat defs
!include "Sections.nsh"

!define MUI_ABORTWARNING

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Set languages (first is default language)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_RESERVEFILE_LANGDLL

Section "${APPNAMEANDVERSION}" Section1

	; Set Section properties
	SetOverwrite on
	SectionIn RO

	; Set Section Files and Shortcuts
	SetOutPath "$INSTDIR\"

	; Copy Essential Libraries
	File "\Qt\4.3.4\bin\QtCore4.dll"
	File "\Qt\4.3.4\bin\QtGui4.dll"
	File "\mingw\bin\mingwm10.dll"

	; Set the dlls and frameworks to hidden
	SetFileAttributes "$INSTDIR\QtCore4.dll" FILE_ATTRIBUTE_HIDDEN
	SetFileAttributes "$INSTDIR\QtGui4.dll" FILE_ATTRIBUTE_HIDDEN
	SetFileAttributes "$INSTDIR\mingwm10.dll" FILE_ATTRIBUTE_HIDDEN
	
	; The Binary :)
	File "..\kiss.exe"
	File "..\doc\ChangeLog.txt"s

	; Shortcuts
	CreateShortCut "$DESKTOP\${APPNAMEANDVERSION}.lnk" "$INSTDIR\kiss.exe"
	CreateDirectory  "$SMPROGRAMS\${APPNAMEANDVERSION}"
	CreateShortCut "$SMPROGRAMS\${APPNAMEANDVERSION}\${APPNAMEANDVERSION}.lnk" "$INSTDIR\kiss.exe"

	; Stuff for the GCC Target

	; Copy the basic target stuff over
	SetOutPath "$INSTDIR\targets\gcc\"
	File "..\deploy\targets\gcc\gcc.target"
	File "..\deploy\targets\gcc\gcc.api"
	File "..\deploy\targets\gcc\manual.html"
	File "..\deploy\targets\gcc\template.c"
	File "..\deploy\targets\gcc\libgcc_plugin.dll"

	SetFileAttributes "$INSTDIR\targets\gcc\libgcc_plugin.dll" FILE_ATTRIBUTE_HIDDEN

	; Copy over mingw gcc
	File /r "..\deploy\targets\gcc\mingw"
	
	; Includes
	SetOutPath "$INSTDIR\targets\gcc\include\"

	File "..\deploy\targets\gcc\include\kiss-serial.h"
	File "..\deploy\targets\gcc\include\kiss-graphics.h"
	File "..\deploy\targets\gcc\include\kiss-input.h"
	File "..\deploy\targets\gcc\include\kiss-compat.h"
	File "..\deploy\targets\gcc\include\kiss-create.h"
	SetOutPath "$INSTDIR\targets\gcc\include\GL"
	File "..\deploy\targets\gcc\include\GL\GLee.h"
	File "..\deploy\targets\gcc\include\GL\glfw.h"

	; Libs
	SetOutPath "$INSTDIR\targets\gcc\lib"
	File "..\deploy\targets\gcc\lib\libGLee.a"
	File "..\deploy\targets\gcc\lib\libglfw.a"
	File "..\deploy\targets\gcc\lib\libkiss.a"

SectionEnd


Section -FinishSection

	WriteRegStr HKLM "Software\${APPNAMEANDVERSION}" "" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAMEANDVERSION}" "DisplayName" "${APPNAMEANDVERSION}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAMEANDVERSION}" "UninstallString" "$INSTDIR\uninstall.exe"
	WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

; Modern install component descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${Section1} "Kiss with MinGW Included"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;Uninstall section

Section Uninstall

	;Remove from registry...
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
	DeleteRegKey HKLM "SOFTWARE\${APPNAME}"

	; Delete self
	Delete "$INSTDIR\uninstall.exe"

	; Delete Shortcuts
	Delete "$DESKTOP\${APPNAMEANDVERSION}.lnk"
	Delete "$SMPROGRAMS\${APPNAMEANDVERSION}\${APPNAMEANDVERSION}.lnk"
	Delete "$SMPROGRAMS\${APPNAMEANDVERSION}\Uninstall.lnk"


	; Clean up KISS-C
	Delete "$INSTDIR\kiss.exe"
	Delete "$INSTDIR\QtCore4.dll"
	Delete "$INSTDIR\QtGui4.dll"
	Delete "$INSTDIR\mingwm10.dll"
	
	; Clean up the Gcc target

	Delete "$INSTDIR\gcc.target"
	Delete "$INSTDIR\gcc.api"
	Delete "$INSTDIR\manual.html"
	Delete "$INSTDIR\template.c"
	Delete "$INSTDIR\libgcc_plugin.dll"

	; Copy over mingw gcc
	RMDir /r "$INSTDIR\mingw"
	
	Delete "$INSTDIR\include\kiss-serial.h"
	Delete "$INSTDIR\include\kiss-graphics.h"
	Delete "$INSTDIR\include\kiss-input.h"
	Delete "$INSTDIR\include\kiss-compat.h"
	Delete "$INSTDIR\include\kiss-create.h"
	Delete "$INSTDIR\include\GL\GLee.h"
	Delete "$INSTDIR\include\GL\glfw.h"

	; Libs
	Delete "$INSTDIR\lib\libGLee.a"
	Delete "$INSTDIR\lib\libglfw.a"
	Delete "$INSTDIR\lib\libkiss.a"
	
	RMDir "$INSTDIR\include\GL"
	RMDir "$INSTDIR\include"
	RMDIR "$INSTDIR\lib"
	
SectionEnd

BrandingText "KISS Institute for Practical Robotics"

; eof
