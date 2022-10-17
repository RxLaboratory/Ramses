#"!/bin/bash"

# Installs the ramses server in a "ramses" subdir of the destination

# DO NOT USE THIS SCRIPT TO INSTALL THE SERVER IN PRODUCTION
# as it does copy developmet scripts which are not secured (the "dev" folder)

destination=/var/www/html

# php
src=../src/
install=../src/install
update=../src/update
dev=../src/dev
# convert to absolute paths
src=$(cd "$src"; pwd)
install=$(cd "$install"; pwd)
update=$(cd "$update"; pwd)
dev=$(cd "$dev"; pwd)

# create
rm -r -f "$destination/ramses"
mkdir "$destination/ramses"
mkdir "$destination/ramses/install"
mkdir "$destination/ramses/update"
mkdir "$destination/ramses/dev"

for file in $install/*; do
    ln -s -t "$destination/ramses/install/" "$file"
    echo "Linked $file"
done

for file in $update/*; do
    ln -s -t "$destination/ramses/update/" "$file"
    echo "Linked $file"
done

for file in $dev/*; do
    ln -s -t "$destination/ramses/dev/" "$file"
    echo "Linked $file"
done

for file in $src/*.php; do
    ln -s -t "$destination/ramses" "$file"
    echo "Linked $file"
done

echo "Done!"