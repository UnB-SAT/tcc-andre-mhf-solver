#!/bin/bash

timeLimit=1800;

for file in $(find "./" -name "*.cnf");
do
    echo Solving $file

    clasp $file --time-limit=$timeLimit &> output/$file.clasp;

    timeout $timeLimit ~/glucose-syrup-4.1/simp/glucose $file &> output/$file.glucose;
done
