# mediapack - self extracting archive creator

    mediapack is a self-extracting archive creator used on iMedia Linux systems. It creates archives with a BASH stub which can be executed to extract the contents, adds information like: description, files inside archive, author and has the ability to run a script after self-unpacking.
     On iMedia Linux the mediapack is used to create self-extracting software packages or updates packages.


## Standalone Usage (mediapack)

	Usage: ./mediapck [params] package_source package_dest target_dir package_descr [startup_script] [args] 
	params can be one of those : 
	--version : Print out mediapck version number and exit 
	--bzip2 : Compress using bzip2 instead of gzip
	--nocomp : Do not compress the data 
	--notemp : The archive will create archive_dir in the current directory and uncompress in ./archive_dir 
	--nox11 : Disable automatic spawn of a xterm 
	--nowait : Do not wait for user input after executing embedded program from an xterm 
	--lsm file : LSM file describing the package Do not forget to give a fully qualified startup script name (i.e. with a ./ prefix if inside the archive).


## Package Manager Usage

    mediapck can be used to create software packages which can be used to install software on any linux distribution.
 For this there are some additional files needed which you will find on the archive below. To create a software package you need to define a build.spec file 
(like a package spec from rpm file for example) which contains description of the package and what files should be added to the archive. 
The example include is a build.spec for a package that installs bzip2 utilities.


Simply execute the build.spec file and the package will be created. PackageVersion, PackageCategory, PackageTarget etc, are used on iMedia Linux and written on .lsm file (use package_name -lsm to show it).
These can be changed to suit your needs.

The dynamic-package-prolog and dynamic-package-epilog are 2 scripts, the first one prepares the package tree and the seconds builds the package/self-extracting archive.
You will also need a file named install that will be automatically executed after the archive was extracted (if you use -target dir when executing the package this file is not used).
 An example file is given below that is used to move the contents of the archive from the temporary folder to the / folder.

	#!/bin/bash 
	rm $0 echo "Performing Install ..." 
	cp -Raf * / 
	cd .. 
	echo "Done..."

## Self-Extracting archive Options

You can execute the self-extracting archive/package with the following options:

	-info Print info : title, default target directory, embedded script ... 
	-lsm Print embedded lsm entry 
	-list Print the list of files in the archive 
	-check Checks integrity of the archive 
	-confirm Ask before running embedded script 
	-keep Do not erase target directory after running embedded script
	-target NewDirectory Extract in NewDirectory
