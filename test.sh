#!/bin/bash

echo "tsp12"
./main "data/tsp12.dat" -m
echo "tsp60"
./main "data/tsp60.dat" -m
echo "rnd60"
./main "data/rnd60.dat" -m
echo "fb60"
./main "data/fb60.dat" -m
echo "rnd80"
./main "data/rnd80.dat" -m
echo "fb80"
./main "data/fb80.dat" -m
echo "rnd100"
./main "data/rnd100.dat" -m
echo "rnd100_2"
./main "data/rnd100_2.dat" -m
echo "fb100"
./main "data/fb100.dat" -m
