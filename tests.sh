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

red_color='\033[31m'
reset_color='\033[0m'
green_color='\033[32m'
funky_color='\033[34m'

failed=0
failed_lex=0
failed_syn=0
failed_int=0

for file in $files
do
    if [[ $file =~ (...)_([^_]*)_([0-9])(_input)?(_output)?.ifj$ ]]; then
        module_name="${BASH_REMATCH[1]}"
        test_name="${BASH_REMATCH[2]}"
        return_value="${BASH_REMATCH[3]}"
        input_flag="${BASH_REMATCH[4]}"
        output_flag="${BASH_REMATCH[5]}"
        echo -e "RUNNING $funky_color$test_name$reset_color for module $funky_color$module_name$reset_color"
        if [[ $output_flag = "_output" ]]; then
            value=$(<$file)
            if [[ $value =~ $outputs_regex ]]; then
                expected_output="${BASH_REMATCH[2]}"
            else
                echo -e "\t$red_color TEST SPECIFIED OUTPUT VALUE BUT I DID NOT FIND IT $reset_color"
            fi
        else
            expected_output=""
        fi

        if [[ $input_flag = "_input" ]]; then
            value=$(<$file)
            if [[ $value =~ $inputs_regex ]]; then
                expected_input="${BASH_REMATCH[2]}"
            else
                echo -e "\t$red_color TEST SPECIFIED INPUT VALUE BUT I DID NOT FIND IT $reset_color"
            fi
        else
            expected_input=""
        fi

        correct_return="yes"
        correct_output="yes"

        actual_output=""
        actual_output=$(echo $expected_input | ./release $file 2> /dev/null)
        actual_return_value=$?
        if [[ $actual_return_value != $return_value ]]; then
            correct_return="no"
        fi
        if [[ $actual_output != $expected_output ]]; then
            correct_output="no"
        fi

        if [[ $correct_return = "yes" ]] && [[ $correct_output = "yes" ]]; then
                echo -e "\t$green_color TEST $test_name PASSED!! $reset_color"
        else
                ((failed++))
                if [[ $module_name = "lex" ]]; then
                    ((failed_lex++))
                elif [[ $module_name = "syn" ]]; then
                    ((failed_syn++))
                elif [[ $module_name = "int" ]]; then
                    ((failed_int++))
                fi

                echo -e "\t$red_color TEST $test_name FAILED:"
                if [[ $correct_output != "yes" ]]; then
                    echo -e "\t\t expected output: '$expected_output', got: '$actual_output'$reset_color"
                fi
                if [[ $correct_return != "yes" ]]; then
                    echo -e "\t\t expected return value of $return_value, got $actual_return_value$reset_color"
                fi
        fi
    else
        echo -e "SKIPPING $funky_color$file$reset_color BECAUSE IT HAS A WRONG NAME"
    fi
done

echo ""

if [[ $failed -gt 0 ]]; then
    echo -e "$red_color  ## $failed TESTS HAVE FAILED!!! ##  $reset_color"
    echo -e "\t$red_color$failed_lex$reset_color in module LEX"
    echo -e "\t$red_color$failed_syn$reset_color in module SYN"
    echo -e "\t$red_color$failed_int$reset_color in module INT"
else
    echo -e"$green_color  ## ALL TESTS PASSED :^)) ##  $reset_color"
fi
