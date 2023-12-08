#!/bin/bash
thread_range_start=1
thread_range_end=272
scale=15
folder="benchmark/"

mkdir $folder
make clean

# benchmark omp version
make graph500_omp_bfs
omp_out="omp_threads_benchmark"
rm $folder$omp_out

for (( i = thread_range_start; i <= thread_range_end; i *= 2))
do
    echo ==== omp, scale = $scale, num_thread = $i ==== >> $folder$omp_out
    ./graph500_omp_bfs $scale 16 $i >> $folder$omp_out
done

# benchmark atomic version
make graph500_atomic_bfs
atomic_out="atomic_threads_benchmark"
rm $folder$atomic_out

for (( i = thread_range_start; i <= thread_range_end; i *= 2))
do
    echo ==== atomic, scale = $scale, num_thread = $i ==== >> $folder$atomic_out
    ./graph500_atomic_bfs $scale 16 $i >> $folder$atomic_out
done

make clean