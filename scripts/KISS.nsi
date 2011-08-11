; Define KISS application name and version number
!define APPNAME "KISS IDE"
!define APPMAJORVERSION "3"
!define APPMINORVERSION "0"
!define BUILDNUMBER "0"


; Standard Release app name and version
;!define VERSION "${APPMAJORVERSION}.${APPMINORVERSION}.${BUILDNUMBER}"
;!define INSTALL_FILENAME "KISS-IDE-${APPMAJORVERSION}.${APPMINORVERSION}.${BUILDNUMBER}"

;; Beta release app name and version
;; uncomment this section for beta releases
!define VERSION "${APPMAJORVERSION}.${APPMINORVERSION}.${BUILDNUMBER} Beta 4"
!define INSTALL_FILENAME "KISS IDE ${APPMAJORVERSION}.${APPMINORVERSION}.${BUILDNUMBER} Beta 4"

!define APPNAMEANDVERSION "KISS IDE ${VERSION}"

;Files driver installer utility files
!include "nsis-lib\winver.nsh"
!include "nsis-lib\drvsetup.nsh"
!include "nsis-lib\windrvinstall.nsh"


; Main Install settings

;; Where are the files to build in to the installer?
!define DEPLOYDIR "..\deploy"

;; What do we call the installer?
Name "${APPNAMEANDVERSION}"

;; Where should the installer put files when it runs?
InstallDir "$PROGRAMFILES\${APPNAMEANDVERSION}"

;; where do we put the installer after building it?
OutFile "..\releases\${INSTALL_FILENAME}.exe"

; Modern interface settings
!include "MUI.nsh"

!define MUI_ABORTWARNING

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "License.txt"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Set languages (first is default language)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_RESERVEFILE_LANGDLL

Section "KISS-IDE" KISSIDE

	; Set Section properties
	SetOverwrite on  ; yes, we want to overwrite existing files
	SectionIn RO     ; no, we don't want people to be able to uncheck this section   

  	
	; Set KISS Files ;;;;;;;;;;;;	
	; The easiest way to update these is by creating a new script with the 
	; New Script Wizard in Venis IX and copying this section from it
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	SetOutPath "$INSTDIR\targets"
	SetOutPath "$INSTDIR\lexers"
	SetOutPath "$INSTDIR\phonon_backend"
	File "C:\Projects\kiss\depends\phonon_ds94.dll"
	SetOutPath "$INSTDIR\"
	File "C:\Projects\kiss\deploy\KISS.exe"
	File "C:\Projects\kiss\depends\libgcc_s_dw2-1.dll"
	File "C:\Projects\kiss\depends\mingwm10.dll"
	File "C:\Projects\kiss\depends\QtCore4.dll"
	File "C:\Projects\kiss\depends\QtWebKit4.dll"
	File "C:\Projects\kiss\depends\QtNetwork4.dll"
	File "C:\Projects\kiss\depends\QtGui4.dll"
	File "C:\Projects\kiss\depends\phonon4.dll"
	File "C:\Projects\kiss\depends\qscintilla2.dll"
	File "C:\Projects\kiss-targets\root\gcc_target_win.kiss"
	File "C:\Projects\kiss-targets\root\cbc2_target_win.kiss"
	File "C:\Projects\kiss-targets\root\java_target_win.kiss"
	File "C:\Projects\kiss-targets\root\python_target_win.kiss"
	File "C:\Projects\kiss-targets\root\shell_target_win.kiss"
	File "C:\Projects\kiss-lexers\root\c_lexer_win.kiss"
	File "C:\Projects\kiss-lexers\root\cpp_lexer_win.kiss"
	File "C:\Projects\kiss-lexers\root\java_lexer_win.kiss"
	File "C:\Projects\kiss-lexers\root\python_lexer_win.kiss"
	File "C:\Projects\kiss-lexers\root\bash_lexer_win.kiss"
	
	
	ExecWait '"$INSTDIR\KISS.exe" --install gcc_target_win.kiss'
	ExecWait '"$INSTDIR\KISS.exe" --install cbc2_target_win.kiss'
	ExecWait '"$INSTDIR\KISS.exe" --install java_target_win.kiss'
	#ExecWait '"$INSTDIR\KISS.exe" --install python_target_win.kiss'
	#ExecWait '"$INSTDIR\KISS.exe" --install shell_target_win.kiss'
	ExecWait '"$INSTDIR\KISS.exe" --install c_lexer_win.kiss'
	ExecWait '"$INSTDIR\KISS.exe" --install cpp_lexer_win.kiss'
	ExecWait '"$INSTDIR\KISS.exe" --install java_lexer_win.kiss'
	ExecWait '"$INSTDIR\KISS.exe" --install python_lexer_win.kiss'
	ExecWait '"$INSTDIR\KISS.exe" --install bash_lexer_win.kiss'
	
	Delete "$INSTDIR\gcc_target_win.kiss"
	Delete "$INSTDIR\cbc2_target_win.kiss"
	Delete "$INSTDIR\java_target_win.kiss"
	Delete "$INSTDIR\python_target_win.kiss"
	Delete "$INSTDIR\shell_target_win.kiss"
	Delete "$INSTDIR\c_lexer_win.kiss"
	Delete "$INSTDIR\cpp_lexer_win.kiss"
	Delete "$INSTDIR\java_lexer_win.kiss"
	Delete "$INSTDIR\python_lexer_win.kiss"
	Delete "$INSTDIR\bash_lexer_win.kiss"
	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;End of KISS Files 
	
	
	; The change log isn't in the deploy directory so I'd probably forget it
	; File "..\doc\ChangeLog.txt"

	; Set the dlls and frameworks to hidden files on Windows
	SetFileAttributes "$INSTDIR\QtCore4.dll" FILE_ATTRIBUTE_HIDDEN
	SetFileAttributes "$INSTDIR\QtGui4.dll" FILE_ATTRIBUTE_HIDDEN
	SetFileAttributes "$INSTDIR\QtWebKit4.dll" FILE_ATTRIBUTE_HIDDEN
	SetFileAttributes "$INSTDIR\QtNetwork4.dll" FILE_ATTRIBUTE_HIDDEN
	SetFileAttributes "$INSTDIR\QtGui4.dll" FILE_ATTRIBUTE_HIDDEN
	SetFileAttributes "$INSTDIR\phonon4.dll" FILE_ATTRIBUTE_HIDDEN
	SetFileAttributes "$INSTDIR\mingwm10.dll" FILE_ATTRIBUTE_HIDDEN
	SetFileAttributes "$INSTDIR\libgcc_s_dw2-1.dll" FILE_ATTRIBUTE_HIDDEN
	SetFileAttributes "$INSTDIR\qscintilla2.dll" FILE_ATTRIBUTE_HIDDEN
	SetFileAttributes "$INSTDIR\phonon_backend" FILE_ATTRIBUTE_HIDDEN
	
	; Set Up Start Menu Entry and Desktop Short Cut
	
	; Shortcuts
	CreateShortCut "$DESKTOP\${APPNAMEANDVERSION}.lnk" "$INSTDIR\kiss.exe"
	CreateDirectory  "$SMPROGRAMS\${APPNAMEANDVERSION}"
	CreateShortCut "$SMPROGRAMS\${APPNAMEANDVERSION}\${APPNAMEANDVERSION}.lnk" "$INSTDIR\kiss.exe"


