#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0

RED='\033[0;31m'
Green='\033[0;32m'
NC='\033[0m' # No Color

file2=(1.txt t2.txt)
file3=('1.txt t2.txt' '1.txt t2.txt')
file4=( '1.txt' '1.txt' '1.txt t2.txt')

opt0=('')
opt1=( -i -v -c -l -n -h -s -e)
opt2=(-iv -ic -il -in -ih -is -ie -vc -vl -vn -vh -vs -ve -cl -cn -ch -cs -ce -ln -lh -ls -le -nh -ns -ne -hs -he -se)

pattern1=(a b c d e f h 1)
pattern2=(he val va)
pattern3=('^he' 'l$' 'hello' )

for i in "${opt0[@]}" "${opt1[@]}" "${opt2[@]}";
    do
        for j in "${file2[@]}" "${file3[@]}" "${file4[@]}";
            do
                for p in "${pattern1[@]}" "${pattern2[@]}" "${pattern3[@]}";
                    do
                        echo grep $i $p $j
                        grep $i $p $j > grep.test.txt
                        ./s21_grep $i $p $j > s21_grep.test.txt
                        (( COUNTER++ ))
                        if cmp -s "grep.test.txt" "s21_grep.test.txt"; then
                            (( SUCCESS++ ))
                            echo "${Green}Same${NC}"
                        else
                            echo grep $i $p $j >> error.log
                            (( FAIL++ ))
                            echo  "${RED}Differ${NC}"
                        fi
                        
                    done
            done
    done

echo "--------"
echo "Count: $COUNTER : ${Green}Success:$SUCCESS${NC} -- ${RED}Fail:$FAIL${NC}"