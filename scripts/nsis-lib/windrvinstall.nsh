;--------------------------------
;Driver Auto Install
;
; Written by Kuba Ober
; Copyright (c) 2004 Kuba Ober
;
; Permission is hereby granted, free of charge, to any person obtaining a 
; copy of this software and associated documentation files (the "Software"), 
; to deal in the Software without restriction, including without limitation 
; the rights to use, copy, modify, merge, publish, distribute, sublicense, 
; and/or sell copies of the Software, and to permit persons to whom the 
; Software is furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in 
; all copies or substantial portions of the Software.
; <
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
; FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
; DEALINGS IN THE SOFTWARE.
 
;
; U S A G E
;
; Push "c:\program files\yoursoftware\driver" 
;  -- the directory of the .inf file
; Push "c:\program files\yoursoftware\driver\driver.inf"
;  -- the filepath of the .inf file (directory + filename)
; Push "USB\VID_1234&PID_5678"
;  -- the HID (Hardware ID) of your device
; Call InstallUpgradeDriver
;
; Your driver (minimally the .inf and .sys files) should already by installed
; by your NSIS script.
;
; Typically, you would put the driver either in $INSTDIR or $INSTDIR\Driver
; It's up to you, of course.
;
; The driver (i.e. .inf, .sys and related files) must be present for the
; lifetime of your application, you shouldn't remove them after calling
; this function!
;
; You DON'T want to put the driver in any of system directories. Windows
; will do it when the device is first plugged in.
 
Function InstallUpgradeDriver
 
 Pop $R0 ; HID
 Pop $R1 ; INFPATH
 Pop $R2 ; INFDIR
 
 ; Get the Windows version
 Call GetWindowsVersion
 Pop $R3 ; Windows Version
 ;DetailPrint 'Windows Version: $R3'
 StrCmp $R3 '2000' lbl_upgrade
 StrCmp $R3 'XP' lbl_upgrade
 StrCmp $R3 '2003' lbl_upgrade
 DetailPrint "Windows $R3 doesn't support automatic driver updates."
 
 ; Upgrade the driver if the device is already plugged in
 Goto lbl_noupgrade
lbl_upgrade:
 System::Get '${sysUpdateDriverForPlugAndPlayDevices}'
 Pop $0
 StrCmp $0 'error' lbl_noapi
 DetailPrint "Updating the driver..."
 ; 0, HID, INFPATH, 0, 0
 Push $INSTDIR ; Otherwise this function will swallow it, dunno why
 System::Call '${sysUpdateDriverForPlugAndPlayDevices}?e (0, R0, R1, 0, 0) .r0'
 Pop $1 ; last error
 Pop $INSTDIR
 IntCmp $0 1 lbl_done
 IntCmp $1 ${ERROR_NO_SUCH_DEVINST} lbl_notplugged
 
 DetailPrint "Driver update has failed. ($R3:$0,$1)"
 Goto lbl_noupgrade
lbl_notplugged:
 DetailPrint "The device is not plugged in, cannot update the driver."
 Goto lbl_noupgrade
lbl_noapi:
 DetailPrint "Your Windows $R3 doesn't support driver updates."
 
lbl_noupgrade:
 ; Pre-install the driver
 System::Get '${sysSetupCopyOEMInf}'
 Pop $0
 StrCmp $0 'error' lbl_inoapi
 DetailPrint "Installing the driver..."
 ; INFPATH, INFDIR, SPOST_PATH, "", 0, 0, 0, 0<
 System::Call '${sysSetupCopyOEMInf}?e (R1, R2, ${SPOST_PATH}, 0, 0, 0, 0, 0) .r0'
 Pop $1 ; last error
 IntCmp $0 1 lbl_nodriver
 DetailPrint 'Driver pre-installation has failed with error #$1 ($R3)'
 Goto lbl_done
lbl_inoapi:
 DetailPrint "Your Windows $R3 doesn't support driver pre-installation."
lbl_nodriver:
lbl_done:
 
FunctionEnd