#!/bin/bash

# Include the 'dynamic package' standard prolog
. /imedia/packages/dynamic-package-prolog

#------------- Define your 'PackageDescription' and all pre-build operations here --------------
PackageVersion="1.0.3"
PackageCategory="System Tools"
PackageTarget="common"
PackageArch="i386"
PackageDescription="A file compression utility"
PackageDetails="Bzip2 is a freely available, patent-free, high-quality data compressor
that uses the same command line flags as gzip."

mkdir -p 				bin
cp -a /bin/bunzip2			bin
cp -a /bin/bzcat			bin
cp -a /bin/bzcmp			bin
cp -a /bin/bzdiff			bin
cp -a /bin/bzgrep			bin
cp -a /bin/bzip2			bin
cp -a /bin/bzip2recover			bin
cp -a /bin/bzless			bin
cp -a /bin/bzmore			bin
chown -R root:root			bin

mkdir -p				lib
cp -a /lib/libbz2.so*			lib
chown -R root:root			lib


#----------- End your 'PackageDescription' definition and all pre-build operations here --------

# Include the 'dynamic package' standard epilog
. /imedia/packages/dynamic-package-epilog
