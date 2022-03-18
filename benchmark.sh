#!/bin/bash

satValue=20;
timeLimit=5;
outputVariableName="./solverOutput.cnf";
totalTests=1;
testFileName="./test.cnf";
logFileName="./log.csv";

touch $logFileName;
> $logFileName;
echo "File path, Clasp runtime, Solver runtime, Clasp return, Solver return, Clasp new return" >> $logFileName;

# Get all tests sorted with maximum of totalTests
for file in $(find "./" -name "*cnf.zst" |sort -R |tail -$totalTests);
do
    # Unpack
    zstd -d $file -o $testFileName -f &> /dev/null;

    # Run clasp
    claspStartTime=$(date +%s.%N);
    {
        clasp $testFileName --time-limit=$timeLimit;
        claspReturn=$?;
    } &> /dev/null;
    claspEndTime=$(date +%s.%N);
    
    # Run solver
    {
        ./prog $testFileName $timeLimit $outputVariableName;
        solverReturn=$?;
    } &> /dev/null;
    solverEndTime=$(date +%s.%N);

    if [ $solverReturn = $claspReturn ] && [$solverReturn = $satValue];
    then
        {
            clasp $outputVariableName --time-limit=$timeLimit;
            claspNewReturn=$?;
        } &> /dev/null;
    else
        claspNewReturn="";
    fi

    echo "$file, $( echo "$claspEndTime - $claspStartTime" | bc -l ), $( echo "$solverEndTime - $claspEndTime" | bc -l ), $claspReturn, $solverReturn, $claspNewReturn" >> $logFileName;
done