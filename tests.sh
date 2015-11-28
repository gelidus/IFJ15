#! /bin/bash

echo "##### FANCY FUCKING TESTS #####"

files="programs/*"
# zbuildime
make &> /dev/null

for file in $files
do
    if [[ $file =~ test_._(...)_.* ]]; then
        result="${BASH_REMATCH[1]}"
        if [[ $result == "syn" ]]; then
            expected=2
        fi

        if [[ $result == "sem" ]]; then
            expected=3
        fi

        if [[ $result == "lex" ]]; then
            expected=1
        fi
    else
        expected=0
    fi
    echo "abot to test $file"
    ./release $file &> /dev/null
    actual=$?
    if [[ $actual -ne $expected ]]; then
        echo "TEST $file FAILED: result was $expected, actual was $actual"
    else
        echo "TEST $file PASSED!!"
    fi

done
