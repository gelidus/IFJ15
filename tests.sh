#! /bin/bash

# vzor na nazev souboru
filename="lex_uvozovky-ve-stringu_1.ifj"

echo "##### FANCY FUCKING TESTS 2.0 #####"

files="programs/*"

# zbuildime
make &> /dev/null
if [[ $? -ne 0 ]]; then
    echo -e "\033[31m"
    echo "ANI TO NEJDE PRELOZIT PROC SEM VUBEC CHODIS"
    echo -e "\033[0m"
    exit
fi

outputs_regex='(/\*@outputs
\")([^(\*/)]*)\"
\*/
'

inputs_regex='(/\*@inputs
\")([^(\*/)]*)\"
\*/
'

for file in $files
do
    if [[ $file =~ ..._([^_]*)_([0-9])(_input)?(_output)?.ifj$ ]]; then
        test_name="${BASH_REMATCH[1]}"
        return_value="${BASH_REMATCH[2]}"
        input_flag="${BASH_REMATCH[3]}"
        output_flag="${BASH_REMATCH[4]}"
        # echo "running $test_name with expected return $return_value, input: $input_flag, output: $output_flag"
        if [[ $output_flag = "_output" ]]; then
            value=$(<$file)
            if [[ $value =~ $outputs_regex ]]; then
                expected_output="${BASH_REMATCH[2]}"
            else
                echo -e "\033[31m"
                echo "TEST SPECIFIED OUTPUT VALUE BUT I DID NOT FIND IT"
                echo -e "\033[0m"
            fi
        else
            expected_output=""
        fi

        if [[ $input_flag = "_input" ]]; then
            value=$(<$file)
            if [[ $value =~ $inputs_regex ]]; then
                expected_input="${BASH_REMATCH[2]}"
            else
                echo -e "\033[31m"
                echo "TEST SPECIFIED INPUT VALUE BUT I DID NOT FIND IT"
                echo -e "\033[0m"
            fi
        else
            expected_input=""
        fi

        correct_return="yes"
        correct_output="yes"

        actual_output=""
        actual_output=$(echo $expected_input | ./release $file)
        actual_return_value=$?
        if [[ $actual_return_value != $return_value ]]; then
            correct_return="no"
        fi
        if [[ $actual_output != $expected_output ]]; then
            correct_output="no"
        fi

        if [[ $correct_return = "yes" ]] && [[ $correct_output = "yes" ]]; then
                echo -e "\033[32m"
                echo -e "TEST $test_name PASSED!!"
                echo -e "\033[0m"
        else
                echo -e "\033[31m"
                echo "TEST $test_name FAILED:"
                if [[ $correct_output != "yes" ]]; then
                    echo "expected output: '$expected_output', got: '$actual_output'"
                fi
                if [[ $correct_return != "yes" ]]; then
                    echo "expected return value of $return_value, got $actual_return_value"
                fi
                echo -e "\033[0m"
        fi
    else
        echo "SKIPPING $file BECAUSE IT HAS A WRONG NAME"
    fi
done
