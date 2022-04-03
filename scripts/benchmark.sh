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

# TODO get fixed formulas - (1s - 1m - 10m - 20m - 30m+)
# TODO get tests for clasp, linge ling, glucose --limit 30m
# Get all tests sorted with maximum of totalTests
for file in $(find "./" -name "*$fileType" |sort -R |tail -$totalTests);
do 
    testName=${file/"./benchmarks/"/""};
    testName=${testName/".pddl/"/"-"};
    testName=${testName//"/"/"-"};
    testName=${testName/"$fileType"/""};

    testDir=$outputDir/$testName;
    mkdir $testDir;

    # Unpack
    zstd -d $file -o $testFileName -f &> /dev/null;

    # Run clasp
    {
        clasp $testFileName --time-limit=$timeLimit;
        claspReturn=$?;
    } &> $testDir/clasp.txt;
    
    # Run solver
    {
        ./prog $testFileName $timeLimit $outputVariableName;
        solverReturn=$?;
    } &> $testDir/solver.txt;

    # Crosscheck if SAT
    if [ $solverReturn = $claspReturn ] && [$solverReturn = $satValue];
    then
        {
            clasp $outputVariableName --time-limit=$timeLimit;
        } &> $testDir/crossCheck.txt;
    fi
done