@echo off
PATH=C:\FDOS\BIN
if [%config%]==[0] GOTO END
echo Loading CD-ROM extensions ...
c:\fdos\bin\shsucdx.exe /d:USBCD
:begin
if [%cdrom%]==[NONE] goto error
GOTO END
:error
echo No drive found ! Try unpluging all other USB devices except USBCDROM !
echo Reboot and try other autodetection modes.
echo If nothing works try editing c:\config.sys and instead of USBCD.SYS
echo use USB_CD.SYS driver.
GOTO END
:END

