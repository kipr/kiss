; Script generated with the Venis Install Wizard

; Define your application name
!define APPNAME "KISS-C"
!define APPMAJORVERSION "2"
!define APPMINORVERSION "1"
!define BUILDNUMBER "1"

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
	File "\Qt\4.4.3\bin\QtCore4.dll"
	File "\Qt\4.4.3\bin\QtGui4.dll"
	File "\mingw\bin\mingwm10.dll"

	; Set the dlls and frameworks to hidden
	SetFileAttributes "$INSTDIR\QtCore4.dll" FILE_ATTRIBUTE_HIDDEN
	SetFileAttributes "$INSTDIR\QtGui4.dll" FILE_ATTRIBUTE_HIDDEN
	SetFileAttributes "$INSTDIR\mingwm10.dll" FILE_ATTRIBUTE_HIDDEN
	
	; The Binary :)
	File "..\kiss.exe"
	File "..\doc\ChangeLog.txt"

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

	; Stuff for the CBC Target

	; Copy the basic target stuff over
	SetOutPath "$INSTDIR\targets\cbc\"
	File "..\deploy\targets\cbc\cbc.target"
	File "..\deploy\targets\cbc\cbc.api"
	File "..\deploy\targets\cbc\template.c"
	File "..\deploy\targets\cbc\libcbc_plugin.dll"
	SetFileAttributes "$INSTDIR\targets\cbc\libcbc_plugin.dll" FILE_ATTRIBUTE_HIDDEN

	File /r "..\deploy\targets\cbc\manual"
	
	; Includes
	SetOutPath "$INSTDIR\targets\cbc\include\"
	File "..\deploy\targets\cbc\include\cbc-sim.h"
	file "..\deploy\targets\cbc\include\botball.c"

	; Libs
	SetOutPath "$INSTDIR\targets\cbc\lib"
	File "..\deploy\targets\cbc\lib\libcbc_sim.a"

	; Stuff for the CBC 2 Target

	; Copy the basic target stuff over
	SetOutPath "$INSTDIR\targets\cbc2\"
	File "..\deploy\targets\cbc2\cbc2.target"
	File "..\deploy\targets\cbc2\cbc2.api"
	File "..\deploy\targets\cbc2\template.c"
	File "..\deploy\targets\cbc2\libcbc2_plugin.dll"
	SetFileAttributes "$INSTDIR\targets\cbc2\libcbc2_plugin.dll" FILE_ATTRIBUTE_HIDDEN

	File /r "..\deploy\targets\cbc2\manual"
	
	; Includes
	SetOutPath "$INSTDIR\targets\cbc2\include\"
	File "..\deploy\targets\cbc2\include\cbc2-sim.h"
	file "..\deploy\targets\cbc2\include\botball.c"

	; Libs
	SetOutPath "$INSTDIR\targets\cbc2\lib"
	File "..\deploy\targets\cbc2\lib\libcbc2_sim.a"

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

	Delete "$INSTDIR\targets\gcc\gcc.target"
	Delete "$INSTDIR\targets\gcc\gcc.api"
	Delete "$INSTDIR\targets\gcc\manual.html"
	Delete "$INSTDIR\targets\gcc\template.c"
	Delete "$INSTDIR\targets\gcc\libgcc_plugin.dll"

	; clean up mingw gcc
	RMDir /r "$INSTDIR\mingw"
	
	Delete "$INSTDIR\targets\gcc\include\kiss-serial.h"
	Delete "$INSTDIR\targets\gcc\include\kiss-graphics.h"
	Delete "$INSTDIR\targets\gcc\include\kiss-input.h"
	Delete "$INSTDIR\targets\gcc\include\kiss-compat.h"
	Delete "$INSTDIR\targets\gcc\include\kiss-create.h"
	Delete "$INSTDIR\targets\gcc\include\GL\GLee.h"
	Delete "$INSTDIR\targets\gcc\include\GL\glfw.h"

	; Libs
	Delete "$INSTDIR\targets\gcc\lib\libGLee.a"
	Delete "$INSTDIR\targets\gcc\lib\libglfw.a"
	Delete "$INSTDIR\targets\gcc\lib\libkiss.a"
	
	RMDir "$INSTDIR\include\GL"
	RMDir "$INSTDIR\include"
	RMDIR "$INSTDIR\lib"
	
SectionEnd

BrandingText "KISS Institute for Practical Robotics"

; eof
