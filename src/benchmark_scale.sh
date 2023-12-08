#!/bin/bash
scale_range_start=10
scale_range_end=20
num_thread=16
folder="benchmark/"

mkdir $folder
make clean

# benchmark serial version
make graph500_serial_bfs
serial_out="serial_scale_benchmark"
rm $folder$serial_out

for (( i=scale_range_start; i<=scale_range_end; i++))
do
    echo ==== serial, scale = $i ==== >> $folder$serial_out
    ./graph500_serial_bfs $i 16 >> $folder$serial_out
done

# benchmark omp version
make graph500_omp_bfs
omp_out="omp_scale_benchmark"
rm $folder$omp_out

for (( i = scale_range_start; i <= scale_range_end; i++))
do
    echo ==== omp, scale = $i, num_thread = $num_thread ==== >> $folder$omp_out
    ./graph500_omp_bfs $i 16 $num_thread >> $folder$omp_out
done

# benchmark atomic version
make graph500_atomic_bfs
atomic_out="atomic_scale_benchmark"
rm $folder$atomic_out

for (( i = scale_range_start; i <= scale_range_end; i++))
do
    echo ==== atomic, scale = $i, num_thread = $num_thread ==== >> $folder$atomic_out
    ./graph500_atomic_bfs $i 16 $num_thread >> $folder$atomic_out
done

make clean