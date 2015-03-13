#bin/bash
make
./bin/generator data/test_2  150
./bin/N2single data/test_2
#sdiff output_N2single data/test_2 
