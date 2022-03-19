#!/bin/bash

satValue=20;
timeLimit=3;
totalTests=3;
fileType=".cnf.zst";
outputVariableName="./solverOutput.cnf";
testFileName="./test.cnf";
outputDir="./output";

# New output everytime
rm -rf $outputDir;
mkdir $outputDir;

# Get all tests sorted with maximum of totalTests
for file in $(find "./" -name "*$fileType" |sort -R |tail -$totalTests);
do
    testDir=$outputDir/$(basename $file $fileType);
    mkdir $testDir;
    # Unpack
    zstd -d $file -o $testFileName -f &> /dev/null;

    # Run clasp
    {
        clasp $testFileName --time-limit=$timeLimit;
        claspReturn=$?;
    } &> $testDir/clasp;
    
    # Run solver
    {
        ./prog $testFileName $timeLimit $outputVariableName;
        solverReturn=$?;
    } &> $testDir/solver;

    # Crosscheck if SAT
    if [ $solverReturn = $claspReturn ] && [$solverReturn = $satValue];
    then
        {
            clasp $outputVariableName --time-limit=$timeLimit;
        } &> $testDir/crossCheck;
    fi
done