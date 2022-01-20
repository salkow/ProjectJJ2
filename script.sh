#!/bin/bash

rm -rf all_results.txt


rm -rf cmake-build-release
mkdir cmake-build-release

max_num_of_threads=$1
num_of_iterations=$2

for i in $( eval echo {1..$max_num_of_threads} )
do

  	cd cmake-build-release	 

	cmake -DCMAKE_BUILD_TYPE=Release -DNUM=$i .. > /dev/null

	cd ..

	cmake --build cmake-build-release --target inverted_google > /dev/null
  
	mo=0

	for j in $( eval echo {1..$num_of_iterations} )
	do

		cmake-build-release/inverted_google ./test_data/small_test.txt 60

		time_taken=$(cat result.txt | rg -o "Time = \d+ " | cut -d "=" -f2 | tr -d "[:space:]")

		echo $time_taken

		mo=$((mo+time_taken))

	done

	mo=$((mo/num_of_iterations))

	echo $mo >> all_results.txt

done

cat all_results.txt
