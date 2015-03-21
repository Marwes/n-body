#!/bin/bash
num_tests=5
iters=35000
##iters=10
theta=0.8
threads=(1 2 4)
programs=(./bin/N2single ./bin/N2parallel ./bin/BH_single ./bin/BH_parallel)
tests=(data/120_bodies data/180_bodies data/240_bodies)


function run_test
{
	echo $1 $2 $3 $4 $5 $6 $7
	for ((i=0; i<num_tests; i++));
	do
		$1 $2 $3 $4 $5 $6 $7
	done
	echo -e "\n"
}



for t in ${tests[*]}
do
    for np in ${threads[*]}
    do
        run_test ./bin/N2parallel $t $iters $np
        run_test ./bin/BH_parallel $t $iters $theta $np
    done
    run_test ./bin/N2single $t $iters
    run_test ./bin/BH_single $t $iters $theta
done



