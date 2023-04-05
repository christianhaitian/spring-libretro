#!/bin/bash

if [[ "$1" == "clean" ]]; then
  make clean
  rm *.so
  echo "Directory has been cleaned of created .so files."
  exit 0
fi

if [[ ! -f "spring_libretro.so" ]]; then
  make
  strip spring_libretro.so
fi

for info in *.info
do
     if [[ $info == *"copy"* ]] || [[ $info == "spring_libretro.info" ]]; then
       echo "Skipping the $info file"
       sleep 1
     else
       filename=$(basename $info .info)
       cp spring_libretro.so $filename.so
       if [[ $? != "0" ]]; then
        echo " "
        echo "There was an error while creating a so file for $info.  Stopping here."
        exit 1
       fi
       echo "Created $filename.so"
     fi
done
