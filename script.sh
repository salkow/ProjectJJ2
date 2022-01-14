#!/bin/bash

rm all_results.txt
rm -rf cmake-build-release/*

for i in {1..16}
do

  	cd cmake-build-release	 

	cmake -DCMAKE_BUILD_TYPE=Release -DNUM=$i ..

	cd ..

	cmake --build cmake-build-release --target inverted_google

	mo=0

	for i in {1..5}
	do

		cmake-build-release/inverted_google

		time_taken=$(cat result.txt | rg -o "Time = \d+ " | cut -d "=" -f2 | tr -d "[:space:]")

		mo=$(($mo+$time_taken))

	done

	mo=$(($mo/5))

	echo $mo >> all_results.txt


done
