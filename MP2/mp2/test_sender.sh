#/bin/bash
testnum=0
function test(){
    local file=$1
    local bytes=$2
    testnum=$((testnum+1));
    echo -e "\e[33m==Test ${testnum}==\e[0m"
    ./reliable_sender 127.0.0.1 1234 $file $bytes
    echo -e "\e[32m==Passed==\e[0m"
}

test msg1.txt 100
test msg2.txt 100
test msg3.txt 45
test msg3.txt 1300
test msg3.txt 6000
test msg4.txt 50000
test msg4.txt 10000000000
