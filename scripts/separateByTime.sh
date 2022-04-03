#!/bin/bash

cppFile="scripts/separateByTime.cpp";
cppOutFile="separateByTime";

cnfClaspResultFileType=".cnf.clasp";

g++ $cppFile -o $cppOutFile;

for file in $(find "./" -name "*$cnfClaspResultFileType");
do
    ./$cppOutFile $file;
done