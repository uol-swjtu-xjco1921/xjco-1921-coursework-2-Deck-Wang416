# Clean up and recompile
make clean
make all

# Keep track of score and total count of tests
total=0
score=0

run_unit_test () 
    { 
    message=$($1 $2 $3) 
    echo "Test case: $1 $2 $3" 
    $1 $2 $3 > null 
    out=$?
    total=$(( $total+2 ))

    if [ $out == $4 ]
    then
        echo "PASSED [return value]: expected output value \"$4\" got $out"
        (( score++ ))
    else
        echo "FAILED [return value]: expected output value \"$4\" got $out"
    fi

    if [ "$message" == "$5" ] 
    then
        echo "PASSED [output message]: expected printout message \"$5\" got \"$message\""
        (( score++ ))
    else
        echo "FAILED [output message]: expected printout message \"$5\" got \"$message\""
    fi
    } 

run_shortest_path_test () 
{
    expected_output=$1
    total=$(( $total+1 ))

    # Use expect to simulate user inputs
    output=$(expect -c "
    spawn ./main test_data/shortest_path.map
    send \"A\r\"
    send \"1601125096\r\"
    send \"1187324692\r\"
    expect eof
    ")

    # Compare the output to the expected output
    if [[ "$output" == *"$expected_output"* ]]
    then
        echo "PASSED [interactive test]: printout matches expected printout"
        (( score++ ))
    else
        echo "FAILED [interactive test]: printout does not match expected printout"
    fi
}

run_fastest_path_test () 
{
    expected_output=$1
    total=$(( $total+1 ))

    # Use expect to simulate user inputs
    output=$(expect -c "
    spawn ./main test_data/fastest_path.map
    send \"B\r\"
    send \"21069423\r\"
    send \"244449810\r\"
    expect eof
    ")

    # Compare the output to the expected output
    if [[ "$output" == *"$expected_output"* ]]
    then
        echo "PASSED [interactive test]: printout matches expected printout"
        (( score++ ))
    else
        echo "FAILED [interactive test]: printout does not match expected printout"
    fi
}

run_shortest_path_with_locations_test () 
{
    expected_output=$1
    total=$(( $total+1 ))

    # Use expect to simulate user inputs
    output=$(expect -c "
    spawn ./main test_data/path_with_locations.map
    send \"C\r\"
    send \"1601125096\r\"
    send \"244449810\r\"
    send \"L\r\"
    send \"21069423\r\"
    send \"Y\r\"
    send \"1187324692\r\"
    send \"N\r\"
    expect eof
    ")

    # Compare the output to the expected output
    if [[ "$output" == *"$expected_output"* ]]
    then
        echo "PASSED [interactive test]: printout matches expected printout"
        (( score++ ))
    else
        echo "FAILED [interactive test]: printout does not match expected printout"
    fi
}

run_shortest_path_with_pois_test () 
{
    expected_output=$1
    total=$(( $total+1 ))

    # Use expect to simulate user inputs
    output=$(expect -c "
    spawn ./main test_data/path_with_pois.map
    send \"C\r\"
    send \"920423600\r\"
    send \"921304562\r\"
    send \"P\r\"
    send \"8\r\"
    send \"N\r\"
    expect eof
    ")

    # Compare the output to the expected output
    if [[ "$output" == *"$expected_output"* ]]
    then
        echo "PASSED [interactive test]: printout matches expected printout"
        (( score++ ))
    else
        echo "FAILED [interactive test]: printout does not match expected printout"
    fi
}

run_incorrect_input_type_test () 
{
    expected_output=$1
    total=$(( $total+1 ))

    # Use expect to simulate user inputs
    output=$(expect -c "
    spawn ./main test_data/shortest_path.map
    send \"A\r\"
    send \"1701812266.0416\r\"
    expect eof
    ")

    # Compare the output to the expected output
    if [[ "$output" == *"$expected_output"* ]]
    then
        echo "PASSED [interactive test]: printout matches expected printout"
        (( score++ ))
    else
        echo "FAILED [interactive test]: printout does not match expected printout"
    fi
}

run_extra_characters_test () 
{
    expected_output=$1
    total=$(( $total+1 ))

    # Use expect to simulate user inputs
    output=$(expect -c "
    spawn ./main test_data/shortest_path.map
    send \"A\r\"
    send \"1701812266leeds\r\"
    expect eof
    ")

    # Compare the output to the expected output
    if [[ "$output" == *"$expected_output"* ]]
    then
        echo "PASSED [interactive test]: printout matches expected printout"
        (( score++ ))
    else
        echo "FAILED [interactive test]: printout does not match expected printout"
    fi
}

run_existence_detection_test ()
{
    expected_output=$1
    total=$(( $total+1 ))

    # Use expect to simulate user inputs
    output=$(expect -c "
    spawn ./main test_data/shortest_path.map
    send \"A\r\"
    send \"1601124953\r\"
    expect eof
    ")

    # Compare the output to the expected output
    if [[ "$output" == *"$expected_output"* ]]
    then
        echo "PASSED [interactive test]: printout matches expected printout"
        (( score++ ))
    else
        echo "FAILED [interactive test]: printout does not match expected printout"
    fi
}

run_same_start_and_end_nodes_test ()
{
    expected_output=$1
    total=$(( $total+1 ))

    # Use expect to simulate user inputs
    output=$(expect -c "
    spawn ./main test_data/shortest_path.map
    send \"A\r\"
    send \"920423604\r\"
    send \"920423604\r\"
    expect eof
    ")

    # Compare the output to the expected output
    if [[ "$output" == *"$expected_output"* ]]
    then
        echo "PASSED [interactive test]: printout matches expected printout"
        (( score++ ))
    else
        echo "FAILED [interactive test]: printout does not match expected printout"
    fi
}

run_value_out_of_range_test ()
{
    expected_output=$1
    total=$(( $total+1 ))

    # Use expect to simulate user inputs
    output=$(expect -c "
    spawn ./main test_data/shortest_path.map
    send \"D\r\"
    send \"2\r\"
    send \"1187324692\r\"
    send \"lat\r\"
    send \"59.110033\r\"
    expect eof
    ")

    # Compare the output to the expected output
    if [[ "$output" == *"$expected_output"* ]]
    then
        echo "PASSED [interactive test]: printout matches expected printout"
        (( score++ ))
    else
        echo "FAILED [interactive test]: printout does not match expected printout"
    fi
}

run_option_out_of_range_test ()
{
    expected_output=$1
    total=$(( $total+1 ))

    # Use expect to simulate user inputs
    output=$(expect -c "
    spawn ./main test_data/shortest_path.map
    send \"E\r\"
    expect eof
    ")

    # Compare the output to the expected output
    if [[ "$output" == *"$expected_output"* ]]
    then
        echo "PASSED [interactive test]: printout matches expected printout"
        (( score++ ))
    else
        echo "FAILED [interactive test]: printout does not match expected printout"
    fi
}

run_missing_speed_attributes_test ()
{
    expected_output=$1
    total=$(( $total+1 ))

    # Use expect to simulate user inputs
    output=$(expect -c "
    spawn ./main test_data/shortest_path.map
    send \"B\r\"
    send \"21069423\r\"
    send \"244449810\r\"
    expect eof
    ")

    # Compare the output to the expected output
    if [[ "$output" == *"$expected_output"* ]]
    then
        echo "PASSED [interactive test]: printout matches expected printout"
        (( score++ ))
    else
        echo "FAILED [interactive test]: printout does not match expected printout"
    fi
}

EXES=(main)

for testExecutable in ${EXES[@]}
do 
    path="test_data/"
    echo "-------------- UNIT TESTING $testExecutable --------------"

    echo ""
    echo "Usage"
    run_unit_test ./$testExecutable "" "" 0 "Usage: ./$testExecutable inputFile.map"

    echo ""
    echo "Bad Arguments"
    run_unit_test ./$testExecutable "1" "2" 1 "ERROR: Bad Argument Count"

    echo ""
    echo "Bad Filename"
    run_unit_test ./$testExecutable "1" "" 2 "ERROR: Bad File Name (1)"

    echo ""
    echo "Bad Attributes"
    filename="bad_attributes.map"
    full_path=$path$filename
    run_unit_test ./$testExecutable $full_path "" 5 "ERROR: Bad Attributes ($full_path)"

    echo ""
    echo "Bad Labels"
    filename="bad_labels.map"
    full_path=$path$filename
    run_unit_test ./$testExecutable $full_path "" 6 "ERROR: Bad Labels ($full_path)"

done

for testExecutable in ${EXES[@]}
do
    echo "-------------- INTERGRATION TESTING $testExecutable --------------"

    echo ""
    echo "The Shortest Path Test"
    expected_output="1601125096 -> 1601125103 -> 1601125136 -> 1601125109 -> 21545939 -> 1601125172 -> 288958054 -> 54070620 -> 21545938 -> 288958059 -> 52905141 -> 1668868122 -> 21545937 -> 54070579 -> 354734667 -> 354734674 -> 54060543 -> 1377220131 -> 54060603 -> 54060534 -> 1668111629 -> 1668111630 -> 54060530 -> 1668111631 -> 54060527 -> 54060524 -> 984231425 -> 1187324690 -> 1187324665 -> 1187324678 -> 1187324676 -> 1187324692"
    run_shortest_path_test "$expected_output"

    echo ""
    echo "The Fastest Path Test"
    expected_output="21069423 -> 247293286 -> 244449810"
    run_fastest_path_test "$expected_output"

    echo ""
    echo "The Shortest Path with Given Locations Test"
    expected_output="1601125103 -> 1601125136 -> 1160269594 -> 1160269542 -> 1659456404 -> -2053181555 -> -2053181562 -> 1873019789 -> 54070615 -> 454256160 -> 21069422 -> 247293248 -> 21069423 -> 247293248 -> 21069422 -> 247956997 -> 21069421 -> 31004286 -> -1887621620 -> 354734671 -> 1644818713 -> 1644818714 -> 1644818715 -> 1644818711 -> -2470 -> 57620248 -> -2474 -> 57620252 -> -2420 -> 1187324679 -> 1187324666 -> -2560 -> 1187324670 -> 1187324682 -> 1187324692 -> 1187324682 -> 1187324670 -> -2560 -> 1187324666 -> 1187324679 -> -2420 -> 57620252 -> -2474 -> 57620248 -> -2470 -> 1644818711 -> 1644818715 -> 1644818714 -> 1644818713 -> 354734671 -> -1887621620 -> 31004286 -> 21069421 -> 247956997 -> 21069422 -> 247293248 -> 21069423 -> 288953461 -> 244449810"
    run_shortest_path_with_locations_test "$expected_output"

    echo ""
    echo "The Shortest Path with Given POIs Test"
    expected_output="920423604 -> 49510397 -> 49509575 -> 31004238 -> 1643862532 -> 31004237 -> 31004236 -> 31004235 -> 31004234 -> 31004233 -> 48945804 -> 7270052 -> 7270055 -> 7270056 -> 7270058 -> 50773839 -> 50773846 -> 1150101889 -> 50773842 -> 1150101900 -> 1150101859 -> 41337027 -> 41337031 -> 247957031 -> 247957030 -> 319432030 -> 247293201 -> 1160269686 -> 1160269507 -> 247293200 -> -2450 -> 247293217 -> -2364 -> 247293216 -> 985084904 -> 1160269683 -> 247957021 -> 1160269664 -> 247957023 -> 247957019 -> 247957018 -> 247956995 -> -2498 -> 54070505 -> 31004285 -> 1644818711 -> 1644818715 -> -1887621628 -> 1644818715 -> 1644818711 -> 31004285 -> 54070505 -> -2498 -> 247956995 -> 247957018 -> 247957019 -> 247957023 -> 1160269664 -> 247957021 -> 1160269683 -> 985084904 -> 247293216 -> -2364 -> 247293217 -> -2450 -> 247293200 -> 1160269507 -> 1160269686 -> 247293201 -> 319432030 -> 247957030 -> 247957031 -> 41337031 -> 41337027 -> 41337023 -> 1160269578 -> 41337020 -> 41337014 -> 921304562"
    run_shortest_path_with_pois_test "$expected_output"

    echo ""
    echo "Incorrect Input Type Test"
    expected_output="Usage: Enter An Integer"
    run_incorrect_input_type_test "$expected_output"

    echo ""
    echo "Extra Characters Test"
    expected_output="Usage: Enter An Integer with No Extra Characters"
    run_extra_characters_test "$expected_output"

    echo ""
    echo "Existence Detection Test"
    expected_output="Usage: Enter An Existing Node ID"
    run_existence_detection_test "$expected_output"

    echo ""
    echo "Same Start and End Nodes Test"
    expected_output="Usage: Enter An End Node ID Different from The Start Node ID"
    run_same_start_and_end_nodes_test "$expected_output"

    echo ""
    echo "Value Out of Range Test"
    expected_output="Usage: Enter A Valid Value between 53.801600 and 53.811000"
    run_value_out_of_range_test "$expected_output"

    echo ""
    echo "Option Out of Range Test"
    expected_output="Usage: Enter Either A, B or C"
    run_option_out_of_range_test "$expected_output"

    echo ""
    echo "Missing Speed Attributes Test"
    expected_output="Add Speed Limits to The Links"
    run_missing_speed_attributes_test "$expected_output"

done

echo "------------------------------------------------------------------------------"
echo "You passed $score out of $total"
echo "------------------------------------------------------------------------------"

# Clean up
rm null
make clean