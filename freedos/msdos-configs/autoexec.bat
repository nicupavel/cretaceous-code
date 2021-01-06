@echo off
PATH=C:\DOS
IF "%CONFIG%"==ABORT GOTO END
echo Loading CD-ROM extensions ...
c:\dos\mscdex.exe /d:USBCD
GOTO END
:ERROR
echo No drive found ! Try unpluging all other USB devices except USBCDROM !
echo Reboot and try other autodetection modes.
echo If nothing works try editing c:\config.sys and instead of USBCD.SYS
echo use USB_CD.SYS driver.
GOTO END
:END

