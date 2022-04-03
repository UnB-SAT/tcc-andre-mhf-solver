#!/bin/bash

cnfPackFileType=".cnf.zst";

# for all files.fileType
for file in $(find "./" -name "*$cnfPackFileType");
do
    # unpack
    zstd -d $file -f;
done