SectionEnd

Section "CBC V2 Driver"  CBC
  
	SetOutPath "$WINDIR\CBCV2Driver"
	
	File kiprcbc.inf
	
	Push "$WINDIR\CBCV2Driver" 
	;  -- the directory of the .inf file
	Push "$WINDIR\CBCV2Driver\kiprcbc.inf"
	;  -- the filepath of the .inf file (directory + filename)
	Push "USB\VID_03EB&PID_6119"
	;  -- the HID (Hardware ID) of your device
  
	Call InstallUpgradeDriver

SectionEnd  

Section -FinishSection

	WriteRegStr HKLM "Software\${APPNAMEANDVERSION}" "" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAMEANDVERSION}" "DisplayName" "${APPNAMEANDVERSION}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAMEANDVERSION}" "UninstallString" "$INSTDIR\uninstall.exe"
	WriteUninstaller "$INSTDIR\uninstall.exe"
	
SectionEnd


; Modern install component descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${KISSIDE} "KISS IDE with MinGW "
!insertmacro MUI_DESCRIPTION_TEXT ${CBC} "CBC v2 Driver"
!insertmacro MUI_FUNCTION_DESCRIPTION_END


;Uninstall section
Section Uninstall

	;Remove from registry...
	DeleteRegKey HKLM "SOFTWARE\${APPNAMEANDVERSION}"
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAMEANDVERSION}"

	; Delete self
	Delete "$INSTDIR\uninstall.exe"

	; Delete Desktop Short Cut and Start Menu Entry
	Delete "$DESKTOP\${APPNAMEANDVERSION}.lnk"
	Delete "$SMPROGRAMS\${APPNAMEANDVERSION}\${APPNAMEANDVERSION}.lnk"
  RMDir  "$SMPROGRAMS\${APPNAMEANDVERSION}"

	; Clean up KISS-C
	RMDir /r "$INSTDIR\"

SectionEnd

BrandingText "KISS Institute For Practical Robotics"

; eof