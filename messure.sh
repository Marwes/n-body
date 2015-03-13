num_tests=1
#iters=50000
iters=10
theta=0.5
threads=(1 2 4)
programs=(./bin/N2single ./bin/N2parallel ./bin/BH_single)
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



for p in ${programs[*]}
do
	for t in ${tests[*]}
	do
		if [[ $p == *"parallel"* ]]
		then
			for np in ${threads[*]}
			do
				run_test $p $t $iters $theta $np
			done
		else
			run_test $p $t $iters
		fi
	done
done



