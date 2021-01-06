
# FreeDOS USB CDROM detection


Free DOS Compact flash image with CDROM and USB CDROM detection script. Script will run at boot time allowing user to detect several types of CDROM/USB CDROM.
 It has emm386/himem support. It was installed by default in several types of boxes that came with compact flash so users can install a windows variant or to run their old MSDOS software.

# How To

1. Power up your box and connect the USB CDROM. If you have an external USB CDROM that require aditional power from a power supply make sure that you first connect the power cord before pluging the USB cable on motherboard connector.

2. When system boots from Compact Flash you will see a small message : F1 ... DOS . Pressing enter will boot FreeDOS from compact flash.

3. After boot a menu will apear with multiple options numbered from 1 to 9. This let you choose and try multiple drivers for your USB CDROM. For all EPIA motherboards and motherboards with Intel chipset first option should work best. 0 option will allow you to skip USB CDROM install.

4. Press the key number for the option that is suited best for your system. Please wait till autodetection finish. At the end a message will show telling you which drive letter was assigned to your USB CDROM.

5. Before starting installing windows you should partition and format your hard drive. The simplest method is to write at dos prompt the following commands:

c:\>fdisk /auto 2

This command will automaticaly partition your second disk making 1 primary FAT16 of 2GB and an extended partition.
You might need to reboot so DOS can see your new partitions. For more advanced options you can use FDISK.EXE without any parameters or PART.EXE as an altenative disk partitioning. Also make sure that you create FAT16 partitions so FreeDOS kernel will be able to access them.

c:\>format d: /u

This command will format the primary partition on the second disk preparing the disk for windows setup. DOS format it's a little slow and will take some time to format the whole disk.

c:\>mkdir d:\temp

This command will create a directory named temp on the hard drive that will be used for temporary windows setup files.

It's advisable that you copy the contents of Windows CDROM to your hard drive because if something goes wrong during windows setup you will have the windows setup files on your hard drive and you won't be forced to use USB CDROM.

c:\>mkdir d:\winsetup

This command will create a directory named winsetup on the hard drive on which you will copy windows setup files.

To copy windows setup files execute (we will consider that USB CDROM was assigned drive letter F:) :

c:\>xcopy f:\ d:\winsetup\

8. To start the windows install you'll have to run setup.exe (for windows 95/98/ME) or winnt.exe (for Windows 2000/XP).
(We will consider that you copied windows files to winsetup directory on your hard drive)

For Windows 95/98/ME run:

c:\> d:
d:\> cd winsetup
d:\winsetup\>setup.exe /nm /is /t:d:\temp

For Windows 2000/XP run:

c:\> d:
d:\> cd winsetup\i386
d:\winsetup\i368\>winnt.exe /t:d:\temp

When installer prompts you to select destination path make sure that you specify D: (hard drive) as target (eg. d:\windows).

The compact flash image which can be written to flash disk using dd command under linux: freedos-cf64mb.img which contains all USB drivers and autodetection scripts.

For users that want to create their own FreeDOS or MS DOS compact flash boot disk:

The USBCD drivers directory which should be copied to root of compact flash.

The config.sys and autoexec.bat files for FreeDOS (freedos-configs.tgz) or for MS-DOS (msdos-configs.tgz). This files should be copied to root of compact flash.

Also MSDOS users should make sure that they have mscdex.exe and himem.sys on C:\DOS\ on compact flash.