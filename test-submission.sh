#!/bin/bash

echoerr()
{
    echo "$@" >&2
}

get_data_number()
{
    precision=$1
    local_num_data=$(cat test-data-$precision.txt | wc -l)
    echo $((1 + $RANDOM % $local_num_data))
}

get_seed_from_data()
{
    precision=$1
    echo $(head -n $2 test-data-$precision.txt | tail -n 1 | cut -d ' ' -f1)
}

get_checksum_from_data()
{
    precision=$1
    echo $(head -n $2 test-data-$precision.txt | tail -n 1 | cut -d ' ' -f2)
}

TEST1="1. Test single precision 2000 flops with 1 thread                             "
TEST1_ARGS="1 flops single 2000 1"
TEST2="2. Test single precision 2000 flops with 2 thread                             "
TEST2_ARGS="2 flops single 2000 2"
TEST3="3. Test single precision 2000 flops with 4 thread                             "
TEST3_ARGS="3 flops single 2000 4"
TEST4="4. Test single precision 2000 flops with 8 thread                             "
TEST4_ARGS="4 flops single 2000 8"
TEST5="5. Test double precision 2000 flops with 1 thread                             "
TEST5_ARGS="5 flops double 2000 1"
TEST6="6. Test double precision 2000 flops with 2 thread                             "
TEST6_ARGS="6 flops double 2000 2"
TEST7="7. Test double precision 2000 flops with 4 thread                             "
TEST7_ARGS="7 flops double 2000 4"
TEST8="8. Test double precision 2000 flops with 8 thread                             "
TEST8_ARGS="8 flops double 2000 8"
TEST9="9. Test double precision 2000x2000 matrix multiplication with 1 thread        "
TEST9_ARGS="9 matrix double 2000 1"
TEST10="10. Test double precision 2000x2000 matrix multiplication with 2 thread      "
TEST10_ARGS="10 matrix double 2000 2"
TEST11="11. Test double precision 2000x2000 matrix multiplication with 4 threads     "
TEST11_ARGS="11 matrix double 2000 4"
TEST12="12. Test double precision 2000x2000 matrix multiplication with 8 threads     "
TEST12_ARGS="12 matrix double 2000 8"

NUM_TESTS=12

STATUS=0

TEST()
{
    local testnum=$1
    local operation=$2
    local precision=$3
    local size=$4
    local threads=$5

    if [ ! -f cpubench ]
    then
        local var="TEST$testnum"
        local msg1="${!var} failed!"
        local msg2= "*** cpubench binary is missing ***"
        echo -e "$msg1\n$msg2" 
        STATUS=2
    else
        data_number=$(get_data_number $precision)
        seed=$(get_seed_from_data $precision $data_number)
        checksum=0
        if [ "$operation" == "matrix" ]
        then
            checksum=$(get_checksum_from_data $precision $data_number)
        fi
        
        cmd="./cpubench $seed $operation $precision $size $threads false"
        eval $cmd &> cpubench.log
        
        local rc=$(cat cpubench.log | tail -n 1 | grep "checksum" | cut -d ' ' -f8 | cut -d '=' -f2)
        if [ "$rc" == "" ]
        then
            local rc=$(($checksum + 1))
        fi
        
        if [ $rc -eq $checksum ]
        then
            local var="TEST$testnum"
            local msg1="${!var} passed!"
            local msg2="*** Test $testnum run log ***"
            local msg3="./cpubench $seed $operation $precision $size $threads false"
            local msg4=$(cat cpubench.log)
            local msg5="*** Run successful ***"
            echo -e "$msg1\n$msg2\n$msg3\n$msg4\n$msg5" 
        else
            local var="TEST$testnum"
            local msg1="${!var} failed!"
            local msg2="*** Test $testnum run log ***"
            local msg3="./cpubench $seed $operation $precision $size $threads false"
            local msg4=$(cat cpubench.log)
            local msg5="*** Expected checksum = $checksum ***"
            echo -e "$msg1\n$msg2\n$msg3\n$msg4\n$msg5"
            STATUS=1
        fi
    fi
}

HOW_TO_USE="HOW TO USE: bash .github/workflows/test-submission.sh [<check number> | list | all]"

if [ $# -ne 1 ]
then
    echoerr "$HOW_TO_USE"
    exit 1
fi

arg1=$1

if [ "$arg1" == "list" ]
then
    echo "List of available checks:"
    for((i=1;i<=$NUM_TESTS;i++))
    do
        var="TEST$i"
        echo "${!var}"
    done
    exit 0
fi

if [ "$arg1" == "all" ]
then
    for((i=1;i<=$NUM_TESTS;i++))
    do
        var="TEST${i}_ARGS"
        cmd="TEST ${!var}"
        eval $cmd
    done
    
    if [ $STATUS -ne 0 ]
    then
        exit 1
    fi
    
    exit 0
fi

if [ $arg1 -eq $arg1 ]
then
    var="TEST${i}_ARGS"
    cmd="TEST ${!var}"
    eval $cmd
    
    if [ $STATUS -ne 0 ]
    then
        exit 1
    fi

    exit 0
else
    echoerr "$HOW_TO_USE"
    exit 2
fi

exit 0