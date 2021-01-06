# This is what I used to build the new tarball.list for an old loki installer
# Nicu Pavel <npavel@ituner.com>
tar -tzvof CivCTP-data.tar.gz | perl -e 'while (<>){ $_=~ /^.*?\s.*?\s+(\d+)\s.*?\s.*?\s(.*?)$/ig;print "$1\t$2\n";}' > tarball.list