#!/bin/bash


# CHECK THE PATH OF YOUR TEST FILE
TEST_FILE=../../map_5x3.txt

echo "> start"
echo "$ compile"
rm -f _mini_paint* _our_mini_paint* _gen*
gcc -Wall -Wextra -Werror ../../mini_paint.c -lm  -o _mini_paint
gcc -Wall -Wextra -Werror our_mini_paint.c -lm  -o _our_mini_paint
g++ -Wall -Wextra -Werror generate_example.cpp -lm -o _gen
echo "$ test"
counter=1
max=2
our_res=-1
bad_res=-1

i=0
#while [ $counter -le $max ]
while (( $i < 1 ))
do
	#./_gen
	if [ $? ]
	then
		sleep .01
		#./_our_mini_paint  example_ > coutput 2>&1
		./_our_mini_paint  $TEST_FILE  > coutput 2>&1
		our_res=$?
		#../../_mini_paint  example_ > output 2>&1
		./_mini_paint  $TEST_FILE  > output 2>&1
		bad_res=$?
		printf "Echo return from _mini_paint: $?\n"
		if [ $our_res -ne $bad_res ]
		then
			printf "\n: different return result, our \e[1;31m$our_res\e[0m and yours \e[1;32m$bad_res\e[0m !\n"
			exit 1
		fi
		diff -y --suppress-common-lines coutput output
		printf "Echo return diff result: $?\n"
		if [ $? -ne 0 ]
		then
			printf "\e[1;31m: Difference in output: coutput is ours, output is yours, and the example is in example_ !\e[0m\n"
			exit 2
		fi
		if [ $(( $counter % 50 )) = 0 ]
		then
			printf "\e[1;34m[$counter]\e[0m"
		fi
		if [ $our_res -ne 0 ]
		then
			printf "\e[1;33m$our_res\e[0m"
		else
			printf "\e[1;32m$our_res\e[0m"
		fi
	fi
	#max=$((max + 1))
	#counter=$((counter + 1))
	i=$(( i+1 ))
done
#rm -f _mini_paint* _our_mini_paint* _gen* example_* output coutput
printf "\n> done"
